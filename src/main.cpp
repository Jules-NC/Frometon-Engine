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
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
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
    GLuint programID = LoadShaders("/lel.fs", "/lol.vs" );
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    // ======================== DESSIN ============================
    char * path = "../../parpaing.jpg";
    char * objSquare = "../../square.obj";

    CShape square = CShape();

    square.init(objSquare, path);

    // ======================== /DESSIN ===========================


    double lastTime = glfwGetTime();
    int nbFrames = 0;
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        double currentTime = glfwGetTime();

        nbFrames++;
        float deltaTime = currentTime - lastTime;

        if (currentTime - lastTime >= 1.0) {
            printf("%f ms/frame\n",1000- 1000.0 / double(deltaTime));
            nbFrames = 0;
            lastTime += 1.0;
        }

        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 MVP;

        MVP = ProjectionMatrix * ViewMatrix;
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
