#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"


class LightSource {

  private:
    GLuint VAO, VBO;

  public:
    LightSource() { };
    void set_object_color(glm::vec4 color);
    void set_emission_color(glm::vec4 color);

};