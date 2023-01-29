#pragma once

#include "../glcall.h"

#include "../transform/transform.h"
#include "../model/model.h"

struct InstanceData {

  std::string name = "no_name";

  GLuint VBO;
  std::vector<glm::mat4>   model_matrices;
  std::vector<Transform *> model_transforms;
  Model *model;

  void genVBO();
  void addInstance(Model *model, Transform *transform);
  void setVertexAttribs();
  void perFrameUpdate();

};