#pragma once

#include <iostream>
#include <lua.hpp>

#include "../GameEngine/GameEngine.h"
#include "../include/glm/glm.hpp"


namespace LuaInterface {

  // Data Structures
  //--------------------------------------------------


  //--------------------------------------------------

  // Variables
  //--------------------------------------------------
  extern lua_State *L;
  // extern std::vector<int> IDs;
  // extern std::vector<std::string> scripts;
  // extern std::vector<glm::vec3> positions;
  // extern std::vector<glm::vec3> velocities;
  //--------------------------------------------------




  // Methods
  //--------------------------------------------------
  void compile(void);
  void begin(void);
  void execute(void);
  //--------------------------------------------------



  // Namespaces
  //--------------------------------------------------
  namespace ToLua {

    template <typename T> void number(T value, std::string name)
    {

    }

    void array_bool(bool arrOfBool[], int size, std::string name);

    void stdvec_int(std::vector<int> vecOfInt, std::string name);
    void stdvec_bool(std::vector<bool> vecOfBool, std::string name);
    void stdvec_stdstring(std::vector<std::string> vecOfString, std::string name);

    void stdvec_vec3(std::vector<glm::vec3> vecOfVec3, std::string name);
    void gameobject(GameObject *object, int objectID);
    void keylog(KeyLog keylog, std::string name);
  };


  namespace ToCPP {

    template <typename T> void number(T value, std::string name)
    {

    }

    void stdvec_vec3(std::vector<glm::vec3> *vec, std::string name);
    void gameobject(GameObject *object, int objectID);
  };
  //--------------------------------------------------

};

