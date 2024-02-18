//
// Created by Tony Adriansen on 2/16/24.
//

#ifndef RAY_TRACE_SRC_SCENEMANAGER_HPP_
#define RAY_TRACE_SRC_SCENEMANAGER_HPP_

#include "raytrace_pch.hpp"
#include "Scene.hpp"

class SceneManager {
 public:
  void setActiveScene(const std::shared_ptr<Scene>& scene);
  void createSphere(glm::vec3 pos, float radius, int materialIndex);
  void createLambertian(glm::vec3 albedo);
  void createMetal(glm::vec3 albedo, float fuzz);
  void createDielectric(float refractionIndex);
  [[nodiscard]] Scene* scene() const { return m_activeScene.get(); }
 private:


  std::shared_ptr<Scene> m_activeScene = nullptr;
};

#endif //RAY_TRACE_SRC_SCENEMANAGER_HPP_
