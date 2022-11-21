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
  unsigned char *data = stbi_load(this->m_filename.c_str(), &width, &height, &bpp, STBI_rgb);
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
  this->setName(filepath);

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

      glm::vec3 face_normal = glm::normalize(glm::cross(positions[p2-1] - positions[p1-1], positions[p3-1] - positions[p1-1]));

      this->vertices[poly_count+0].position     = positions[p1-1];
      this->vertices[poly_count+0].normal       = normals[n1-1];
      this->vertices[poly_count+0].face_normal  = face_normal;
      this->vertices[poly_count+0].texture      = uvs[t1-1];

      this->vertices[poly_count+1].position     = positions[p2-1];
      this->vertices[poly_count+1].normal       = normals[n2-1];
      this->vertices[poly_count+1].face_normal  = face_normal;
      this->vertices[poly_count+1].texture      = uvs[t2-1];

      this->vertices[poly_count+2].position     = positions[p3-1];
      this->vertices[poly_count+2].normal       = normals[n3-1];
      this->vertices[poly_count+2].face_normal  = face_normal;
      this->vertices[poly_count+2].texture      = uvs[t3-1];

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
  char asset_path[128] = "assets/model/";
  
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'N' && buffer[1] == 's')
    {
      sscanf(buffer, "Ns %f", &x);
      this->material.spec_exponent = x;
    }

    else if (buffer[0] == 'K' && buffer[1] == 'a')
    {
      sscanf(buffer, "Ka %f %f %f", &x, &y, &z);
      this->material.ambient = {x, y, z};
    }

    else if (buffer[0] == 'K' && buffer[1] == 's')
    {
      sscanf(buffer, "Ks %f %f %f", &x, &y, &z);
      // this->material.specular = {x, y, z};
    }

    else if (buffer[0] == 'K' && buffer[1] == 'd')
    {
      sscanf(buffer, "Kd %f %f %f", &x, &y, &z);
      // this->material.diffuse = {x, y, z};
    }

    else if (buffer[0] == 'm' && buffer[1] == 'a')
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

      this->material.diffuse.load(asset_path);
      this->material.specular.load("assets/model/container_specular.png");

      break;
    }
  }

  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
  glBufferData(GL_ARRAY_BUFFER, this->num_vertices * sizeof(Vertex), this->vertices, GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  // Vertex normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // Face normal
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  // UV
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(9*sizeof(float)));
  glEnableVertexAttribArray(3);
  
  // Indexing
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->num_indices * sizeof(GLuint), this->indices, GL_STATIC_DRAW);


}


void Model::draw(Camera *cam)
{
  this->shader.use();
  // Get the uniform variables location. You've probably already done that before...
  GLuint diffloc = glGetUniformLocation(this->shader.get(), "material.diffuse");
  GLuint specloc  = glGetUniformLocation(this->shader.get(), "material.specular");

  // Then bind the uniform samplers to texture units:
  glUseProgram(this->shader.get());
  glUniform1i(diffloc, 0);
  glUniform1i(specloc, 1);

 this->material.diffuse.bind(GL_TEXTURE0);
 this->material.specular.bind(GL_TEXTURE1);


  glBindVertexArray(this->VAO);


  this->shader.setFloat("material.spec_exponent", this->material.spec_exponent);

  this->shader.setVec3("light.position",  cam->lightsource.position);
  this->shader.setVec3("light.ambient",  cam->lightsource.ambient);
  this->shader.setVec3("light.diffuse",  cam->lightsource.diffuse); // darken diffuse light a bit
  this->shader.setVec3("light.specular", cam->lightsource.specular); 


  this->shader.setVec3("light_pos", glm::vec3(0, 0, 0));
  this->shader.setVec3("view_pos", cam->pos);
  
  this->shader.setMat4("transform", this->transform_mat);
  this->shader.setMat4("model", this->model_mat);
  this->shader.setMat4("parent_model", this->parent_model_mat);
  this->shader.setMat4("view", cam->view);
  this->shader.setMat4("projection", cam->projection);

  glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, (void *)0);
  glBindVertexArray(0);
}

void Model::set_pos(glm::vec3 point)
{
  this->pos = point;
  this->model_mat = glm::mat4(1.0f);
  this->model_mat = glm::translate(this->transform_mat, point);
}

void Model::setName(const char *name_str)
{
  strcpy(this->name, name_str);
}

void Model::setShader(Shader shader_obj)
{
  this->shader = shader_obj;
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



