cmake_minimum_required(VERSION 3.1)
project(Frometon-Engine)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
add_executable(Frometon-Engine src/main.cpp)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)


include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
target_link_libraries(Frometon-Engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLFW_LIBRARIES})
