#include <functional>

#include "physics.h"
#include "state.h"


Player::Player(Renderer *ren)
{
  this->cam = &ren->cam;
  this->cam->pos = this->getPos();

  // this->pos = &this->cam->pos;
  this->cam->dir = &this->dir;

  this->useWeapon(WEAPON_SHOTGUN);
  this->getWeapon()->loadModel("assets/player/gun/");
  this->getWeapon()->hip_pos = glm::vec3(+0.10f, -0.10f, -0.15f);
  this->getWeapon()->aim_pos = glm::vec3( 0.00f, -0.015f, -0.10f);
}

void Player::setObjectPtr(GameObject *ptr)
{
  this->m_gameobject = ptr;

  this->cam->pos = &this->pos_worldspace;
  this->cam->dir = &this->dir;

}

void Player::changeState(PlayerState state)
{
  this->current_state = state;

  switch (state)
  {
    case (PSTATE_GROUNDED):
      break;

    case (PSTATE_FALLING):
      break;
  }

}

void Player::collideWith(GameObject *object)
{
  if (object->isEnvironmental())
  {
    this->m_collision_transforms.push_back(*object->getTransform());
    this->m_collision_meshes.push_back(object->getCollisionMesh());
  }
}

void Player::perFrameUpdate(void)
{

  glm::vec3 ps = *this->getPos();

  this->pos_worldspace = this->getTransform()->getModelMatrix() * glm::vec4(ps.x, ps.y, ps.z, 1.0f);

  float nearest_dist = INFINITY;
  int nearest_i = -1;
  int nearest_j = -1;

  
  glm::vec3 ray_up    =  glm::vec3( 0.0f, +1.0f,  0.0f); 
  glm::vec3 ray_down  =  glm::vec3( 0.0f, -1.0f,  0.0f);
  glm::vec3 ray_left  = -this->cam->right;
  glm::vec3 ray_right =  this->cam->right;
  glm::vec3 ray_front =  this->cam->front;
  glm::vec3 ray_back  = -this->cam->front;

  glm::vec3 ray_traveldir = glm::normalize(this->vel);


  for (int i=0; i<this->m_collision_meshes.size(); i++)
  {
    Mesh *mesh = this->m_collision_meshes[i];
    glm::mat4 model = this->m_collision_transforms[i].getModelMatrix();


    for (int j=0; j<mesh->num_vertices; j+=3)
    {
      glm::vec3 v0 = mesh->vertices[j+0].position;
      glm::vec3 v1 = mesh->vertices[j+1].position;
      glm::vec3 v2 = mesh->vertices[j+2].position;

      v0 = model * glm::vec4(v0.x, v0.y, v0.z, 1.0f);
      v1 = model * glm::vec4(v1.x, v1.y, v1.z, 1.0f);
      v2 = model * glm::vec4(v2.x, v2.y, v2.z, 1.0f);

      glm::vec3 normal = glm::mat3(model) * mesh->vertices[j+2].face_normal;
      glm::normalize(normal);

      // Find nearest "down"
      if (glm::dot(ray_down, normal) <= 0)
      {
        glm::vec3 intersect_point;
        bool intersects = ray_intersect_triangle(*this->getPos(), ray_down, v0, v1, v2, &intersect_point);
        if (intersects)
        {
          float dist = glm::distance(*this->getPos(), intersect_point);
          if (dist < nearest_dist)
          {
            nearest_dist = dist;
            nearest_i = i;
            nearest_j = j;
          }
        }
      }

      if (glm::dot(ray_up, normal) <= 0)
        player_collide(this, ray_up,      v0, v1, v2, normal, this->height/2, false);

      if (glm::dot(ray_left, normal) <= 0)
        player_collide(this, ray_left,    v0, v1, v2, normal, this->height/2, false);

      if (glm::dot(ray_right, normal) <= 0)
        player_collide(this, ray_right,   v0, v1, v2, normal, this->height/2, false);

      if (glm::dot(ray_front, normal) <= 0)
        player_collide(this, ray_front,   v0, v1, v2, normal, this->height/2, false);

      if (glm::dot(ray_back, normal) <= 0)
        player_collide(this, ray_back,    v0, v1, v2, normal, this->height/2, false);

      // player_collide(player, ray_traveldir, v0, v1, v2, normal, player->height/2, false);
    }
  }

  if (nearest_i == -1)
  {
    this->m_collision_transforms.clear();
    this->m_collision_meshes.clear();
    return;
  }

  glm::mat4 model = this->m_collision_transforms[nearest_i].getModelMatrix();

  glm::vec3 v0 = this->m_collision_meshes[nearest_i]->vertices[nearest_j+0].position;
  glm::vec3 v1 = this->m_collision_meshes[nearest_i]->vertices[nearest_j+1].position;
  glm::vec3 v2 = this->m_collision_meshes[nearest_i]->vertices[nearest_j+2].position;
  v0 = model * glm::vec4(v0.x, v0.y, v0.z, 1.0f);
  v1 = model * glm::vec4(v1.x, v1.y, v1.z, 1.0f);
  v2 = model * glm::vec4(v2.x, v2.y, v2.z, 1.0f);

  glm::vec3 normal = glm::mat3(model) * this->m_collision_meshes[nearest_i]->vertices[nearest_j+2].face_normal;

  player_collide(this, ray_down, v0, v1, v2, normal, this->height, true);

  this->m_collision_transforms.clear();
  this->m_collision_meshes.clear();
}


SDL_bool mouse_capture = SDL_TRUE;

void Player::key_input(Renderer *ren)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  switch (this->current_state)
  {
    case (PSTATE_FALLING):
      this->vel.y -= ren->gravity * ren->deltaTime;
      break;

    case (PSTATE_GROUNDED):
      if (state[SDL_SCANCODE_SPACE])
      {
        this->vel.y += 25 * this->jump_force * ren->deltaTime;
        this->changeState(PSTATE_FALLING);
      }
      break;
  }


  this->vel.y *= 0.999f;
  float damping = 1 / (1 + (ren->deltaTime * this->friction));
  this->vel.x *= damping;
  this->vel.z *= damping;

  this->vel = glm::clamp(this->vel, glm::vec3(-4.5, -INFINITY, -4.5), glm::vec3(4.5, INFINITY, 4.5));
  *this->getPos() += this->vel * ren->deltaTime;

  this->cam->input();


  glm::vec3 temp_front = { this->cam->front.x, 0.0f, this->cam->front.z };
  temp_front = glm::normalize(temp_front);

  bool headbob = false;

  if (state[SDL_SCANCODE_W])
  {
    this->vel += this->move_speed * ren->deltaTime * temp_front;
    headbob = true;
  }

  if (state[SDL_SCANCODE_S])
  {
    this->vel -= this->move_speed * ren->deltaTime * temp_front;
    headbob = true;
  }

  if (state[SDL_SCANCODE_A])
  {
    this->vel += this->move_speed * ren->deltaTime * ren->cam.right;
    headbob = true;
  }

  if (state[SDL_SCANCODE_D])
  {
    this->vel -= this->move_speed * ren->deltaTime * ren->cam.right;
    headbob = true;
  }



}

void Player::mouse_input(Renderer *ren, SDL_Event *event)
{

  switch (event->type)
  {
    case SDL_MOUSEBUTTONDOWN:
      if (event->button.button == SDL_BUTTON_RIGHT)
      {
        this->getWeapon()->aiming = true;
        this->getWeapon()->sway /= 8;
        this->move_speed /= 2;
      }
    break;
  
    case SDL_MOUSEBUTTONUP:
      if (event->button.button == SDL_BUTTON_RIGHT)
      {
        this->getWeapon()->aiming = false;
        this->getWeapon()->sway *= 8;
        this->move_speed *= 2;
      }
    break;
  }

  if (event->type == SDL_MOUSEMOTION && SDL_GetRelativeMouseMode())
  {
    this->cam->yaw   += this->cam->rot_speed * ren->deltaTime * event->motion.xrel;
    this->cam->pitch -= this->cam->rot_speed * ren->deltaTime * event->motion.yrel;
    this->getWeapon()->movement_offset.x -= this->cam->rot_speed * ren->deltaTime * 0.001f * event->motion.xrel;
    this->getWeapon()->movement_offset.y += this->cam->rot_speed * ren->deltaTime * 0.001f * event->motion.yrel;
  }

  else if (event->type == SDL_KEYDOWN)
  {
    switch (event->key.keysym.sym)
    {
      case SDLK_ESCAPE:
        mouse_capture = (mouse_capture == SDL_TRUE) ? SDL_FALSE : SDL_TRUE;
        SDL_SetRelativeMouseMode(mouse_capture);
        break;
    }
  }

}


void Player::draw(Renderer *ren)
{
  this->getWeapon()->draw(ren);
}