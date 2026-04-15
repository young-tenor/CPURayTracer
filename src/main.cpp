#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/shader.h"
#include "graphics/ray.h"
#include "scene/object.h"
#include "scene/sphere.h"
#include "scene/triangle.h"
#include "scene/rectangle.h"
#include "scene/light.h"

#include <glm/glm.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

static const int width = 1280 * 2;
static const int height = 720 * 2;
static const char* title = "CPU Ray Tracer";

static const float quadVertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
};

static glm::vec3 traceRay(const Ray& ray, const glm::vec3& cameraCenter, const Light& light, const std::vector<std::unique_ptr<Object>>& objects)
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
    auto L = glm::normalize(light.pos - hitPoint);
    auto V = glm::normalize(cameraCenter - hitPoint);
    auto H = glm::normalize(L + V);

    Ray shadowRay;
    shadowRay.orig = hitPoint + N * 1e-4f;
    shadowRay.dir = L;
    float lightDist = glm::length(light.pos - hitPoint);
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

    glm::vec3 color = hit.material.ambient;
    if (!inShadow)
    {
        auto diffuse = hit.material.diffuse * light.strength * glm::max(glm::dot(N, L), 0.0f);
        auto specular = hit.material.specular * light.strength * glm::pow(glm::max(glm::dot(N, H), 0.0f), hit.material.shininess);
        color += diffuse + specular;
    }
    return color;
}

static glm::vec3 samplePixel(int i, int j, int width, int height, float aspectRatio, const glm::vec3& cameraCenter, const Light& light, const std::vector<std::unique_ptr<Object>>& objects)
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

            accum += traceRay(ray, cameraCenter, light, objects);
        }
    }
    return accum * 0.25f;
}

static void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, width, height);

    // CPU pixel buffer — ray tracer writes here
    std::vector<glm::vec4> pixels(width * height);

    // Orthogonal projection — one ray per pixel center, direction along -z
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    glm::vec3 cameraCenter(0.0f, 0.0f, 1.0f);
    Light light(glm::vec3(1.0f, 1.0f, 0.0f), 1.0f);

    std::vector<std::unique_ptr<Object>> objects;

    Material material(
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.7f, 0.7f, 0.7f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        32.0f
    );

    objects.push_back(std::make_unique<Sphere>(glm::vec3(0.0f, 0.0f, -0.5f), 0.5f, material));

    // Floor at y = -0.5 (sphere bottom tangent), spanning the visible scene area
    objects.push_back(std::make_unique<Rectangle>(
        glm::vec3(-3.0f, -0.5f, 2.0f),
        glm::vec3(3.0f, -0.5f, 2.0f),
        glm::vec3(3.0f, -0.5f, -3.0f),
        glm::vec3(-3.0f, -0.5f, -3.0f),
        material
    ));

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            auto color = samplePixel(i, j, width, height, aspectRatio, cameraCenter, light, objects);
            pixels[j * width + i] = glm::vec4(color, 1.0f);
        }
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
                 GL_RGBA, GL_FLOAT, pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    Shader shader(SHADER_DIR "quad.vert", SHADER_DIR "quad.frag");
    shader.use();
    shader.setInt("screenTexture", 0);

    unsigned int vao, vbo;
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

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shader.ID);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
