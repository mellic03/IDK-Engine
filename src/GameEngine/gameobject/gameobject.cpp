#include "gameobject.h"

#include "../physics.h"

namespace PE = PhysicsEngine;

GameObject::GameObject(void)
{

}

bool GameObject::_groundTest(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal)
{
  if (glm::dot(ray, normal) > 0)
    return false;

  glm::vec3 intersect_point;

  this->pos_worldspace = this->getTransform()->getPos_worldspace();

  bool intersects = PE::ray_intersects_triangle(this->pos_worldspace, ray, v0, v1, v2, &intersect_point);

  if (intersects)
  {
    float dist = glm::distance(this->pos_worldspace, intersect_point);
    if (dist >= 0 && dist < (this->capsulecollider.bottom + 0.05f))
    {
      float overlap = (this->capsulecollider.bottom + 0.05f) - dist;
      this->getPos()->y += overlap / 2.0f;
      return true;
    }
  }

  return false;
}


void GameObject::collideWithMeshes(void)
{
  glm::vec3 ray_down  =   glm::vec4(0.0f, -1.0f,  0.0f,  0.0f);

  this->changePhysState(PHYSICS_FALLING);


  for (size_t i=0; i<this->_collision_meshes.size(); i++)
  {
    CollisionMesh *mesh = this->_collision_meshes[i];
    glm::mat4 model = this->_collision_transforms[i]->getModelMatrix();

    for (size_t j=0; j<mesh->m_vertices.size(); j+=3)
    {
      Vertex vert0 = mesh->m_vertices[j+0];
      Vertex vert1 = mesh->m_vertices[j+1];
      Vertex vert2 = mesh->m_vertices[j+2];

      vert0.position = model * glm::vec4(vert0.position.x, vert0.position.y, vert0.position.z, 1.0f);
      vert0.normal   = model * glm::vec4(vert0.normal.x, vert0.normal.y, vert0.normal.z, 0.0f);
      vert1.position = model * glm::vec4(vert1.position.x, vert1.position.y, vert1.position.z, 1.0f);
      vert2.position = model * glm::vec4(vert2.position.x, vert2.position.y, vert2.position.z, 1.0f);

      glm::vec3 dir;
      bool edge_collision = false;
      float dist = INFINITY;
      
      this->capsulecollider.pos = *this->getPos();
      this->capsulecollider.vel = *this->getVel();
      PE::capsule_triangle_detect(&this->capsulecollider, vert0, vert1, vert2, &dist, &edge_collision, &dir);
      *this->getPos() = this->capsulecollider.pos;
      *this->getVel() = this->capsulecollider.vel;
      
      
      if (this->getPhysState() == PHYSICS_FALLING)
        if (this->_groundTest(ray_down, vert0.position, vert1.position, vert2.position, vert0.normal))
          this->changePhysState(PHYSICS_GROUNDED);
    }
  }


  this->_collision_transforms.clear();
  this->_collision_meshes.clear();
}


std::string GameObject::physStateString(void)
{
  switch (this->m_physics_state)
  {
    default:                  return "unknown";
    case (PHYSICS_NONE):      return "PHYSICS_NONE";
    case (PHYSICS_GROUNDED):  return "PHYSICS_GROUNDED";
    case (PHYSICS_FALLING):   return "PHYSICS_FALLING";
  }
}

std::string GameObject::navStateString(void)
{
  switch (this->m_navigation_state)
  {
    default:                  return "unknown";
    case (NAVIGATION_REST):   return "NAVIGATION_REST";
    case (NAVIGATION_SEEK):   return "NAVIGATION_SEEK";
  }
}

PhysicsState GameObject::getPhysState(void)
{
  return this->m_physics_state;
}

NavigationState GameObject::getNavState(void)
{
  return this->m_navigation_state;
}

std::string GameObject::getStateString(StateType state_type)
{
  switch (state_type)
  {
    default:                  return "Unknown state";
    case (STATE_PHYSICS):     return this->physStateString();
    case (STATE_NAVIGATION):  return this->navStateString();
  }
}

void GameObject::changePhysState(PhysicsState new_state)
{
  this->m_physics_state = new_state;

  switch (new_state)
  {
    case (PHYSICS_GROUNDED):
      this->getVel()->y = 0.0f;
      break;

    case (PHYSICS_FALLING): break;
  }
}
void GameObject::changePhysState(std::string new_state)
{
  PhysicsState state = PHYSICS_NONE;

  if (new_state == "PHYSICS_NONE")
    state = PHYSICS_NONE;
  else if (new_state == "PHYSICS_GROUNDED")
    state = PHYSICS_GROUNDED;
  else if (new_state == "PHYSICS_FALLING")
    state = PHYSICS_FALLING;

  this->m_physics_state = state;

  switch (state)
  {
    case (PHYSICS_GROUNDED):
      this->getVel()->y = 0.0f;
      break;

    case (PHYSICS_FALLING):
      break;
  }
}

void GameObject::changeNavState(NavigationState new_state)
{
  this->m_navigation_state = new_state;

  switch (new_state)
  {
    case (NAVIGATION_REST):
      break;
    
    
    case (NAVIGATION_SEEK):
      break;
  }
}

std::string GameObject::getObjectTypeString(void)
{
  switch (this->getObjectType())
  {
    default:
    case (GAMEOBJECT_UNDEFINED):    return "GAMEOBJECT_UNDEFINED";
    case (GAMEOBJECT_TERRAIN):      return "GAMEOBJECT_TERRAIN";
    case (GAMEOBJECT_STATIC):       return "GAMEOBJECT_STATIC";
    case (GAMEOBJECT_BILLBOARD):    return "GAMEOBJECT_BILLBOARD";
    case (GAMEOBJECT_ACTOR):        return "GAMEOBJECT_ACTOR";
    case (GAMEOBJECT_PLAYER):       return "GAMEOBJECT_PLAYER";
    case (GAMEOBJECT_LIGHTSOURCE):  return "GAMEOBJECT_LIGHTSOURCE";
  }
}

void GameObject::perFrameUpdate(Renderer *ren)
{
  if (this->getPhysState() == PHYSICS_NONE)
    return;

  // Per frame, add velocity to position, then check physics state
  float damping;

  this->getVel()->y *= 0.999f;
  damping = 1 / (1 + (ren->deltaTime * 5.0f));
  this->getVel()->x *= damping;
  this->getVel()->z *= damping;
  

  glm::mat4 inv_model = glm::inverse(this->getTransform()->getModelMatrix_noLocalTransform());
  glm::vec3 tempvel = inv_model * this->getTransform()->getVel_vec4();
  *this->getPos() += tempvel * ren->deltaTime;

  this->pos_worldspace = this->getTransform()->getPos_worldspace();

  this->collideWithMeshes();

  switch (this->getPhysState())
  {
    case (PHYSICS_GROUNDED):
      break;

    case (PHYSICS_FALLING):
      this->getVel()->y -= PE::gravity * ren->deltaTime;
      break;
  }


  switch (this->getNavState())
  {
    case (NAVIGATION_REST):

      break;


    case (NAVIGATION_SEEK):

      if (this->m_path.size() == 0)
      {
        this->changeNavState(NAVIGATION_REST);
        break;
      }

      if (glm::distance(this->_transform.position, this->m_path[this->m_path.size()-1] + glm::vec3(0.0f, 0.5f, 0.0f)) < 0.5f)
      {
        this->m_path.pop_back();
      }

      else
      {
        glm::vec3 move_towards_dir = 0.01f * glm::normalize(this->m_path[this->m_path.size()-1] - this->_transform.position);
        this->_transform.position += move_towards_dir;
      }
  
      break;
  }
}


void GameObject::clearParent(void)
{
  if (this->m_parent == nullptr)
    return;

  *this->getPos() = this->m_parent->getTransform()->localToWorld(this->getTransform()->getPos_vec4());
  // if (this->getID() != 0)
  // {
    Transform *thisTransform = this->getTransform();
    thisTransform->orientation = this->m_parent->getTransform()->orientation * thisTransform->orientation;
  // }

  this->m_parent->removeChild(this);

  this->m_parent = nullptr;
  this->parentID = -1;
  this->_transform.parent = nullptr;
}

void GameObject::giveChild(GameObject *child, bool keepGlobalPos)
{
  // return if child is actually parent
  if (child->isChild(this))
    return;

  this->m_children.push_back(child);
  child->clearParent();

  child->setParent(this, keepGlobalPos);
}

void GameObject::removeChild(GameObject *child)
{
  for (size_t i=0; i<this->m_children.size(); i++)
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

void GameObject::setParent(GameObject *parent, bool keepGlobalPos)
{
  *this->getPos() = parent->getTransform()->worldToLocal(this->getTransform()->getPos_vec4());

  if (keepGlobalPos)
  {
    Transform *thisTransform = this->getTransform();
    thisTransform->orientation = glm::inverse(parent->getTransform()->orientation) * thisTransform->orientation;
  }

  this->m_parent = parent;
  this->parentID = parent->getID();
  this->_transform.parent = &parent->_transform;
}


/**     object.collideWithObject(ground);
 * E.g. cube.collideWithObject(terrain);
 */
void GameObject::collideWithObject(GameObject *object)
{
  if (this->getID() == object->getID())
    return;

  if (this->getObjectType() == GAMEOBJECT_BILLBOARD)
    return;

  if (this->getPhysState() == PHYSICS_NONE)
    return;

  glm::vec3 p = object->m_model->bounding_sphere_pos;
  p = object->getTransform()->getModelMatrix() * glm::vec4(p.x, p.y, p.z, 1.0f);

  if (glm::distance2(this->getTransform()->getPos_worldspace(), p) > object->m_model->bounding_sphere_radiusSQ)
    return;


  this->_collision_meshes.push_back(object->getCollisionMesh());
  this->_collision_transforms.push_back(object->getTransform());
}


