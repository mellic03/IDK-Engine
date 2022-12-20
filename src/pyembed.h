#pragma once

#include <string>

#include "include/glm/glm.hpp"


#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>


namespace c2p {

  void vec3(glm::vec3 vec, PyObject *locals);

};

namespace p2c {

  glm::vec3 vec3(PyObject *locals);

};



