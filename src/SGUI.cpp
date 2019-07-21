#define GLFW_INCLUDE_NONE

#ifndef SGUI_I
    #define SGUI_I
    #include "SGUI.h"
#endif
#ifndef SCONTROLS_I
    #define SCONTROLS_I
    #include "SControls.h"
#endif

#ifndef CSHAPE_I
    #define CSHAPE_I
    #include "CShape.h"
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

    static std::vector<CShape*> shapes = CShape::list;
    static int numShapes = shapes.size();

    {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        static bool t = false;
        ImGui::Begin("Debug object");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Spacing();
        ImGui::Checkbox("demo window", &t);
        if(t) ImGui::ShowDemoWindow();
        ImGui::Spacing();

        ImGui::LabelText("num Objs", "%d", shapes.size());
        for(int s=0; s<numShapes; ++s)
        {
            const aiScene * scene = shapes[s]->scene;
            if(ImGui::TreeNode((void*)(intptr_t)s, "Obj %d", s))
            {
                if(scene->HasMeshes() && ImGui::TreeNode("Meshes"))
                {
                    ImGui::LabelText("num Meshes", "%d", scene->mNumMeshes);
                    ImGui::TreePop();
                    aiMesh ** meshes = scene->mMeshes;
                    for(int i=0; i<scene->mNumMeshes; ++i)
                    {
                        aiMesh * mesh = meshes[i];
                        if(ImGui::TreeNode((void*)(intptr_t)i, "Mesh %d", i))
                        {
                            ImGui::LabelText("num vertices", "%d", mesh->mNumVertices);
                            ImGui::TreePop();
                        }
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();

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

/*

    {
        static bool t = false;
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        ImGui::Text("Windows");
        ImGui::SliderFloat("speed", &rot_speed, -0.03f, .03f);
        ImGui::SliderFloat("x", &rot_x, -1.f, 1.f);
        ImGui::SliderFloat("y", &rot_y, -1.f, 1.f);
        ImGui::SliderFloat("z", &rot_z, -1.f, 1.f);
        ImGui::Checkbox("demo window", &t);
        if(t) ImGui::ShowDemoWindow();

*/
