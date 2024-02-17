//
// Created by Tony Adriansen on 2/12/24.
//

#ifndef RAY_TRACE_SRC_SCENE_HPP_
#define RAY_TRACE_SRC_SCENE_HPP_
#include "raytrace_pch.hpp"
#include "Hittable.hpp"

using ObjectList = std::vector<std::shared_ptr<Hittable>>;

struct Material {
  glm::vec3 albedo{1.0f};
  float roughness = 0.0f;
  float metallic = 0.0f;
  glm::vec3 emissionColor{0.0f};
  float emissionPower = 0.0f;
  [[nodiscard]] glm::vec3 emission() const { return emissionColor * emissionPower; }

  Material(glm::vec3 albedo_, float roughness_, float metallic_, glm::vec3 emissionColor_, float emissionPower_)
      : albedo(albedo_),
        roughness(roughness_),
        metallic(metallic_),
        emissionColor(emissionColor_),
        emissionPower(emissionPower_) {}
};

struct Scene {
  Scene() = default;
//  void clear() { m_objects.clear(); }
//  ObjectList& objects() { return m_objects; }



  std::vector<std::shared_ptr<Hittable>> hittables;
  std::vector<std::shared_ptr<Material>> materials;

};

#endif //RAY_TRACE_SRC_SCENE_HPP_
