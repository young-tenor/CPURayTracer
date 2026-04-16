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
<img width="1282" height="752" alt="PerspectiveProjection" src="https://github.com/user-attachments/assets/3d1351dc-b0ee-417f-b1bf-898ee965d553" />

### Supersampling Anti-Aliasing
<img width="1282" height="752" alt="SuperSampling" src="https://github.com/user-attachments/assets/6d79499a-7065-4150-b0f1-6cc37ae49a97" />

### Blinn-Phong Shading
<img width="1282" height="752" alt="BlinnPhong" src="https://github.com/user-attachments/assets/20d89630-4857-4ce8-93a8-9486904a8ab5" />

### Shadow
<img width="1282" height="752" alt="Shadow" src="https://github.com/user-attachments/assets/5b7197b1-5e17-445c-8906-5cb5a0ebfa85" />

### Texturing
Supports clamp and repeat wrap modes, and nearest and linear filter modes.

<img width="1282" height="752" alt="NearestClamp" src="https://github.com/user-attachments/assets/1b2971b6-5a97-4017-addc-54ed2747fba3" />
<img width="1282" height="752" alt="NearestRepeat" src="https://github.com/user-attachments/assets/36960edd-2689-430a-b6f4-61ea71bcc978" />
<img width="1282" height="752" alt="LinearClamp" src="https://github.com/user-attachments/assets/1a7044ec-4d54-4076-9261-25c8187a2668" />
<img width="1282" height="752" alt="LinearRepeat" src="https://github.com/user-attachments/assets/0de27e2a-4a50-4a2f-858f-6bcac3d14c24" />

### Skybox & Reflection
<img width="1282" height="752" alt="SkyboxAndReflection" src="https://github.com/user-attachments/assets/2218e97c-6076-499f-9b44-275b102afea3" />

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
