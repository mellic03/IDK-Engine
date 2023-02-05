#include "texture.h"

bool Texture::load(std::string filepath, bool useSRGB)
{
  stbi_set_flip_vertically_on_load(1);
  int width, height, bpp;
  

  unsigned char *data;

  // If file exists, load it.
  // Otherwise, load the default -xxxx.png
  //------------------------------------------------------------
  std::ifstream stream(filepath);

  if (stream.good())
  {
    // std::cout << "successfully loaded file: " << filepath << "\n";
    data = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
  }

  else
  {
    std::string temp_filepath = filepath;
    size_t pos = temp_filepath.find("-");
    
    // Diffuse map missing
    if (pos == std::string::npos)
    {
      data = stbi_load("src/GraphicsEngine/model/defaultimages/default.png", &width, &height, &bpp, STBI_rgb_alpha);
    }

    // -specular, -normal or -emission map missing
    else
    {
      // std::cout << "could not find file: " << filepath << "\n";
      temp_filepath.erase(0, pos);
      filepath = "src/GraphicsEngine/model/defaultimages/default" + temp_filepath;
      // printf("New filepath: %s\n", filepath.c_str());
      data = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    }
  }
  //------------------------------------------------------------


  if (!data)
  {
    printf("Error loading texture from \"%s\" --%s\n", filepath.c_str(), stbi_failure_reason());
    exit(1);
  }


  glGenTextures(1, &this->m_texture_obj);
  glBindTexture(GL_TEXTURE_2D, this->m_texture_obj);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  if (useSRGB)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  return true;
}


void Texture::bind(GLenum texture_unit)
{
  GLCALL( glActiveTexture(texture_unit) );
  GLCALL( glBindTexture(GL_TEXTURE_2D, this->m_texture_obj) );
}


void Texture::unbind(GLenum texture_unit)
{
  GLCALL( glActiveTexture(texture_unit) );
  GLCALL( glBindTexture(GL_TEXTURE_2D, 0) );
}