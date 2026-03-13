#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <iostream>

/* TODO: ADD THIS
const char *fragmentShaderSource =
#version 330 core
out vec4 FragColor;
  
uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main()
{
    FragColor = ourColor;
}
*/
/*
const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos; // the position variable has attribute position 0\n"
"\n"
"out vec4 vertexColor; // specify a color output to the fragment shader\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n"
"    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color\n"
"}\n\0";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\n\0";
*/

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


float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

int main() {
    initialize();

    glGenBuffers(1, &VBO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //might wanna stwich static to dynamic later, if we change graphics at run time
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    //^ buffers | v vertex array objects

    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    compile_shaders(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    render_loop();

    return 0;
}

void window_resize_adjust_viewport(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, WIDTH, HEIGHT);
}

//compile fragment and vertex shaders
void compile_shaders(unsigned int vertexShader, unsigned int fragmentShader) {
    int tempFlag = 0;
    char infoLog[512];

    //create and complie vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &tempFlag);
    if(!tempFlag) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::exit(EXIT_FAILURE);
    }

    //create and complie fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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

float timeValue = 0;
float greenValue = 0;

void render_loop(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //default using glPolygonMode(GL_FRONT_AND_BACK, ***GL_FILL).
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();    
        
        
        timeValue = glfwGetTime();
        greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        //draws from indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 3 is the number of verticies, possibly add more per triangle (like 3 * triangleNum)
        //antiquated, draws fro vertices
        //glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glfwTerminate();

}