#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../../include/rapidxml/rapidxml.hpp"
#include "../../include/glm/glm.hpp"
#include "../../transform.h"

#include "daeloader.h"
#include "material.h"


struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texcoords;
  glm::vec3 tangent;
};


class Mesh {

  private:


  public:

    std::string m_dae_id;

    GLuint VAO, VBO;
    std::vector<GLuint> IBOS;
    std::vector<std::vector<GLuint>> indices;

    std::vector<Material> materials;
    std::vector<Vertex> vertices;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 2.0f);

    Mesh(void);

    void transformByMatrix(glm::mat4 matrix);

    void load(std::string filepath);
    void setBufferData(void);


};

