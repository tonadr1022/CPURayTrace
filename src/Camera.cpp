//
// Created by Tony Adriansen on 2/9/24.
//

#include "Camera.hpp"
#include "raytrace_pch.hpp"

Camera::Camera() = default;

void Camera::update(float dt) {

}

void Camera::setDimensions(int width, int height) {
  if (m_width != width || m_height != height) {
    m_width = width;
    m_height = height;
    onResize();
  }
}

void Camera::setFOV(float fov_) {
  m_fov = fov_;
  onResize();
}

void Camera::setLookAt(glm::vec3 pos) {
  lookAt = pos;
  onResize();
}

void Camera::setLookFrom(glm::vec3 pos) {
  lookFrom = pos;
  onResize();
}

void Camera::setVUp(glm::vec3 pos) {
  vup = pos;
  onResize();
}

void Camera::onResize() {
  m_movementSpeed = 5.0f;

  position = lookFrom;
  focalLength = glm::length(lookFrom - lookAt);

  float theta = Math::toRadians(m_fov);
  float h = tan(theta / 2);
  viewportHeight = 2.0f * h * focalLength;
  viewportWidth = viewportHeight * static_cast<float>(m_width) / static_cast<float>(m_height);

  w = glm::normalize(lookFrom - lookAt);
  u = glm::normalize(glm::cross(vup, w));
  v = glm::cross(w, u);

  // vectors across horizontal and down vertical viewport edges
//  glm::vec3 viewportU = glm::vec3(viewportWidth, 0, 0);
//  glm::vec3 viewportV = glm::vec3(0, -viewportHeight, 0);
  glm::vec3 viewportU = viewportWidth * u;
  glm::vec3 viewportV = viewportHeight * -v;

  pixelDeltaU = viewportU / (float) m_width;
  pixelDeltaV = viewportV / (float) m_height;
  // from camera center to the viewport center, then to upper left of viewport
  glm::vec3 viewportUpperLeft = position - focalLength * w - viewportU / 2.0f - viewportV / 2.0f;
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
