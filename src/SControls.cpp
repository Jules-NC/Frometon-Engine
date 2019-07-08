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
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->WINDOW = glfwCreateWindow(width, height, "Tutorial 6650 - Keyboard and Mouse", NULL, NULL);
	if (WINDOW == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
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

