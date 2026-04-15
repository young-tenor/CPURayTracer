#pragma once

#include "graphics/texture.h"
#include <glm/glm.hpp>

#include <memory>

class Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float reflection;
    std::shared_ptr<Texture> texture;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, float reflection = 0.0f, std::shared_ptr<Texture> texture = nullptr)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), reflection(reflection), texture(std::move(texture)) {}
};
