#include "luascripting.h"

void luaInit(SceneGraph *graph)
{
  LuaInterface::init(graph);
  LuaInterface::compile();
}

void luaMain(Renderer *ren, Player *player, std::list<GameObject> *gameobjects)
{
  std::vector<int> IDs;
  std::vector<std::string> scripts;
  std::vector<glm::vec3> positions, velocities;

  auto element = gameobjects->begin();
  for (int i=0; i<gameobjects->size(); i++)
  {
    GameObject *object = &(*element);

    for (auto &component: object->script_components)
    {
      if (component.script_changed)
        LuaInterface::compile();

      IDs.push_back(object->getID() + 1);
      scripts.push_back(component.script_name);
    }
    element = std::next(element);
  }

  LuaInterface::ToLua::number(ren->deltaTime, "DeltaTime");
  // LuaInterface::ToLua::keylog(player->keylog, "KeyPressed");
  
  LuaInterface::ToLua::stdvec_int(IDs, "IDs");
  LuaInterface::ToLua::stdvec_stdstring(scripts, "Scripts");


  LuaInterface::execute();

}

