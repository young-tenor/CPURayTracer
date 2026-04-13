cmake_minimum_required(VERSION 3.20)
project(CPURayTracer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(CPURayTracer
    src/main.cpp
)

target_link_libraries(CPURayTracer PRIVATE
    OpenGL::GL
    glfw
    glm::glm
    glad::glad
)

target_compile_options(CPURayTracer PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)
