cmake_minimum_required(VERSION 3.19)
project(openGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(openGL src/main.cpp)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(openGL glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(openGL OpenGL::GL)
find_package(GLEW REQUIRED)
target_link_libraries(openGL GLEW)


