#pragma once

#include "graphics/ray.h"
#include "graphics/hit.h"

class Object {
public:
    virtual bool intersect(Ray& ray, Hit& hit) = 0;
    virtual ~Object() = default;
};
