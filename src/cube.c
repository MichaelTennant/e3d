#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h> // Should be imported before glfw3
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION // Required to import stb_image.h
#include <stb_image.h>

/* GLSL Shaders */
const char* SIMPLE_VERTEX_SHADER_SRC =                          \
    "#version 330 core                                      \n" \
    "layout (location = 0) in vec3 aPos;                    \n" \
    "void main() {                                          \n" \
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);   \n" \
    "}                                                      \0";

const char* SIMPLE_FRAGMENT_SHADER_SRC =                        \
    "#version 330 core                                      \n" \
    "out vec4 fragColor;                                    \n" \
    "void main() {                                          \n" \
    "   fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);           \n" \
    "}                                                      \0";

const char* RAINBOW_VERTEX_SHADER_SRC =                         \
    "#version 330 core                                      \n" \
    "layout (location = 0) in vec3 aPos;                    \n" \
    "layout (location = 1) in vec3 aColor;                  \n" \
    "out vec3 fragInColor;                                  \n" \
    "void main() {                                          \n" \
    "   gl_Position = vec4(aPos, 1.0f);                     \n" \
    "   fragInColor = aColor;                               \n" \
    "}                                                      \0";

const char* STATIC_RAINBOW_FRAGMENT_SHADER_SRC =                \
    "#version 330 core                                      \n" \
    "in vec3 fragInColor;                                   \n" \
    "out vec4 fragColor;                                    \n" \
    "void main() {                                          \n" \
    "   fragColor = vec4(fragInColor, 1.0f);                \n" \
    "}                                                      \0";

const char* CHANGING_RAINBOW_FRAGMENT_SHADER_SRC =              \
    "#version 330 core                                      \n" \
    "in vec3 fragInColor;                                   \n" \
    "uniform float time;                                    \n" \
    "out vec4 fragColor;                                    \n" \
    "void main() {                                          \n" \
    "   fragColor = vec4(                                   \n" \
    "       max(sin(fragInColor.x + time), 0.0f),           \n" \
    "       max(sin(fragInColor.y + time), 0.0f),           \n" \
    "       max(sin(fragInColor.z + time), 0.0f),           \n" \
    "       1.0f                                            \n" \
    "   );                                                  \n" \
    "}                                                      \0";

// Not bothering with projection just yet
// Will write projection vertex shader when code is refactored
const char* CUBE_RAINBOW_VERTEX_SHADER_SRC =                    \
    "#version 330 core                                      \n" \
    "layout (location = 0) in vec3 aPos;                    \n" \
    "layout (location = 1) in vec3 aColor;                  \n" \
    "uniform float time;                                    \n" \
    "out vec3 fragInColor;                                  \n" \
    "void main() {                                          \n" \
    "   vec3 theta = vec3(3*time, 2*time, time);            \n" \
    "   float a = aPos.y * sin(theta.z)                     \n" \
    "               + aPos.x * cos(theta.z);                \n" \
    "   float b = aPos.y * cos(theta.z)                     \n" \
    "               - aPos.x * sin(theta.z);                \n" \
    "   float c = aPos.z * cos(theta.y)                     \n" \
    "               + sin(theta.y) * a;                     \n" \
    "   gl_Position = vec4(                                 \n" \
    "       0.5 * (cos(theta.y)*a - aPos.z*sin(theta.y)),   \n" \
    "       0.5 * (sin(theta.x)*c + cos(theta.x)*b),        \n" \
    "       0.5 * (cos(theta.x)*c - sin(theta.x)*b),        \n" \
    "       1.0f                                            \n" \
    "   );                                                  \n" \
    "   fragInColor = aColor;                               \n" \
    "}                                                      \0";

// TODO
const char* CUBE_IMAGE_VERTEX_SHADER_SRC;
const char* CUBE_IMAGE_FRAGMENT_SHADER_SRC;

#define PI2BY3 2.0f * M_PI / 3.0f
#define PI4BY3 4.0f * M_PI / 3.0f

void processInput(GLFWwindow* window) {
    /* Close window if escape key pressed */
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    GLFWwindow* window;

    /* Cube Verticies - now with extra colors */
    float vertices[] = {
        -1.0f, -1.0f, -1.0f,  0.0f,   PI2BY3, PI4BY3,
        -1.0f, -1.0f,  1.0f,  PI4BY3, 0.0f,   PI2BY3,
        -1.0f,  1.0f, -1.0f,  PI2BY3, PI4BY3, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,   PI2BY3, PI4BY3, 
         1.0f, -1.0f, -1.0f,  PI4BY3, 0.0f,   PI2BY3,
         1.0f, -1.0f,  1.0f,  PI2BY3, PI4BY3, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,   PI2BY3, PI4BY3,
         1.0f,  1.0f,  1.0f,  PI4BY3, 0.0f,   PI2BY3
    };

    /* Cube Edges */
    unsigned int indicies [] = {
        2, 6, 7, 
        2, 7, 3, 
        2, 3, 1, 
        2, 1, 0, 
        3, 7, 5, 
        3, 5, 1, 
        7, 6, 4, 
        7, 4, 5, 
        1, 5, 4, 
        1, 4, 0, 
        0, 4, 6, 
        0, 6, 2, 
    };

    unsigned int VBO;   // Vertex Buffer Object
    unsigned int VAO;   // Vertex Array Object
    unsigned int EBO;   // Edge Buffer Object
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
    int success;
    char infoLog[512];
    float time;
    int uniformTimeLocation;

    /* Initialized glfw */
    if (!glfwInit()) {
        fprintf(stderr, "CRITICAL ERR: Failed to initialize glfw!\n");
        return -1;
    }

    /* Create 640px x 480px window */
    window = glfwCreateWindow(640, 480, "GLFW Window", NULL, NULL);
    if (!window) {
        fprintf(stderr, "CRITICAL ERR: Failed to create glfw window!\n");
        glfwTerminate();
        return -1;
    }

    /* The program should pay attention to this window right now */
    glfwMakeContextCurrent(window);


    /* Initalize OpenGL Context GLAD - note I am using my own glad not GLFWs
     * ... this function must be called after the window is the current context. */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "CRITICAL ERR: Failed to load OpenGL context glad.\n");
        return -1;
    }
    
    /* Set the background colour to a desaturated purple */
    glClearColor(.4f, .3f, .4f, .0f);

    /* Magic.... Bind vertices to VBO???? */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Same thing for EBO */
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    /* Compile Vertex Shaders */
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &CUBE_RAINBOW_VERTEX_SHADER_SRC, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "CRITICAL ERR: Failed to compile vertex shader! See below for info log.\n %s\n", infoLog);
        return -1;
    }

    /* Compile Fragment Shaders (colour) */
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &CHANGING_RAINBOW_FRAGMENT_SHADER_SRC, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "CRITICAL ERR: Failed to compile fragment shader! See below for info log.\n %s/n", infoLog);
        return -1;
    }

    /* Combine shaders into shader program */
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "CRITICAL ERR: Failed to link shader program! See below for info log.\n %s\n", infoLog);
        return -1;
    }

    /* Now that the shaders have been linked - we don't need the shader objects anymore */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* Use our shaders! And pass the the time uniform! (Uniforms are global vars...) */
    glUseProgram(shaderProgram);
    
    time = glfwGetTime();
    uniformTimeLocation = glGetUniformLocation(shaderProgram, "time");
    glUniform1f(uniformTimeLocation, time);

    /* Some rendering magic happens here and I'm not really sure what... */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Vertex Position Data; at index 0, offset 0 * sizeof(float) */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Vertex Color Data; at index 1, offset 3 * sizeof(float)*/
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Draw stuff at back first! */
    glEnable(GL_DEPTH_TEST);

    /* Loop until user closes the window! */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* I honestly don't know why this is needed to be executed every frame */
        glUseProgram(shaderProgram);

        /* Update time uniform */
        time = glfwGetTime();
        uniformTimeLocation = glGetUniformLocation(shaderProgram, "time");
        glUniform1f(uniformTimeLocation, time);

        /* Render the triangle*/
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, sizeof(indicies), GL_UNSIGNED_INT, 0);

        /* Update window (like pygames flip()) */
        glfwSwapBuffers(window);

        /* Check if any events - but no event struct???
         *     ... ohhh, it uses global vars - ewwww */
        glfwPollEvents();

        /* Process user inputs */
        processInput(window);
    }

    glfwTerminate();
    return 0;
}
