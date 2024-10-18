/* Following along https://learnopengl.com/book/book_pdf.pdf
 *
 * DAY 1 - Version 0.0.1
 * HOW DOES THIS WORK!?!?
 * I spent too long working on it it and I'm both asstonished and apauled by what fixed it
 * Completed up to chapter 5.3
 *
 * Day 2 - Version 0.0.2 
 * Continued from chapter 5.4 - ._. where triangle 
 * ahhh nvm I'm stupid I didn't have the triangle draw section in the mainloop...
 * Didn't spend as long today - will make the triangle rainbow tmrw
 * Completed up to chapter 5.5
 *
 * TODO Day 3 - Version 0.0.3
 * WIll Skip to Chapter 6 which covers shaders in more detail
 *  ... AKA RAINBOW TRIANGLE
 * */

#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h> // Should be imported before glfw3
#include <GLFW/glfw3.h>

const char* SIMPLE_VERTEX_SHADER_SRC =                      \
    "#version 330 core                                  \n" \
    "layout (location = 0) in vec3 aPos;                \n" \
    "void main() {                                      \n" \
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);   \n" \
    "}                                                  \0";

const char* SIMPLE_FRAGMENT_SHADER_SRC =                    \
    "#version 330 core                                  \n" \
    "out vec4 FragColor;                                \n" \
    "void main() {                                      \n" \
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);          \n" \
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
    unsigned int VAO;   // Vertex Array Object
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
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

    /* Compile Vertex Shaders */
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &SIMPLE_VERTEX_SHADER_SRC, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "CRITICAL ERR: Failed to compile vertex shader! See below for info log.\n %s\n", infoLog);
        return -1;
    }

    /* Compile Fragment Shaders (colour) */
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &SIMPLE_FRAGMENT_SHADER_SRC, NULL);
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

    /* Use our shaders!*/
    glUseProgram(shaderProgram);

    /* Some rendering magic happens here and I'm not really sure what... */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Loop until user closes the window! */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw Our Triangle!*/
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
