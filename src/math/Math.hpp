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
    if (glm::dot(p, p) < 1)
      return p;
  }
}

inline glm::vec3 randomUnitVec3() {
  return glm::normalize(randInUnitSphere());
}

#define EPSILON 1e-5

inline bool nearZeroVec3(const glm::vec3& vec) {
  return (fabs(vec.x) < EPSILON) && (fabs(vec.y) < EPSILON) && (fabs(vec.z) < EPSILON);
}

inline glm::vec3 randOnHemisphere(const glm::vec3& normal) {
  glm::vec3 onUnitSphere = randomUnitVec3();
  if (glm::dot(onUnitSphere, normal) > 0) {
    return onUnitSphere;
  } else {
    return -onUnitSphere;
  }
}

inline glm::vec3 randOnUnitDisk() {
  while (true) {
    glm::vec3 point = {randomFloat(-1, 1), randomFloat(-1,1), 0};
    if (glm::dot(point, point) < 1) {
      return point;
    }
  }
}

inline glm::vec3 reflectVec3(const glm::vec3& vec, const glm::vec3& normal) {
  return vec - 2 * glm::dot(vec, normal) * normal;
}

inline float lengthSquared(const glm::vec3& vec) {
  return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

inline glm::vec3 refract(const glm::vec3& uv, const glm::vec3& normal, float etaiOverEtat) {
  float cos_theta = fmin(glm::dot(-uv, normal), 1.0f);
  glm::vec3 rOutPerp = etaiOverEtat * (uv + cos_theta * normal);
  glm::vec3 rOutParallel = -sqrt(fabs(1.0f - glm::dot(rOutPerp, rOutPerp))) * normal;
  return rOutPerp + rOutParallel;
}

/**
 * Uses schlick's approximation
 *
 */
inline float reflectance(float cosine, float refractionIndex) {
  float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
  r0 = r0 * r0;
  return static_cast<float>(r0 + (1.0f - r0) * pow((1.0f - cosine), 5));
}
}

#endif //RAY_TRACE_SRC_MATH_HPP_
