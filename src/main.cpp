#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip> 
#include <fstream>
#include <sstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

const int WIDTH = 800;
const int HEIGHT = 600;

GLFWwindow* window;
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
void window_resize_adjust_viewport(GLFWwindow* window, int width, int height);
void initialize(void);
void render_loop(void);
void compile_shaders(unsigned int vertexShader, unsigned int fragmentShader);
std::string load_file(const char* path);


std::vector<float> vertices = {
    // First triangle
     0.1f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.9f, 0.0f,  0.0f, 0.0f, 1.0f,   // top 

    // Second triangle
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   // bottom right
     0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,   // bottom left
     0.25f, 0.5f, 0.0f,  1.0f, 0.0f, 1.0f    // top
};

// unsigned int indices[] = {  // note that we start from 0!
//     0, 1, 3,   // first triangle
//     1, 2, 3    // second triangle
// };  

std::vector<unsigned int> indices = {
    0, 1, 2,
    3, 4, 5
};

std::vector<float> texCoords = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};

//make a function that populates an array of indices when given vertexes. 
void populate_indices(std::vector<float> vertices, std::vector<unsigned int> indices) {
    float temp = 0.0;
    for (std::size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "Index " << i << ": " << vertices[i] << "\n";
        temp = vertices[i];
        for (std::size_t j = 0; j < vertices.size(); ++j) {
            std::cout << "Index " << i << ": " << vertices[i] << "\n";
        }
    }
}

int main() {

    initialize();
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //might wanna stwich static to dynamic later, if we change graphics at run time
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    //^ buffers | v vertex array objects
    /*
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    */
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    compile_shaders(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    render_loop();

    return 0;
}



std::string load_file(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void window_resize_adjust_viewport(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, WIDTH, HEIGHT);
}

//compile fragment and vertex shaders
void compile_shaders(unsigned int vertexShader, unsigned int fragmentShader) {
    std::string fragSource = load_file("src/shaders/fragment.glsl");
    std::string vertSource = load_file("src/shaders/vertex.glsl");

    const char* fragPtr = fragSource.c_str();
    const char* vertPtr = vertSource.c_str();
    
    int tempFlag = 0;
    char infoLog[512];

    //create and complie vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertPtr, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &tempFlag);
    if(!tempFlag) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::exit(EXIT_FAILURE);
    }

    //create and complie fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragPtr, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &tempFlag);
    if(!tempFlag) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::exit(EXIT_FAILURE);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);
}


void initialize(void) {
    std::cout << "Lo, and behold!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Lo, and behold!", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }   

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, window_resize_adjust_viewport); 
}
//test
float timeValue = 0;
float greenValue = 0;

//TODO: Loading and creating textures

void render_loop(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    //you need to do it for R as well for 3D
    //default using glPolygonMode(GL_FRONT_AND_BACK, ***GL_FILL OR GL_LINE).
    while(!glfwWindowShouldClose(window)) {
        
        
        // clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        

        //timeValue = glfwGetTime();
        //greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

            
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();    

        //draws from indices
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        // 3 is the number of verticies, possibly add more per triangle (like 3 * triangleNum)
        //antiquated, draws from vertices
        //glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glfwTerminate();

}