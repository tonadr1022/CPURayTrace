//
// Created by Tony Adriansen on 2/12/24.
//

#ifndef RAY_TRACE_SRC_INTERVAL_HPP_
#define RAY_TRACE_SRC_INTERVAL_HPP_
#include "raytrace_pch.hpp"
class Interval {
 public:
  float min, max;
  Interval() : min(Math::infinity), max(-Math::infinity) {}
  Interval(float _min, float _max) : min(_min), max(_max) {}

  [[nodiscard]] bool contains(float x) const {
    return min <= x && x <= max;
  }

  [[nodiscard]] bool surrounds(float x) const {
    return min < x && x < max;
  }

  [[nodiscard]] float clamp(float x) const {
    return x < min ? min : x > max ? max : x;
  }

  static const Interval empty, universe;
};

const static Interval empty(Math::infinity, -Math::infinity);
const static Interval universe(-Math::infinity, Math::infinity);

#endif //RAY_TRACE_SRC_INTERVAL_HPP_
