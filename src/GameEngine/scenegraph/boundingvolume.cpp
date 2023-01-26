#include "boundingvolume.h"


static void swapNodes( BVNode *a , BVNode *b)
{
  BVNode *temp = a;
  a = b;
  b = temp;
}


// static bool A_inside_B(float distance, GameObject *A, GameObject *B)
// {
//   return (distance < B->bounding_sphere_radius - A->bounding_sphere_radius);
// }

// static bool A_outside_B(float distance, GameObject *A, GameObject *B)
// {
//   return (distance > B->bounding_sphere_radius + A->bounding_sphere_radius);
// }

// static bool A_overlaps_B(float distance, GameObject *A, GameObject *B)
// {
//   float min_radius = glm::min(A->bounding_sphere_radius, B->bounding_sphere_radius);
//   float max_radius = A->bounding_sphere_radius + B->bounding_sphere_radius - min_radius;

//   bool c1 = distance >= max_radius - min_radius;
//   bool c2 = distance <= max_radius + min_radius;

//   return c1 && c2;
// }



static void recursiveInsert(BVNode *node, GameObject *obj)
{
  glm::vec3 nodePos = node->objectptr->getCullingData()->bounding_sphere_pos;
  float nodeRadiusSQ  = node->objectptr->getCullingData()->bounding_sphere_radiusSQ;

  glm::vec3 objectptrPos = obj->getCullingData()->bounding_sphere_pos;
  float objectptrRadiusSQ  = obj->getCullingData()->bounding_sphere_radiusSQ;


  // If objects sphere is larger than node objects sphere,
  // it is definitely not inside, so check if the 
  // nodes sphere is inside the objects sphere.
  float distSQ = glm::distance2(objectptrPos, nodePos);

  if (objectptrRadiusSQ >= nodeRadiusSQ)
  {
    if (distSQ < objectptrRadiusSQ - nodeRadiusSQ)
    {
      // Replace node with new node(obj)
      // old node becomes left node of new node(obj);

      BVNode *temp = new BVNode();
      temp->objectptr = node->objectptr;
      temp->left = node->left;
      temp->right = node->right;

      node->objectptr = obj;
      node->left = temp;
      node->right = nullptr;
    }

    else
    {
      if (node->right == nullptr)
        node->right = new BVNode(obj);
      else
        recursiveInsert(node->right, obj);
    }

    return;
  }
   

  // If small is less than big.radius - small.radius away from big.center, small is inside big.
  // Already established that objectptrs sphere is smaller than node objects sphere.
  if (distSQ < nodeRadiusSQ - objectptrRadiusSQ)
  {
    if (node->left == nullptr)
      node->left = new BVNode(obj);
    else
      recursiveInsert(node->left, obj);

    return;
  }
  else
  {
    if (node->right == nullptr)
      node->right = new BVNode(obj);
    else
      recursiveInsert(node->right, obj);
  }
}



static void printBVTree(const std::string &prefix, const BVNode *node, bool is_left)
{
  if (node == nullptr)
  {
    std::cout << prefix << (is_left ? "├─" : "└─" ) << "D" << std::endl;
    return;
  }
  std::cout << prefix;
  std::cout << (is_left ? "├─" : "└─" );
  std::cout << "(" << node->objectptr->getName() << ")" << std::endl;

  printBVTree( prefix + (is_left ? "│   " : "    "), node->left, true);
  printBVTree( prefix + (is_left ? "│   " : "    "), node->right, false);
}


void BVTree::print()
{
  printf("\n\n");
  if (this->root != nullptr)
    printBVTree(std::string(""), this->root, 0);
  printf("\n\n");
}



void BVTree::insert(GameObject *obj)
{
  if (this->root == nullptr)
    this->root = new BVNode(obj);

  else
    recursiveInsert(this->root, obj);
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


static void recurseVisible(BVNode *node, std::list<GameObject *> visible_objects[])
{
  if (node->left != nullptr)
    recurseVisible(node->left, visible_objects);

  if (node->right != nullptr)
    recurseVisible(node->right, visible_objects);

  visible_objects[node->objectptr->getObjectType()].push_back(node->objectptr);
}


static void weewoo(BVNode *node, Frustum *frustum, std::list<GameObject *> visible_objects[])
{
  if (node->right != nullptr)
    weewoo(node->right, frustum, visible_objects);

  glm::vec3 nodePos = node->objectptr->getCullingData()->bounding_sphere_pos;
  float nodeRadius  = node->objectptr->getCullingData()->bounding_sphere_radius;


  if (frustum->entirelyVisible(nodePos, nodeRadius))
  {
    visible_objects[node->objectptr->getObjectType()].push_back(node->objectptr);
  
    if (node->left != nullptr)
      recurseVisible(node->left, visible_objects);
  }

  else if (frustum->partiallyVisible(nodePos, nodeRadius))
  {
    visible_objects[node->objectptr->getObjectType()].push_back(node->objectptr);
    
    if (node->left != nullptr)
      weewoo(node->left, frustum, visible_objects);
  }
}


void BVTree::cullObjects(Frustum *frustum, std::list<GameObject *> visible_objects[])
{
  if (this->root != nullptr)
    weewoo(this->root, frustum, visible_objects);

  int visible = 0;

  for (int i=0; i<GAMEOBJECT_NUM_TYPES; i++)
    visible += visible_objects[i].size();
}


static void recurse_addObjectPtrsToList(BVNode *node, std::list<GameObject *> *objectptr_list)
{
  if (node == nullptr)
    return;
  
  objectptr_list->push_back(node->objectptr);

  recurse_addObjectPtrsToList(node->left, objectptr_list);
  recurse_addObjectPtrsToList(node->right, objectptr_list);
}



static BVNode *recurse_getPotentialColliders(BVNode *node, BVNode *localRoot, bool *found, GameObject *object, std::list<GameObject *> *potential_colliders)
{
  static int left_traversals = 0;

  if (node == nullptr)
    return nullptr;


  // Traverse tree until object is found.

  // If the object is found, then go back up the tree until
  // reaching the first node where we went left
  // and recursively add all children of that node to the list.
  
  if (node->objectptr->getID() != object->getID())
  {
    BVNode *child = nullptr;
    if (node->left != nullptr)
    {
      left_traversals += 1;
      child = recurse_getPotentialColliders(node->left, node, found, object, potential_colliders);
      left_traversals -= 1;

      if (child != nullptr && left_traversals == 0)
      {
        *found = true;
        return node;
      }

      else if (child != nullptr)
        return child;
    }
    
    if (node->right != nullptr)
    {
      child = recurse_getPotentialColliders(node->right, localRoot, found, object, potential_colliders);
      if (child != nullptr)
        return child;
    }

    return nullptr;
  }

  else
  {
    return node;
  }

}


void BVTree::getPotentialColliders(GameObject *object, std::list<GameObject *> *potential_colliders)
{
  // if (this->root != nullptr)
  // {
  //   BVNode *node = this->root;
  //   while (node != nullptr)
  //   {
  //     float dist = glm::distance(node->objectptr->m_model->bounding_sphere_pos, object->m_model->bounding_sphere_pos);

  //     if (dist - object->m_model->bounding_sphere_radius < node->objectptr->m_model->bounding_sphere_radius)
  //     {
  //       potential_colliders->push_back(node->objectptr);
  //       recurse_addObjectPtrsToList(node->left, potential_colliders);
  //     }

  //     node = node->right;
  //   }

  // }
}
