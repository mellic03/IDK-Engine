#pragma once

#include <string>
#include <vector>

#include "../include/glm/glm.hpp"

struct Node {

  glm::vec3 position;

  std::vector<Node *> neighbours;
  std::vector<float> distances;   // distance to each neighbour, distances[i] is the distance to neighbours[i].
};

class NavMesh {

  private:

  public:
    std::vector<Node> nodes;

    NavMesh() { };
    void load(std::string filepath);

    std::vector<Node> getPathTo(glm::vec3 point);
  
};