#include "../../glcall.h"
#include "../../transform/transform.h"


enum PrimitiveType {
  PRIMITIVE_SPHERE
};

struct SpherePrimitive {
  Model model;
  float radius;
};

struct BoxPrimitive {
  Model model;
};

struct Primitives {
  
  SpherePrimitive sphere_primitive;
  BoxPrimitive box_primitive;
};


