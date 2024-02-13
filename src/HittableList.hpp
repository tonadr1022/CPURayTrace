//
// Created by Tony Adriansen on 2/12/24.
//

#ifndef RAY_TRACE_SRC_HITTABLELIST_HPP_
#define RAY_TRACE_SRC_HITTABLELIST_HPP_

#include "Hittable.hpp"

class HittableList : public Hittable {
 public:
  std::vector<std::shared_ptr<Hittable>> objects;
  HittableList() = default;
  void clear() { objects.clear(); }


};

#endif //RAY_TRACE_SRC_HITTABLELIST_HPP_
