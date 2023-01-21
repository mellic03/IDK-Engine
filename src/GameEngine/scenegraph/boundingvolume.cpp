#include "boundingvolume.h"



static void recursiveInsert(BVNode *node, GameObject *objectptr)
{
  if (node == nullptr)
  {
    node = new BVNode(objectptr);
    return;
  }

  glm::vec3 nodePos = node->objectptr->m_model->bounding_sphere_pos;
  nodePos = node->objectptr->getTransform()->getModelMatrix() * glm::vec4(nodePos.x, nodePos.y, nodePos.z, 1.0f);
  float nodeRadius  = node->objectptr->m_model->bounding_sphere_radius;

  glm::vec3 objectptrPos = objectptr->m_model->bounding_sphere_pos;
  objectptrPos = objectptr->getTransform()->getModelMatrix() * glm::vec4(objectptrPos.x, objectptrPos.y, objectptrPos.z, 1.0f);
  float objectptrRadius  = objectptr->m_model->bounding_sphere_radius;


  // If objects sphere is larger than node objects sphere,
  // it is definitely not inside, so insert right.
  if (objectptrRadius >= nodeRadius)
  { 
    // printf("%s -> %s\n", node->objectptr->getName().c_str(), objectptr->getName().c_str());
    
    if (node->right == nullptr)
      node->right = new BVNode(objectptr);
    else
      recursiveInsert(node->right, objectptr);
    
    return;
  }
   

  // If small is less than big.radius - small.radius away from big.center, small is inside big.
  // Already established that objectptrs sphere is smaller than node objects sphere.
  float dist = glm::distance(objectptrPos, nodePos);

  if (dist < nodeRadius - objectptrRadius)
  {
    // printf("%s <- %s\n", objectptr->getName().c_str(),  node->objectptr->getName().c_str());
    
    if (node->left == nullptr)
      node->left = new BVNode(objectptr);
    else
      recursiveInsert(node->left, objectptr);

    return;
  }
  else
  {
    // printf("%s -> %s\n", node->objectptr->getName().c_str(), objectptr->getName().c_str());

    if (node->right == nullptr)
      node->right = new BVNode(objectptr);
    else
      recursiveInsert(node->right, objectptr);
  }
}


void BVTree::insert(GameObject *objectptr)
{
  if (this->root == nullptr)
  {
    // printf("root: %s\n", objectptr->getName().c_str());
    this->root = new BVNode(objectptr);
    return;
  }

  // printf("\nInserting: %s\n", objectptr->getName().c_str());

  recursiveInsert(this->root, objectptr);

  // printf("DONE!\n\n\n\n");
}




static void clear(BVNode *node)
{
  if (node == nullptr)
    return;
  
  clear(node->left);
  delete node->left;

  clear(node->right);
  delete node->right;
}


void BVTree::clearTree()
{
  clear(this->root);
  this->root = nullptr;
}