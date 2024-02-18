//
// Created by Tony Adriansen on 2/4/24.
//

#include "Renderer.hpp"
#include "math/Ray.hpp"
#include <algorithm>
#include <thread>
#include "Camera.hpp"
#include "Scene.hpp"
#include <fstream>
#include <iostream>

namespace Utils {

static uint32_t convertToRGBA(const glm::vec4& color) {
  // linear to gamma transform, inverse of gamma 2, sqrt
  auto r = (uint8_t) (std::sqrt(color.r) * 255.999f);
  auto g = (uint8_t) (std::sqrt(color.g) * 255.999f);
  auto b = (uint8_t) (std::sqrt(color.b) * 255.999f);
  auto a = (uint8_t) (color.a * 255.999f);

  uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

static glm::ivec3 fromPackedRGBA(uint32_t packed) {
  return {packed & 0xFF, (packed & 0xFF00) >> 8, (packed & 0xFF0000) >> 16};
}

}

bool Renderer::hitAny(const Ray& r, Interval rayT, HitRecord& rec) const {
  HitRecord tempRec;
  bool hitAny = false;
  float closestSoFar = rayT.max;

  for (const auto& sphere : m_activeScene->spheres) {
    if (hitSphere(sphere, r, Interval(rayT.min, closestSoFar), tempRec)) {
      hitAny = true;
      tempRec.materialIndex = sphere.materialIndex;
      closestSoFar = tempRec.t;
      rec = tempRec;
    }
  }

  return hitAny;
}

glm::vec3 Renderer::rayColor(const Ray& r, int depth) {
  // return if no bounces left
  if (depth <= 0) {
    // Return sky if no hit
    glm::vec3 unitDir = glm::normalize(r.direction);
    //[-1,1 ] -> [0,1]
    float sky = (unitDir.y + 1.f) * 0.5f;
    // lerp between sky color and white
    return (1.0f - sky) * glm::vec3(1, 1, 1) + sky * glm::vec3(0.5, 0.7, 1.0);
  }

  HitRecord hitRec;
  // floating point inaccuracies make the correction factor necessary.
  // otherwise, the ray might intersect on the wrong side of the sphere
  if (hitAny(r, Interval(0.001, Math::infinity), hitRec)) {
    auto& material = m_activeScene->materials[hitRec.materialIndex];
    bool scattered = false;
    glm::vec3 albedo;
    Ray scatteredRay;
    if (material.type == Metal) {
      // metals reflect the ray hit perfectly
      glm::vec3 reflectedDirection = Math::reflectVec3(r.direction, hitRec.normal);
      // fuzz factor is
      scatteredRay = {hitRec.point, reflectedDirection + material.fuzz * Math::randomUnitVec3()};
      albedo = material.albedo;
      // if scattered ray is not absorbed (is same direction as normal, it's scattered)
      // in some cases, the fuzz factor may cause scattered direction to point in
      scattered = glm::dot(scatteredRay.direction, hitRec.normal) > 0;
    } else if (material.type == Lambertian) {
      // Lambertian distribution, random point on unit sphere tangent to hit point
      glm::vec3 scatterDirection = hitRec.normal + Math::randomUnitVec3();
      // if random unit vector is opposite normal, 0 scatter direction vector == bad
      if (Math::nearZeroVec3(scatterDirection))
        scatterDirection = hitRec.normal;

      scatteredRay = {hitRec.point, scatterDirection};
      albedo = material.albedo;
      scattered = true;
    } else if (material.type == Dielectric) {
      // if front face normal, n1 is air, n2 is material
      float refractionRatio = hitRec.frontFace ? (1.0f / material.refractionIndex) : material.refractionIndex;
      glm::vec3 unitDirection = glm::normalize(r.direction);
      float cos_theta = fmin(glm::dot(-unitDirection, hitRec.normal), 1.0f);
      float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
      bool cannotRefract = refractionRatio * sin_theta > 1.0f;
      glm::vec3 scatteredDirection;
      if (cannotRefract || Math::reflectance(cos_theta, refractionRatio) > Math::randomFloat()) {
        scatteredDirection = Math::reflectVec3(unitDirection, hitRec.normal);
      } else {
        scatteredDirection = Math::refract(unitDirection, hitRec.normal, refractionRatio);
      }

      albedo = glm::vec3(1.0f);
      scatteredRay = {hitRec.point, scatteredDirection};
      scattered = true;
    }

    glm::vec3 color =  scattered ? albedo * rayColor(scatteredRay, depth - 1) : glm::vec3(0.0f);
    return color;
  }

  // Return sky if no hit
  glm::vec3 unitDir = glm::normalize(r.direction);
  //[-1,1 ] -> [0,1]
  float sky = (unitDir.y + 1.f) * 0.5f;
  // lerp between sky color and white
  return (1.0f - sky) * glm::vec3(1, 1, 1) + sky * glm::vec3(0.5, 0.7, 1.0);
}

Renderer::Renderer() {
  m_settings.accumulate = true;
  m_settings.maxBounceDepth = 20;
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

#if PAR_EX
  m_ImageHorizontalIter.resize(m_width);
  m_ImageVerticalIter.resize(m_height);
  for (int i = 0; i < m_width; i++)
    m_ImageHorizontalIter[i] = i;
  for (int i = 0; i < m_height; i++)
    m_ImageVerticalIter[i] = i;
#endif
}

void Renderer::render(const Camera& camera, const Scene& scene) {
  m_activeCamera = &camera;
  m_activeScene = &scene;
  if (m_frameIndex == 1) {
    memset(m_accumulationData, 0, m_width * m_height * sizeof(glm::vec4));
  }

  // can't use parallel execution on clang, so use futures with thread pool instead

#if PAR_EX
  std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
                [this](int y) {
                  std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
                                [this, y](int x) {
                                  glm::vec4 color = colorPerPixel(x, y);
                                  m_accumulationData[y * m_width + x] += color;
                                  glm::vec4 accumulatedColor = m_accumulationData[y * m_width + x];
                                  accumulatedColor /= m_frameIndex;
                                  accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0), glm::vec4(1));
                                  m_imageData[y * m_width + x] = Utils::convertToRGBA(accumulatedColor);
                                });
                });
#else
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

//
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      glm::vec4 color = colorPerPixel(x, y);
      m_accumulationData[y * m_width + x] += color;
      glm::vec4 accumulatedColor = m_accumulationData[y * m_width + x];
      accumulatedColor /= m_frameIndex;
      accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0), glm::vec4(1));
      m_imageData[y * m_width + x] = Utils::convertToRGBA(accumulatedColor);
    }
  }

#endif

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
bool Renderer::hitSphere(const Sphere& sphere, const Ray& r, Interval rayT, HitRecord& rec) const {
  const glm::vec3 oc = r.origin - sphere.center;
  const float half_b = (glm::dot(r.direction, oc));
  const float a = glm::dot(r.direction, r.direction);
  const float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
  const float discriminant = half_b * half_b - a * c;
  if (discriminant < 0) return false;
  const float sqrtDiscriminant = std::sqrt(discriminant);

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
  glm::vec3 outwardNormal = (rec.point - sphere.center) / sphere.radius;
  rec.setFaceNormal(r, outwardNormal);

  return true;
}

void Renderer::screenshot() {

  std::ofstream out("screenshot.ppm");
  if (!out.is_open()) {
    return;
  }
  out << "P3\n" << m_width << ' ' << m_height << "\n255\n";
  for (int j = 0; j < m_height; j++) {
    for (int i = 0; i < m_width; i++) {
        glm::vec3 color = Utils::fromPackedRGBA(m_imageData[j * m_width + i]);
        out << color.r << ' ' << color.g << ' ' << color.b << '\n';
    }
  }
  out.close();

}
