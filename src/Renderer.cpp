//
// Created by Tony Adriansen on 2/4/24.
//

#include "Renderer.hpp"
#include "Ray.hpp"
#include <algorithm>
#include <execution>
#include <thread>

namespace Utils {

static uint32_t convertToRGBA(const glm::vec4& color) {
  auto r = (uint8_t) (color.r * 255.0f);
  auto g = (uint8_t) (color.g * 255.0f);
  auto b = (uint8_t) (color.b * 255.0f);
  auto a = (uint8_t) (color.a * 255.0f);

  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

}

glm::vec3 Renderer::rayColor(const Ray& r, int depth) {
  HitRecord hitRec;
  // return if no bounces left
  if (depth <= 0) {
    return {0,0,0};
  }

  if (m_activeScene->hitAny(r, Interval(0.001, Math::infinity), hitRec)) {
    // [-1,1] -> [0,1]
    glm::vec3 direction = Math::randOnHemisphere(hitRec.normal);
    // floating point inaccuracies make the correction factor necessary.
    // otherwise, the ray might intersect on the wrong side of the sphere
//    return 0.5f * rayColor(Ray(hitRec.point + hitRec.normal * CORRECTION_FACTOR, direction), depth-1);
    return 0.5f * rayColor(Ray(hitRec.point, direction), depth-1);
    return 0.5f * (hitRec.normal + glm::vec3(1, 1, 1));
  }

  glm::vec3 unitDir = glm::normalize(r.direction);

  //[-1,1 ] -> [0,1]
  float sky = (unitDir.y + 1.f) * 0.5f;
  // lerp between sky color and white
  return (1.0f - sky) * glm::vec3(1, 1, 1) + sky * glm::vec3(0.5, 0.7, 1.0);
}

Renderer::Renderer() {
  m_settings.accumulate = true;
  m_settings.maxBounceDepth = 5;
  m_imageData = new uint32_t[WIDTH * HEIGHT];
  m_numThreads = static_cast<int>(std::thread::hardware_concurrency());
}

Renderer::~Renderer() {
  delete[] m_imageData;
  delete[] m_accumulationData;
}

void Renderer::onResize(int width, int height) {
  if (m_finalTexture) {
    if (m_finalTexture->width() == width && m_finalTexture->height() == height) {
      return;
    }
    m_finalTexture->resize(m_width, m_height);
    reset();
  } else {
    m_finalTexture = std::make_shared<Texture<GL_TEXTURE_2D>>(width, height);
    m_finalTexture->init();
    m_finalTexture->setWrapFilter(GL_CLAMP_TO_BORDER);
    m_finalTexture->setMagFilter(GL_LINEAR, GL_LINEAR);
    m_finalTexture->bind();
    m_finalTexture->resize(width, height);
  }

  m_width = width;
  m_height = height;
  delete[] m_imageData;
  m_imageData = new uint32_t[m_width * m_height];
  delete[] m_accumulationData;
  m_accumulationData = new glm::vec4[m_width * m_height];
}

void Renderer::render(const Camera& camera, const Scene& scene) {
  m_activeCamera = &camera;
  m_activeScene = &scene;
  if (m_frameIndex == 1) {
    memset(m_accumulationData, 0, m_width * m_height * sizeof(glm::vec4));
  }
  int portion_height = m_height / m_numThreads;
  int start_y = 0, end_y = 0;
  std::vector<std::future<void>> futures;
  for (int i = 0; i < m_numThreads; ++i) {
    start_y = i * portion_height;
    end_y = (i == m_numThreads - 1) ? m_height : (i + 1) * portion_height;

    futures.push_back(m_threadPool.submit_task([start_y, end_y, this]() {
      for (int y = start_y; y < end_y; ++y) {
        for (int x = 0; x < m_width; ++x) {
          glm::vec4 color = colorPerPixel(x, y);
          m_accumulationData[y * m_width + x] += color;
          glm::vec4 accumulatedColor = m_accumulationData[y * m_width + x];
          accumulatedColor /= m_frameIndex;
          accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0), glm::vec4(1));
          m_imageData[y * m_width + x] = Utils::convertToRGBA(accumulatedColor);
        }
      }
    }));
  }

  for (auto& future : futures) {
    future.wait();
  }

  m_finalTexture->load2dData(m_imageData, static_cast<int>(m_width), static_cast<int>(m_height));

  if (m_settings.accumulate) {
    m_frameIndex++;
  } else {
    m_frameIndex = 1;
  }

}

glm::vec4 Renderer::colorPerPixel(int x, int y) {
  Ray r = m_activeCamera->getRay(x, y);
  glm::vec3 color = rayColor(r, m_settings.maxBounceDepth);
  return {color, 1.0f};
}

void Renderer::reset() {
  delete[] m_imageData;
  m_imageData = new uint32_t[m_width * m_height];
  delete[] m_accumulationData;
  m_accumulationData = new glm::vec4[m_width * m_height];
  m_frameIndex = 1;
}