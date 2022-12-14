#include "gameobject.h"

#include "physics.h"

void GameObject::attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards)
{
  glm::vec3 intersect_point;

  bool intersects = ray_intersect_triangle(this->m_transform.position, ray, v0, v1, v2, &intersect_point);


  if (intersects)
  {
    float dist = glm::distance(this->m_transform.position, intersect_point);

    if (0 < dist && dist < d)
    {
      float impulse_1d = calculate_impulse(this->m_transform.velocity, normal, 0.5);
      glm::vec3 impulse = impulse_1d * normal;

      // if (downwards)
      // {
      //   float overlap = d - dist;
      //   this->m_transform.position.y += overlap;
      //   this->m_transform.velocity.y = 0;
      // }

      // else
      // {
        this->m_transform.velocity += (1.0f) * impulse;
        this->m_transform.position = this->m_transform.position - (d-dist)*ray;
      // }
    }
  }
}


void GameObject::collideWithMesh(Mesh *collisionmesh)
{
  for (int i=0; i<collisionmesh->num_vertices; i+=3)
  {
    glm::vec3 v0 = collisionmesh->vertices[i+0].position;
    glm::vec3 v1 = collisionmesh->vertices[i+1].position;
    glm::vec3 v2 = collisionmesh->vertices[i+2].position;

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


std::string GameObject::physicsStateString(void)
{
  switch (this->m_physics_state)
  {
    default:
      return "unknown";

    case (PHYSICS_GROUNDED):
      return "grounded";
    
    case (PHYSICS_FALLING):
      return "falling";
  }
}


std::string GameObject::navigationStateString(void)
{
  switch (this->m_navigation_state)
  {
    default:
      return "unknown";

    case (NAVIGATION_REST):
      return "rest";
    
    case (NAVIGATION_SEEK):
      return "seek";
  }
}

std::string GameObject::getStateString(StateType state_type)
{
  switch (state_type)
  {
    case (STATE_PHYSICS):       return this->physicsStateString();
    case (STATE_NAVIGATION):    return this->navigationStateString();
  }
}



void GameObject::changePhysState(PhysicsState new_state)
{
  this->m_physics_state = new_state;

  switch (new_state)
  {
    case (PHYSICS_GROUNDED): this->m_transform.velocity.y = 0.0f; break;
    case (PHYSICS_FALLING): break;
  }
}


void GameObject::changeNavState(NavigationState new_state)
{
  this->m_navigation_state = new_state;

  switch (new_state)
  {
    case (NAVIGATION_REST):
      this->getAnimController()->useAnimation(ANIM_REST); 
      break;
    
    
    case (NAVIGATION_SEEK):
      this->getAnimController()->useAnimation(ANIM_REST); 
      break;
  }
}


void GameObject::perFrameUpdate(Renderer *ren)
{
  // Logic doesn't apply to environmental objects.
  if (this->isEnvironmental())
    return;

  switch (this->m_physics_state)
  {
    float damping;

    case (PHYSICS_GROUNDED):
      damping = 1 / (1 + (ren->deltaTime * 5.0f));
      this->m_transform.velocity.x *= damping;
      this->m_transform.velocity.z *= damping;

      // this->m_transform.velocity = glm::clamp(this->m_transform.velocity, glm::vec3(-4.5), glm::vec3(4.5));
      this->m_transform.position += this->m_transform.velocity * ren->deltaTime;
      break;


    case (PHYSICS_FALLING):
      this->m_transform.velocity.y -= ren->gravity * ren->deltaTime;

      this->m_transform.velocity.y *= 0.999f;
      damping = 1 / (1 + (ren->deltaTime * 5.0f));
      this->m_transform.velocity.x *= damping;
      this->m_transform.velocity.z *= damping;

      // this->m_transform.velocity = glm::clamp(this->m_transform.velocity, glm::vec3(-4.5), glm::vec3(4.5));
      this->m_transform.position += this->m_transform.velocity * ren->deltaTime;

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

      if (glm::distance(this->m_transform.position, this->m_path[this->m_path.size()-1] + glm::vec3(0.0f, 0.5f, 0.0f)) < 0.5f)
      {
        this->m_path.pop_back();
      }

      else
      {
        glm::vec3 move_towards_dir = 0.01f * glm::normalize(this->m_path[this->m_path.size()-1] - this->m_transform.position);
        this->m_transform.position += move_towards_dir;
        this->m_model->setModelMat(glm::inverse(glm::lookAt(this->m_transform.position, this->m_transform.position + move_towards_dir, {0.0f, 1.0f, 0.0f})));
      }
  
      break;
  }
}


void GameObject::useModel(Model *model)
{
  this->m_name = model->getName();

  model->useTransform(&this->m_transform);
  this->m_model = model;
  this->m_is_npc = model->isNPC();

  this->m_is_environmental = model->isEnvironmental();
  if (this->isEnvironmental())
    this->m_bounding_sphere_radius = INFINITY;

  this->m_is_animated = model->isAnimated();
  if (this->isAnimated())
  {
    this->m_model->useAnimController(this->getAnimController());
    this->getAnimController()->assignAnimations(this->m_model->getAnimations());
    this->getAnimController()->useAnimation(ANIM_REST);
  }

  this->m_name = model->getName();
}

void GameObject::clearParent(void)
{
  if (this->m_parent != nullptr)
    this->m_parent->removeChild(this);
  this->m_parent = nullptr;
  this->m_transform.parent = nullptr;
}

void GameObject::giveChild(GameObject *child)
{
  // return if child is actually parent
  if (child->isChild(this))
    return;

  this->m_children.push_back(child);
  child->clearParent();
  child->setParent(this);
}

void GameObject::removeChild(GameObject *child)
{
  for (int i=0; i<this->m_children.size(); i++)
    if (this->m_children[i]->getID() == child->getID())
      this->m_children.erase(std::next(this->m_children.begin(), i));
}

void GameObject::clearChildren(void)
{
  this->m_children.clear();
}

bool GameObject::isChild(GameObject *object)
{
  bool is_child = false;

  for (auto &child: this->m_children)
  {
    if (child->getID() == object->getID())
      return true;

    if (child->isChild(object))
      return true;
  }

  return is_child;
}

void GameObject::setParent(GameObject *parent)
{
  this->m_parent = parent;
  this->m_transform.parent = &parent->m_transform;
}


/**     object.collideWithObject(ground);
 * E.g. cube.collideWithObject(terrain);
 */
void GameObject::collideWithObject(GameObject *object)
{
  if (this->isEnvironmental() || this == object)
    return;

  // if (glm::distance(this->m_transform.position, object->getPos()) > object->boundingSphereRadius())
  //   return;

  // this->collideWithMesh(object->getCollisionMesh());
}


void GameObject::draw(Renderer *ren)
{
  this->getAnimController()->useTransform(this->getTransform());
  this->m_model->useTransform(this->getTransform());
  this->m_model->useAnimController(this->getAnimController());
  this->m_model->draw(ren);
}

