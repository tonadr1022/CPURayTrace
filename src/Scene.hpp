//
// Created by Tony Adriansen on 2/12/24.
//

#ifndef RAY_TRACE_SRC_SCENE_HPP_
#define RAY_TRACE_SRC_SCENE_HPP_

#include "raytrace_pch.hpp"
#include "Hittable.hpp"

enum MaterialType {
  Lambertian,
  Metal,
  Dielectric
};

struct Material {
  MaterialType type;
  glm::vec3 albedo = {0.0f, 0.0f, 0.0f};
  float fuzz = 0;
  float refractionIndex = 0;
};

struct Sphere {
  glm::vec3 center;
  float radius;
  int materialIndex;

  Sphere(glm::vec3 center_, float radius_, int materialIndex_)
      : center(center_), radius(radius_), materialIndex(materialIndex_) {
  }
};

struct Scene {
  Scene() = default;
//  void clear() { m_objects.clear(); }
//  ObjectList& objects() { return m_objects; }
  std::vector<Sphere> spheres;
  std::vector<Material> materials;

};

#endif //RAY_TRACE_SRC_SCENE_HPP_
