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

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::shared_ptr<Texture> texture = nullptr)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), texture(std::move(texture)) {}
};
