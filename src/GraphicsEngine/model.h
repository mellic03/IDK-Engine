#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "camera.h"

#define ELEMENTS_PER_VERTEX (3 + 3 + 2)

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 face_normal;
  glm::vec2 texture;
};

struct Material {
  glm::vec3 ambient = glm::vec3(0.0f);
  glm::vec3 diffuse = glm::vec3(0.0f);
  glm::vec3 specular = glm::vec3(0.0f);
  float spec_exponent = 0.5f;
};

struct Light {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 ambient = glm::vec3(0.0f);
  glm::vec3 diffuse = glm::vec3(0.0f);
  glm::vec3 specular = glm::vec3(0.0f);
};

class Texture {

  private:
    std::string m_filename;
    GLenum m_texture_obj;

  public:
    Texture() {};

    bool load(const char *filepath);
    void bind(GLenum texture_unit);

};

class Model {

  public:

    GLuint VAO, VBO, IBO;
    float *positions;
    float *normals;
    int num_polygons;
    Vertex *vertices; int num_vertices;
    int *indices; int num_indices;

    char name[64] = "empty";

    Texture texture;
    Material material;
    
    glm::mat4 *view_mat;
    glm::mat4 model_mat;
    glm::mat4 parent_model_mat = glm::mat4(1.0f);
    glm::mat4 transform_mat = glm::mat4(1.0f);

    glm::vec3 dir = {0, 0, 1.0f};
    glm::vec3 pos = {0, 0, 0};
    glm::vec3 rot = {0, 0, 0};


    Model *next;

    Model();

    void load(const char *filepath);
    void draw();

    void setName(const char *name_str);

    void set_pos(glm::vec3 position);
    void translate(glm::vec3 translation);

    void set_rot_x(float theta);
    void rot_x(float theta);
    void rot_y(float theta);
    void rot_z(float theta);

    void set_parent(glm::mat4 model_matrix);

};



