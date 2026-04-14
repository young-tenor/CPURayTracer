#pragma once

#include "scene/object.h"
#include <glm/glm.hpp>

class Sphere : public Object {
public:
    glm::vec3 center;
    float radius;

    Sphere(glm::vec3 center, float radius) : center(center), radius(radius) {}

    // https://en.wikipedia.org/wiki/Line-sphere_intersection
    bool intersect(Ray& ray, Hit& hit) override {
        glm::vec3 oc = ray.orig - center;
        float a = glm::dot(ray.dir, ray.dir);
        float b = glm::dot(oc, ray.dir);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;
        if (discriminant < 0.0f)
            return false;
        float t = (-b - glm::sqrt(discriminant)) / a;
        hit.dist = t;
        hit.normal = glm::normalize(ray.orig + t * ray.dir - center);
        return true;
    }
};
