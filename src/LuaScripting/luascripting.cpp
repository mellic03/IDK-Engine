#include "luascripting.h"


void luaTest(void)
{
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> velocities;
  positions.push_back(glm::vec3(-34.0f, 0.0f, 10.0f));
  positions.push_back(glm::vec3(12.0f, 7.0f, 1.0f));
  
  velocities.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
  velocities.push_back(glm::vec3(0.0f, -2.0f, 1.0f));

  for (auto &vec: positions)
    printf("pos (pre-lua): %f %f %f\n", vec.x, vec.y, vec.z);
  for (auto &vec: velocities)
    printf("vel (pre-lua): %f %f %f\n", vec.x, vec.y, vec.z);



  LuaInterface::compile();



  {
    LuaInterface::begin();

    LuaInterface::ToLua::stdvec_vec3(positions, "Positions");
    LuaInterface::ToLua::stdvec_vec3(velocities, "Velocities");

    LuaInterface::execute();
  }



  LuaInterface::ToCPP::stdvec_vec3(&positions, "Positions");
  LuaInterface::ToCPP::stdvec_vec3(&velocities, "Velocities");

  for (auto &vec: positions)
    printf("pos (post-lua): %f %f %f\n", vec.x, vec.y, vec.z);
  for (auto &vec: velocities)
    printf("vel (post-lua): %f %f %f\n", vec.x, vec.y, vec.z);


}

