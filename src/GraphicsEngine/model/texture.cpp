#include "texture.h"

bool Texture::load(std::string filepath, bool useSRGB)
{
  stbi_set_flip_vertically_on_load(1);
  int width, height, bpp;
  unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
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
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, this->m_texture_obj);
}

void Texture::unbind(GLenum texture_unit)
{
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, 0);
}