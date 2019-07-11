#include <iostream>
#include "SControls.h"
#include <glm/glm.hpp>					// |glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool SControls::init(int width, int height)
{
	this->WIDTH = width;
	this->HEIGHT = height;

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

