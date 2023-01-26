#pragma once

#include <list>

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

    BVNode *getRootNode() { return this->root; };

    void clearTree();
    void insert(GameObject *objectptr);
    void cullObjects(Frustum *frustum, std::list<GameObject *> visible_objects[]);
    
    
    void getPotentialColliders(GameObject *object, std::list<GameObject *> *potential_colliders);

    void print();



};




