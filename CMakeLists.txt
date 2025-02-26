cmake_minimum_required(VERSION 3.10.0)
project(learnopengl VERSION 0.1.0)

add_executable(learnopengl src/main.cpp src/glad.c src/shader.hpp src/shader.cpp) 

# glfw
include_directories("/home/lpdgrl/Project/code/learnopengl/src/includes/")

# glad 
include_directories("/home/lpdgrl/Project/code/learnopengl/src/includes/glad/")
include_directories("/home/lpdgrl/Project/code/learnopengl/src/includes/glad/khr")

# link libglfw3.a
target_link_libraries(learnopengl "/home/lpdgrl/Project/code/learnopengl/src/libs/libglfw3.a")
