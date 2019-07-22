#define GLFW_INCLUDE_NONE

#ifndef SGUI_I
    #define SGUI_I
    #include "SGUI.h"
#endif
#ifndef SCONTROLS_I
    #define SCONTROLS_I
    #include "SControls.h"
#endif


#ifndef IMGUI_I
    #define IMGUI_I
    #include "imgui.h"
    #include "imgui_impl_glfw.h"
    #include "imgui_impl_opengl3.h"
#endif

#include <assimp/scene.h>
#include <sstream>
#include <string>
#include <iostream>


void SGUI::init(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & p_io = ImGui::GetIO();
    (void)p_io;
    io = p_io;
    ImGui_ImplGlfw_InitForOpenGL(SControls::getInstance().getWindows(), true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsClassic();
    auto font_default = io.Fonts->AddFontDefault();
}


void SGUI::beginDrawFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        static bool t = false;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Spacing();
        ImGui::Checkbox("demo window", &t);
        if(t) ImGui::ShowDemoWindow();
        ImGui::Spacing();
    }

}

void SGUI::endDrawFrame(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SGUI::cleanup(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
