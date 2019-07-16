#include <iostream>
#include "SControls.h"
#include <glm/glm.hpp>					// |glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>


const float PIs2 = M_1_PI/2*10;


void clampVerticalAngle(float * verticalAngle);

void clampVerticalAngle(float * verticalAngle){
    float val = *verticalAngle;
    if(val < PIs2 || val > -PIs2){
        return;
    }

    if(val > PIs2){
        *verticalAngle = PIs2;
    }
    else if (val < -PIs2){
        *verticalAngle = -PIs2;
    }
}

bool SControls::init(int width, int height)
{
	this->WIDTH = width;
	this->HEIGHT = height;

    this->lockView = true;
    this->firstTickOfUnLock = !(this->lockView);

    this->lastLockTimestamp = 0.f;   // Can unlock instatntaneously
    this->horizontalAngle = 3.14f;
    this->verticalAngle = 0.f;
    this->fov = 45.f;
    this->cameraSpeed = 1.f;

    this->position = glm::vec3(0, 0, 5);
    this->direction = glm::vec3(
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
                );

	if (!glfwInit())
	{
		return false;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->WINDOW = glfwCreateWindow(width, height, "Frometon-Engine", NULL, NULL);
	if (WINDOW == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
        return false;
	}

	glfwMakeContextCurrent(WINDOW);

    glfwSetInputMode(this->WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(WINDOW, GLFW_STICKY_KEYS, GL_TRUE);
    return true;
}

GLFWwindow * SControls::getWindows()
{
	return this->WINDOW;
}

void SControls::computeMatrices()
{

}

glm::mat4 SControls::getViewMatrix(){}

glm::mat4 SControls::getProjectionMatrix(){}
