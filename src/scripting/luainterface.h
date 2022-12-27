#pragma once

#include <iostream>
#include <lua.hpp>
#include <LuaCpp.hpp>

#include "../GameEngine/GameEngine.h"
#include "../include/glm/glm.hpp"

using namespace LuaCpp;
using namespace LuaCpp::Registry;
using namespace LuaCpp::Engine;



namespace LuaInterface {

  // Data Structures
  //--------------------------------------------------
  struct TableReference {
    std::string name;
    LuaTTable table;
  };
  //--------------------------------------------------

  // Variables
  //--------------------------------------------------
  extern LuaContext context;
  extern std::unique_ptr<LuaState> L;

  extern std::vector<TableReference> table_references;



  extern std::vector<int> IDs;
  extern std::vector<std::string> scripts;
  extern std::vector<glm::vec3> positions;
  extern std::vector<glm::vec3> velocities;
  //--------------------------------------------------




  // Methods
  //--------------------------------------------------
  void compile(void);
  void begin(void);
  void sendVectors(void);
  void execute(void);
  void retrieveVectors(void);
  LuaTTable *tablePtr(std::string name);
  //--------------------------------------------------



  // Namespaces
  //--------------------------------------------------
  namespace ToLua {

    template <typename T> void number(T value, std::string name)
    {
      LuaTNumber val(value);
      val.PushGlobal(*LuaInterface::L, name);
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
      LuaTNumber val;
      val.PopGlobal(*LuaInterface::L);
      return (T)val.getValue();
    }

    void stdvec_vec3(std::vector<glm::vec3> *vec, std::string name);
    void gameobject(GameObject *object, int objectID);
  };
  //--------------------------------------------------

};

