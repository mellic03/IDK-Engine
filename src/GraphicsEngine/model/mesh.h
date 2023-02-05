#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../../include/rapidxml/rapidxml.hpp"
#include "../../include/glm/glm.hpp"
#include "../transform/transform.h"

#include "daeloader.h"
#include "material.h"
#include "animation/animation.h"


struct Vertex {
  glm::vec3 position    = glm::vec3(0.0f);
  glm::vec3 normal      = glm::vec3(0.0f);
  glm::vec2 texcoords   = glm::vec2(0.0f);
  glm::vec3 tangent     = glm::vec3(0.0f);
  glm::vec4 color       = glm::vec4(0.0f);
  glm::vec4 weights     = glm::vec4(0.0f);
  glm::ivec4 joint_ids  = glm::ivec4(-1);
};

// inline bool &glm::vec3::operator == (const glm::vec3 &v1, const glm::vec3 &v2)
// {
//   bool result = (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
//   return result;
// }

inline bool operator == (const Vertex &v1, const Vertex &v2)
{
  return (
    (v1.position == v2.position)    &&
    (v1.normal == v2.normal)        &&
    (v1.texcoords == v2.texcoords)
  );
}


class Mesh {

  private:

    void _createIndexBuffer();


  public:

    std::string m_dae_id;

    GLuint VAO, VBO;
    std::vector<GLuint> IBOS;
    std::vector<std::vector<GLuint>> indices;

    std::vector<Material> materials;
    std::vector<Vertex> vertices;


    Mesh() { };

    void transformByMatrix(glm::mat4 matrix);
    void setBufferData();

};

