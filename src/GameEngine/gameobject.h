#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"
#include "./state.h"
#include "../transform.h"



class GameObject {

  private:
    std::string m_given_name = "DEFAULT";
    std::string m_name = "DEFAULT";
    int m_ID = 0;

    std::vector<GameObject *> m_children;
    GameObject *m_parent = nullptr;

    bool m_is_environmental = false;
    bool m_is_animated = false;
    bool m_is_npc = false;

    bool m_hidden = false;

    PhysicsState m_physics_state = PHYSICS_FALLING;
    NavigationState m_navigation_state = NAVIGATION_REST;

    std::vector<glm::vec3> m_path;

    float m_bounding_sphere_radius = 1.0f;

    glm::vec3 ray_up    = glm::vec3( 0.0f, +1.0f,  0.0f);
    glm::vec3 ray_down  = glm::vec3( 0.0f, -1.0f,  0.0f);
    glm::vec3 ray_left  = glm::vec3(-1.0f,  0.0f,  0.0f);
    glm::vec3 ray_right = glm::vec3(+1.0f,  0.0f,  0.0f);
    glm::vec3 ray_front = glm::vec3( 0.0f,  0.0f, +1.0f);
    glm::vec3 ray_back  = glm::vec3( 0.0f,  0.0f, -1.0f);

    Transform m_transform;
    AnimationController m_animation_controller;

  public:
    Model *m_model;

    bool selected = false;

    GameObject(): m_transform(&this->m_ID) {};
      
    // Member access
    //---------------------------------------------------------------------------------------------
    glm::vec3 *getPos(void) /*.......................*/ { return &this->m_transform.position; };
    glm::vec3 *getVel(void) /*.......................*/ { return &this->m_transform.velocity; };
    glm::vec3 *getRot(void) /*.......................*/ { return &this->m_transform.rotation; };
    Transform *getTransform(void) /*.................*/ { return &this->m_transform; };
    AnimationController *getAnimController(void) /*..*/ { return &this->m_animation_controller; };
    std::string getName(void) /*.....................*/ { return this->m_name; };
    void setName(std::string name) /*................*/ { this->m_name = name; };
    void setGivenName(std::string name)                 { this->m_given_name = name; };
    std::string getGivenName(void)                      { return this->m_given_name; };
    std::string *getGivenNamePtr(void)                   { return &this->m_given_name; };
    int getID(void) /*...............................*/ { return this->m_ID; };
    int *getIDptr(void) /*...........................*/ { return &this->m_ID; };
    void setID(int id) /*............................*/ { this->m_ID = id; };
    bool *getHidden(void) /*.........................*/ { return &this->m_hidden; };

    bool hasParent(void)                                { return this->m_parent != nullptr; };
    bool hasChildren(void)                              { return this->m_children.size() > 0; };
    std::vector<GameObject *> getChildren(void)         { return this->m_children; };

    float boundingSphereRadius(void)  { return this->m_bounding_sphere_radius; };

    std::string physicsStateString(void);
    std::string navigationStateString(void);
    std::string getStateString(StateType state_type);

    inline bool isNPC(void)           { return this->m_is_npc; };
    inline bool isEnvironmental(void) { return this->m_is_environmental; };
    inline bool isAnimated(void)      { return this->m_is_animated; };
    inline bool isHidden(void)        { return this->m_hidden; };

    void setParent(GameObject *parent);
    void clearParent(void);
    void giveChild(GameObject *child);
    void removeChild(GameObject *child);
    void clearChildren(void);
    bool isChild(GameObject *object);

    GameObject *getParent(void) { return this->m_parent; };
    
    Mesh *getCollisionMesh(void) { return this->m_model->getCollisionMesh(); };
    //---------------------------------------------------------------------------------------------

    void collideWithObject(GameObject *object);
    void collideWithMesh(Mesh *collisionmesh);
    void attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards);

    void useModel(Model *model);
    
    void perFrameUpdate(Renderer *ren);

    void changePhysState(PhysicsState new_state);
    void changeNavState(NavigationState new_state);

    void setPath(std::vector<glm::vec3> path) { this->m_path = path; this->changeNavState(NAVIGATION_SEEK); };

    void draw(Renderer *ren);

};


