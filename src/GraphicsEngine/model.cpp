#include <stdio.h>
#include <iostream>

#include "../include/stb_image/stb_image.h"

#include "renderer.h"
#include "camera.h"
#include "model.h"


Model::Model(void)
{
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->IBO);
}

void Model::load(const char *filepath, std::string name)
{
  std::string obj_path(filepath);
  obj_path.append(name);
  obj_path.append(".obj");

  std::string mtl_path(filepath);
  mtl_path.append(name);
  mtl_path.append(".mtl");

  std::string diffuse_path(filepath);
  std::string specular_path(filepath);
  std::string emission_path(filepath);
  std::string normal_path(filepath);
  diffuse_path.append(name + "_diffuse.png");
  specular_path.append(name + "_specular.png");
  emission_path.append(name + "_emission.png");
  normal_path.append(name + "_normal.png");



  FILE *fh = fopen(obj_path.c_str(), "r");
  if (fh == NULL)
  {
    printf("Error opening obj: \"%s\"", obj_path.c_str());
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
      uvs[tex_count] = glm::vec2(x, y);
      tex_count += 1;
    }
  
    else if (buffer[0] == 'f' && buffer[1] == ' ')
    {
      sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);

      glm::vec3 face_normal = glm::normalize(glm::cross(positions[p2-1] - positions[p1-1], positions[p3-1] - positions[p1-1]));

      // tangent/bitangent calculation
      //----------------------------------------------------
      glm::vec3 edge1 = positions[p2-1] - positions[p1-1];
      glm::vec3 edge2 = positions[p3-1] - positions[p1-1];
      glm::vec2 deltaUV1 = uvs[t2-1] - uvs[t1-1];
      glm::vec2 deltaUV2 = uvs[t3-1] - uvs[t1-1];

      float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
      glm::vec3 tangent = {
        f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
        f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
        f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
      };

      this->vertices[poly_count+0].tangent = tangent;
      this->vertices[poly_count+1].tangent = tangent;
      this->vertices[poly_count+2].tangent = tangent;

      glm::vec3 bitangent = {
        f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
        f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
        f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
      };

      this->vertices[poly_count+0].bitangent = bitangent;
      this->vertices[poly_count+1].bitangent = bitangent;
      this->vertices[poly_count+2].bitangent = bitangent;
      //----------------------------------------------------


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
  fh = fopen(mtl_path.c_str(), "r");
  if (fh == NULL)
  {
    printf("Error opening mtl: \"%s\"\n", mtl_path.c_str());
    exit(1);
  }

  
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

    else if (buffer[0] == 'm' && buffer[1] == 'a')
    {
      char *token = strtok(buffer, " ");
      token = strtok(NULL, " ");
      for (int i=0; i<strlen(token); i++)
      {
        if (token[i] == '\n')
          token[i] = '\0';
      }

      this->material.diffuse.load(diffuse_path.c_str(), true);
      this->material.specular.load(specular_path.c_str(), false);
      this->material.emission.load(emission_path.c_str(), false);
      this->material.normal.load(normal_path.c_str(), false);
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
  
  // Tangent
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(11*sizeof(float)));
  glEnableVertexAttribArray(4);

  // Bitangent
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(14*sizeof(float)));
  glEnableVertexAttribArray(5);

  // Indexing
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->num_indices * sizeof(GLuint), this->indices, GL_STATIC_DRAW);

}


void Model::draw(Renderer *ren)
{
  ren->active_shader.setInt("material.diffuseMap", 4);
  ren->active_shader.setInt("material.specularMap", 5);
  ren->active_shader.setInt("material.emissionMap", 6);
  ren->active_shader.setInt("material.normalMap", 7);
  ren->active_shader.setFloat("material.spec_exponent", this->material.spec_exponent);

  this->material.diffuse.bind(GL_TEXTURE4);
  this->material.specular.bind(GL_TEXTURE5);
  this->material.emission.bind(GL_TEXTURE6);
  this->material.normal.bind(GL_TEXTURE7);

  glBindVertexArray(this->VAO);

  char buffer[64];

  for (int i=0; i<ren->NM_DIRLIGHTS; i++)
  {
    sprintf(buffer, "dirlights[%d].direction", i);
    ren->active_shader.setVec3(buffer,  ren->dirlights[i].direction);

    sprintf(buffer, "dirlights[%d].ambient", i);
    ren->active_shader.setVec3(buffer,  ren->dirlights[i].ambient);

    sprintf(buffer, "dirlights[%d].diffuse", i);
    ren->active_shader.setVec3(buffer,  ren->dirlights[i].diffuse);

    sprintf(buffer, "dirlights[%d].specular", i);
    ren->active_shader.setVec3(buffer,  ren->dirlights[i].specular);
  }

  for (int i=0; i<ren->NM_POINTLIGHTS; i++)
  {
    sprintf(buffer, "pointlights[%d].position", i);
    ren->active_shader.setVec3(buffer,  ren->pointlights[i].position);

    sprintf(buffer, "pointlights[%d].ambient", i);
    ren->active_shader.setVec3(buffer,  ren->pointlights[i].ambient);

    sprintf(buffer, "pointlights[%d].diffuse", i);
    ren->active_shader.setVec3(buffer,  ren->pointlights[i].diffuse);

    sprintf(buffer, "pointlights[%d].specular", i);
    ren->active_shader.setVec3(buffer,  ren->pointlights[i].specular);

    sprintf(buffer, "pointlights[%d].constant", i);
    ren->active_shader.setFloat(buffer,  ren->pointlights[i].constant); 

    sprintf(buffer, "pointlights[%d].linear", i);
    ren->active_shader.setFloat(buffer,  ren->pointlights[i].linear); 

    sprintf(buffer, "pointlights[%d].quadratic", i);
    ren->active_shader.setFloat(buffer,  ren->pointlights[i].quadratic); 
  }

  for (int i=0; i<ren->NM_SPOTLIGHTS; i++)
  {
    sprintf(buffer, "spotlights[%d].position", i);
    ren->active_shader.setVec3(buffer,  ren->spotlights[i].position);

    sprintf(buffer, "spotlights[%d].direction", i);
    ren->active_shader.setVec3(buffer,  ren->spotlights[i].direction);

    sprintf(buffer, "spotlights[%d].ambient", i);
    ren->active_shader.setVec3(buffer,  ren->spotlights[i].ambient);

    sprintf(buffer, "spotlights[%d].diffuse", i);
    ren->active_shader.setVec3(buffer,  ren->spotlights[i].diffuse);

    sprintf(buffer, "spotlights[%d].specular", i);
    ren->active_shader.setVec3(buffer,  ren->spotlights[i].specular);

    sprintf(buffer, "spotlights[%d].constant", i);
    ren->active_shader.setFloat(buffer,  ren->spotlights[i].constant);

    sprintf(buffer, "spotlights[%d].linear", i);
    ren->active_shader.setFloat(buffer,  ren->spotlights[i].linear);

    sprintf(buffer, "spotlights[%d].quadratic", i);
    ren->active_shader.setFloat(buffer,  ren->spotlights[i].quadratic);

    sprintf(buffer, "spotlights[%d].inner_cutoff", i);
    ren->active_shader.setFloat(buffer,  glm::cos(glm::radians(ren->spotlights[i].inner_cutoff)));

    sprintf(buffer, "spotlights[%d].outer_cutoff", i);
    ren->active_shader.setFloat(buffer,  glm::cos(glm::radians(ren->spotlights[i].outer_cutoff)));

    sprintf(buffer, "spotlights[%d].intensity", i);
    ren->active_shader.setFloat(buffer,  ren->spotlights[i].intensity);
  }


  ren->active_shader.setVec3("viewPos", ren->cam.pos);
  ren->active_shader.setVec3("diffuse", this->material.diffuse_color);

  
  ren->active_shader.setMat4("transform", this->transform_mat);
  ren->active_shader.setMat4("model", this->model_mat);
  ren->active_shader.setMat4("view", *this->view_mat);
  ren->active_shader.setMat4("projection", *this->projection_mat);


  glDrawElements(GL_TRIANGLES, this->num_indices, GL_UNSIGNED_INT, (void *)0);
  glBindVertexArray(0);
}

void Model::set_pos(glm::vec3 point)
{
  this->pos = point;
  this->model_mat = glm::mat4(1.0f);
  this->model_mat = glm::translate(this->model_mat, this->pos);
}

void Model::bindRenderer(Renderer *ren)
{
  this->view_mat = &ren->cam.view;
  this->projection_mat = &ren->cam.projection;
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



