/* Following along https://learnopengl.com/book/book_pdf.pdf
 *
 * DAY 1 - Version 0.0.1
 * HOW DOES THIS WORK!?!?
 * I spent too long working on it it and I'm both asstonished and apauled by what fixed it
 * Completed up to chapter 5.3
 *
 * Day 2 - Version 0.0.2 
 * TBC at chapter 5.4 - fragment shader */

#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h> // Should be imported before glfw3
#include <GLFW/glfw3.h>

const char *SIMPLE_VERTEX_SHADER_SRC =                      \
    "#version 330 core                                  \n" \
    "layout (location = 0) in vec3 aPos;                \n" \
    "void main() {                                      \n" \
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);   \n" \
    "}                                                  \0";


void processInput(GLFWwindow* window) {
    /* Close window if escape key pressed */
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    GLFWwindow* window;

    /* Triangle Verticies */
    float vertices[9] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;   // Vertex Buffer Object
    unsigned int vertexShader;
    int success;
    char infoLog[512];

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

    /* Compiling Vertex Shaders? Not really sure what they are tbh... */
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &SIMPLE_VERTEX_SHADER_SRC, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "CRITICAL ERR: Failed to compile vertex shader! See below for info log.\n %s\n", infoLog);
        return -1;
    }


    /* Loop until user closes the window! */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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
