#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>
#include <GL/glu.h>


#define ELEMENTS_PER_ATTRIB 3
#define ATTRIBS_PER_VERTEX 1
#define ELEMENTS_PER_VERTEX ((ELEMENTS_PER_ATTRIB)*(ATTRIBS_PER_VERTEX))

/*
  { px, py, pz,  nx, ny, nz,    px, py, pz,  nx, ny, nz, ... }
*/


class Model {

  private:
    GLuint VAO, VBO, IBO;

  public:

    Model();

    void load(const char *filepath);
    void draw(void);

};