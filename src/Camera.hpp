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
  void onResize(int width, int height);
  [[nodiscard]] Ray getRay(int x, int y) const;
  [[nodiscard]] glm::vec3 pixelSampleSquare() const;

  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec3 pixel00Loc{};
  glm::vec3 pixelDeltaU{};
  glm::vec3 pixelDeltaV{};
  float focalLength = 1.0;
  float viewportWidth{};
  float viewportHeight{};
  float m_movementSpeed{};
 private:


};

#endif //RAY_TRACE_SRC_CAMERA_HPP_
