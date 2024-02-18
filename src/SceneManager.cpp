//
// Created by Tony Adriansen on 2/16/24.
//

#include "SceneManager.hpp"
#include "Scene.hpp"

void SceneManager::createSphere(glm::vec3 pos, float radius, int materialIndex) {
  m_activeScene->spheres.emplace_back(pos, radius, materialIndex);
}

void SceneManager::setActiveScene(const std::shared_ptr<Scene>& scene) {
  m_activeScene = scene;
}

void SceneManager::createLambertian(glm::vec3 albedo) {
  Material material;
  material.type = Lambertian;
  material.albedo = albedo;
  m_activeScene->materials.push_back(material);
}

void SceneManager::createMetal(glm::vec3 albedo, float fuzz) {
  Material material;
  material.type = Metal;
  material.albedo = albedo;
  material.fuzz = fuzz;
  m_activeScene->materials.push_back(material);
}

void SceneManager::createDielectric(float refractionIndex) {
  Material material;
  material.type = Dielectric;
  material.albedo = {1.0f,1.0f,1.0f};
  material.refractionIndex = refractionIndex;
  m_activeScene->materials.push_back(material);
}