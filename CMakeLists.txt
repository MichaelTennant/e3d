# This file caused more grief than it should have...

cmake_minimum_required(
    VERSION 3.30.0)

# VERSIONING SCHEME (I KNOW IT'S BAD - IT'S A HOBBY PROJECT, I AIN'T HAVE NO API RN)
# Patch version incremented each day I work on the project
# Minor version incremented each distinct file I write
# Major version incremented for each major feature added (i.e rending/physics engine complete)
project(e3d
    VERSION 0.0.3
    LANGUAGES C)

# OpenGL Requirement
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Glad Requirement (Don't use GLFW's glad - couldn't get that to work)
include_directories(external/glad/include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
set(GLAD_GL "external/glad/src/glad.c")

# GLFW Requirement
add_subdirectory(external/glfw)

# Compile C files
add_executable(${PROJECT_NAME} 
    src/triangles.c
    ${GLAD_GL})

# Link Headers
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES} 
    glfw)
