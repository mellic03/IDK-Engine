#pragma once

#include <iostream>
#include <lua.hpp>

#include "../GameEngine/GameEngine.h"
#include "../scene/scene.h"
#include "../include/glm/glm.hpp"


void register_luaLibrary();


namespace LuaInterface {

  // Variables
  //--------------------------------------------------
  extern lua_State *L;
  //--------------------------------------------------


  // Methods
  //--------------------------------------------------
  void compile(void);
  void execute(void);
  //--------------------------------------------------


  // Namespaces
  //--------------------------------------------------
  namespace ToLua {

    template <typename T> void number(T value, std::string name)
    {
      lua_pushnumber(LuaInterface::L, value);
      lua_setglobal(LuaInterface::L, name.c_str());
    }

    void array_bool(bool arrOfBool[], int size, std::string name);

    void stdvec_int(std::vector<int> vecOfInt, std::string name);
    void stdvec_bool(std::vector<bool> vecOfBool, std::string name);
    void stdvec_stdstring(std::vector<std::string> vecOfString, std::string name);

    void stdvec_vec3(std::vector<glm::vec3> vecOfVec3, std::string name);
    void gameobject(GameObject *object, int objectID);
    void keylog(KeyLog keylog, std::string name);
  };
  //--------------------------------------------------

};

