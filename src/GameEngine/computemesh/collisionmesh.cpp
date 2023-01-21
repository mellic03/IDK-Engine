#include "collisionmesh.h"

void CollisionMesh::computeBoundingSphere(void)
{
  // glm::vec3 total = glm::vec3(0.0f);
  // for (auto &vertex: this->m_vertices)
  //   total += vertex.position;

  // glm::vec3 avg = total / (float)this->m_vertices.size();
  // this->bounding_sphere_pos = avg;


  // for (auto &vertex: this->m_vertices)
  // {
  //   float distSQ = glm::distance2(avg, vertex.position);
  //   if (distSQ > this->bounding_sphere_radiusSQ)
  //     this->bounding_sphere_radiusSQ = distSQ;
  // }
}


void CollisionMesh::load(std::string filepath)
{
  FILE *fh = fopen(filepath.c_str(), "r");
  
  if (fh == NULL)
  {
    printf("Error opening file: \"%s\"\n", filepath.c_str());
    exit(1);
  }

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;

  int p1, n1, p2, n2, p3, n3, dummy;
  float x, y, z;

  char buffer[64];
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (sscanf(buffer, "v %f %f %f", &x, &y, &z))
      positions.push_back(glm::vec3(x, y, z));

    if (sscanf(buffer, "vn %f %f %f", &x, &y, &z))
      normals.push_back(glm::vec3(x, y, z));

    if (sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &p1, &dummy, &n1, &p2, &dummy, &n2, &p3, &dummy, &n3))
    {
      Vertex v1, v2, v3;

      v1.position = positions[p1 - 1];
      v2.position = positions[p2 - 1];
      v3.position = positions[p3 - 1];

      v1.normal = normals[n1 - 1];
      v2.normal = normals[n2 - 1];
      v3.normal = normals[n3 - 1];

      this->m_vertices.push_back(v1);
      this->m_vertices.push_back(v2);
      this->m_vertices.push_back(v3);
    }
  }

  this->computeBoundingSphere();

}