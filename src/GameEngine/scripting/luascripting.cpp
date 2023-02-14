#include "luascripting.h"

void luaInit()
{
  LuaInterface::compile();
}


void luaMain(Renderer *ren, KeyLog *keylog, std::list<GameObject> *gameobjects)
{
  std::vector<int> IDs;
  std::vector<std::string> scripts;
  std::vector<glm::vec3> positions, velocities;

  auto element = gameobjects->begin();
  

  for (auto &object: *gameobjects)
  {
    for (ScriptComponent component: *object.getComponents()->getScriptComponents())
    {
      if (component.script_changed)
      {
        LuaInterface::compile();
        component.script_changed = false;
      }

      IDs.push_back(object.getID() + 1);
      scripts.push_back(component.script_name);
    }
  }

  LuaInterface::ToLua::number(ren->deltaTime, "DeltaTime");
  LuaInterface::ToLua::keylog(keylog, "KeyPressed");
  
  LuaInterface::ToLua::stdvec_int(IDs, "IDs");
  LuaInterface::ToLua::stdvec_stdstring(scripts, "Scripts");


  LuaInterface::execute();

}

