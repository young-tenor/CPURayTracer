#pragma once

#include "scene/object.h"
#include <glm/glm.hpp>

class Triangle : public Object {
public:
    glm::vec3 v0, v1, v2;
    glm::vec2 uv0, uv1, uv2;

    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2, Material material)
        : Object(material), v0(v0), v1(v1), v2(v2), uv0(uv0), uv1(uv1), uv2(uv2) {}

    bool intersect(const Ray& ray, Hit& hit) override {
        const float epsilon = 1e-6f;

        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;
        glm::vec3 N = glm::cross(e1, e2);

        float denom = glm::dot(N, ray.dir);
        if (glm::abs(denom) < epsilon)
            return false;

        float t = glm::dot(N, v0 - ray.orig) / denom;
        if (t < epsilon)
            return false;

        glm::vec3 P = ray.orig + t * ray.dir;
        auto c0 = glm::cross(e1, P - v0);
        auto c1 = glm::cross(v2 - v1, P - v1);
        auto c2 = glm::cross(-e2, P - v2);
        if (glm::dot(N, c0) < 0.0f) return false;
        if (glm::dot(N, c1) < 0.0f) return false;
        if (glm::dot(N, c2) < 0.0f) return false;

        float dn = glm::dot(N, N);
        float lambda0 = glm::dot(N, c1) / dn;
        float lambda1 = glm::dot(N, c2) / dn;
        float lambda2 = glm::dot(N, c0) / dn;

        hit.dist = t;
        hit.normal = glm::normalize(N);
        hit.material = material;
        hit.uv = lambda0 * uv0 + lambda1 * uv1 + lambda2 * uv2;

        hit.uv *= 4.0f;

        return true;
    }
};
