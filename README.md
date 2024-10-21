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

## Development Notes
### src/cube.c
```
Following along https://learnopengl.com/book/book_pdf.pdf

DAY 1 - Version 0.0.1
 HOW DOES THIS WORK!?!?
 I spent too long working on it it and I'm both asstonished and apauled by what fixed it
 Most of the problems where with my CMakeLists.txt file tbf
 Completed up to chapter 5.3

Day 2 - Version 0.0.2 
 Continued from chapter 5.4 - ._. where triangle 
 ahhh nvm I'm stupid I didn't have the triangle draw section in the mainloop...
 Didn't spend as long today - will make the triangle rainbow tmrw
 Completed up to chapter 5.5

Day 3 - Version 0.0.3
 Skiped to Chapter 6 which covers shaders in more detail
 ... AKA RAINBOW TRIANGLE - You know, that was easier than expected
 ... LETS MAKE IT CHANGE COLOR - That was fun, didn't change color like I expected, 
         but it's good enough, I'll maybe figure out the maths to make the colors 
         rotate tomorrow if I can be asked, if not I'll add more triangles and look 
         into adding textures to the triangles.
 Completed up to chapter 6.5 - I'm programming in C I'm not gonna make my own shader
     class like suggested in chapter 6.6, but I'll prob write my own functions to 
     compile and use them...

Day 4 - Version 0.0.4
 Modified the maths of the rainbow triangle look better but it still doesn't rotate 
     as wanted - it's good enough though...
 Added second triangle - easier than expected, will add entire cube
 OH GOD I SPENT 6 HOURS TRAVELING TODAY - I AM DEAD
 Made cube rotate - WHERE TF IS CUBE, ARE ONLY 2 TRIANGLES BEING RENDERED???
     ... All of the faces are being rendered now - but that's defo not a cube...
 Fixed it! Just a few problems with the rotation algorithm
     ... Although it is definatley rendering sides of the cube that it shouldn't
     ... Will fix tmrw - it's getting pretty late and I am decked after that trip...

Day 5 - Version 0.0.5
 Fixed the order triangles are rendered - there can defo be some more shader optomizations
     ... but that'ts a future me problem.
 Started looking into textures from chapter 7 - added stb's repo of single-file libraries 
     ... for stb_image.h
 ```
