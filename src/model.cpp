#include <stdio.h>

#include "model.h"

Model::Model()
{
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glGenBuffers(1, &this->IBO);
};

void Model::load(const char *filepath)
{
  FILE *fh = fopen(filepath, "r");
  if (fh == NULL)
  {
    printf("Error opening \"%s\"", filepath);
    return;
  }

  int p_count = 0, n_count= 0;
 
  char buffer[64];
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'v' && buffer[1] == ' ')
      p_count += 1;
    else if (buffer[0] == 'v' && buffer[1] == 'n')
      n_count += 1;
  }
  rewind(fh);

  float *positions = (float *)malloc(3 * p_count * sizeof(float));
  float *normals   = (float *)malloc(3 * n_count * sizeof(float));

  p_count = 0, n_count = 0;

  float x, y, z;

  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'v' && buffer[1] == ' ')
    {
      sscanf(buffer, "v %f %f %f", &x, &y, &z);
      positions[p_count+0] = x;
      positions[p_count+1] = y;
      positions[p_count+2] = z;
      p_count += 3;
    }

    else if (buffer[0] == 'v' && buffer[1] == 'n')
    {
      sscanf(buffer, "vn %f %f %f", &x, &y, &z);
      normals[n_count+0] = x;
      normals[n_count+1] = y;
      normals[n_count+2] = z;
      n_count += 3;
    }
  }

  // There now exists two arrays: positions and normals.
  // positions = {x, y, z, x, y, z, x, y, z, ... }
  // normals   = {x, y, z, x, y, z, x, y, z, ... }

}


void Model::draw(void)
{

  float vertices2[] = {
    -0.5, 0,   0,
    0,   0.5, 0,
    0.5, 0,   0,
    
    -0.6, 0.5, 0,
    -0.4, 0.5, 0
  };

  GLuint indices2[] = {
    0, 1, 2,
    0, 3, 4
  };

  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, 5*ELEMENTS_PER_ATTRIB*sizeof(float), vertices2, GL_STATIC_DRAW);

  // Positions
  glVertexAttribPointer(0, ELEMENTS_PER_ATTRIB, GL_FLOAT, GL_FALSE, ELEMENTS_PER_VERTEX*sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indices2, GL_STATIC_DRAW);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

  // for (int i=0; i<this->vertex_count; i++)
  // {
  //   this->vertices[i] -= 0.00001;
  // }

  // glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2)/3);
}