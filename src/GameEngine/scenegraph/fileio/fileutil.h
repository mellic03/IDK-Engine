#include "../scenegraph.h"

#define ToTextLaziness (std::ofstream &stream, GameObject *object, std::string indentation)
#define FromTextLaziness (std::ifstream &stream, GameObject *object)

namespace FileUtil {

  namespace ToText {
    
    void objectheader(std::ofstream &stream, GameObject *object, std::string indentation);
    void transform(std::ofstream &stream, Transform *t, std::string indentation);
    void scripts(std::ofstream &stream, GameObject *object, std::string indentation);
    void pointlight(std::ofstream &stream, PointLight *pointlight, std::string indentation);
    void physics(std::ofstream &stream, GameObject *object, std::string indentation);

    void spherecollider   ToTextLaziness;
    void capsulecollider  ToTextLaziness;
    void meshcollider     ToTextLaziness;
    void navigation       ToTextLaziness;

  };


  namespace FromText {
    
    GameObjectHeader objectheader(std::ifstream &stream, GameObject *object);
    void transform(std::ifstream &stream, Transform *t);
    void scripts(std::ifstream &stream, GameObject *object);
    void pointlight(std::ifstream &stream, PointLight *pointlight);

    void spherecollider   FromTextLaziness;
    void capsulecollider  FromTextLaziness;
    void navigation       FromTextLaziness;
  };



  namespace ToBinary {
    
  };


  namespace FromBinary {

  };


};

