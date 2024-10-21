# This file caused more grief than it should have...

cmake_minimum_required(
    VERSION 3.30.0)

project(e3d
    VERSION 0.0.5
    LANGUAGES C)

# OpenGL Requirement
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Glad Requirement (I don't use GLFW's glad since I couldn't get that to work)
include_directories(external/glad/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
set(GLAD_GL "external/glad/src/glad.c")

# GLFW Requirement
add_subdirectory(external/glfw)

# STB Library Requirement - used for reading images
include_directories(external/stb)

# Compile C files
add_executable(${PROJECT_NAME} 
    src/cube.c
    ${GLAD_GL})

# Link Headers
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES} 
    glfw)
