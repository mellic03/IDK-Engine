#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"


class Texture {

  public:
    std::string m_filename;
    GLenum m_texture_obj;

    Texture() {};

    bool load(const char *filepath, bool useSRGB);
    void bind(GLenum texture_unit);
    void unbind(GLenum texture_unit);
};