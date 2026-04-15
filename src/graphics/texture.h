#pragma once

#include "graphics/sampler.h"
#include <stb_image.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

class Texture {
public:
    int width = 0;
    int height = 0;
    std::vector<glm::vec3> image;

    Texture(int width, int height, std::vector<glm::vec3> image)
        : width(width), height(height), image(std::move(image)) {}

    Texture(const std::string& path) {
        int channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 3);
        if (data) {
            image.resize(width * height);
            for (int i = 0; i < width * height; i++)
                image[i] = glm::vec3(data[i * 3] / 255.0f, data[i * 3 + 1] / 255.0f, data[i * 3 + 2] / 255.0f);
            stbi_image_free(data);
        } else {
            std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ: " << path << "\n";
        }
    }

    glm::vec3 sample(const Sampler& sampler, const glm::vec2& texCoord) const {
        if (sampler.filter == Sampler::Filter::nearest)
            return sampleNearest(texCoord, sampler.wrap == Sampler::Wrap::repeat);
        return sampleLinear(texCoord, sampler.wrap == Sampler::Wrap::repeat);
    }

private:
    glm::vec3 sampleNearest(const glm::vec2& uv, bool wrap) const {
        auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
        int j = static_cast<int>(std::round(xy.y));
        int i = static_cast<int>(std::round(xy.x));
        return wrap ? repeatedColor(j, i) : clampedColor(j, i);
    }

    glm::vec3 sampleLinear(const glm::vec2& uv, bool wrap) const {
        auto xy = uv * glm::vec2(width, height) - glm::vec2(0.5f);
        int j = static_cast<int>(std::floor(xy.y));
        int i = static_cast<int>(std::floor(xy.x));
        float dy = xy.y - j;
        float dx = xy.x - i;
        glm::vec3 c00, c01, c10, c11;
        if (wrap) {
            c00 = repeatedColor(j, i);     c01 = repeatedColor(j, i + 1);
            c10 = repeatedColor(j + 1, i); c11 = repeatedColor(j + 1, i + 1);
        } else {
            c00 = clampedColor(j, i);     c01 = clampedColor(j, i + 1);
            c10 = clampedColor(j + 1, i); c11 = clampedColor(j + 1, i + 1);
        }
        return glm::mix(glm::mix(c00, c01, dx), glm::mix(c10, c11, dx), dy);
    }

    glm::vec3 clampedColor(int j, int i) const {
        j = std::clamp(j, 0, height - 1);
        i = std::clamp(i, 0, width - 1);
        return image[j * width + i];
    }

    glm::vec3 repeatedColor(int j, int i) const {
        j = ((j % height) + height) % height;
        i = ((i % width) + width) % width;
        return image[j * width + i];
    }
};
