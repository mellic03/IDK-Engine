#include "luainterface.h"



void LuaInterface::ToCPP::stdvec_vec3(std::vector<glm::vec3> *vecOfVec3, std::string name)
{
  LuaTTable *table = tablePtr(name);
  table->PopGlobal(*LuaInterface::L);

  auto element = vecOfVec3->begin();

  for (int i=0; i<vecOfVec3->size(); i++)
  {
    LuaTTable vec = *(LuaTTable *)&table->getValue(Table::Key(i+1));

    *element = glm::vec3(
      (*(LuaTNumber *)&vec.getValue(Table::Key("x"))).getValue(),
      (*(LuaTNumber *)&vec.getValue(Table::Key("y"))).getValue(),
      (*(LuaTNumber *)&vec.getValue(Table::Key("z"))).getValue()
    );
    element = std::next(element);
  }
}


void LuaInterface::ToCPP::gameobject(GameObject *object, int objectID)
{
  *object->getPos() = object->getTransform()->worldToLocal_noLocalTransform(LuaInterface::positions[objectID], false);
  *object->getVel() = LuaInterface::velocities[objectID];
}

