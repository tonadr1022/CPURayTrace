//
// Created by Tony Adriansen on 2/16/24.
//

#ifndef RAY_TRACE_SRC_SCENEMANAGER_HPP_
#define RAY_TRACE_SRC_SCENEMANAGER_HPP_

#include "Scene.hpp"

class SceneManager {
 public:
  void setActiveScene(const std::shared_ptr<Scene>& scene);
  void createSphere(glm::vec3 pos, float radius, int materialIndex);
  void createMaterial(glm::vec3 albedo = {1.0f, 1.0f, 1.0f},
                    float roughness = 1.0f,
                    float metallic = 1.0f,
                    glm::vec3 emissionColor = {0.0f, 0.0f, 0.0f},
                    float emissionPower = 0.0f);
  [[nodiscard]] Scene* scene() const { return m_activeScene.get(); }
 private:
  std::shared_ptr<Scene> m_activeScene = nullptr;
};

#endif //RAY_TRACE_SRC_SCENEMANAGER_HPP_
