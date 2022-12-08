#include "gameobject.h"

#include "physics.h"

void GameObject::attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards)
{
  glm::vec3 intersect_point;

  bool intersects = ray_intersect_triangle(this->pos, ray, v0, v1, v2, &intersect_point);


  if (intersects)
  {
    float dist = glm::distance(this->pos, intersect_point);

    if (0 < dist && dist < d)
    {
      float impulse_1d = calculate_impulse(this->vel, normal, 0.5);
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

void GameObject::collideWithMesh(Mesh *collisionmesh)
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

void GameObject::changePhysState(PhysicsState new_state)
{
  this->m_physics_state = new_state;

  switch (new_state)
  {
    case (PHYSICS_GROUNDED): this->vel.y = 0.0f; break;
    case (PHYSICS_FALLING): break;
  }
}

void GameObject::changeNavState(NavigationState new_state)
{
  this->m_navigation_state = new_state;

  switch (new_state)
  {
    case (NAVIGATION_REST): break;
    case (NAVIGATION_SEEK): break;
  }
}

void GameObject::perFrameUpdate(Renderer *ren)
{
  if (this->m_collideWith == true)
    return;


  switch (this->m_physics_state)
  {
    float damping;

    case (PHYSICS_GROUNDED):
      damping = 1 / (1 + (ren->deltaTime * 5.0f));
      this->vel.x *= damping;
      this->vel.z *= damping;

      // this->vel = glm::clamp(this->vel, glm::vec3(-4.5), glm::vec3(4.5));
      this->pos += this->vel * ren->deltaTime;
      break;


    case (PHYSICS_FALLING):
      this->vel.y -= ren->gravity * ren->deltaTime;

      this->vel.y *= 0.999f;
      damping = 1 / (1 + (ren->deltaTime * 5.0f));
      this->vel.x *= damping;
      this->vel.z *= damping;

      // this->vel = glm::clamp(this->vel, glm::vec3(-4.5), glm::vec3(4.5));
      this->pos += this->vel * ren->deltaTime;

      break;
  }



  switch (this->m_navigation_state)
  {
    case (NAVIGATION_REST):

      break;

    case (NAVIGATION_SEEK):

      if (this->m_path.size() == 0)
      {
        this->changeNavState(NAVIGATION_REST);
        break;
      }

      if (glm::distance(this->pos, this->m_path[this->m_path.size()-1] + glm::vec3(0.0f, 0.5f, 0.0f)) < 0.5f)
      {
        this->m_path.pop_back();
      }

      else
      {
        glm::vec3 move_towards_dir = 0.01f * glm::normalize(this->m_path[this->m_path.size()-1] - this->pos);
        this->pos += move_towards_dir;
      }
  
      break;
  }
}


void GameObject::addModel(Model *model)
{
  model->setPos(&this->pos);
  model->setRot(&this->rot);
  this->m_model = model;
  this->m_is_environmental = model->isEnvironmental();
  this->m_is_animated = model->isAnimated();
}


/**     object.collideWithObject(ground);
 * E.g. cube.collideWithObject(terrain);
 */
void GameObject::collideWithObject(GameObject *object)
{
  // Two environmental objects cannot collide with one another
  if (this->isEnvironmental() && object->isEnvironmental())
    return;

  if (!object->canCollideWith())
    return;

  this->collideWithMesh(object->m_model->getCollisionMesh());
}


void GameObject::collideWithPlayer(Player *player)
{
  if (this->m_collideWith)
    this->m_model->collideWithPlayer(player);
}


void GameObject::draw(Renderer *ren)
{
  this->m_model->draw(ren);
}

