#include "newboundingvolume.h"



static bool a_fits_in_b(GameObject* &a, GameObject* &b)
{
    float d = glm::distance2(a->spherecollider.pos, b->spherecollider.pos);
    
    if (d > a->spherecollider.radius && d > b->spherecollider.radius)
        return true;

    return false;    
}



void newBVTree::_insert(GameObject *objectptr, newBVNode* &node)
{
    
    // If object fits inside node.object.sphere
    if (0)
    {
        if (node->left == nullptr)
            node->left = new newBVNode(objectptr);
        else
            _insert(objectptr, node->left);
    }

    // If object does not fit inside node.object.sphere
    if (0)
    {
        if (node->right == nullptr)
            node->right = new newBVNode(objectptr);
        else
            _insert(objectptr, node->right); 
    }

}


void newBVTree::insert(GameObject *objectptr)
{
    if (_root == nullptr)
    {
        _root = new newBVNode(objectptr);
        return;
    }

    

}


