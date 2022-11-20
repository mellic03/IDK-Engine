



#include "physics.h"





void ModelContainer::add(Model *model)
{
  if (this->head == NULL)
  {
    this->head = model;
    this->head->next = NULL;
  }
  else
  {
    Model *modelptr = model;
    modelptr->next = this->head;
    this->head = modelptr;
  }
}

void ModelContainer::draw()
{
  Model *modelptr = this->head;
  while (modelptr != NULL)
  {
    modelptr->draw();
    modelptr = modelptr->next;
  }
}


bool ray_intersect_triangle(  glm::vec3 ray_pos, glm::vec3 ray_dir,
                              glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
                              glm::vec3 *intersect_point  )
{
  const float EPSILON = 0.0000001;
  glm::vec3 edge1, edge2, h, s, q;
  float a,f,u,v;
  edge1 = v1 - v0;
  edge2 = v2 - v0;

  h = glm::cross(ray_dir, edge2);
  a = glm::dot(edge1, h);
  if (a > -EPSILON && a < EPSILON)
    return false;

  f = 1.0/a;
  s = ray_pos - v0;
  u = f * glm::dot(s, h);
  if (u < 0.0 || u > 1.0)
    return false;

  q = glm::cross(s, edge1);
  v = f * glm::dot(ray_dir, q);
  if (v < 0.0 || u + v > 1.0)
    return false;

  float t = f * glm::dot(edge2, q);
  if (t > EPSILON)
  {
    *intersect_point = ray_pos + ray_dir * t;
    return true;
  }
  else
    return false;
}


float calculate_impulse(glm::vec3 vel, glm::vec3 face_normal, float mass)
{
  float relative_to_normal = glm::dot(vel, face_normal);
  if (relative_to_normal > 0)
    return 0.0f;
  
  float j = -1 * relative_to_normal;
  j /= (1/mass) + 0.00001;

  return j;
}


void player_collide(Player *player, glm::vec3 ray, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal, float d)
{

  glm::vec3 intersect_point;

  bool intersects = ray_intersect_triangle(player->temp_pos, ray, v0, v1, v2, &intersect_point);

  if (intersects)
  {
    float dist = glm::distance(player->temp_pos, intersect_point);
    if (0 < dist && dist < d)
    {
      float impulse_1d = calculate_impulse(*player->vel, normal, 0.1);
      glm::vec3 impulse = impulse_1d * normal;
      *player->vel += (1.0f/0.2f) * impulse;

      *player->pos = *player->pos - (d-dist)*ray;
    }
  }
}


void ModelContainer::collide(Player *player)
{
  Model *modelptr = this->head;
  while (modelptr != NULL)
  {
    for (int i=0; i<modelptr->num_vertices; i+=3)
    {
      glm::vec3 v0 = modelptr->vertices[i+0].position + modelptr->pos;
      glm::vec3 v1 = modelptr->vertices[i+1].position + modelptr->pos;
      glm::vec3 v2 = modelptr->vertices[i+2].position + modelptr->pos;
      glm::vec3 normal = modelptr->vertices[i+2].normal;
      // glm::vec4 tnorm = glm::vec4(normal.x, normal.y, normal.z, 0.0f) * modelptr->transform_mat;
      // normal = glm::vec3(tnorm.x, tnorm.y, tnorm.z);

      player->temp_pos = glm::vec4(player->pos->x, player->pos->y, player->pos->z, 1.0f) * modelptr->transform_mat;

      glm::vec4 ray_up    = glm::vec4( 0.0f, +1.0f,  0.0f,  0.0f);
      glm::vec4 ray_down  = glm::vec4( 0.0f, -1.0f,  0.0f,  0.0f);
      glm::vec3 ray_left  = -player->cam->right;
      glm::vec3 ray_right =  player->cam->right;
      glm::vec3 ray_front =  player->cam->front;
      glm::vec3 ray_back  = -player->cam->front;

      player_collide(player, ray_down , v0, v1, v2, normal, player->height);
      player_collide(player, ray_up   , v0, v1, v2, normal, player->height/2);
      player_collide(player, ray_left , v0, v1, v2, normal, player->height/2);
      player_collide(player, ray_right, v0, v1, v2, normal, player->height/2);
      player_collide(player, ray_front, v0, v1, v2, normal, player->height/2);
      player_collide(player, ray_back , v0, v1, v2, normal, player->height/2);
    }

    modelptr = modelptr->next;
  }
}














// bool AABB::intersects(glm::vec3 point)
// {
//   glm::vec4 trans = glm::vec4(point.x, point.y, point.z, 1.0f) * (this->parent_model->transform_mat);

//   bool x_cond = this->xmin <= trans.x && trans.x <= this->xmax;
//   bool y_cond = this->ymin <= trans.y && trans.y <= this->ymax;
//   bool z_cond = this->zmin <= trans.z && trans.z <= this->zmax;

//   return x_cond & y_cond & z_cond;
// }



// void PhysicsWorld::add(Model *object)
// {
//   float xmin = object->vertices[0].position.x, xmax = object->vertices[0].position.x;
//   float ymin = object->vertices[0].position.y, ymax = object->vertices[0].position.y;
//   float zmin = object->vertices[0].position.z, zmax = object->vertices[0].position.z;

//   for (int i=0; i<object->num_vertices; i++)
//   {
//     xmin = (xmin <  object->vertices[i].position.x) ? xmin : object->vertices[i].position.x;
//     xmax = (xmax >= object->vertices[i].position.x) ? xmax : object->vertices[i].position.x;

//     ymin = (ymin <  object->vertices[i].position.y) ? ymin : object->vertices[i].position.y;
//     ymax = (ymax >= object->vertices[i].position.y) ? ymax : object->vertices[i].position.y;

//     zmin = (zmin <  object->vertices[i].position.z) ? zmin : object->vertices[i].position.z;
//     zmax = (zmax >= object->vertices[i].position.z) ? zmax : object->vertices[i].position.z;
//   }

//   this->objects[this->num_objects].xmin = xmin;
//   this->objects[this->num_objects].xmax = xmax;
//   this->objects[this->num_objects].ymin = ymin;
//   this->objects[this->num_objects].ymax = ymax;
//   this->objects[this->num_objects].zmin = zmin;
//   this->objects[this->num_objects].zmax = zmax;
//   this->objects[this->num_objects].parent_model = object;
//   this->objects[this->num_objects].pos = &object->pos;
//   this->num_objects += 1;
//   printf("x: %f %f\ny: %f %f\nz: %f %f\n", xmin, xmax, ymin, ymax, zmin, zmax);

// }


// void PhysicsWorld::tick(Player *player)
// {
  // player->vel->y -= 0.0005f;

  // for (int i=0; i<this->num_objects; i++)
  // {
  //   AABB *box = &this->objects[i];

  //   glm::vec3 point = glm::vec3(*player->pos - glm::vec3(0.0f, player->height, 0.0f));
  //   glm::vec4 trans = glm::vec4(point.x, point.y, point.z, 1.0f) * (box->parent_model->transform_mat);

  //   bool x_cond = box->xmin <= trans.x && trans.x <= box->xmax;
  //   bool y_cond = box->ymin <= trans.y && trans.y <= box->ymax;
  //   bool z_cond = box->zmin <= trans.z && trans.z <= box->zmax;

  //   if (x_cond & y_cond & z_cond)
  //   {
  //     float y_overlap = box->ymax - trans.y;
  //     player->pos->y += y_overlap/2;
  //     player->vel->y /= 2;
  //   }
  // }
// }