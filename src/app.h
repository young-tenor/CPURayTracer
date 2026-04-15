#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/shader.h"
#include "graphics/ray.h"
#include "graphics/hit.h"
#include "graphics/sampler.h"
#include "graphics/texture.h"
#include "graphics/material.h"
#include "scene/object.h"
#include "scene/rectangle.h"
#include "scene/light.h"

#include <glm/glm.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <vector>

class App {
public:
    bool init()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW\n";
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, "CPU Ray Tracer", nullptr, nullptr);
        if (!window)
        {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetKeyCallback(window, keyCallback);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            std::cerr << "Failed to initialize GLAD\n";
            glfwDestroyWindow(window);
            window = nullptr;
            glfwTerminate();
            return false;
        }

        glViewport(0, 0, width, height);

        aspectRatio = static_cast<float>(width) / static_cast<float>(height);

        pixels.resize(width * height);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
                     GL_RGBA, GL_FLOAT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        shader.emplace(SHADER_DIR "quad.vert", SHADER_DIR "quad.frag");
        shader->use();
        shader->setInt("screenTexture", 0);

        static const float quadVertices[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        cameraCenter = glm::vec3(0.0f, 0.0f, 1.0f);
        light.emplace(glm::vec3(0.0f, 1.0f, 1.0f), 1.0f);
        sampler.emplace(Sampler::Wrap::repeat, Sampler::Filter::nearest);

        std::vector<glm::vec3> checkerImage(4 * 4);
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                checkerImage[j * 4 + i] = (i + j) % 2 == 0 ? glm::vec3(1.0f) : glm::vec3(0.0f);
        auto checkerTexture = std::make_shared<Texture>(4, 4, std::move(checkerImage));

        Material material(
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.7f, 0.7f, 0.7f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            32.0f,
            checkerTexture
        );

        objects.push_back(std::make_unique<Rectangle>(
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(-0.5f, 0.5f, 0.0f),
            material
        ));

        return true;
    }

    void update()
    {
        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++)
            {
                auto color = samplePixel(i, j);
                pixels[j * width + i] = glm::vec4(color, 1.0f);
            }

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                        GL_RGBA, GL_FLOAT, pixels.data());
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool shouldClose() const
    {
        return glfwWindowShouldClose(window);
    }

    ~App()
    {
        if (window)
        {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteTextures(1, &texture);
            if (shader)
                glDeleteProgram(shader->ID);
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }

private:
    static const int width = 1280 * 2;
    static const int height = 720 * 2;

    GLFWwindow* window = nullptr;

    float aspectRatio = 0.0f;
    glm::vec3 cameraCenter = glm::vec3(0.0f);
    std::optional<Light> light;
    std::optional<Sampler> sampler;
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<glm::vec4> pixels;

    unsigned int texture = 0;
    unsigned int vao = 0;
    unsigned int vbo = 0;
    std::optional<Shader> shader;

    static void framebufferSizeCallback(GLFWwindow* /*window*/, int w, int h)
    {
        glViewport(0, 0, w, h);
    }

    static void keyCallback(GLFWwindow* win, int key, int /*scancode*/, int action, int /*mods*/)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(win, GLFW_TRUE);
    }

    glm::vec3 traceRay(const Ray& ray) const
    {
        Hit hit;
        bool hitAnything = false;
        auto closestDist = std::numeric_limits<float>::infinity();

        for (const auto& object : objects)
        {
            Hit tempHit;
            if (object->intersect(ray, tempHit) && tempHit.dist < closestDist)
            {
                closestDist = tempHit.dist;
                hit = tempHit;
                hitAnything = true;
            }
        }

        if (!hitAnything)
            return glm::vec3(0.0f);

        auto hitPoint = ray.orig + hit.dist * ray.dir;
        auto N = hit.normal;
        auto L = glm::normalize(light->pos - hitPoint);
        auto V = glm::normalize(cameraCenter - hitPoint);
        auto H = glm::normalize(L + V);

        Ray shadowRay;
        shadowRay.orig = hitPoint + N * 1e-4f;
        shadowRay.dir = L;
        float lightDist = glm::length(light->pos - hitPoint);
        bool inShadow = false;
        for (const auto& object : objects)
        {
            Hit shadowHit;
            if (object->intersect(shadowRay, shadowHit) && shadowHit.dist < lightDist)
            {
                inShadow = true;
                break;
            }
        }

        glm::vec3 diffuseColor = hit.material.texture
            ? hit.material.texture->sample(*sampler, hit.uv)
            : hit.material.diffuse;

        glm::vec3 color = hit.material.ambient;
        if (!inShadow)
        {
            auto diffuse = diffuseColor * light->strength * glm::max(glm::dot(N, L), 0.0f);
            auto specular = hit.material.specular * light->strength * glm::pow(glm::max(glm::dot(N, H), 0.0f), hit.material.shininess);
            color += diffuse + specular;
        }
        return color;
    }

    glm::vec3 samplePixel(int i, int j) const
    {
        glm::vec3 accum(0.0f);
        for (int sj = 0; sj < 2; sj++)
        {
            for (int si = 0; si < 2; si++)
            {
                glm::vec3 sampleCenter(
                    (2.0f * (i + (si + 0.5f) * 0.5f) / static_cast<float>(width) - 1.0f) * aspectRatio,
                    (2.0f * (j + (sj + 0.5f) * 0.5f) / static_cast<float>(height) - 1.0f),
                    0.0f
                );

                Ray ray;
                ray.orig = cameraCenter;
                ray.dir = glm::normalize(sampleCenter - cameraCenter);

                accum += traceRay(ray);
            }
        }
        return accum * 0.25f;
    }
};
