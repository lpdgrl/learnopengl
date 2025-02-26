#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <cmath> 
#include "shader.hpp"
#include "image_loader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput (GLFWwindow* window);
void processInputKeyOfMove (GLFWwindow* window, Shader& shader);
float changeValueInterFragmentShader(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/learnopengl/src/shaders/shader.vs";
const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/learnopengl/src/shaders/shader.fs";
const char* PATH_TO_TEXTURE1 = "/home/lpdgrl/Project/code/learnopengl/data/textures/container.jpg";
const char* PATH_TO_TEXTURE2 = "/home/lpdgrl/Project/code/learnopengl/data/textures/awesomeface.png";

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    Shader shader(PATH_TO_FILE_VERTEX_SHADER, PATH_TO_FILE_FRAGMENT_SHADER);

    // vertices of triangle
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.45f, 0.45f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // top left 
    };

    /*// vertices of rectangle 
    float vertices[] = {
        // first triangle
        0.5f,   0.5f,   0.0f,   // top right
        0.5f,  -0.5f,   0.0f,   // bottom right
       // second triangle
       -0.5f,  -0.5f,   0.0f,   // bottom left
       -0.5f,   0.5f,   0.0f    // top left
    };*/

    float vertices_second[] = {
        // first triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    }; 

    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // load and create a texture
    unsigned int texture1, texture2; 
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // setting wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // setting filtration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load file of texture
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(PATH_TO_TEXTURE1, &width, &height, &nrChannels, 0);

    if (data) {
        std::cout << "size of data = " << sizeof(data) << std::endl;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    // setting wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // setting filtration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(PATH_TO_TEXTURE2, &width, &height, &nrChannels, 0);

    if (data) {
        std::cout << "size of data = " << sizeof(data) << std::endl;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int nrAttrubutes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttrubutes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttrubutes << std::endl;

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    float value_inter = 0.2f;
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
     
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* float timeValue = glfwGetTime();
        float greenValue = std::sin(timeValue) / 2.0f + 0.5f;

        int vertexColorLocation = glGetUniformLocation(shaderYellowProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); */
        
        // processInputKeyOfMove(window, shader);
        float change_value_fr_shader = changeValueInterFragmentShader(window);

        if (change_value_fr_shader != -1.0f) {
            value_inter += change_value_fr_shader;
            if (value_inter >= 1.0f) {
                value_inter = 1.0f;
            }
            else if (value_inter <= 0.0f) {
                value_inter = 0.0f;
            }
        } 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.setFloat("value_inter", value_inter);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

float changeValueInterFragmentShader(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        return 0.001f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        return -0.001f;
    }
    else {
        return -1.0f;
    }
}

void processInputKeyOfMove(GLFWwindow* window, Shader& shader) 
{
    float offset = 0.0;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        offset = -0.5;
        shader.setFloat("xOffset", offset);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        offset = 0.5;
        shader.setFloat("xOffset", offset);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        offset = 0.5;
        shader.setFloat("yOffset", offset);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        offset = -0.5;
        shader.setFloat("yOffset", offset);
    }
    else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        offset = 0.0;
        shader.setFloat("xOffset", offset);
        shader.setFloat("yOffset", offset);
    }
}

// processed ESCAPE key to exit 
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        std::cout << "Window been closed!" << std::endl;
    }
}

// callback function for resizing window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

