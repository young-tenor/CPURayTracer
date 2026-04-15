#pragma once

#include "graphics/texture.h"
#include "graphics/sampler.h"
#include <glm/glm.hpp>

#include <array>
#include <cmath>
#include <string>

class Skybox {
public:
    // Face order: +X, -X, +Y, -Y, +Z, -Z
    Skybox(
        const std::string& posX, const std::string& negX,
        const std::string& posY, const std::string& negY,
        const std::string& posZ, const std::string& negZ
    ) : faces{Texture(posX), Texture(negX), Texture(posY), Texture(negY), Texture(posZ), Texture(negZ)},
        sampler(Sampler::Wrap::clamp, Sampler::Filter::linear)
    {}

    // https://en.wikipedia.org/wiki/Cube_mapping
    glm::vec3 sample(const glm::vec3& dir) const {
        float ax = std::abs(dir.x);
        float ay = std::abs(dir.y);
        float az = std::abs(dir.z);

        int face;
        float sc, tc, ma;

        if (ax >= ay && ax >= az) {
            if (dir.x > 0.0f) { face = 0; sc = dir.z; tc = -dir.y; ma = ax; } // +X
            else { face = 1; sc = -dir.z; tc = -dir.y; ma = ax; } // -X
        } else if (ay >= az) {
            if (dir.y > 0.0f) { face = 2; sc = dir.x; tc = -dir.z; ma = ay; } // +Y
            else { face = 3; sc = dir.x; tc = dir.z; ma = ay; } // -Y
        } else {
            if (dir.z < 0.0f) { face = 4; sc = dir.x; tc = -dir.y; ma = az; } // +Z
            else { face = 5; sc = -dir.x; tc = -dir.y; ma = az; } // -Z
        }

        glm::vec2 uv((sc / ma + 1.0f) * 0.5f, (tc / ma + 1.0f) * 0.5f);
        return faces[face].sample(sampler, uv);
    }

private:
    std::array<Texture, 6> faces;
    Sampler sampler;
};
