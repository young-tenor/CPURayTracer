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

# Build Debug
cmake --build build --config Debug

# Build Release
cmake --build build --config Release
```

Run the binary from `build/Debug/CPURayTracer.exe` or `build/Release/CPURayTracer.exe`.

Press **Escape** to close the window.

## Architecture

The project renders ray-traced images on the CPU, then displays the result via OpenGL.

- **References**: [Ray Tracing in One Weekend](https://raytracing.github.io/) for the ray tracer math/structure; [LearnOpenGL](https://learnopengl.com/) for the OpenGL display layer.
- **Language**: C++20, no exceptions/RTTI required.
- **Math**: `glm` for vectors/matrices shared between CPU ray tracer and any GPU-side display work.
- **Windowing/GL context**: GLFW 3 + GLAD (OpenGL 3.3 Core).

### Planned pipeline (to be built incrementally)

1. CPU ray tracer writes pixel colors into a `float` or `uint8_t` buffer.
2. Buffer is uploaded to a GL texture each frame (or once, if the image is static).
3. A full-screen quad shader samples the texture and outputs to the framebuffer.

### Coding style

- Follow the naming and code structure style of the two reference books.
- Naming: camelCase throughout, no prefixes (e.g., `windowWidth`, `framebufferSizeCallback`). GLSL: `texCoord`, `fragColor`, `pos`.
- GLFW callbacks must be free functions (no lambdas with captures as C callbacks).
- Include order: platform/GL headers first (`glad` before `GLFW`), then standard library.
- Use `auto` when the type is long (e.g., `unique_ptr`, iterator types), when it aids maintainability, or when the type is obvious from the right-hand side. Don't force it where the explicit type is clearer.
- Do not add alignment whitespace (extra spaces to align columns across lines). It creates maintenance burden and adds unnecessary tokens.
