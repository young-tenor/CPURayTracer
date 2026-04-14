#pragma once

#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 pos;
    float strength;

    Light(glm::vec3 pos, float strength) : pos(pos), strength(strength) {}
};
