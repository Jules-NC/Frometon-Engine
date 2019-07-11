cmake_minimum_required(VERSION 3.1)
project(Frometon-Engine)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(assimp REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)

link_directories(${ASSIMP_LIBRARY_DIRS})

add_executable(
	Frometon-Engine
	src/main.cpp
	src/SControls.h
	src/SControls.cpp
	src/CShape.h
	src/CShape.cpp
	src/shader.h
	src/shader.cpp
	src/controls.h
	src/controls.cpp
	)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(
	${OPENGL_INCLUDE_DIRS}
	${GLUT_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	)

target_link_libraries(
	Frometon-Engine
	${OPENGL_LIBRARIES}
	${GLUT_LIBRARY}
	${GLFW_LIBRARIES}
	${ASSIMP_LIBRARIES}
	${GLEW_LIBRARIES}
	)
