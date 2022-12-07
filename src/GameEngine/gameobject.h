#pragma once

#include "../GraphicsEngine/GraphicsEngine.h"
#include "player.h"

enum GameObjectState { GSTATE_ATREST, GSTATE_MOVETOWARDS };


class OldGameObject {

  private:

  public:
    GameObjectState state = GSTATE_ATREST;
    Mesh *model;

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
    
    OldGameObject(Mesh *model = NULL)
    {
      if (model != NULL)
        this->model = model;

      this->model->pos = &this->pos;
    };

    void changeState(GameObjectState new_state);

    void bindModel(Mesh *model) { this->model = model; };
    void perFrameUpdate(Renderer *ren);
    void attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards);

    void collideWithMesh(Mesh *collisionmesh);
    void draw(Renderer *ren);
};


class GameObject {

  private:
    bool m_collideWith = false;
    GameObjectState state = GSTATE_ATREST;
    std::vector<Model *> models;

  public:
    GameObject() { };

    void collide(bool collide) { this->m_collideWith = collide; };
    void collideWithPlayer(Player *player);

    void addModel(Model *model) { this->models.push_back(model); };
    void draw(Renderer *ren);

};


class ObjectContainer {

  public:
    std::vector<OldGameObject *> objects;

    ObjectContainer() { };

    void addObject(OldGameObject *game_object) { this->objects.push_back(game_object); };

    void draw(Renderer *ren);

};