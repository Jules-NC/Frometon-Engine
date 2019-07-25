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

#include "stb_image.h"

GLFWwindow * WINDOW;
unsigned int WIDTH = 1920 ;//>> 1;
unsigned int HEIGHT = 1080;// >> 1;



void StyleColorsSofty(ImGuiStyle* dst = NULL);

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;

            stbi_image_free(data);
            exit(0);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


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


    glfwSwapInterval(true); // Enable vsync

    gladLoadGL((GLADloadfunc) glfwGetProcAddress);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    SGUI::getInstance().init();

    const std::string OPENGL_VERSION = std::string((char *)glGetString(GL_VERSION));

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLuint programID = LoadShaders("../../src/lol.vs", "../../src/lel.fs");
    //GLuint cubemapID = LoadShaders("../../src/cubemap.vs", "../../src/cubemap.fs");

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
    glUseProgram(programID);

    MShape sqd  = MShape();
    sqd.load("../../res/sponza.obj");

    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.01, 0.01, 0.01));


    // CUBEMAP
    GLuint cubemapID = LoadShaders("../../src/cubemap.vs", "../../src/cubemap.fs");
    GLuint cubemapMVID = glGetUniformLocation(cubemapID, "MV");
    glm::vec3 sunPos = glm::vec3(1.f, 0.f, 0.f);

    GLuint SunPosID = glGetUniformLocation(cubemapID, "uSunPos");

    std::vector<std::string> textures_faces = std::vector<std::string>();
    textures_faces.push_back("../../res/purplenebula_bk.tga");
    textures_faces.push_back("../../res/purplenebula_dn.tga");
    textures_faces.push_back("../../res/purplenebula_ft.tga");
    textures_faces.push_back("../../res/purplenebula_lf.tga");
    textures_faces.push_back("../../res/purplenebula_rt.tga");
    textures_faces.push_back("../../res/purplenebula_up.tga");
    unsigned int cubemapTexture = loadCubemap(textures_faces);

    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    // CUBEMAP

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glfwPollEvents();
        SControls::getInstance().computeInputs();
        glm::mat4 ProjectionMatrix = SControls::getInstance().getProjectionMatrix();
        glm::mat4 ViewMatrix = SControls::getInstance().getViewMatrix();
        glm::mat4 MVP;





        glUseProgram(programID);

        ModelMatrix = glm::rotate(ModelMatrix, rot_speed, glm::vec3(rot_x, rot_y, rot_z));

        MVP = ProjectionMatrix * ViewMatrix* sqd.getModelMatrix();
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        SGUI::getInstance().beginDrawFrame();

        sqd.draw();

        ImGui::Text("Version : %s", OPENGL_VERSION.c_str());

        ImGui::SliderFloat("Sun X", &sunPos[0], -3.14, 3.14);
        ImGui::SliderFloat("Sun Y", &sunPos[1], -0.14, .3);
        ImGui::SliderFloat("Sun Z", &sunPos[2], -3.14, 3.14);

        sqd.showGUI();



        // CUBEMAP 2
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

        glUseProgram(cubemapID);

        glm::mat4 view = glm::mat4(glm::mat3(SControls::getInstance().getViewMatrix()));
        MVP = ProjectionMatrix*view;

        glUniformMatrix4fv(cubemapMVID, 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(SunPosID, 1, &sunPos[0]);


        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 36);

       glBindVertexArray(0);

       glDepthFunc(GL_LESS); // set depth function back to default
        // END CUBEMAP 2


       SGUI::getInstance().endDrawFrame();

        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);


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
