#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"


enum GameObjectState { GSTATE_ATREST, GSTATE_MOVETOWARDS };


class GameObject {

  private:

  public:
    GameObjectState state = GSTATE_ATREST;
    Model *model;

    bool usePhysics = false;

    glm::vec3 ray_up    = glm::vec3( 0.0f, +1.0f,  0.0f);
    glm::vec3 ray_down  = glm::vec3( 0.0f, -1.0f,  0.0f);
    glm::vec3 ray_left  = glm::vec3(-1.0f,  0.0f,  0.0f);
    glm::vec3 ray_right = glm::vec3(+1.0f,  0.0f,  0.0f);
    glm::vec3 ray_front = glm::vec3( 0.0f,  0.0f, +1.0f);
    glm::vec3 ray_back  = glm::vec3( 0.0f,  0.0f, -1.0f);

    glm::vec3 move_towards = glm::vec3(0.0f, 1.0f, 0.0f);

    std::vector<glm::vec3> path;

    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 vel = glm::vec3(0.0f);
    
    GameObject(Model *model = NULL)
    {
      if (model != NULL)
        this->model = model;

      this->model->pos = &this->pos;
    };

    void changeState(GameObjectState new_state);

    void bindModel(Model *model) { this->model = model; };
    void perFrameUpdate(Renderer *ren);
    void attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards);

    void collideWithMesh(Model *collisionmesh);
    void draw(Renderer *ren);
};


class ObjectContainer {

  public:
    std::vector<GameObject *> objects;

    ObjectContainer() { };

    void addObject(GameObject *game_object) { this->objects.push_back(game_object); };

    void draw(Renderer *ren);

};