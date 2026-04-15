#pragma once

#include <glm/glm.hpp>

class Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float reflection;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
};
