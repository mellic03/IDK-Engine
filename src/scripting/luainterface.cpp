#include "luainterface.h"


LuaContext LuaInterface::context;
std::unique_ptr<LuaState> LuaInterface::L;

std::vector<LuaInterface::TableReference> LuaInterface::table_references;

std::vector<int> LuaInterface::IDs;
std::vector<std::string> LuaInterface::scripts;
std::vector<glm::vec3> LuaInterface::positions;
std::vector<glm::vec3> LuaInterface::velocities;


LuaTTable *LuaInterface::tablePtr(std::string name)
{
  for (auto &ref: table_references)
    if (ref.name == name)
      return &ref.table;
  return nullptr;
}

void LuaInterface::compile(void)
{
	LuaInterface::context.CompileFile("main", "./LuaScripting/main.lua");
}

void LuaInterface::begin(void)
{
  LuaInterface::table_references.clear();
  LuaInterface::IDs.clear();
  LuaInterface::scripts.clear();
  LuaInterface::positions.clear();
  LuaInterface::velocities.clear();

	L = context.newStateFor("main");
}


void LuaInterface::sendVectors(void)
{
  LuaInterface::ToLua::stdvec_int(LuaInterface::IDs, "IDs");
  LuaInterface::ToLua::stdvec_stdstring(LuaInterface::scripts, "Scripts");
  LuaInterface::ToLua::stdvec_vec3(LuaInterface::positions, "Positions");
  LuaInterface::ToLua::stdvec_vec3(LuaInterface::velocities, "Velocities");
}

void LuaInterface::execute(void)
{
	int res = lua_pcall(*L, 0, LUA_MULTRET, 0);
  if (res != LUA_OK ) {
		std::cout << "Error Executing " << res << " " << lua_tostring(*L, 1) << std::endl;	
	}
}

void LuaInterface::retrieveVectors(void)
{
  LuaInterface::ToCPP::stdvec_vec3(&LuaInterface::positions, "Positions");
  LuaInterface::ToCPP::stdvec_vec3(&LuaInterface::velocities, "Velocities");
  
}

