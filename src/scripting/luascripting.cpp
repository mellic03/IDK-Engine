#include "luascripting.h"

void luaInit(void)
{
  LuaInterface::compile();
}

void luaMain(Renderer *ren, Player *player, std::list<GameObject> *gameobjects)
{
  LuaInterface::begin();


  std::vector<int> IDs;
  std::vector<std::string> scripts;
  std::vector<glm::vec3> positions, velocities;

  auto element = gameobjects->begin();
  for (int i=0; i<gameobjects->size(); i++)
  {
    GameObject *object = &(*element);

    for (auto &script: object->script_components)
    {
      IDs.push_back(object->getID());
      scripts.push_back(script.script_name);
    }

    positions.push_back(*object->getPos());
    velocities.push_back(*object->getVel());

    element = std::next(element);
  }

  // LuaInterface::ToLua::number(ren->deltaTime, "DeltaTime");
  // LuaInterface::ToLua::keylog(player->keylog, "KeyPressed");
  
  LuaInterface::ToLua::stdvec_int(IDs, "IDs");
  LuaInterface::ToLua::stdvec_stdstring(scripts, "Scripts");
  LuaInterface::ToLua::stdvec_vec3(positions, "Positions");
  LuaInterface::ToLua::stdvec_vec3(velocities, "Velocities");

  LuaInterface::execute();


  // element = gameobjects->begin();
  // for (int i=0; i<gameobjects->size(); i++)
  // {
  //   LuaInterface::ToCPP::gameobject(&*element, (*element).getID());
  //   element = std::next(element);
  // }
}

