#include "gameobject.h"

#include "physics.h"


GameObject::GameObject(std::string directory)
{
  std::string filepath = directory + "asset.txt";

  FILE *fh = fopen(filepath.c_str(), "r");

  char buffer[256];
  char stringdata[256];
  int intdata;
  while (fgets(buffer, 256, fh) != NULL)
  {



  }

}


// void GameObject::attemptCollision(glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d, bool downwards)
// {
//   glm::vec3 intersect_point;

//   bool intersects = ray_intersect_triangle(this->pos_worldspace, ray, v0, v1, v2, &intersect_point);

//   if (intersects)
//   {
//     float dist = glm::distance(this->pos_worldspace, intersect_point);

//     if (0 < dist && dist < d)
//     {
//       float impulse_1d = calculate_impulse(*this->getVel(), normal, 0.5);
//       glm::vec3 impulse = impulse_1d * normal;

//       *this->getVel() += (1.0f) * impulse;

//       if (downwards)
//       {
//         float overlap = d - dist;
//         this->getPos()->y += overlap;
//         this->changePhysState(PHYSICS_GROUNDED);
//       }

//       else
//       {
//         glm::vec3 v = (d-dist)*ray;

//         *this->getPos() = *this->getPos() - this->getTransform()->worldToLocal({v.x, v.y, v.z, 0.0f});
//         // this->pos_worldspace = this->pos_worldspace - (d-dist)*ray;
//       }
//     }

//     else if (dist > d && downwards)
//       this->changePhysState(PHYSICS_FALLING);
//   }
// }

// void GameObject::collideWithMeshes(void)
// {
//   float nearest_dist = INFINITY;
//   int nearest_i = -1;
//   int nearest_j = -1;

//   glm::mat4 thismodelmat = this->getTransform()->getModelMatrix();

//   glm::vec3 ray_up    =  thismodelmat * glm::vec4( 0.0f, +1.0f,  0.0f,  0.0f);
//   glm::vec3 ray_down  =  thismodelmat * glm::vec4( 0.0f, -1.0f,  0.0f,  0.0f);
//   glm::vec3 ray_left  =  thismodelmat * glm::vec4(-1.0f,  0.0f,  0.0f,  0.0f);
//   glm::vec3 ray_right =  thismodelmat * glm::vec4( 1.0f,  0.0f,  0.0f,  0.0f);
//   glm::vec3 ray_front =  thismodelmat * glm::vec4( 0.0f,  0.0f,  1.0f,  0.0f);
//   glm::vec3 ray_back  =  thismodelmat * glm::vec4( 0.0f,  0.0f, -1.0f,  0.0f);

//   glm::vec3 ray_traveldir = glm::normalize(*this->getVel());


//   for (int i=0; i<this->m_collision_meshes.size(); i++)
//   {
//     Mesh *mesh = this->m_collision_meshes[i];
//     glm::mat4 model = this->m_collision_transforms[i].getModelMatrix();

//     for (int j=0; j<mesh->vertices.size(); j+=3)
//     {
//       glm::vec3 v0 = mesh->vertices[j+0].position;
//       glm::vec3 v1 = mesh->vertices[j+1].position;
//       glm::vec3 v2 = mesh->vertices[j+2].position;
//       v0 = model * glm::vec4(v0.x, v0.y, v0.z, 1.0f);
//       v1 = model * glm::vec4(v1.x, v1.y, v1.z, 1.0f);
//       v2 = model * glm::vec4(v2.x, v2.y, v2.z, 1.0f);

//       glm::vec3 normal = glm::mat3(model) * mesh->vertices[j+2].face_normal;

//       glm::normalize(normal);

//       // Find nearest "down"
//       if (glm::dot(ray_down, normal) <= 0)
//       {
//         glm::vec3 intersect_point;
//         bool intersects = ray_intersect_triangle(this->pos_worldspace, ray_down, v0, v1, v2, &intersect_point);
//         if (intersects)
//         {
//           float dist = glm::distance(this->pos_worldspace, intersect_point);
//           if (dist < nearest_dist)
//           {
//             nearest_dist = dist;
//             nearest_i = i;
//             nearest_j = j;
//           }
//         }
//       }

//       if (glm::dot(ray_up, normal) <= 0)
//         this->attemptCollision( ray_up,     v0, v1, v2, normal, this->width, false);

//       if (glm::dot(ray_left, normal) <= 0)
//         this->attemptCollision( ray_left,   v0, v1, v2, normal, this->width, false);

//       if (glm::dot(ray_right, normal) <= 0)
//         this->attemptCollision( ray_right,  v0, v1, v2, normal, this->width, false);

//       if (glm::dot(ray_front, normal) <= 0)
//         this->attemptCollision( ray_front,  v0, v1, v2, normal, this->width, false);

//       if (glm::dot(ray_back, normal) <= 0)
//         this->attemptCollision( ray_back,   v0, v1, v2, normal, this->width, false);
     
//       if (glm::dot(ray_traveldir, normal) <= 0)
//         this->attemptCollision( ray_traveldir,   v0, v1, v2, normal, this->width, false);
//     }
//   }

//   if (nearest_i == -1)
//   {
//     this->changePhysState(PHYSICS_FALLING);
//     this->m_collision_transforms.clear();
//     this->m_collision_meshes.clear();
//     return;
//   }

//   glm::mat4 model = this->m_collision_transforms[nearest_i].getModelMatrix();
//   glm::mat4 inv_model = glm::inverse(model);
//   ray_down  =  thismodelmat * glm::vec4( 0.0f, -1.0f,  0.0f,  0.0f);

//   glm::vec3 v0 = this->m_collision_meshes[nearest_i]->vertices[nearest_j+0].position;
//   glm::vec3 v1 = this->m_collision_meshes[nearest_i]->vertices[nearest_j+1].position;
//   glm::vec3 v2 = this->m_collision_meshes[nearest_i]->vertices[nearest_j+2].position;
//   v0 = model * glm::vec4(v0.x, v0.y, v0.z, 1.0f);
//   v1 = model * glm::vec4(v1.x, v1.y, v1.z, 1.0f);
//   v2 = model * glm::vec4(v2.x, v2.y, v2.z, 1.0f);

//   glm::vec3 normal = glm::mat3(model) * this->m_collision_meshes[nearest_i]->vertices[nearest_j+2].face_normal;

//   this->attemptCollision( ray_down,   v0, v1, v2, normal, this->height/2, true);

//   this->m_collision_transforms.clear();
//   this->m_collision_meshes.clear();
// }

// std::string GameObject::physicsStateString(void)
// {
//   switch (this->m_physics_state)
//   {
//     default:                  return "unknown";
//     case (PHYSICS_GROUNDED):  return "grounded";
//     case (PHYSICS_FALLING):   return "falling";
//   }
// }

// std::string GameObject::navigationStateString(void)
// {
//   switch (this->m_navigation_state)
//   {
//     default:                  return "unknown";
//     case (NAVIGATION_REST):   return "rest";
//     case (NAVIGATION_SEEK):   return "seek";
//   }
// }

// PhysicsState GameObject::getPhysState(void)
// {
//   return this->m_physics_state;
// }

// NavigationState GameObject::getNavState(void)
// {
//   return this->m_navigation_state;
// }

// std::string GameObject::getStateString(StateType state_type)
// {
//   switch (state_type)
//   {
//     default:                  return "Unknown state";
//     case (STATE_PHYSICS):     return this->physicsStateString();
//     case (STATE_NAVIGATION):  return this->navigationStateString();
//   }
// }

// void GameObject::changePhysState(PhysicsState new_state)
// {
//   this->m_physics_state = new_state;

//   switch (new_state)
//   {
//     case (PHYSICS_GROUNDED):
//       this->getVel()->y = 0.0f;
//       break;

//     case (PHYSICS_FALLING): break;
//   }
// }

// void GameObject::changeNavState(NavigationState new_state)
// {
//   this->m_navigation_state = new_state;

//   switch (new_state)
//   {
//     case (NAVIGATION_REST):
//       break;
    
    
//     case (NAVIGATION_SEEK):
//       break;
//   }
// }


// void GameObject::perFrameUpdate(Renderer *ren)
// {
//   // Logic doesn't apply to environmental objects.
//   if (this->isEnvironmental())
//     return;


//   // Per frame, add velocity to position, then check physics state
//   float damping;

//   this->getVel()->y *= 0.999f;
//   damping = 1 / (1 + (ren->deltaTime * 5.0f));
//   this->getVel()->x *= damping;
//   this->getVel()->z *= damping;
  
//   glm::mat4 inv_model = glm::inverse(this->getTransform()->getModelMatrix());
//   glm::vec3 tempvel = inv_model * this->getTransform()->getVel_vec4();
//   *this->getPos() += tempvel * ren->deltaTime;

//   this->pos_worldspace = this->getTransform()->getPos_worldspace();


//   this->collideWithMeshes();


//   switch (this->getPhysState())
//   {
//     case (PHYSICS_GROUNDED):
//       break;

//     case (PHYSICS_FALLING):
//       this->getVel()->y -= ren->gravity * ren->deltaTime;
//       break;
//   }

//   switch (this->m_navigation_state)
//   {
//     case (NAVIGATION_REST):

//       break;

//     case (NAVIGATION_SEEK):

//       if (this->m_path.size() == 0)
//       {
//         this->changeNavState(NAVIGATION_REST);
//         break;
//       }

//       if (glm::distance(this->m_transform.position, this->m_path[this->m_path.size()-1] + glm::vec3(0.0f, 0.5f, 0.0f)) < 0.5f)
//       {
//         this->m_path.pop_back();
//       }

//       else
//       {
//         glm::vec3 move_towards_dir = 0.01f * glm::normalize(this->m_path[this->m_path.size()-1] - this->m_transform.position);
//         this->m_transform.position += move_towards_dir;
//         this->m_model->setModelMat(glm::inverse(glm::lookAt(this->m_transform.position, this->m_transform.position + move_towards_dir, {0.0f, 1.0f, 0.0f})));
//       }
  
//       break;
//   }

// }



// void GameObject::clearParent(void)
// {
//   if (this->m_parent != nullptr)
//   {
//     *this->getPos() = this->m_parent->getTransform()->localToWorld(this->getTransform()->getPos_vec4());
//     // *this->getRot() = this->m_parent->getTransform()->localToWorld(this->getTransform()->getRot_vec4());
//     this->m_parent->removeChild(this);
//   }
//   this->m_parent = nullptr;
//   this->m_transform.parent = nullptr;
// }

// void GameObject::giveChild(GameObject *child)
// {
//   // return if child is actually parent
//   if (child->isChild(this))
//     return;


//   this->m_children.push_back(child);
//   child->clearParent();
//   child->setParent(this);
// }

// void GameObject::removeChild(GameObject *child)
// {
//   for (int i=0; i<this->m_children.size(); i++)
//     if (this->m_children[i]->getID() == child->getID())
//       this->m_children.erase(std::next(this->m_children.begin(), i));
// }

// void GameObject::clearChildren(void)
// {
//   this->m_children.clear();
// }

// bool GameObject::isChild(GameObject *object)
// {
//   bool is_child = false;

//   for (auto &child: this->m_children)
//   {
//     if (child->getID() == object->getID())
//       return true;

//     if (child->isChild(object))
//       return true;
//   }

//   return is_child;
// }

// void GameObject::setParent(GameObject *parent)
// {
//   *this->getPos() = parent->getTransform()->worldToLocal(this->getTransform()->getPos_vec4());
//   // *this->getRot() = parent->getTransform()->worldToLocal(this->getTransform()->getRot_vec4());

//   this->m_parent = parent;
//   this->m_transform.parent = &parent->m_transform;
// }


// /**     object.collideWithObject(ground);
//  * E.g. cube.collideWithObject(terrain);
//  */
// void GameObject::collideWithObject(GameObject *object)
// {
//   if (this->isEnvironmental() || this->isStatic() || this->getID() == object->getID())
//     return;

//   // if (glm::distance(this->m_transform.position, object->getPos()) > object->boundingSphereRadius())
//   //   return;

//   this->m_collision_meshes.push_back(object->getCollisionMesh());
//   this->m_collision_transforms.push_back(*object->getTransform());

// }


// void GameObject::draw(Renderer *ren)
// {

// }

