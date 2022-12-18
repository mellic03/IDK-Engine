#include <stdio.h>
#include <iostream>

#include <fstream>
#include <sstream>

#include "mesh.h"


Mesh::Mesh(void)
{
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
}


void Mesh::load(std::string filepath)
{


}


void Mesh::setBufferData(void)
{
  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
  glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  // Vertex normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // UV
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  // // Face normal
  // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(9*sizeof(float)));
  // glEnableVertexAttribArray(3);


  // // Tangent
  // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(11*sizeof(float)));
  // glEnableVertexAttribArray(4);


  // Indexing
  for (int i=0; i<this->IBOS.size(); i++)
  {
    glGenBuffers(1, &this->IBOS[i]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBOS[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices[i].size() * sizeof(GLuint), &this->indices[i][0], GL_STATIC_DRAW);
  }
}


void Mesh::transformByMatrix(glm::mat4 matrix)
{
  for (auto &v: this->vertices)
  {
    v.position = matrix * glm::vec4(v.position.x, v.position.y, v.position.z, 1.0f);
    v.normal = matrix * glm::vec4(v.normal.x, v.normal.y, v.normal.z, 0.0f);
  }
}
