# CPURayTracer

CPU ray tracer with OpenGL display, based on [Ray Tracing in One Weekend](https://raytracing.github.io/) and [LearnOpenGL](https://learnopengl.com/).

## Table of Contents

- [Features](#features)
  - [Perspective Projection](#perspective-projection)
  - [Supersampling Anti-Aliasing](#supersampling-anti-aliasing)
  - [Blinn-Phong Shading](#blinn-phong-shading)
  - [Shadow](#shadow)
  - [Texturing](#texturing)
  - [Skybox & Reflection](#skybox--reflection)
- [Dependencies](#dependencies)
- [Build](#build)

---

## Features

### Perspective Projection

<!-- ![Perspective Projection](images/perspective_projection.png) -->

### Supersampling Anti-Aliasing

<!-- ![Supersampling Anti-Aliasing](images/ssaa.png) -->

### Blinn-Phong Shading

<!-- ![Blinn-Phong Shading](images/blinn_phong.png) -->

### Shadow

<!-- ![Shadow](images/shadow.png) -->

### Texturing

Supports clamp and repeat wrap modes, and nearest and linear filter modes.

<!-- ![Texturing](images/texturing.png) -->

### Skybox & Reflection

<!-- ![Skybox & Reflection](images/skybox_reflection.png) -->

---

## Dependencies

**System**
- Visual Studio 2022 (MSVC, x64)
- CMake 3.20+
- vcpkg (`VCPKG_ROOT` env var required)
- OpenGL

**vcpkg (auto-installed)**
- glfw3
- glm
- glad (OpenGL 3.3 Core)
- stb (stb_image)

## Build

```bash
cmake --preset windows-msvc
cmake --build --preset windows-msvc-release
```

Binary: `build/Release/CPURayTracer.exe`

Press **Escape** to close.
