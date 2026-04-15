#pragma once

#include "graphics/ray.h"
#include "graphics/hit.h"
#include "graphics/material.h"

class Object {
public:
    Material material;

    Object(Material material) : material(material) {}

    virtual bool intersect(const Ray& ray, Hit& hit) = 0;
    virtual ~Object() = default;
};
