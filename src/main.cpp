// ImGui - standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
//#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions. You may freely use any other OpenGL loader such as: glew, glad, glLoadGen, etc.
//#include <glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.h"
//#include "controls.h"
#include "SControls.h"
#include "CShape.h"
#include <iostream>


GLFWwindow * WINDOW;
unsigned int WIDTH = 1920;
unsigned int HEIGHT = 1080;



static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void StyleColorsSofty(ImGuiStyle* dst = NULL);

int main(int, char**)
{
    float rot_speed = 0;
    float  rot_x = 1;
    float  rot_y = 0;
    float  rot_z = 0;
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SControls::getInstance().init(WIDTH, HEIGHT);
    GLFWwindow* window = SControls::getInstance().getWindows();
    WINDOW = window;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(false); // Enable vsync

    gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    glEnable(GL_CULL_FACE);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    ImGui::StyleColorsClassic();

    // Load Fonts
    auto font_default = io.Fonts->AddFontDefault();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLuint programID = LoadShaders("../../src/lol.vs", "../../src/lel.fs");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    CShape cddd = CShape();
    cddd.init("../../res/square.obj", "../../res/TextureGrid.jpg");

    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, 18.f, glm::vec3(1.f, 0.f, 0.f));
    glUseProgram(programID);
    int lol2 = 0.01f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glfwPollEvents();

        SControls::getInstance().computeInputs();
        glm::mat4 ProjectionMatrix = SControls::getInstance().getProjectionMatrix();
        glm::mat4 ViewMatrix = SControls::getInstance().getViewMatrix();
        glm::mat4 MVP;
        ModelMatrix = glm::rotate(ModelMatrix, rot_speed, glm::vec3(rot_x, rot_y, rot_z));

        MVP = ProjectionMatrix * ViewMatrix* ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        cddd.draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(font_default);

        {
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::Text("Windows");
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        if (show_another_window)
        {
            bool lol22 = true;
            ImGui::Begin("Another Window", &lol22);
            ImGui::SliderFloat("speed", &rot_speed, -0.03f, .03f);

            ImGui::SliderFloat("x", &rot_x, -1.f, 1.f);
            ImGui::SliderFloat("y", &rot_y, -1.f, 1.f);
            ImGui::SliderFloat("z", &rot_z, -1.f, 1.f);

            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        ImGui::PopFont();

        ImGui::Render();
        int display_w, display_h;


        //glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);

    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
