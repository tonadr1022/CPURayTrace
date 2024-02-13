//
// Created by Tony Adriansen on 2/12/24.
//

#ifndef RAY_TRACE_SRC_MATH_HPP_
#define RAY_TRACE_SRC_MATH_HPP_

namespace Math {
constexpr float infinity = std::numeric_limits<float>::infinity();
constexpr float pi = 3.1415926535897932;

inline float toRadians(float degrees) {
  return degrees * pi / 180.0f;
}

inline float randomFloat() {
  static std::uniform_real_distribution<float> distribution(0.0, 1.0);
  static thread_local std::mt19937 generator;
  return distribution(generator);
}

inline float randomFloat(float min, float max) {
  return min + randomFloat() * (max - min);
}

inline glm::vec3 randVec3() {
  return {randomFloat(), randomFloat(), randomFloat()};
}

inline glm::vec3 randVec3(float min, float max) {
  return {randomFloat(min, max), randomFloat(min, max), randomFloat(min, max)};
}

inline glm::vec3 randInUnitSphere() {
  while (true) {
    glm::vec3 p = randVec3(-1, 1);
    if (p.x * p.x + p.y * p.y + p.z * p.z < 1)
      return p;
  }
}

inline glm::vec3 randomUnitVec3() {
  return glm::normalize(randInUnitSphere());
}

inline glm::vec3 randOnHemisphere(const glm::vec3& normal) {
  glm::vec3 onUnitSphere = randomUnitVec3();
  if (glm::dot(onUnitSphere, normal) > 0) {
    return onUnitSphere;
  } else {
    return -onUnitSphere;
  }
}

}

#endif //RAY_TRACE_SRC_MATH_HPP_
