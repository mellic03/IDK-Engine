#include "luascripting.h"


void luaTest(void)
{
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> velocities;

  LuaInterface::compile();
  LuaInterface::begin();

  {
    positions.push_back(glm::vec3(-34.0f, 0.0f, 10.0f));
    positions.push_back(glm::vec3(12.0f, 7.0f, 1.0f));
    
    velocities.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    velocities.push_back(glm::vec3(0.0f, -2.0f, 1.0f));


    LuaInterface::ToLua::stdvec_vec3(positions, "Positions");
    LuaInterface::ToLua::stdvec_vec3(velocities, "Velocities");
  }
  LuaInterface::execute();

  {
    LuaInterface::ToCPP::stdvec_vec3(&positions, "Positions");
    LuaInterface::ToCPP::stdvec_vec3(&velocities, "Velocities");

    for (auto &vec: positions)
      printf("pos (in C++): %f %f %f\n", vec.x, vec.y, vec.z);

    for (auto &vec: velocities)
      printf("vel (in C++): %f %f %f\n", vec.x, vec.y, vec.z);
  }

  LuaInterface::begin();

  {
    positions.push_back(glm::vec3(21.0f, 3.0f, -7.0f));
    positions.push_back(glm::vec3(12.0f, 21.0f, 1.0f));
    
    velocities.push_back(glm::vec3(3.0f, 0.0f, 0.0f));
    velocities.push_back(glm::vec3(0.0f, 232.0f, 1.0f));


    LuaInterface::ToLua::stdvec_vec3(positions, "Positions");
    LuaInterface::ToLua::stdvec_vec3(velocities, "Velocities");
  }

  LuaInterface::execute();

  {
    LuaInterface::ToCPP::stdvec_vec3(&positions, "Positions");
    LuaInterface::ToCPP::stdvec_vec3(&velocities, "Velocities");

    for (auto &vec: positions)
      printf("pos (in C++): %f %f %f\n", vec.x, vec.y, vec.z);

    for (auto &vec: velocities)
      printf("vel (in C++): %f %f %f\n", vec.x, vec.y, vec.z);
  }


}

