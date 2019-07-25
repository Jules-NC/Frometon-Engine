#include <iostream>
#include "SControls.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#ifndef GLM_I
    #define GLM_I
    #include <glm/glm.hpp>
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <math.h>


const float PIs2 = M_1_PI/2*10;


void clampVerticalAngle(float * verticalAngle);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void glfw_error_callback(int error, const char* description);


bool SControls::init(int width, int height)
{
    this->width = width;
    this->height = height;

    this->lockView = true;
    this->firstTickOfUnLock = true;

    this->lastLockTimestamp = 0.f;   // Can unlock instatntaneously
    this->horizontalAngle = 3.14f;
    this->verticalAngle = 0.f;
    this->fov = 45.f;
    this->cameraSpeed = .5f;
    this->mouseSpeed = 0.005f;

    this->position = glm::vec3(0, 0, 5);
    this->direction = glm::vec3(
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
                );

    glfwSetErrorCallback(glfw_error_callback); // TODO: know why
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

    //glfwSetInputMode(this->WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(WINDOW, GLFW_STICKY_KEYS, GL_TRUE);
    return true;
}

GLFWwindow * SControls::getWindows()
{
	return this->WINDOW;
}

void SControls::computeInputs()
{
    if(glfwGetKey(WINDOW, GLFW_KEY_ESCAPE)  == GLFW_PRESS){
        glfwSetWindowShouldClose(WINDOW, true);
    }

    if(firstTickOfUnLock){
        glfwSetCursorPos(WINDOW, width/2, height/2);
        firstTickOfUnLock = false;
    }

    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    double xpos, ypos;
    glfwGetCursorPos(WINDOW, &xpos, &ypos);

    if(!lockView){
        glfwSetCursorPos(WINDOW, width / 2, height / 2);
        horizontalAngle += mouseSpeed * float(width / 2 - xpos);
        verticalAngle += mouseSpeed * float(height / 2 - ypos);
        clampVerticalAngle(&verticalAngle);
    }

    direction = glm::vec3(
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
                );

    glm::vec3 right = glm::vec3(
                sin(horizontalAngle - 3.14f / 2.0f),
                0,
                cos(horizontalAngle - 3.14f / 2.0f)
                );

    glm::vec3 up = glm::cross(right, direction);


    if(glfwGetKey(WINDOW, GLFW_KEY_P) == GLFW_PRESS){
        float now = glfwGetTime();
        if(now - lastLockTimestamp > 0.15){
            lockView = !lockView;
            lastLockTimestamp = now;
            firstTickOfUnLock = true;
            if(!lockView){
                glfwSetInputMode(SControls::getInstance().getWindows(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else{
                glfwSetInputMode(SControls::getInstance().getWindows(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }


    if (glfwGetKey(WINDOW, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * cameraSpeed;
    }
    if (glfwGetKey(WINDOW, GLFW_KEY_S) == GLFW_PRESS){
        position -= direction * deltaTime * cameraSpeed;
    }

    if (glfwGetKey(WINDOW, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * cameraSpeed;
    }
    if (glfwGetKey(WINDOW, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * cameraSpeed;
    }

    if (glfwGetKey(WINDOW, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += up * deltaTime * cameraSpeed;
    }
    if (glfwGetKey(WINDOW, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        position -= up * deltaTime * cameraSpeed;
    }

    this->projMatrix = glm::perspective(this->fov, 16.0f / 9.0f, 0.01f, 10000.f);
    this->viewMatrix = glm::lookAt(position, position+direction, up);

    //g


    glfwSetScrollCallback(WINDOW, scroll_callback);

    lastTime = currentTime;
}

glm::mat4 SControls::getViewMatrix(){
    return this->viewMatrix;
}

glm::mat4 SControls::getProjectionMatrix(){
    return this->projMatrix;
}

float SControls::getCameraSpeed(){
    return this->cameraSpeed;
}

void SControls::setCameraSpeed(float f){
    this->cameraSpeed = f;
}


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    float speed = SControls::getInstance().getCameraSpeed();
    if (yoffset > 0) {
        SControls::getInstance().setCameraSpeed(speed*1.25);
    }
    else {
        SControls::getInstance().setCameraSpeed(speed/1.25);
    }
}

void clampVerticalAngle(float * verticalAngle){
    float val = *verticalAngle;
    if(val > PIs2){
        *verticalAngle = PIs2;
    }
    else if (val < -PIs2){
        *verticalAngle = -PIs2;
    }
}
