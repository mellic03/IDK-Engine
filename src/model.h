#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>
#include <GL/glu.h>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#define ELEMENTS_PER_ATTRIB 3
#define ATTRIBS_PER_VERTEX 1
#define ELEMENTS_PER_VERTEX ((ELEMENTS_PER_ATTRIB)*(ATTRIBS_PER_VERTEX))

class Model {

  private:
    GLuint VAO, VBO, IBO;
    int num_positions, num_indices;
    float *positions;
    int *indices;


  public:
    Model();

    void load(const char *filepath);
    void draw(void);

    void rot_x(int theta);
    void rot_y(int theta);
    void rot_z(int theta);

};