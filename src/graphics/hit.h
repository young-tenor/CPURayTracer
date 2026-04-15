#pragma once

#include <glm/glm.hpp>
#include "graphics/material.h"

class Hit {
public:
    glm::vec3 normal;
    glm::vec2 uv = glm::vec2(0.0f);
    float dist = 0.0f;
    Material material { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f };
};
