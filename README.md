# E3D
## About
Learning OpenGL and using it to create a 3D graphics and (hopefully) a 3D physics engine.

## Setup and Compiling
*Note that the CMakeLists.txt file may need to be modified to work on windows systems. (See [GLFW-CMake-Starter](https://github.com/enkisoftware/GLFW-CMake-starter/blob/main/CMakeLists.txt)).*

```
$ git clone https://github.com/MichaelTennant/e3d.git
$ cd ./e3d
$ git submodule update --init --recursive

$ mkdir build
$ cd build
$ cmake -S ..

$ ./e3d
```
