// bool ray_intersects_triangle(Vector3 ray_origin, Vector3 ray_vector, Polygon *tri_in, Vector3 *intersect_point)
// {
//   const float EPSILON = 0.0000001;
//   glm::vec3 vertex0 = tri_in->vertices[0];
//   glm::vec3 vertex1 = tri_in->vertices[1];  
//   glm::vec3 vertex2 = tri_in->vertices[2];
//   Vector3 edge1, edge2, h, s, q;
//   float a,f,u,v;

//   edge1 = vector3_sub(vertex1, vertex0);
//   edge2 = vector3_sub(vertex2, vertex0);
//   h = vector3_cross(ray_vector, edge2);
//   a = vector3_dot(edge1, h);
//   if (a > -EPSILON && a < EPSILON)
//     return false;

//   f = 1.0/a;
//   s = vector3_sub(ray_origin, vertex0);
//   u = f * vector3_dot(s, h);
//   if (u < 0.0 || u > 1.0)
//     return false;

//   q = vector3_cross(s, edge1);
//   v = f * vector3_dot(ray_vector, q);
//   if (v < 0.0 || u + v > 1.0)
//       return false;

//   float t = f * vector3_dot(edge2, q);
//   if (t > EPSILON)
//   {
//     *intersect_point = vector3_add(ray_origin, vector3_scale(ray_vector, t));
//     return true;
//   }
//   else
//     return false;
// }

// void player_collide(Player *player, Polygon *tri, Vector3 ray_direction, float d)
// {
//   Vector3 intersect;
//   if (ray_intersects_triangle(player->game_object->pos, ray_direction, tri, &intersect))
//   {
//     float dist = vector3_dist(player->game_object->pos, intersect);
//     if (0 < dist && dist < d)
//     {
//       float impulse_1d = calculate_impulse(player->game_object->phys_object->vel, tri->face_normal, player->game_object->phys_object->mass, 0, player->game_object->phys_object->elasticity);
//       Vector3 impulse = vector3_scale(tri->face_normal, impulse_1d);
//       player->game_object->phys_object->vel.y += player->game_object->phys_object->inv_mass * impulse.y;

//       player->game_object->pos = vector3_sub(player->game_object->pos, vector3_scale(ray_direction, (d-dist)));
//       // player->game_object->pos.y -= (4-dist);
//     }
//   }
// }

// void player_collision(Player *player)
// {
//   GameObject *obj = head;

//   float nearest_dist = 1000;

//   while (obj != NULL)
//   {
//     if (obj->model != NULL && obj->object_tag != 15)
//     {
//       for (int i=0; i<obj->model->poly_count; i++)
//       {
//         player_collide(player, &obj->model->polygons[i], player->ray_up   , 2);
//         player_collide(player, &obj->model->polygons[i], player->ray_down , 4);
        
//         player_collide(player, &obj->model->polygons[i], player->ray_left , 2);
//         player_collide(player, &obj->model->polygons[i], player->ray_right, 2);
//         player_collide(player, &obj->model->polygons[i], player->ray_front, 2);
//         player_collide(player, &obj->model->polygons[i], player->ray_back , 2);
        
//         // Vector3 intersect_point;
//         // if (ray_intersects_triangle(*player->cam->pos, player->cam->dir, &obj->model->polygons[i], &intersect_point))
//         // {
//         //   float dist = vector3_dist(player->game_object->pos, intersect_point);
//         //   if (dist < nearest_dist)
//         //   {
//         //     nearest_dist = dist;
//         //     Vector3 dir = vector3_sub(*GE_cam->pos, intersect_point);
//         //     vector3_normalise(&dir);
//         //     lightsource.pos = vector3_add(intersect_point, dir);
//         //   }
//         // }

//       }
//     }
//     obj = obj->next;
//   }
// }
