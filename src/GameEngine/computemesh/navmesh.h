#pragma once

#include <string>
#include <vector>

#include "../../include/glm/glm.hpp"

class Node {

  public:
    int id = 0;
    glm::vec3 position = glm::vec3(0.0f);
    std::vector<int> neighbours;
    int previous = -1;

    bool visited = false;
    float tent_dist = INFINITY;


    Node() { };

    void reset(void);

    void removeDuplicateNeighbours(void);

    float distanceTo(Node *neighbour) { return glm::distance(this->position, neighbour->position); };

    void setVisited(int *visited_count)
    {
      if (this->visited == false)
      {
        this->visited = true;
        *visited_count += 1;
      }
    }

};

class NavMesh {


  public:
    std::vector<Node> nodes;

    NavMesh() { };

    /** Load a navmesh from an .obj file */
    void load(std::string filepath);

    int indexOfNode(glm::vec3 pos);
    Node *closestNode(glm::vec3 pos);
    int unvisitedNeighbours(Node *node);
    
    std::vector<glm::vec3> path(glm::vec3 from, glm::vec3 to);

};