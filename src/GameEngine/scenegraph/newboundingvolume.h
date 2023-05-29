#pragma once

#include "../gameobject/gameobject.h"


struct newBVNode
{
    newBVNode *left, *right;
    GameObject *objectptr;
    newBVNode(GameObject *objptr): objectptr(objptr), left(nullptr), right(nullptr) {  };
};


class newBVTree
{
private:
    newBVNode *_root = nullptr;
    void _insert(GameObject *objectptr, newBVNode* &node);

public:
    newBVTree() {  };

    void clear();
    void insert(GameObject *objectptr);
    void cullObjects(Frustum *frustum, std::list<GameObject *> visible_objects[]);
};
