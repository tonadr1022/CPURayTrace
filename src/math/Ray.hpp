//
// Created by Tony Adriansen on 2/4/24.
//

#ifndef RAY_TRACE_CMAKE_BUILD_DEBUG_RAY_HPP_
#define RAY_TRACE_CMAKE_BUILD_DEBUG_RAY_HPP_

#include "glm/glm/glm.hpp"

struct Ray {
 public:
  Ray() = default;
  Ray(const glm::vec3& origin, const glm::vec3& direction) : origin(origin), direction(direction) {}

  [[nodiscard]] glm::vec3 at(const float t) const {
    return origin  + t * direction;
  }

  glm::vec3 origin{};
  glm::vec3 direction{};

 private:
};

#endif //RAY_TRACE_CMAKE_BUILD_DEBUG_RAY_HPP_
