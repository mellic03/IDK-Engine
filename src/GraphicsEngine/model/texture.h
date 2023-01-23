#pragma once

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"

#include "../../include/stb_image/stb_image.h"

class Texture {

  public:
    std::string m_dae_id;
    GLenum m_texture_obj;

    Texture() {};
    Texture(std::string filepath, std::string dae_id, bool useSRGB = false)
    {
      this->m_dae_id = dae_id;
      this->load(filepath, useSRGB);
    };

    bool load(std::string filepath, bool useSRGB);
    void bind(GLenum texture_unit);
    void unbind(GLenum texture_unit);
};