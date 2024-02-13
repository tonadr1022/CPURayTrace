//
// Created by Tony Adriansen on 2/4/24.
//

#ifndef RAY_TRACE__COLOR_HPP_
#define RAY_TRACE__COLOR_HPP_
#include "raytrace_pch.hpp"

void writeColor(std::ostream &out, const glm::vec3 &pixelColor) {
  out << static_cast<int>(255.999 * pixelColor.x) << ' '
      << static_cast<int>(255.999 * pixelColor.y) << ' '
      << static_cast<int>(255.999 * pixelColor.z) << '\n';
}

#endif //RAY_TRACE__COLOR_HPP_
