cmake_minimum_required(VERSION 3.1)
project(Frometon-Engine)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(assimp REQUIRED)
find_package(PkgConfig REQUIRED)

link_directories(${ASSIMP_LIBRARY_DIRS})

add_executable(
	Frometon-Engine
	src/main.cpp
	src/SControls.h
  src/SControls.cpp
	)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(
	${OPENGL_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	)

target_link_libraries(
	Frometon-Engine
	${OPENGL_LIBRARIES}
	${GLUT_LIBRARY}
	${GLFW_LIBRARIES}
	${ASSIMP_LIBRARIES}
	)
