#include "instancedata.h"


void InstanceData::genVBO()
{
  GLCALL( glDeleteBuffers(1, &this->VBO) );
  GLCALL( glGenBuffers(1, &this->VBO) );
  GLCALL( glBindBuffer(GL_ARRAY_BUFFER, this->VBO) );
  GLCALL( glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->model_matrices.size(), &this->model_matrices[0], GL_STATIC_DRAW) );
  GLCALL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}


void InstanceData::addInstance(Model *model, Transform *transform)
{
  this->name = model->m_name;

  this->model_matrices.push_back(transform->getModelMatrix());
  this->model_transforms.push_back(transform);
  this->model = model;
}


void InstanceData::setVertexAttribs()
{
  this->genVBO();
  GLCALL( glBindBuffer(GL_ARRAY_BUFFER, this->VBO) );

  for (auto &mesh: this->model->m_meshes)
  {
    GLCALL( glBindVertexArray(mesh.VAO) );

    int pos = 5;
    int pos1 = pos + 0;
    int pos2 = pos + 1;
    int pos3 = pos + 2;
    int pos4 = pos + 3;

    GLCALL( glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(0)) );
    GLCALL( glEnableVertexAttribArray(pos1) );

    GLCALL( glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1*sizeof(glm::vec4))) );
    GLCALL( glEnableVertexAttribArray(pos2) );

    GLCALL( glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2*sizeof(glm::vec4))) );
    GLCALL( glEnableVertexAttribArray(pos3) );

    GLCALL( glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3*sizeof(glm::vec4))) );
    GLCALL( glEnableVertexAttribArray(pos4) );

    GLCALL( glVertexAttribDivisor(pos1, 1) );
    GLCALL( glVertexAttribDivisor(pos2, 1) );
    GLCALL( glVertexAttribDivisor(pos3, 1) );
    GLCALL( glVertexAttribDivisor(pos4, 1) );

    GLCALL( glBindVertexArray(0) );
  }
  
  GLCALL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

void InstanceData::perFrameUpdate()
{
  GLCALL( glBindBuffer(GL_ARRAY_BUFFER, this->VBO) );
  GLCALL( glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->model_matrices.size(), &this->model_matrices[0], GL_STATIC_DRAW) );

  for (size_t i=0; i<this->model_transforms.size(); i++)
  {
    this->model_matrices[i] = this->model_transforms[i]->getModelMatrix();
  }

  GLCALL( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}