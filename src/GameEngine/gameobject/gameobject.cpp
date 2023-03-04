#include "gameobject.h"

#include "../physics.h"

namespace PE = PhysicsEngine;

GameObject::GameObject(void)
{
  this->sphereColliders[0].position_offset = glm::vec3(1.0f, 0.0f, 0.0f);
  this->sphereColliders[1].position_offset = glm::vec3(-1.0f, 0.0f, 0.0f);
  this->sphereColliders[2].position_offset = glm::vec3(0.0f, 0.0f, 1.0f);
  this->sphereColliders[3].position_offset = glm::vec3(0.0f, 0.0f, -1.0f);
}

bool GameObject::_groundTest(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal)
{
  if (glm::dot(ray, normal) > 0)
    return false;

  glm::vec3 intersect_point;

  bool intersects = PE::ray_intersects_triangle(*this->getPos(), ray, v0, v1, v2, &intersect_point);

  if (intersects)
  {
    float dist = glm::distance(*this->getPos(), intersect_point);
    if (dist >= 0 && dist < (this->sphereColliders[0].radius - this->sphereColliders[0].position_offset.y + 0.05f))
    {
      float overlap = (this->sphereColliders[0].radius - this->sphereColliders[0].position_offset.y + 0.05f) - dist;
      this->getPos()->y += overlap / 2.0f;
      return true;
    }
  }

  return false;
}


void GameObject::_followPath()
{
  using namespace Navigation;

  NavData *nData = this->getData()->navData();
  NavState *nState = &nData->state;


  switch (*nState)
  {
    case (NavState::NONE):
      break;


    case (NavState::REST):
      break;


    case (NavState::SEEK):

      if (nData->path.size() == 0)
      {
        *nState = NavState::REST;
        break;
      }

      glm::vec3 objectPos = *this->getPos();
      objectPos.y = 0.0f;
      glm::vec3 nodePos = nData->path[nData->path.size()-1];
      nodePos.y = 0.0f;

      if (glm::distance(objectPos, nodePos) < 0.5f)
      {
        nData->path.pop_back();
      }

      else
      {
        glm::vec3 dir = nData->speed * glm::normalize(nodePos - objectPos);
        *this->getVel() += dir;
      }
  
      break;
  }
}


void GameObject::_perFrameUpdate_navigation()
{
  if (this->getComponents()->hasComponent(COMPONENT_NAVIGATION) == false)
    return;

  this->_followPath();

  glm::vec3 vel2D = glm::vec3(this->getVel()->x, 0.0f, this->getVel()->z);

  if (glm::length2(vel2D) > 0.2f)
    this->getTransform()->orientation = glm::quatLookAt(glm::normalize(-vel2D), glm::vec3(0.0f, 1.0f, 0.0f));

}


void GameObject::_perFrameUpdate_physics(Renderer *ren)
{

  this->pos_worldspace = this->getTransform()->getPos_worldspace();

  PhysicsData *pData = this->getData()->physData();

  if (pData->flags()->get() == PhysicsFlag::NONE)
    return;


  if (pData->flags()->get(PhysicsFlag::VELOCITY))
  {
    float damping;

    this->getVel()->y *= 0.999f;
    damping = 1 / (1 + (ren->deltaTime * 5.0f));
    this->getVel()->x *= damping;
    this->getVel()->z *= damping;

    glm::mat4 inv_model = glm::inverse(this->getTransform()->getModelMatrix_noLocalTransform());
    glm::vec3 tempvel = inv_model * this->getTransform()->getVel_vec4();
    *this->getPos() += tempvel * ren->deltaTime;
  }

  this->collideWithMeshes();

  if (pData->flags()->get(PhysicsFlag::GRAVITY))
  {
    PhysicsData *pData = this->getData()->physData();

    switch (pData->state)
    {
      case (PhysicsState::GROUNDED):
        break;

      case (PhysicsState::FALLING):
        this->getVel()->y -= PE::gravity * ren->deltaTime;
        break;
    }
  }

}


void GameObject::collideWithMeshes(void)
{
  glm::vec3 ray_down  =   glm::vec4(0.0f, -1.0f,  0.0f,  0.0f);

  PhysicsState *pState = &this->getData()->physData()->state;

  *pState = PhysicsState::FALLING;


  for (size_t i=0; i<this->_collision_meshes.size(); i++)
  {
    CollisionMesh *mesh = this->_collision_meshes[i];
    glm::mat4 model = glm::inverse(this->getTransform()->getModelMatrix_noLocalTransform()) * this->_collision_transforms[i]->getModelMatrix();

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


      if (this->getComponents()->hasComponent(COMPONENT_SPHERE_COLLIDER))
      {
        for (int i=0; i<4; i++)
        {
          PE::SphereCollider &spherecollider = this->sphereColliders[i];

          spherecollider.last_pos = spherecollider.pos;

          spherecollider.pos = *this->getPos() + spherecollider.position_offset;
          spherecollider.pos = glm::vec3(this->getTransform()->getModelMatrix() * glm::vec4(spherecollider.pos, 1.0f));
          spherecollider.vel = *this->getVel();

          if (PE::sphere_triangle_detect(&spherecollider, vert0, vert1, vert2, &dist, &edge_collision, &dir))
            PE::sphere_triangle_response(&spherecollider, vert0, vert1, vert2, dist, edge_collision, dir);

          spherecollider.pos -= glm::vec3(this->getTransform()->getModelMatrix() * glm::vec4(spherecollider.position_offset, 1.0f));
          *this->getPos() = spherecollider.pos;
          *this->getVel() = spherecollider.vel;
        }

      }

      // if (this->getComponents()->hasComponent(COMPONENT_CAPSULE_COLLIDER))
      // {
      //   this->capsulecollider.pos = *this->getPos();
      //   this->capsulecollider.vel = *this->getVel();

      //   PE::capsule_triangle_detect(&this->capsulecollider, vert0, vert1, vert2, &dist, &edge_collision, &dir);

      //   *this->getPos() = (this->capsulecollider.pos);
      //   *this->getVel() = (this->capsulecollider.vel);
      // }
      
      
      if (*pState == PhysicsState::FALLING)
        if (this->_groundTest(ray_down, vert0.position, vert1.position, vert2.position, vert0.normal))
          *pState = PhysicsState::GROUNDED;
    }
  }

  // if (this->getID() != 0 && this->getComponents()->hasComponent(COMPONENT_SPHERE_COLLIDER))
  // {
  //   for (int i=0; i<4; i++)
  //   {
  //     PE::SphereCollider &spherecollider = this->sphereColliders[i];

  //     glm::mat4 rot = glm::lookAt(*this->getPos(), spherecollider.pos, glm::vec3(0.0f, 1.0f, 0.0f));//TransformUtil::rotationFromPositions(spherecollider.last_pos + spherecollider.position_offset, spherecollider.pos + spherecollider.position_offset);
  //     this->getTransform()->orientation = glm::quat_cast(rot) * this->getTransform()->orientation;
  //   }
  // }


  this->_collision_transforms.clear();
  this->_collision_meshes.clear();
}


void GameObject::perFrameUpdate(Renderer *ren)
{
  this->getTransform()->getModelMatrix();

  glm::vec3 p = this->getCullingData()->getLocalBoundingSpherePos();
  p = this->getTransform()->getModelMatrix_stale() * glm::vec4(p.x, p.y, p.z, 1.0f);
  this->getCullingData()->bounding_sphere_pos = p;


  this->_perFrameUpdate_physics(ren);
  this->_perFrameUpdate_navigation();

}


void GameObject::clearParent(void)
{
  if (this->m_parent == nullptr)
    return;

  *this->getPos() = this->m_parent->getTransform()->localToWorld(this->getTransform()->getPos_vec4());
 
  Transform *thisTransform = this->getTransform();
  thisTransform->orientation = this->m_parent->getTransform()->orientation * thisTransform->orientation;
  
  this->m_parent->removeChild(this);

  this->m_parent = nullptr;
  this->parentID = -1;
  this->_transform.parent = nullptr;
}

void GameObject::giveChild(GameObject *child)
{
  if (child->isChild(this))
    return;

  this->m_children.push_back(child);
  child->clearParent();

  child->setParent(this);
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


void GameObject::setParent(GameObject *parent)
{
  *this->getPos() = parent->getTransform()->worldToLocal(this->getTransform()->getPos_vec4());

  Transform *thisTransform = this->getTransform();
  thisTransform->orientation = glm::inverse(parent->getTransform()->orientation) * thisTransform->orientation;

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

  if (this->getData()->physData()->flags()->get() == PhysicsFlag::NONE)
    return;

  glm::vec3 p0 = this->getTransform()->getPos_worldspace();
  glm::vec3 p1 = object->getCullingData()->bounding_sphere_pos;

  if (glm::distance2(p0, p1) > object->getCullingData()->bounding_sphere_radiusSQ)
    return;

  this->_collision_meshes.push_back(object->getCollisionMesh());
  this->_collision_transforms.push_back(object->getTransform());
}

