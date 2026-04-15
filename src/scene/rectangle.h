#pragma once

#include "scene/object.h"
#include "scene/triangle.h"
#include <glm/glm.hpp>

// Axis-aligned or arbitrary quad defined by four coplanar vertices (v0, v1, v2, v3 in order).
// Split into two triangles: (v0, v1, v2) and (v0, v2, v3).
class Rectangle : public Object {
public:
    Triangle t0, t1;

    Rectangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Material material)
        : Object(material),
          t0(v0, v1, v2, {0,0}, {1,0}, {1,1}, material),
          t1(v0, v2, v3, {0,0}, {1,1}, {0,1}, material) {}

    bool intersect(const Ray& ray, Hit& hit) override {
        Hit h0, h1;
        bool hit0 = t0.intersect(ray, h0);
        bool hit1 = t1.intersect(ray, h1);

        if (!hit0 && !hit1)
            return false;

        if (hit0 && hit1)
            hit = (h0.dist < h1.dist) ? h0 : h1;
        else
            hit = hit0 ? h0 : h1;

        return true;
    }
};
