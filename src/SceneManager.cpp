//
// Created by Tony Adriansen on 2/16/24.
//

#include "SceneManager.hpp"

#include <utility>
#include "Sphere.hpp"

void SceneManager::createSphere(glm::vec3 pos, float radius, int materialIndex) {
  m_activeScene->hittables.push_back(std::make_shared<Sphere>(pos, radius, materialIndex));
}

void SceneManager::setActiveScene(const std::shared_ptr<Scene>& scene) {
  m_activeScene = scene;
}

void SceneManager::createMaterial(glm::vec3 albedo,
                                  float roughness,
                                  float metallic,
                                  glm::vec3 emissionColor,
                                  float emissionPower) {
  m_activeScene->materials.push_back(std::make_shared<Material>(albedo, roughness,
                                                                metallic, emissionColor,
                                                                emissionPower));
}
