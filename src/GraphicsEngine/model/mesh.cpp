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
  // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));
  // glEnableVertexAttribArray(2);


  
  // // Tangent
  // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(11*sizeof(float)));
  // glEnableVertexAttribArray(4);


  // Indexing
  // for (int i=0; i<this->IBOS.size(); i++)
  // {
    // glGenBuffers(1, &this->IBOS[0]);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBOS[0]);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices[0].size() * sizeof(GLuint), &this->indices[i][0], GL_STATIC_DRAW);
  // }
}



void Mesh::draw(void)
{
  // if (this->m_transform == nullptr)
  // {
  //   printf("Mesh::m_transform == nullptr\n");
  //   exit(1);
  // }


  // glBindVertexArray(this->VAO);

  // ren->active_shader->setVec3("diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
  // ren->active_shader->setMat4("model", this->m_transform->getModelMatrix());
  // ren->active_shader->setMat4("view", ren->cam.view);
  // ren->active_shader->setMat4("projection", ren->cam.projection);


  // char buffer[64];

  // for (int i=0; i<this->IBOS.size(); i++)
  // {
  //   this->materials[i].diffuse.bind(  GL_TEXTURE0 );
  //   this->materials[i].specular.bind(  GL_TEXTURE1 );
  //   this->materials[i].normal.bind(  GL_TEXTURE2 );
  //   this->materials[i].emission.bind(  GL_TEXTURE3 );
  
  //   ren->active_shader->setInt("material.diffuseMap", 0);
  //   ren->active_shader->setInt("material.specularMap", 1);
  //   ren->active_shader->setInt("material.normalMap", 2);
  //   ren->active_shader->setInt("material.emissionMap", 3);
  //   ren->active_shader->setFloat("material.spec_exponent", this->materials[i].spec_exponent);


  //   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBOS[i]);
  //   glDrawElements(GL_TRIANGLES, this->indices[i].size(), GL_UNSIGNED_INT, (void *)0);
  //   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //   unbindTextureUnit(GL_TEXTURE0);
  //   unbindTextureUnit(GL_TEXTURE1);
  //   unbindTextureUnit(GL_TEXTURE2);
  //   unbindTextureUnit(GL_TEXTURE3);
  // }

  // glBindVertexArray(0);
}


