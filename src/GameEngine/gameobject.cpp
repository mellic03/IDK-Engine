#include "gameobject.h"

#include "physics.h"


void GameObject::changeState(GameObjectState new_state)
{
  this->state = new_state;

  switch (new_state)
  {
    case (GSTATE_ATREST):

      break;
  }
}


void GameObject::perFrameUpdate(Renderer *ren)
{
  this->vel.y -= ren->gravity * ren->deltaTime;

  this->vel.y *= 0.999f;
  float damping = 1 / (1 + (ren->deltaTime * 5.0f));
  this->vel.x *= damping;
  this->vel.z *= damping;

  // this->vel = glm::clamp(this->vel, glm::vec3(-4.5), glm::vec3(4.5));
  this->pos += this->vel * ren->deltaTime;


  switch (this->state)
  {
    case (GSTATE_ATREST):

      break;

    case (GSTATE_MOVETOWARDS):

      if (this->path.size() == 0)
      {
        this->changeState(GSTATE_ATREST);
        break;
      }

      if (glm::distance(this->pos, this->path[this->path.size()-1] + glm::vec3(0.0f, 0.5f, 0.0f)) < 0.5f)
      {
        // printf("%d\n", this->path.size());
        this->path.pop_back();
      }

      else
      {
        glm::vec3 move_towards_dir = 0.004f * glm::normalize(this->path[this->path.size()-1] - this->pos);
        this->pos += move_towards_dir;
      }
  
      break;
  }


}

void GameObject::attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards)
{
  glm::vec3 intersect_point;

  bool intersects = ray_intersect_triangle(this->pos, ray, v0, v1, v2, &intersect_point);


  if (intersects)
  {
    float dist = glm::distance(this->pos, intersect_point);

    if (0 < dist && dist < d)
    {
      float impulse_1d = calculate_impulse(this->vel, normal, 0.1);
      glm::vec3 impulse = impulse_1d * normal;

      if (downwards)
      {
        float overlap = d - dist;
        this->pos.y += overlap;
        this->vel.y = 0;
      }

      else
      {
        this->vel += (1.0f) * impulse;
        this->pos = this->pos - (d-dist)*ray;
      }
    }
  }
}

void GameObject::collideWithMesh(Model *collisionmesh)
{
  for (int i=0; i<collisionmesh->num_vertices; i+=3)
  {
    glm::vec3 v0 = collisionmesh->vertices[i+0].position + *collisionmesh->pos;
    glm::vec3 v1 = collisionmesh->vertices[i+1].position + *collisionmesh->pos;
    glm::vec3 v2 = collisionmesh->vertices[i+2].position + *collisionmesh->pos;

    glm::vec3 normal = collisionmesh->vertices[i].face_normal;

    if (glm::dot(normal, this->ray_down) <= 0)
      this->attemptCollision(this->ray_down, v0, v1, v2, normal, 0.5, true);

    if (glm::dot(normal, this->ray_left) <= 0)
      this->attemptCollision(this->ray_left, v0, v1, v2, normal, 0.25, false);

    if (glm::dot(normal, this->ray_right) <= 0)
      this->attemptCollision(this->ray_right, v0, v1, v2, normal, 0.25, false);

    if (glm::dot(normal, this->ray_front) <= 0)
      this->attemptCollision(this->ray_front, v0, v1, v2, normal, 0.25, false);

    if (glm::dot(normal, this->ray_back) <= 0)
      this->attemptCollision(this->ray_back, v0, v1, v2, normal, 0.25, false);
     
  }
}


void GameObject::draw(Renderer *ren)
{
  this->model->draw(ren);
}


void ObjectContainer::draw(Renderer *ren)
{
  for (int i=0; i<this->objects.size(); i++)
    this->objects[i]->draw(ren);
}