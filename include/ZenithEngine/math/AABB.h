#ifndef AABB_H
#define AABB_H

#include <glm/common.hpp>
#include <glm/vec3.hpp>
#include <glm/fwd.hpp>

struct AABB {
  glm::vec3 minPoint;
  glm::vec3 maxPoint;

  explicit constexpr AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
      : minPoint(minPoint),
        maxPoint(maxPoint) {}

  static AABB fromObjectPosition(const glm::vec3& position, const glm::vec3& size = glm::vec3{1, 1, 1}) {
      return AABB(position, position + size);
  }

  [[nodiscard]] bool intersect(const AABB& aabb) const {
      return (minPoint.x <= aabb.maxPoint.x && maxPoint.x >= aabb.minPoint.x) &&
             (minPoint.y <= aabb.maxPoint.y && maxPoint.y >= aabb.minPoint.y) &&
             (minPoint.z <= aabb.maxPoint.z && maxPoint.z >= aabb.minPoint.z);
  }

  [[nodiscard]] glm::vec3 getMinPoint() const { return minPoint; };
  [[nodiscard]] glm::vec3 getMaxPoint() const { return maxPoint; };

  [[nodiscard]] float getMinX() const { return minPoint.x; };
  [[nodiscard]] float getMinY() const { return minPoint.y; };
  [[nodiscard]] float getMinZ() const { return minPoint.z; };
  [[nodiscard]] float getMaxX() const { return maxPoint.x; };
  [[nodiscard]] float getMaxY() const { return maxPoint.y; };
  [[nodiscard]] float getMaxZ() const { return maxPoint.z; };

  [[nodiscard]] bool contains(const glm::vec3& point) const {
    return (point.x >= minPoint.x && point.x <= maxPoint.x) &&
           (point.y >= minPoint.y && point.y <= maxPoint.y) &&
           (point.z >= minPoint.z && point.z <= maxPoint.z);
  }

  [[nodiscard]] glm::vec3 getCenter() const { return (minPoint + maxPoint) * 0.5f; }

  void expand(const AABB& other) {
    minPoint = glm::min(minPoint, other.minPoint);
    maxPoint = glm::max(maxPoint, other.maxPoint);
  }
};

#endif
