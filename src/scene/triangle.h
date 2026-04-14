#pragma once

#include "scene/object.h"
#include <glm/glm.hpp>

class Triangle : public Object {
public:
    glm::vec3 v0, v1, v2;

    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, Material material)
        : Object(material), v0(v0), v1(v1), v2(v2) {}

    bool intersect(Ray& ray, Hit& hit) override {
        constexpr float epsilon = 1e-6f;

        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;
        glm::vec3 N = glm::cross(e1, e2);

        // Step 1: ray-plane intersection
        float denom = glm::dot(N, ray.dir);
        if (glm::abs(denom) < epsilon)
            return false;

        float t = glm::dot(N, v0 - ray.orig) / denom;
        if (t < epsilon)
            return false;

        // Step 2: check if the hit point is inside the triangle
        glm::vec3 P = ray.orig + t * ray.dir;
        if (glm::dot(N, glm::cross(e1, P - v0)) < 0.0f) return false;
        if (glm::dot(N, glm::cross(v2 - v1, P - v1)) < 0.0f) return false;
        if (glm::dot(N, glm::cross(-e2, P - v2)) < 0.0f) return false;

        hit.dist = t;
        hit.normal = glm::normalize(N);
        hit.material = material;
        return true;
    }
};
