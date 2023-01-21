#pragma once

#include "../gameobject/gameobject.h"


/**
 * Objects whose bounding spheres are contained entirely within anothers
 * are placed to the left of a node.
 * 
*/






struct BVNode {
  GameObject *objectptr = nullptr;
  BVNode *left = nullptr, *right = nullptr;

  BVNode() { };
  BVNode(GameObject *objectptr) { this->objectptr = objectptr; };
};



class BVTree {

  private:
    BVNode *root = nullptr;

  public:
    
    BVTree() { };

    void clearTree();
    void insert(GameObject *objectptr);



};




