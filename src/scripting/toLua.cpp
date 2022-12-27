#include "luainterface.h"


void LuaInterface::ToLua::array_bool(bool arrOfBool[], int size, std::string name)
{
  LuaTTable table;

  for (int i=0; i<size; i++)
    table.setValue(Table::Key(i + 1), std::make_shared<LuaTBoolean>(arrOfBool[i]));

  table.PushGlobal(*LuaInterface::L, name);

  table_references.push_back( { name, table } );
}


void LuaInterface::ToLua::stdvec_int(std::vector<int> vecOfInt, std::string name)
{
  LuaTTable table;

  for (int i=0; i<vecOfInt.size(); i++)
    table.setValue(Table::Key(i + 1), std::make_shared<LuaTNumber>(vecOfInt[i] + 1));

  table.PushGlobal(*LuaInterface::L, name);

  table_references.push_back( { name, table } );
}


void LuaInterface::ToLua::stdvec_bool(std::vector<bool> vecOfBool, std::string name)
{
  LuaTTable table;

  for (int i=0; i<vecOfBool.size(); i++)
    table.setValue(Table::Key(i + 1), std::make_shared<LuaTBoolean>(vecOfBool[i]));

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


void LuaInterface::ToLua::gameobject(GameObject *object, int objectID)
{
  for (auto component: object->script_components)
  {
    if (component.script_name != "LuaScripting/scripts/default");
    LuaInterface::IDs.push_back(objectID);
    LuaInterface::scripts.push_back(component.script_name);
  }

  LuaInterface::positions.push_back(object->getTransform()->getPos_worldspace());
  LuaInterface::velocities.push_back(*object->getTransform()->getVel());
}


void LuaInterface::ToLua::keylog(KeyLog keylog, std::string name)
{
  LuaInterface::ToLua::array_bool(keylog.keys_pressed, SDL_NUM_SCANCODES, name);
}

