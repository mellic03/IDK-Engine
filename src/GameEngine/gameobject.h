#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"

enum GameObjectState { GSTATE_ATREST, GSTATE_MOVETOWARDS };

enum PhysicsState { PHYSICS_GROUNDED, PHYSICS_FALLING };
enum NavigationState { NAVIGATION_REST, NAVIGATION_SEEK };

// class OldGameObject {

//   private:

//   public:

//     GameObjectState state = GSTATE_ATREST;
//     Mesh *model;

//     bool usePhysics = false;

//     glm::vec3 ray_up    = glm::vec3( 0.0f, +1.0f,  0.0f);
//     glm::vec3 ray_down  = glm::vec3( 0.0f, -1.0f,  0.0f);
//     glm::vec3 ray_left  = glm::vec3(-1.0f,  0.0f,  0.0f);
//     glm::vec3 ray_right = glm::vec3(+1.0f,  0.0f,  0.0f);
//     glm::vec3 ray_front = glm::vec3( 0.0f,  0.0f, +1.0f);
//     glm::vec3 ray_back  = glm::vec3( 0.0f,  0.0f, -1.0f);

//     glm::vec3 move_towards = glm::vec3(0.0f, 1.0f, 0.0f);

//     std::vector<glm::vec3> path;

//     glm::vec3 pos = glm::vec3(0.0f);
//     glm::vec3 vel = glm::vec3(0.0f);
    
//     OldGameObject(Mesh *model = NULL)
//     {
//       if (model != NULL)
//         this->model = model;

//       this->model->pos = &this->pos;
//     };

//     void changeState(GameObjectState new_state);

//     void perFrameUpdate(Renderer *ren);
//     void attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards);

//     void collideWithMesh(Mesh *collisionmesh);
// };


class GameObject {

  private:

    std::string m_name;

    bool m_is_environmental = false;
    bool m_is_animated = false;
    bool m_is_npc = false;

    bool m_hidden = false;

    PhysicsState m_physics_state = PHYSICS_FALLING;
    NavigationState m_navigation_state = NAVIGATION_REST;

    std::vector<glm::vec3> m_path;


    glm::vec3 ray_up    = glm::vec3( 0.0f, +1.0f,  0.0f);
    glm::vec3 ray_down  = glm::vec3( 0.0f, -1.0f,  0.0f);
    glm::vec3 ray_left  = glm::vec3(-1.0f,  0.0f,  0.0f);
    glm::vec3 ray_right = glm::vec3(+1.0f,  0.0f,  0.0f);
    glm::vec3 ray_front = glm::vec3( 0.0f,  0.0f, +1.0f);
    glm::vec3 ray_back  = glm::vec3( 0.0f,  0.0f, -1.0f);

  public:
    Model *m_model;

    bool selected = false;

    GameObject() { };
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 vel = glm::vec3(0.0f);
    glm::vec3 rot = glm::vec3(0.0f);
    
    // Member access
    //-----------------------
    glm::vec3 getPos(void)    { return this->pos; };
    std::string getName(void) { return this->m_name; };
    bool *getHidden(void)     { return &this->m_hidden; };
    //-----------------------

    inline bool isNPC(void)           { return this->m_is_npc; };
    inline bool isEnvironmental(void) { return this->m_is_environmental; };
    inline bool isAnimated(void)      { return this->m_is_animated; };
    inline bool isHidden(void)        { return this->m_hidden; };

    void collideWithObject(GameObject *object);
    void collideWithPlayer(Player *player);

    void collideWithMesh(Mesh *collisionmesh);
    void attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards);

    void addModel(Model *model);
    
    void perFrameUpdate(Renderer *ren);

    void changePhysState(PhysicsState new_state);
    void changeNavState(NavigationState new_state);

    void setPath(std::vector<glm::vec3> path) { this->m_path = path; this->changeNavState(NAVIGATION_SEEK); };

    void draw(Renderer *ren);

};


