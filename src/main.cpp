// STANDARDS
#include <stdio.h>						// |
#include <stdlib.h>						// |
#include <vector>						// |
#include <iostream>
                                        // LIBS
#include <GL/glew.h>					// |glew
#include <GLFW/glfw3.h>						// |GLFW
#include <glm/glm.hpp>					// |glm
#include "shader.h"			// |
#include "controls.h"			// |
                                        // LIBS
#include "CShape.h"
#include "SControls.h"
using namespace glm;				// NAMESPACE
unsigned int WIDTH = 1024;
unsigned int HEIGHT = 768;
GLFWwindow * WINDOW;
float SPEED;

bool initGLEW();

glm::mat4 Camera = glm::mat4();


bool initGLEW() {
    glewExperimental = true; // Needed for core profile
    const auto glewInitStatus = glewInit();
    if (glewInitStatus != GLEW_OK)
        std::cout << "Could not initialize glew, error: " << glewGetErrorString(glewInitStatus);
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    return true;
}


int main(void)
{
    SControls::getInstance().init(WIDTH, HEIGHT);
    WINDOW = SControls::getInstance().getWindows();

    initGLEW();
    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION));
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER));

    // SHADERS
    GLuint programID = LoadShaders("../src/lol.vs", "../src/lel.fs" );
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    // ======================== DESSIN ============================
    std::string path = "../res/TextureGrid.jpg";
    std::string objSquare = "../res/square.obj";

    CShape square = CShape();

    square.init(objSquare.c_str(), path.c_str());

    // ======================== /DESSIN ===========================
    float lol2 = 0.01f;

    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, 18.f, glm::vec3(1.f, 0.f, 0.f));

    glUseProgram(programID);
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 MVP;

        ModelMatrix = glm::rotate(ModelMatrix, lol2, glm::vec3(lol2, lol2, lol2));

        MVP = ProjectionMatrix * ViewMatrix* ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        square.draw();

        // Swap buffers
        glfwSwapBuffers(WINDOW);
        glfwPollEvents();
    }

    while (glfwGetKey(WINDOW, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(WINDOW) == 0);

    square.FreeMemory();
    glDeleteProgram(programID);
    glfwTerminate();

    return 0;
}
