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

    this->WINDOW = glfwCreateWindow(width, height, "Frometon-Engine", NULL, NULL);
	if (WINDOW == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(WINDOW);

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

