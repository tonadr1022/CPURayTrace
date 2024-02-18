//
// Created by Tony Adriansen on 2/9/24.
//

#ifndef RAY_TRACE_SRC_CAMERA_HPP_
#define RAY_TRACE_SRC_CAMERA_HPP_
#include <glm/glm.hpp>
#include "math/Ray.hpp"

class Camera {
 public:
  Camera();
  void update(float dt);
  void onResize();

  [[nodiscard]] Ray getRay(int x, int y) const;
  [[nodiscard]] glm::vec3 pixelSampleSquare() const;
  void setDimensions(int width, int height);
  void setFOV(float fov);
  void setLookFrom(glm::vec3 pos);
  void setLookAt(glm::vec3 pos);
  void setVUp(glm::vec3 pos);
  [[nodiscard]] float fov() const { return m_fov; }

  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec3 pixel00Loc{};
  glm::vec3 pixelDeltaU{};
  glm::vec3 pixelDeltaV{};
  float focalLength = 1.0;
  float defocusAngle = 0.0f;
  float focusDistance = 10.0f;

  float viewportWidth{};
  float viewportHeight{};
  float m_movementSpeed{};
 private:
  int m_width = 0, m_height = 0;
  glm::vec3 lookFrom, lookAt, vup;
  float m_fov = 90.0f;

  // basis vectors, u points to camera right, v points to camera up, w points opposite view dir
  glm::vec3 u, v, w;
  glm::vec3 defocusDiskU, defocusDiskV;
  glm::vec3 defocusDiskSample() const;


};

#endif //RAY_TRACE_SRC_CAMERA_HPP_
