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


void LuaInterface::ToLua::stdvec_int(std::vector<int> vecOfInt, std::string name)
{
  LuaTTable table;

  for (int i=0; i<vecOfInt.size(); i++)
    table.setValue(Table::Key(i + 1), std::make_shared<LuaTNumber>(vecOfInt[i] + 1));

  table.PushGlobal(*LuaInterface::L, name);

  table_references.push_back( { name, table } );
}


void LuaInterface::ToLua::stdvec_stdstring(std::vector<std::string> vecOfString, std::string name)
{
  LuaTTable table;

  for (int i=0; i<vecOfString.size(); i++)
    table.setValue(Table::Key(i + 1), std::make_shared<LuaTString>(vecOfString[i]));

  table.PushGlobal(*LuaInterface::L, name);

  table_references.push_back( { name, table } );
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

    // if (name == "Velocities")
    //   std::cout << vec.ToString() << std::endl;

    *element = glm::vec3(
      (*(LuaTNumber *)&vec.getValue(Table::Key("x"))).getValue(),
      (*(LuaTNumber *)&vec.getValue(Table::Key("y"))).getValue(),
      (*(LuaTNumber *)&vec.getValue(Table::Key("z"))).getValue()
    );
    
    element = std::next(element);
  }
}


void LuaInterface::ToLua::gameobject(GameObject *object, int objectID)
{
  LuaInterface::IDs.push_back(objectID);
  LuaInterface::scripts.push_back(object->m_script_name);
  LuaInterface::positions.push_back(*object->getPos());
  LuaInterface::velocities.push_back(*object->getVel());
}



void LuaInterface::ToCPP::gameobject(GameObject *object, int objectID)
{
  *object->getPos() = LuaInterface::positions[objectID];
  *object->getVel() = LuaInterface::velocities[objectID];
}
