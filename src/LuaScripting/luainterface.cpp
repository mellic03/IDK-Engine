#include "luainterface.h"


LuaContext LuaInterface::context;
std::unique_ptr<LuaState> LuaInterface::L;

std::vector<LuaInterface::TableReference> LuaInterface::table_references;



LuaTTable *LuaInterface::tablePtr(std::string name)
{
  for (auto &ref: table_references)
    if (ref.name == name)
      return &ref.table;
  return nullptr;
}

void LuaInterface::compile(void)
{
	LuaInterface::context.CompileFile("main", "./src/LuaScripting/main.lua");
}

void LuaInterface::begin(void)
{
  LuaInterface::L = LuaInterface::context.newStateFor("main");
}

void LuaInterface::execute(void)
{
	int res = lua_pcall(*L, 0, LUA_MULTRET, 0);
}





void LuaInterface::ToLua::stdvec_vec3(std::vector<glm::vec3> vecOfVec3, std::string name)
{
  LuaTTable table;

  for (int i=0; i<vecOfVec3.size(); i++)
  {
    LuaTTable vec;
    vec.setValue(Table::Key(1), std::make_shared<LuaTNumber>(vecOfVec3[i].x));
    vec.setValue(Table::Key(2), std::make_shared<LuaTNumber>(vecOfVec3[i].y));
    vec.setValue(Table::Key(3), std::make_shared<LuaTNumber>(vecOfVec3[i].z));

    table.setValue(Table::Key(i + 1), std::make_shared<LuaTTable>(vec));
  }

  table.PushGlobal(*LuaInterface::L, name);

  table_references.push_back( { name, table } );
}


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

