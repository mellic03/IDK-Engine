#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <assert.h>

#define GLClearError() \
{ \
  while (glGetError() != GL_NO_ERROR); \
}

#ifdef COOMDEBUG
  #define GLCALL(glFunc) \
  { \
    GLClearError(); \
    glFunc; \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) \
    { \
      printf("OpenGL Error: %s\n", gluErrorString(err)); \
      fflush(stdout); \
      assert(err != GL_NO_ERROR); \
    } \
  }
#else
  #define GLCALL(glFunc) \
  { \
    glFunc; \
  }
#endif