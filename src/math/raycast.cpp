#include "ZenithEngine/math/raycast.h"
#ifndef __APPLE__
#include <GL/gl.h>
#endif
#ifdef __APPLE__
#include <glad/glad.h>
#endif
#include <glm/gtc/epsilon.hpp>

RayCast::RayCast(const glm::vec3 &position, const glm::vec3 &direction,
                 float reach, const std::vector<Object> &objects)
    : successful(false) {
  /*for (const auto& object : objects) {*/
  /*  AABB objectAABB = object.computeAABB();*/
  /*  HitTarget tempHit;*/
  /*  if (checkIntersection(position, direction, reach, objectAABB, tempHit))
   * {*/
  /*    successful = true;*/
  /*    hitTarget = tempHit;*/
  /*    break;*/
  /*  }*/
  /*}*/
}

bool RayCast::checkIntersection(const glm::vec3 &position,
                                const glm::vec3 &direction, float reach,
                                const AABB &box, HitTarget &hitInfo) {
  glm::vec3 invDir = 1.0f / direction;
  glm::vec3 t0 = (box.getMinPoint() - position) * invDir;
  glm::vec3 t1 = (box.getMaxPoint() - position) * invDir;

  glm::vec3 tmin = glm::min(t0, t1);
  glm::vec3 tmax = glm::max(t0, t1);

  float tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
  float tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

  if (tNear > tFar || tFar < 0.0f)
    return false;

  float t = (tNear < 0.0f) ? tFar : tNear;
  if (t > reach)
    return false;

  hitInfo.position = position + direction * t;

  if (t == tmin.x)
    hitInfo.normal = glm::vec3(direction.x > 0 ? -1.0f : 1.0f, 0.0f, 0.0f);
  else if (t == tmin.y)
    hitInfo.normal = glm::vec3(0.0f, direction.y > 0 ? -1.0f : 1.0f, 0.0f);
  else if (t == tmin.z)
    hitInfo.normal = glm::vec3(0.0f, 0.0f, direction.z > 0 ? -1.0f : 1.0f);

  return true;
}
