#ifndef RAYCAST_H
#define RAYCAST_H

#include "ZenithEngine/core/window.h"
#include "ZenithEngine/math/AABB.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct HitTarget {
  glm::vec3 position;
  glm::vec3 normal;
};

struct Object {
  std::string name;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec3 color = glm::vec3(1.f);
  glm::vec2 texRepeat = glm::vec2(1.f);
  GLuint texture;
  bool textured = true;
};

class RayCast {
public:
    RayCast(const glm::vec3& position, const glm::vec3& direction, float reach, const std::vector<Object>& objects);

    bool hasHit() const { return successful; }
    HitTarget getHitTarget() const { return hitTarget; }
    explicit operator bool() const { return hasHit(); }

private:
    bool successful;
    HitTarget hitTarget;

    bool checkIntersection(const glm::vec3& position, const glm::vec3& direction, float reach, const AABB& box, HitTarget& hitInfo);
};

#endif
