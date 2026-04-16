# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Prerequisites

- [vcpkg](https://github.com/microsoft/vcpkg) installed and `VCPKG_ROOT` environment variable set
- Visual Studio 2022 with C++ workload
- CMake 3.20+

## Build

vcpkg manifest mode auto-installs dependencies on first configure.

```bash
# Configure (installs glfw3, glm, glad via vcpkg automatically)
cmake --preset windows-msvc

# Build Release
cmake --build --preset windows-msvc-release

# Build Debug
cmake --build --preset windows-msvc-debug
```

Run the binary from `build/Release/CPURayTracer.exe` or `build/Debug/CPURayTracer.exe`.

Press **Escape** to close the window.

## Architecture

The project renders ray-traced images on the CPU, then displays the result via OpenGL.

- **References**: [Ray Tracing in One Weekend](https://raytracing.github.io/) for the ray tracer math/structure; [LearnOpenGL](https://learnopengl.com/) for the OpenGL display layer.
- **Language**: C++20, no exceptions/RTTI required.
- **Math**: `glm` for vectors/matrices shared between CPU ray tracer and any GPU-side display work.
- **Windowing/GL context**: GLFW 3 + GLAD (OpenGL 3.3 Core).

### Render pipeline

1. `App::update()` calls `samplePixel()` for each pixel and writes `glm::vec4` results into a `std::vector<glm::vec4> pixels` buffer.
2. The buffer is uploaded to a `GL_RGBA32F` texture each frame via `glTexSubImage2D`.
3. `App::render()` draws a full-screen quad; `quad.vert` / `quad.frag` sample the texture and write to the framebuffer.

### Ray tracing features

- **Perspective projection**: camera at `cameraCenter`, rays directed toward a virtual screen plane. NDC computed per-sample as `(2*(i+0.5)/width - 1) * aspectRatio`.
- **Supersampling anti-aliasing**: 2×2 grid per pixel (`samplePixel`), averaged (×0.25).
- **Blinn-Phong shading**: `Material` holds `ambient`, `diffuse`, `specular`, `shininess`. Half-vector `H = normalize(L + V)` used for specular.
- **Shadow**: shadow ray cast from hit point (offset by `N * 1e-4f`) toward light; occlusion check against all objects within light distance.
- **Texturing**: `Texture` loads images via `stb_image`. `Sampler` controls wrap (`repeat`/`clamp`) and filter (`nearest`/`linear`). Bilinear interpolation samples a 2×2 texel neighbourhood. UV tiling scale (currently ×4) is applied in `Triangle::intersect`.
- **Skybox**: `Skybox` holds 6 `Texture` faces (±X, ±Y, ±Z). Cube-map face selection and UV computed per [Wikipedia cube mapping](https://en.wikipedia.org/wiki/Cube_mapping). Miss rays return `skybox->sample(ray.dir)`.
- **Reflection**: `Material::reflection` ∈ [0, 1]. `traceRay` recurses up to `depth=4`; reflected color blended with surface color via `glm::mix`.

### Source layout

```
src/
  main.cpp               — entry point, #define STB_IMAGE_IMPLEMENTATION
  app.h                  — App class: GL init, render loop, ray tracing logic
  graphics/
    ray.h                — Ray { orig, dir }
    hit.h                — Hit { normal, uv, dist, material }
    material.h           — Material { ambient, diffuse, specular, shininess, reflection, texture }
    sampler.h            — Sampler { Wrap::{repeat,clamp}, Filter::{nearest,linear} }
    texture.h            — Texture: stb_image load, nearest/linear sampling with wrap/clamp
    shader.h             — Shader: GLSL compile/link helper (no #pragma once — uses include guard)
  scene/
    object.h             — Object: abstract base, virtual intersect()
    sphere.h             — Sphere: analytic ray-sphere intersection, outward normal
    triangle.h           — Triangle: Möller–Trumbore-style, barycentric UV interpolation
    rectangle.h          — Rectangle: two Triangle instances, quad UV layout
    light.h              — Light { pos, strength }
    skybox.h             — Skybox: 6-face cube map, clamp+linear sampler
  shaders/
    quad.vert            — pass-through: pos → gl_Position, aTexCoord → texCoord
    quad.frag            — samples screenTexture, writes fragColor
```

### Coding style

- Follow the naming and code structure style of the two reference books.
- Naming: camelCase throughout, no prefixes (e.g., `windowWidth`, `framebufferSizeCallback`). GLSL: `texCoord`, `fragColor`, `pos`.
- GLFW callbacks must be free functions (no lambdas with captures as C callbacks).
- Include order: platform/GL headers first (`glad` before `GLFW`), then standard library.
- Use `auto` when the type is long (e.g., `unique_ptr`, iterator types), when it aids maintainability, or when the type is obvious from the right-hand side. Don't force it where the explicit type is clearer.
- Do not add alignment whitespace (extra spaces to align columns across lines). It creates maintenance burden and adds unnecessary tokens.
- Use `const` instead of `constexpr` for constants.
