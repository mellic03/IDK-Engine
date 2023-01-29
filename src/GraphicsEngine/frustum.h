#pragma once

#include "camera.h"

struct Plane {
  glm::vec3 normal = glm::vec3(0.0f, 1.0, 0.0f);
  float distance = 0.0f;

	Plane() = default;

	Plane(const glm::vec3& p1, const glm::vec3& norm)
		: normal(glm::normalize(norm)),
		distance(glm::dot(normal, p1))
	{};

	float getSignedDistanceToPlane(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	};
};


struct Frustum {

  Plane planes[6];
  
  void update(Camera *cam, float aspect, float fovY, float near, float far)
  {
    const float halfVSide = far * tanf(fovY * 0.5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = far * cam->front;

    glm::vec3 pos = cam->m_transform->getPos_worldspace();

    glm::vec3 up = glm::cross(cam->right, cam->front);

    this->planes[0] = { pos + near * cam->front, cam->front };
    this->planes[1] = { pos + frontMultFar, -cam->front     };
    this->planes[2] = { pos, glm::cross(frontMultFar - cam->right * halfHSide, up) };
    this->planes[3] = { pos, glm::cross(up, frontMultFar + cam->right * halfHSide) };
    this->planes[4] = { pos, glm::cross(cam->right, frontMultFar - up * halfVSide) };
    this->planes[5] = { pos, glm::cross(frontMultFar + up * halfVSide, cam->right) };
  };


  bool partiallyVisible(glm::vec3 pos, float radius)
  {
    float dist;

    for (int i=0; i<6; i++)
    {
      dist = this->planes[i].getSignedDistanceToPlane(pos);
      if (dist < -radius)
        return false;
    }

    return true;
  };


  bool entirelyVisible(glm::vec3 pos, float radius)
  {
    float dist;

    for (int i=0; i<6; i++)
    {
      dist = this->planes[i].getSignedDistanceToPlane(pos);
      if (dist < radius)
        return false;
    }

    return true;
  };

};

