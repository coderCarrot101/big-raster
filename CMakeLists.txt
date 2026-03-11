cmake_minimum_required(VERSION 3.10)

project(OpenGLGame)

set(CMAKE_CXX_STANDARD 17)

# --------------------------------
# Add GLFW
# --------------------------------
add_subdirectory(glfw-3.4)

# --------------------------------
# Include directories
# --------------------------------
include_directories(
    GLAD/include
    glm-master
    stb-master
)

# --------------------------------
# Source files
# --------------------------------
set(SOURCES
    src/main.cpp
    src/glad.c
)

# --------------------------------
# Create executable
# --------------------------------
add_executable(OpenGLGame ${SOURCES})

# --------------------------------
# Link libraries
# --------------------------------
target_link_libraries(OpenGLGame
    glfw
    opengl32
)