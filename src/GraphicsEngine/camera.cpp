
#include "camera.h"
#include "renderer.h"

int frame_count = 0;

Camera::Camera()
{
  this->pos = glm::vec3(0.0f, 0.0f, 3.0f);
  this->target = glm::vec3(0.0f, 0.0f, 0.0f);
  this->dir = glm::normalize(this->pos - this->target);
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);

  glm::vec3 temp_up = glm::vec3(0.0f, 1.0f, 0.0f);
  this->right = glm::normalize(glm::cross(temp_up, this->dir));

  this->up = glm::cross(this->dir, this->right);

  this->view = glm::lookAt(
    this->pos,
    this->pos + this->front,
    this->up
  ); 
  this->pos = glm::vec3(0.0f, 1.0f, 3.0f);

  this->projection = glm::perspective(glm::radians(this->fov), 1.0f, 0.1f, RENDER_DISTANCE);
}

void Camera::init(void)
{
  // ShaderSource worldspace_src = parse_shader("assets/shaders/basic.glsl");
  // Shader worldspace;
  // worldspace.set(create_shader(worldspace_src.vertex_source, worldspace_src.fragment_source));
  // this->shaders[SHADER_WORLDSPACE] = worldspace;

  // ShaderSource viewspace_src = parse_shader("assets/shaders/basic_viewspace.glsl");
  // Shader viewspace;
  // viewspace.set(create_shader(viewspace_src.vertex_source, viewspace_src.fragment_source));
  // this->shaders[SHADER_VIEWSPACE] = viewspace;

}

void Camera::input()
{
  this->projection = glm::perspective(glm::radians(this->fov), 1.0f, 0.1f, RENDER_DISTANCE);

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (this->pitch <= -85)
    this->pitch = -85;
  else if (this->pitch >= 85)
    this->pitch = 85;

  this->dir.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  this->dir.y = sin(glm::radians(this->pitch));
  this->dir.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

  this->front = glm::normalize(this->dir);
  this->right = glm::normalize(glm::cross(this->up, this->dir));

  this->view = glm::lookAt(
    this->pos,
    this->pos + this->front,
    this->up
  );
}