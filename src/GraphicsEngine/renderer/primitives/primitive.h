#include "../../glcall.h"
#include "../../../transform.h"


enum PrimitiveType {
  PRIMITIVE_SPHERE
};

struct SpherePrimitive {
  Model model;
  float radius;
};


struct Primitives {
  
  SpherePrimitive sphere_primitive;

};


