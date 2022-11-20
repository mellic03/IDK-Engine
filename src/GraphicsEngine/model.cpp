#include <stdio.h>
#include <iostream>

#include "../include/stb_image/stb_image.h"

#include "renderer.h"
#include "camera.h"
#include "model.h"


bool Texture::load(const char *filepath)
{
  this->m_filename = std::string(filepath);

  // stbi_set_flip_vertically_on_load(1);
  int width, height, bpp;
  unsigned char *data = stbi_load(this->m_filename.c_str(), &width, &height, &bpp, 3);
  if (!data)
  {
    printf("Error loading texture from \"%s\" --%s\n", this->m_filename.c_str(), stbi_failure_reason());
    exit(1);
  }
  // printf("Width: %d, height: %d, bpp: %d\n", width, height, bpp);

  glGenTextures(1, &this->m_texture_obj);
  glBindTexture(GL_TEXTURE_2D, this->m_texture_obj);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  return true;
}

void Texture::bind(GLenum texture_unit)
{
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, this->m_texture_obj);
}










Model::Model(void)
{
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->IBO);

  this->model_mat = glm::mat4(1.0f);
}

void Model::load(const char *filepath)
{
  FILE *fh = fopen(filepath, "r");
  if (fh == NULL)
  {
    printf("Error opening \"%s\"", filepath);
    exit(1);
  }

  int pos_count = 0, norm_count= 0, tex_count = 0, poly_count = 0;
 
  char buffer[64];
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'v' && buffer[1] == ' ')
      pos_count += 1;
    else if (buffer[0] == 'v' && buffer[1] == 'n')
      norm_count += 1;
    else if (buffer[0] == 'v' && buffer[1] == 't')
      tex_count += 1;
    else if (buffer[0] == 'f' && buffer[1] == ' ')
      poly_count += 1;
  }
  rewind(fh);

  glm::vec3 *positions = (glm::vec3 *)malloc(pos_count * sizeof(glm::vec3));
  glm::vec3 *normals   = (glm::vec3 *)malloc(norm_count * sizeof(glm::vec3));
  glm::vec2 *uvs       = (glm::vec2 *)malloc(tex_count * sizeof(glm::vec2));

  // poly_count number of polys, each poly contains three vertices
  this->vertices = (Vertex *)malloc(poly_count * 3 * sizeof(Vertex));

  float x, y, z;
  int p1, t1, n1, p2, t2, n2, p3, t3, n3;

  pos_count = 0, norm_count = 0, tex_count = 0, poly_count = 0;

  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'v' && buffer[1] == ' ')
    {
      sscanf(buffer, "v %f %f %f", &x, &y, &z);
      positions[pos_count] = glm::vec3(x, y, z);
      pos_count += 1;
    }

    else if (buffer[0] == 'v' && buffer[1] == 'n')
    {
      sscanf(buffer, "vn %f %f %f", &x, &y, &z);
      normals[norm_count] = glm::vec3(x, y, z);
      norm_count += 1;
    }

    else if (buffer[0] == 'v' && buffer[1] == 't')
    {
      sscanf(buffer, "vt %f %f", &x, &y);
      uvs[tex_count] = glm::vec2(x, 1-y);
      tex_count += 1;
    }
  
    else if (buffer[0] == 'f' && buffer[1] == ' ')
    {
      sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);
      this->vertices[poly_count+0].position = positions[p1-1];
      this->vertices[poly_count+0].normal   = normals[n1-1];
      this->vertices[poly_count+0].texture  = uvs[t1-1];

      this->vertices[poly_count+1].position = positions[p2-1];
      this->vertices[poly_count+1].normal   = normals[n2-1];
      this->vertices[poly_count+1].texture  = uvs[t2-1];

      this->vertices[poly_count+2].position = positions[p3-1];
      this->vertices[poly_count+2].normal   = normals[n3-1];
      this->vertices[poly_count+2].texture  = uvs[t3-1];

      poly_count += 3;
    }
  }
  fclose(fh);

  this->num_vertices = poly_count;
  this->num_indices = this->num_vertices; // This needs to be changed once number of verts != number of indices

  this->indices = (int *)malloc(this->num_vertices * sizeof(int));
  for (int i=0; i<this->num_vertices; i++)
    this->indices[i] = i;


  // load .mtl file
  char filepath_mtl[64];
  strcpy(filepath_mtl, filepath);
  int len = strlen(filepath_mtl);
  filepath_mtl[len-3] = 'm';
  filepath_mtl[len-2] = 't';
  filepath_mtl[len-1] = 'l';

  fh = fopen(filepath_mtl, "r");
  if (fh == NULL)
  {
    printf("Error opening \"%s\"\n", filepath_mtl);
    exit(1);
  }

  char image_path[64];
  char asset_path[128] = "assets/cube/";
  
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'm' && buffer[1] == 'a')
    {
      char *token = strtok(buffer, " ");
      token = strtok(NULL, " ");
      for (int i=0; i<strlen(token); i++)
      {
        if (token[i] == '\n')
          token[i] = '\0';
      }
      strcat(asset_path, token);
      // printf("path: %s\n", asset_path);
      len = strlen(asset_path);
      asset_path[len] = '\0';

      this->texture.load(asset_path);

      break;
    }
  }


  glUseProgram(renderer.mat_shader);

  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
  glBufferData(GL_ARRAY_BUFFER, this->num_vertices * sizeof(Vertex), this->vertices, GL_STATIC_DRAW);

  // Positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  // Normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // UVs
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));
  glEnableVertexAttribArray(2);
  

  this->texture.bind(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(renderer.mat_shader, "gSampler"), 0);


  // Indexing
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->num_indices * sizeof(GLuint), this->indices, GL_STATIC_DRAW);
}


void Model::draw()
{
  glBindVertexArray(this->VAO);

  glUseProgram(renderer.mat_shader);
  this->texture.bind(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(renderer.mat_shader, "gSampler"), 0);

  this->model_mat = glm::mat4(1.0f);
  this->model_mat = glm::translate(this->model_mat, this->pos);

  int emmission_loc = glGetUniformLocation(renderer.mat_shader, "emmission");
  glUniform4f(emmission_loc, this->emmission.x, this->emmission.y, this->emmission.z, this->emmission.w);

  int lightpos_loc = glGetUniformLocation(renderer.mat_shader, "light_pos");
  glUniform3f(lightpos_loc, 0, 0, 0);

  // int view_loc = glGetUniformLocation(renderer.mat_shader, "view_pos");
  // glUniform3f(emmission_loc, 0, 0, 0);

  int transform_loc = glGetUniformLocation(renderer.mat_shader, "transform");
  glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(this->transform_mat));

  int model_loc = glGetUniformLocation(renderer.mat_shader, "model");
  glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(this->model_mat));

  int parent_model_loc = glGetUniformLocation(renderer.mat_shader, "parent_model");
  glUniformMatrix4fv(parent_model_loc, 1, GL_FALSE, glm::value_ptr(this->parent_model_mat));

  int view_loc = glGetUniformLocation(renderer.mat_shader, "view");
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(renderer.cam.view));

  int projection_loc = glGetUniformLocation(renderer.mat_shader, "projection");
  glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(renderer.cam.projection));


  glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, (void *)0);
  glBindVertexArray(0);
}


void Model::set_emmission(glm::vec4 emmission_v)
{
  this->emmission = emmission_v;
}



void Model::translate(glm::vec3 translation)
{
  this->pos += translation;
  this->model_mat = glm::translate(this->model_mat, translation);
}

void Model::set_rot_x(float theta)
{
  this->rot.x = theta;
  this->transform_mat = glm::mat4(1.0f);
  this->transform_mat = glm::rotate(this->transform_mat, glm::radians(theta), glm::vec3(1.0, 0.0, 0.0));
}

void Model::rot_x(float theta)
{
  this->rot.x += theta;
  this->transform_mat = glm::rotate(this->transform_mat, glm::radians(theta), glm::vec3(1.0, 0.0, 0.0));
}

void Model::rot_y(float theta)
{
  this->rot.y += theta;
  this->transform_mat = glm::rotate(this->transform_mat, glm::radians(theta), glm::vec3(0.0, 1.0, 0.0));
}

void Model::rot_z(float theta)
{
  this->rot.z += theta;
  this->transform_mat = glm::rotate(this->transform_mat, glm::radians(theta), glm::vec3(0.0, 0.0, 1.0));
}



void Model::set_parent(glm::mat4 model_matrix)
{
  this->parent_model_mat = model_matrix;
}



