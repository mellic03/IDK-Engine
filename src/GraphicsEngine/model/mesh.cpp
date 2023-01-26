#include <stdio.h>
#include <iostream>

#include <fstream>
#include <sstream>

#include "mesh.h"

#include "../glcall.h"


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
  // if (this->vertices[0].weights[0] != 0.0f && this->vertices.size() < 100)
  // {
  //   int i = 0;
  //   for (Vertex v: this->vertices)
  //   {
  //     printf("this->_vertices[%d]: xyz: { %f %f %f }, IDs: { %d %d %d }, weights: { %f %f %f }\n",
  //       i,
  //       v.position.x,
  //       v.position.y,
  //       v.position.z,
  //       v.joint_ids[0],
  //       v.joint_ids[1],
  //       v.joint_ids[2],
  //       v.weights[0],
  //       v.weights[1],
  //       v.weights[2]
  //     );
    
  //     i += 1;
  //   }
  // }


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
  GLCALL( glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offset) );
  offset += 3 * sizeof(float);
  GLCALL( glEnableVertexAttribArray(5) );

  // Joint IDs
  glVertexAttribIPointer(6, 3, GL_INT, sizeof(Vertex), (void *)offset);
  offset += 3 * sizeof(int);
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
