
list(APPEND MAIN_APP_SOURCES
  main.cpp

  #CShape.cpp
  #CShape.h
  shader.cpp
  shader.h
  SControls.cpp
  SControls.h
  SGUI.cpp
  SGUI.h
  MShape.cpp
  MShape.h
  SubShape.cpp
  SubShape.h
  SubTexture.cpp
  SubTexture.h
  #stb_image.h

  imgui_impl_opengl3.h
  imgui_impl_opengl3.cpp
  imgui/imgui_impl_glfw.h
  imgui/imgui_impl_glfw.cpp

  glad/glad.h
  glad/glad.c

  KHR/khrplatform.h
  )

set(
  MAIN_APP_LIBRARIES
  imgui
  glfw
  )

list(APPEND MAIN_APP_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
  )

set(NAME Frometon-Engine)
add_executable(${NAME} ${MAIN_APP_SOURCES})
target_include_directories(${NAME} PUBLIC ${MAIN_APP_INCLUDE_DIRS})
target_link_libraries(${NAME} ${MAIN_APP_LIBRARIES})

find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
link_directories(${ASSIMP_LIBRARY_DIRS})

pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(
	${NAME}
	${OPENGL_LIBRARIES}
	${GLFW_LIBRARIES}
	${ASSIMP_LIBRARIES}
	${GLEW_LIBRARIES}
)
