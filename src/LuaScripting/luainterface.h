#pragma once

#include <iostream>
#include <lua.hpp>
#include <LuaCpp.hpp>

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
  //--------------------------------------------------




  // Methods
  //--------------------------------------------------
  void compile(void);
  void begin(void);
  void execute(void);
  LuaTTable *tablePtr(std::string name);
  //--------------------------------------------------



  // Namespaces
  //--------------------------------------------------
  namespace ToLua {
    void stdvec_vec3(std::vector<glm::vec3> vecOfVec3, std::string name);
  };


  namespace ToCPP {
    void stdvec_vec3(std::vector<glm::vec3> *vec, std::string name);
  };
  //--------------------------------------------------

};

