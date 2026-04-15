# CPURayTracer

CPU ray tracer with OpenGL display, based on [Ray Tracing in One Weekend](https://raytracing.github.io/) and [LearnOpenGL](https://learnopengl.com/).

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
