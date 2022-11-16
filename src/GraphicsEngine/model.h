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
  glm::vec2 texture;
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

  private:
    
    GLuint VAO, VBO, IBO;
    float *positions;
    float *normals;
    int num_polygons;
    Vertex *vertices; int num_vertices;
    int *indices; int num_indices;

    Texture texture;

    glm::mat4 model_mat;

    glm::vec3 pos = {0, 0, 0};
    glm::vec3 rot = {0, 0, 0};

  public:
    Model();

    void load(const char *filepath, GLuint shader);
    void draw(Camera *cam,  GLuint shader);

    void translate(float x, float y, float z);

    void rot_x(float theta);
    void rot_y(float theta);
    void rot_z(float theta);

};