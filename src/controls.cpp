// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "controls.h"

using namespace glm;

extern GLFWwindow* WINDOW; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.
extern float SPEED; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

extern glm::mat4 CarCamera; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 );
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs() {

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(WINDOW, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(WINDOW, 1024 / 2, 768 / 2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
    verticalAngle += mouseSpeed * float(768 / 2 - ypos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(WINDOW, GLFW_KEY_Z) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    if (glfwGetKey(WINDOW, GLFW_KEY_UP) == GLFW_PRESS) {
        SPEED += 0.05 * deltaTime;
    }
    if (glfwGetKey(WINDOW, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }

    //	Left
    if (glfwGetKey(WINDOW, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    if (glfwGetKey(WINDOW, GLFW_KEY_Q) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }

    //	Up
    if (glfwGetKey(WINDOW, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += up * deltaTime * speed;
    }
    if (glfwGetKey(WINDOW, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        position -= up * deltaTime * speed;
    }


    float FoV = initialFoV;

    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.01f, 1000000.0f);
    ViewMatrix = glm::inverse(CarCamera);//glm::lookAt(position, position+direction, up);

    glfwSetScrollCallback(WINDOW, scroll_callback);


    lastTime = currentTime;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0) {
        speed*=1.25;
    }
    else {
        speed/=1.25;
    }
}
