#include <stdio.h>
#include <iostream>

#include <fstream>
#include <sstream>

#include "mesh.h"

#include "../glcall.h"


Mesh::Mesh(void)  { }



void Mesh::setBufferData(void)
{
  glDeleteVertexArrays(1, &this->VAO);
  glDeleteBuffers(1, &this->VBO);

  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);


  glBindVertexArray(this->VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
  glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

  unsigned long offset = 0;

  // Position
  GLCALL( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offset) );
  offset += 3 * sizeof(float);
  GLCALL( glEnableVertexAttribArray(0) );

  // Vertex normal
  GLCALL( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offset) );
  offset += 3 * sizeof(float);
  GLCALL( glEnableVertexAttribArray(1) );

  // UV
  GLCALL( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offset) );
  offset += 2 * sizeof(float);
  GLCALL( glEnableVertexAttribArray(2) );

  // Tangent
  GLCALL( glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offset) );
  offset += 3 * sizeof(float);
  GLCALL( glEnableVertexAttribArray(3) );

  // Color
  GLCALL( glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offset) );
  offset += 4 * sizeof(float);
  GLCALL( glEnableVertexAttribArray(4) );

  // Joint weights
  GLCALL( glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offset) );
  offset += 4 * sizeof(float);
  GLCALL( glEnableVertexAttribArray(5) );

  // Joint IDs
  glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (void *)offset);
  offset += 4 * sizeof(int);
  GLCALL( glEnableVertexAttribArray(6) );


  // Indexing
  for (size_t i=0; i<this->IBOS.size(); i++)
  {
    GLCALL( glGenBuffers(1, &this->IBOS[i]) );
    GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBOS[i]) );
    GLCALL( glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices[i].size() * sizeof(GLuint), &this->indices[i][0], GL_STATIC_DRAW) );
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
