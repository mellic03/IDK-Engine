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

#define ELEMENTS_PER_VERTEX (3 + 3 + 3 + 2 + 3 + 1 + 3 + 3)

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 face_normal;
  glm::vec2 texture;
  glm::vec3 tangent;
  GLuint material_index;
  glm::vec3 color;
  glm::vec3 anim_next_pos;
};


class Mesh {

  private:

    glm::mat4 *m_model_mat = nullptr;
    bool m_model_mat_set_manually = false;

  public:
    GLuint VAO, VBO, IBO0, IBO1;
    std::vector<GLuint> IBOS;
    std::vector<std::vector<GLuint>> indices;

    std::vector<Material> materials;

    int num_polygons;
    Vertex *vertices;
    int num_vertices, num_indices;

    std::string m_name;

    bool rotate_local = false;

    glm::vec3 dir = {0, 0, 1.0f};

    Mesh();

    void load(const char *filepath, std::string name);
    void setBufferData(void);
    void draw(Renderer *ren);

    glm::mat4 getParentModelMat(void);

    void setModelMat(glm::mat4 *matptr)  { this->m_model_mat = matptr; };


};

