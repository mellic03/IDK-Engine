#include <stdio.h>

#include "navmesh.h"

bool isSameNode(Node node, glm::vec3 pos)
{
  return node.position == pos;
}


int NavMesh::indexOfNode(glm::vec3 pos)
{
  for (int i=0; i<this->nodes.size(); i++)
    if (this->nodes[i].position == pos)
      return i;

  return -1;
}


void Node::removeDuplicateNeighbours(void)
{
  std::vector<int> deduped_neighbours;

  for (int i=0; i<this->neighbours.size(); i++)
  {
    bool is_duplicate = false;

    for (int j=0; j<i; j++)
      if (this->neighbours[i] == this->neighbours[j])
        is_duplicate = true;
  
    if (is_duplicate == false)
      deduped_neighbours.push_back(this->neighbours[i]);
  }

  this->neighbours = deduped_neighbours;
}

void Node::reset(void)
{
  this->previous = 0;
  this->visited = false;
  this->tent_dist = INFINITY;
}

int NavMesh::unvisitedNeighbours(Node *node)
{
  int unvisited_neighbours = 0;

  for (int i=0; i<node->neighbours.size(); i++)
  {
    Node *neighbour = &this->nodes[node->neighbours[i]];
    if (neighbour->visited == false)
      unvisited_neighbours += 1;
  }

  return unvisited_neighbours;
}

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
  int posindices[3];
  int posindex0, posindex1, posindex2, dummy;

  char buffer[64];
  while (fgets(buffer, 64, fh) != NULL)
  {
    if (sscanf(buffer, "v %f %f %f", &x, &y, &z))
    {
      positions.push_back(glm::vec3(x, y, z));
      this->nodes.push_back(Node());
      this->nodes[this->nodes.size()-1].position = glm::vec3(x, y, z);
    }

    if (sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &posindices[0], &dummy, &dummy, &posindices[1], &dummy, &dummy, &posindices[2], &dummy, &dummy)
    )
    {
      glm::vec3 pos0 = positions[posindices[0] - 1];
      glm::vec3 pos1 = positions[posindices[1] - 1];
      glm::vec3 pos2 = positions[posindices[2] - 1];

      // Add each node as neighbour
      //--------------------------------------
      int index0 = this->indexOfNode(pos0);
      int index1 = this->indexOfNode(pos1);
      int index2 = this->indexOfNode(pos2);

      this->nodes[index0].neighbours.push_back(index1);
      this->nodes[index0].neighbours.push_back(index2);
      
      this->nodes[index1].neighbours.push_back(index0);
      this->nodes[index1].neighbours.push_back(index2);

      this->nodes[index2].neighbours.push_back(index0);
      this->nodes[index2].neighbours.push_back(index1);
      //--------------------------------------
    }
  }
  for (int i=0; i<this->nodes.size(); i++)
    this->nodes[i].id = i;

  for (int i=0; i<this->nodes.size(); i++)
  {
    this->nodes[i].removeDuplicateNeighbours();

    // printf("Node: %d, neighbours: ", i);
    // for (int j=0; j<this->nodes[i].neighbours.size(); j++)
    //   printf("%d ", this->nodes[i].neighbours[j]);
    // printf("\n");
  }

}


Node *NavMesh::closestNode(glm::vec3 pos)
{
  int index_of_nearest = 0;
  float shortest_distance = glm::distance(this->nodes[0].position, pos);

  for (int i=0; i<this->nodes.size(); i++)
  {
    float dist = glm::distance(this->nodes[i].position, pos);
    if (dist < shortest_distance)
    {
      shortest_distance = dist;
      index_of_nearest = i;
    }
  }

  return &this->nodes[index_of_nearest];
}


std::vector<glm::vec3> NavMesh::path(glm::vec3 from, glm::vec3 to)
{

  Node *starting_node = this->closestNode(from);
  Node *ending_node = this->closestNode(to);

  printf("start: node %d, (%f, %f, %f)\n", starting_node->id, starting_node->position.x, starting_node->position.y, starting_node->position.z);
  printf("end: node %d, (%f, %f, %f)\n",     ending_node->id,   ending_node->position.x,   ending_node->position.y,   ending_node->position.z);


  for (int i=0; i<this->nodes.size(); i++)
  {
    this->nodes[i].tent_dist = INFINITY;
    this->nodes[i].visited = false;
    this->nodes[i].previous = -1;
  }

  Node *current_node = starting_node;
  current_node->tent_dist = 0.0f;
  current_node->previous = -1;


  int nodes_visited = 0;
  
  while (nodes_visited < this->nodes.size())
  {
    if (current_node == ending_node)
    {
      // printf("ending node: %d\n", current_node->id);
      break;
    }

    for (int i=0; i<current_node->neighbours.size(); i++)
    {
      Node *neighbour = &this->nodes[current_node->neighbours[i]];

      float tent_dist = current_node->tent_dist + current_node->distanceTo(neighbour);

      if (neighbour->visited == false)
      {
        if (tent_dist < neighbour->tent_dist)
        {
          neighbour->tent_dist = tent_dist;
          neighbour->previous = this->indexOfNode(current_node->position);
        }
      }
    }

    current_node->visited = true;
    nodes_visited += 1;
    // printf("current node: %d\n", current_node->id);

    // Find node with smallest tent_dist in ENTIRE graph
    float smallest_tent = INFINITY;
    int indexof_next_node = -1;
    for (int i=0; i<this->nodes.size(); i++)
    {
      if (this->nodes[i].visited == false)
      {
        if (this->nodes[i].tent_dist < smallest_tent)
        {
          smallest_tent = this->nodes[i].tent_dist;
          indexof_next_node = i; 
        }
      }
    }

    current_node = &this->nodes[indexof_next_node];
  }

  // printf("nodes visited: %d\n", nodes_visited);


  std::vector<glm::vec3> navpath;

  while (current_node->previous != -1)
  {
    navpath.push_back(current_node->position);
    // printf("node: %d\n", current_node->id);
    current_node = &this->nodes[current_node->previous];
  }

  // for (int i=0; i<this->nodes.size(); i++)
  //   printf("node %d: %s\n", this->nodes[i].id, (this->nodes[i].visited) ? "visited" : "unvisited");

  return navpath;
}