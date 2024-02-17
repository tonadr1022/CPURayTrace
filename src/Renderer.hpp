//
// Created by Tony Adriansen on 2/4/24.
//

#ifndef RAY_TRACE__RENDERER_HPP_
#define RAY_TRACE__RENDERER_HPP_

#include "raytrace_pch.hpp"
#include "app/Window.hpp"
#include "gl/FrameBuffer.hpp"
#include "gl/Texture.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include <BS_thread_pool.hpp>

class Renderer {
 public:
  struct Settings {
    bool accumulate;
    int maxBounceDepth;
  };

  explicit Renderer();
  ~Renderer();
  void render(const Camera& camera, const Scene& scene);
  [[nodiscard]] std::shared_ptr<Texture<GL_TEXTURE_2D>> getFinalTexture() const { return m_finalTexture; }
  void onResize(int width, int height);

  void reset();
  Settings& settings() { return m_settings; }
 private:
  Settings m_settings{};
  glm::vec4 colorPerPixel(int x, int y);
  glm::vec3 rayColor(const Ray& r, int depth);
  bool hitAny(const Ray& r, Interval rayT, HitRecord& rec) const;

  int m_numThreads;
  BS::thread_pool m_threadPool;

  std::shared_ptr<Texture<GL_TEXTURE_2D>> m_finalTexture;
  const Camera* m_activeCamera = nullptr;
  const Scene* m_activeScene = nullptr;
  int m_width{}, m_height{};
  uint32_t* m_imageData = nullptr;
  glm::vec4* m_accumulationData = nullptr;
  int m_frameIndex = 1;

#if PAR_EX
  std::vector<int> m_ImageHorizontalIter, m_ImageVerticalIter;
#endif

};

#endif //RAY_TRACE__RENDERER_HPP_
