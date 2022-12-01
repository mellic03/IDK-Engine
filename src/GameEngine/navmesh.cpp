#include <stdio.h>

#include "navmesh.h"



void NavMesh::load(std::string filepath)
{
  FILE *fh = fopen(filepath.c_str(), "r");
  
  if (fh == NULL)
  {
    printf("Error opening file: \"%s\"\n", filepath.c_str());
    exit(1);
  }

  std::vector<glm::vec3> positions;
  float x, y, z;
  int pos0, pos1, pos2, dummy;

  char buffer[64];
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (sscanf(buffer, "v %f %f %f", &x, &y, &z))
    {
      positions.push_back(glm::vec3(x, y, z));
      this->nodes.push_back(Node());
      this->nodes[this->nodes.size()-1].position = glm::vec3(x, y, z);
    }

    if (sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &pos0, &dummy, &dummy, &pos1, &dummy, &dummy, &pos2, &dummy, &dummy))
    {
      
    }
  }
}