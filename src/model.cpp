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

  int p_count = 0, n_count= 0, f_count = 0;
 
  char buffer[64];
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'v' && buffer[1] == ' ')
      p_count += 1;
    else if (buffer[0] == 'v' && buffer[1] == 'n')
      n_count += 1;
    else if (buffer[0] == 'f' && buffer[1] == ' ')
      f_count += 1;
  }
  rewind(fh);

  this->positions = (float *)malloc(3 * p_count * sizeof(float));
  float *normals   = (float *)malloc(3 * n_count * sizeof(float));
  this->indices = (int *)malloc(3 * f_count * sizeof(int));

  this->num_positions = p_count;
  this->num_indices = 3*f_count;

  p_count = 0, n_count = 0, f_count = 0;

  float x, y, z;
  int p1, t1, n1, p2, t2, n2, p3, t3, n3;

  while (fgets(buffer, 64, fh) != NULL)
  {
    if (buffer[0] == 'v' && buffer[1] == ' ')
    {
      sscanf(buffer, "v %f %f %f", &x, &y, &z);
      this->positions[p_count+0] = x;
      this->positions[p_count+1] = y;
      this->positions[p_count+2] = z;
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
  

    else if (buffer[0] == 'f' && buffer[1] == ' ')
    {
      sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);
      this->indices[f_count+0] = p1-1;
      this->indices[f_count+1] = p2-1;
      this->indices[f_count+2] = p3-1;
      f_count += 3;
    }
  }

  for (int i=0; i<this->num_indices; i++)
    printf("%d, ", this->indices[i]);
  printf("\n");


  // There now exists two arrays: positions and normals.
  // positions = {x, y, z, x, y, z, x, y, z, ... }
  // normals   = {x, y, z, x, y, z, x, y, z, ... }

}


void Model::draw(void)
{
  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO); 
  glBufferData(GL_ARRAY_BUFFER, this->num_positions * 3 * sizeof(float), this->positions, GL_STATIC_DRAW);

  // Positions
  glVertexAttribPointer(0, ELEMENTS_PER_ATTRIB, GL_FLOAT, GL_FALSE, ELEMENTS_PER_VERTEX*sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->num_indices * sizeof(GLuint), this->indices, GL_STATIC_DRAW);

  glDrawElements(GL_TRIANGLES, this->num_indices , GL_UNSIGNED_INT, (void *)0);
}



void Model::rot_x(int theta)
{

}

void Model::rot_y(int theta)
{

}

void Model::rot_z(int theta)
{

}
