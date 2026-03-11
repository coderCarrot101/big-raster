#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


const int WIDTH = 800;
const int HEIGHT = 600;

GLFWwindow* window;
unsigned int VBO;

void window_resize_adjust_viewport(GLFWwindow* window, int width, int height);
int initialize(void);
int render_loop(void);


float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
}; 


int crashFlag;

int main() {
    crashFlag = initialize();
    if (crashFlag == -1) { return -1;}


    
    glGenBuffers(1, &VBO);  


    crashFlag = render_loop();
    if (crashFlag == -1) { return -1;}
    return 0;
}

void window_resize_adjust_viewport(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, WIDTH, HEIGHT);
}

int initialize(void) {
    std::cout << "Lo, and behold!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Lo, and behold!", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, window_resize_adjust_viewport); 

    return 0;
}



int render_loop(void) {
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();

    return 0;
}