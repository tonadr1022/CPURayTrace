//
// Created by Tony Adriansen on 2/5/24.
//

#ifndef RAY_TRACE_SRC_HITTABLE_HPP_
#define RAY_TRACE_SRC_HITTABLE_HPP_

#include "math/Ray.hpp"
#include "raytrace_pch.hpp"
#include <glm/glm.hpp>
#include "math/Interval.hpp"

struct HitRecord {
  glm::vec3 point{};
  glm::vec3 normal{};
  float t;
  bool frontFace;
  int objectIndex;

  /**
   * sets hit record normal vector. outward normal should be unit length
   * if ray direction and outward normal point in same direction, (pos dot product),
   * the ray is hitting the back face, otherwise it's hitting the front face
   *
   * this ensures the normal always points against the ray hit
   *
   * @param r
   * @param outwardNormal
   */
  void setFaceNormal(const Ray& r, const glm::vec3& outwardNormal) {
    frontFace = glm::dot(r.direction, outwardNormal) < 0.0f;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

class Hittable {
 public:
  Hittable(int materialIndex_) : materialIndex(materialIndex_) {}
  virtual ~Hittable() = default;

  virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
  virtual std::string toString() = 0;

  int materialIndex = 0;
};

#endif //RAY_TRACE_SRC_HITTABLE_HPP_
