#include "texture.h"
#include "../include/stb_image/stb_image.h"

bool Texture::load(const char *filepath)
{
  this->m_filename = std::string(filepath);

  // stbi_set_flip_vertically_on_load(1);
  int width, height, bpp;
  unsigned char *data = stbi_load(this->m_filename.c_str(), &width, &height, &bpp, STBI_rgb);
  if (!data)
  {
    printf("Error loading texture from \"%s\" --%s\n", this->m_filename.c_str(), stbi_failure_reason());
    exit(1);
  }
  // printf("Width: %d, height: %d, bpp: %d\n", width, height, bpp);

  glGenTextures(1, &this->m_texture_obj);
  glBindTexture(GL_TEXTURE_2D, this->m_texture_obj);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  return true;
}

void Texture::bind(GLenum texture_unit)
{
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, this->m_texture_obj);
}