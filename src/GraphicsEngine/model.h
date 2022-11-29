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

#include "renderer.h"
#include "material.h"


#define ELEMENTS_PER_VERTEX (3 + 3 + 2 + 3 + 1)

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 face_normal;
  glm::vec2 texture;
  glm::vec3 tangent;
  GLuint material_index;
};


class Model {

  public:

    GLuint VAO, VBO, IBO0, IBO1;
    std::vector<GLuint> IBOS;
    std::vector<std::vector<GLuint>> indices;

    std::vector<Material> materials;

    Shader *shader;
    float *positions;
    float *normals;
    int num_polygons;
    Vertex *vertices; int num_vertices;
    int num_indices;


    char name[64] = "empty";

    glm::mat4 transform_mat = glm::mat4(1.0f);
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 *view_mat = &this->model_mat, *projection_mat = &this->model_mat;

    glm::vec3 dir = {0, 0, 1.0f};
    glm::vec3 pos = {0, 0, 0};
    glm::vec3 rot = {0, 0, 0};


    Model *next;

    Model();

    void load(const char *filepath, std::string name);
    void draw(Renderer *ren);

    void bindRenderer(Renderer *ren);

    void set_pos(glm::vec3 position);
    void translate(glm::vec3 translation);
    void scale(float alpha);

    void set_rot_x(float theta);
    void rot_x(float theta);
    void rot_y(float theta);
    void rot_z(float theta);

};



