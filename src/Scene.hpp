//
// Created by Tony Adriansen on 2/12/24.
//

#ifndef RAY_TRACE_SRC_SCENE_HPP_
#define RAY_TRACE_SRC_SCENE_HPP_
#include "raytrace_pch.hpp"
#include "Hittable.hpp"

using ObjectList = std::vector<std::shared_ptr<Hittable>>;
class Scene {
 public:
  Scene() = default;
  void clear() { m_objects.clear(); }
  ObjectList& objects() { return m_objects; }

  void addHittable(const std::shared_ptr<Hittable>& hittable) {
    m_objects.push_back(hittable);
  }

  bool hitAny(const Ray& r, Interval rayT, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAny = false;
    float closestSoFar = rayT.max;

    for (const auto& object : m_objects) {
      if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
        hitAny = true;
        closestSoFar = tempRec.t;
        rec = tempRec;
      }
    }
    return hitAny;
  }

 private:
  ObjectList m_objects;
};

#endif //RAY_TRACE_SRC_SCENE_HPP_
