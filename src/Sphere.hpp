//
// Created by Tony Adriansen on 2/5/24.
//

#ifndef RAY_TRACE_SRC_SPHERE_HPP_
#define RAY_TRACE_SRC_SPHERE_HPP_

#include "raytrace_pch.hpp"
#include "Hittable.hpp"
#include "math/Interval.hpp"

/**
 * Notes:
 *
 * point vector minus center vector squared == radius squared means point is on the surface of sphere
 * (P - C) * (P - C) = r^2
 * parameterize by the point along the ray
 * (P(t) - C) * (P(t) - C) = r^2
 * ray is: P(t) = A + tb    b is dir, A is origin
 * ((A + tb) - C) * ((A + tb) - C) = r^2
 * (tb + (A-C) * (tb + (A-C)) = r^2
 * t^2(b * b) + 2tb(A-C) + (A-C)*(A-C) - r^2 = 0
 * a == b * b
 * b == 2b(A-C)
 * c == (A-C)*(A-C) - r^2
 * half b == b(A-C)
 *
 * discriminant == b^2 - 4ac == half_b^2 - ac
 *
 * A-C is pt to Center,
 */
class Sphere : public Hittable {
 public:
  Sphere(glm::vec3 center, float radius, int materialIndex)
      : Hittable(materialIndex), m_center(center), m_radius(radius) {
  }

  bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
    const glm::vec3 oc = r.origin - m_center;
    const float half_b = (glm::dot(r.direction, oc));
    const float a = glm::dot(r.direction, r.direction);
    const float c = glm::dot(oc, oc) - m_radius * m_radius;
    const float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    const float sqrtDiscriminant = sqrt(discriminant);

    // nearest root in the acceptable range, otherwise no hit
    float root = (-half_b - sqrtDiscriminant) / a;
    if (!rayT.surrounds(root)) {
      root = (-half_b + sqrtDiscriminant) / a;
      if (!rayT.surrounds(root)) {
        return false;
      }
    }

    // solve for distance from ray origin to the point of intersection with sphere
    rec.t = root;
    rec.point = r.at(rec.t);
    // ray at point t to sphere center, normalized
    glm::vec3 outwardNormal = (rec.point - m_center) / m_radius;
    rec.setFaceNormal(r, outwardNormal);

    return true;
  }

  std::string toString() override { return "Sphere"; }

 private:
  glm::vec3 m_center;
  float m_radius;
};

#endif //RAY_TRACE_SRC_SPHERE_HPP_
