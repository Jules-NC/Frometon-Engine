// ImGui - standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)


#define GLFW_INCLUDE_NONE
#include <stdio.h>
#include <iostream>

//#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions. You may freely use any other OpenGL loader such as: glew, glad, glLoadGen, etc.
//#include <glew.h>
#ifndef GLAD_I
    #define GLAD_I
    #include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

#ifndef GLM_I
    #define GLM_I
    #include <glm/glm.hpp>
#endif
#include <glm/glm.hpp>

#include "shader.h"

#ifndef SCONTROLS_I
    #define SCONTROLS_I
    #include "SControls.h"
#endif

#ifndef SGUI_I
    #define SGUI_I
    #include "SGUI.h"
#endif

#ifndef IMGUI_I
    #define IMGUI_I
    #include "imgui.h"
    #include "imgui_impl_glfw.h"
    #include "imgui_impl_opengl3.h"
#endif


#include "MShape.h"

GLFWwindow * WINDOW;
unsigned int WIDTH = 1920 >> 1;
unsigned int HEIGHT = 1080 >> 1;



void StyleColorsSofty(ImGuiStyle* dst = NULL);

int main(int, char**)
{
    float rot_speed = 0;
    float  rot_x = 1;
    float  rot_y = 0;
    float  rot_z = 0;


    SControls::getInstance().init(WIDTH, HEIGHT);
    GLFWwindow* window = SControls::getInstance().getWindows();
    WINDOW = window;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(false); // Enable vsync

    gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    SGUI::getInstance().init();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLuint programID = LoadShaders("../../src/lol.vs", "../../src/lel.fs");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
    //ModelMatrix = glm::rotate(ModelMatrix, 18.f, glm::vec3(1.f, 0.f, 0.f));
    glUseProgram(programID);
    //CShape cddd = CShape();
    //cddd.init("../../res/square.obj", "../../res/TextureGrid.jpg", 0);

    MShape sqd  = MShape();
    sqd.load("../../res/sponza.obj");

    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05, 0.05, 0.05));


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glfwPollEvents();

        SControls::getInstance().computeInputs();
        glm::mat4 ProjectionMatrix = SControls::getInstance().getProjectionMatrix();
        glm::mat4 ViewMatrix = SControls::getInstance().getViewMatrix();
        glm::mat4 MVP;
        ModelMatrix = glm::rotate(ModelMatrix, rot_speed, glm::vec3(rot_x, rot_y, rot_z));

        MVP = ProjectionMatrix * ViewMatrix* sqd.getModelMatrix();
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        SGUI::getInstance().beginDrawFrame();

        //cddd.draw();
        sqd.draw();
        sqd.showGUI();

        SGUI::getInstance().endDrawFrame();

        int display_w, display_h;
        //glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);

    }
    sqd.free();

    //cddd.FreeMemory();
    SGUI::getInstance().cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
