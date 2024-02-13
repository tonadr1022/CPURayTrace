//
// Created by Tony Adriansen on 2/9/24.
//

#include "Camera.hpp"
#include "raytrace_pch.hpp"
#include "input/Keyboard.hpp"

Camera::Camera() = default;

void Camera::update(float dt) {

}

void Camera::onResize(int width, int height) {
  m_movementSpeed = 5.0f;

  focalLength = 1.0;
  // normalized device coordinates
  viewportHeight = 2.0;
  viewportWidth = viewportHeight * static_cast<float>(width) / static_cast<float>(height);

  position = glm::vec3(0, 0, 0);

  // vectors across horizontal and down vertical viewport edges
  glm::vec3 viewportU = glm::vec3(viewportWidth, 0, 0);
  glm::vec3 viewportV = glm::vec3(0, -viewportHeight, 0);

  pixelDeltaU = viewportU / (float) width;
  pixelDeltaV = viewportV / (float) height;
  // from camera center to the viewport center, then to upper left of viewport
  glm::vec3 viewportUpperLeft = position - glm::vec3(0, 0, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
  // center of the upper left pixel, half the distance down and to the right of the upper left corner of viewport.
  pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
}
Ray Camera::getRay(int x, int y) const {
  glm::vec3 pixelCenter = pixel00Loc + (pixelDeltaU * (float) x)
      + (pixelDeltaV * float(y));
  glm::vec3 pixelSample = pixelCenter + pixelSampleSquare();
  glm::vec3 rayOrigin = position;
  glm::vec3 rayDir = pixelSample - rayOrigin;

  return {rayOrigin, rayDir};
}
glm::vec3 Camera::pixelSampleSquare() const {
  float px = -0.5f + Math::randomFloat();
  float py = -0.5f + Math::randomFloat();
  return (px * pixelDeltaU) + (py * pixelDeltaV);
}
