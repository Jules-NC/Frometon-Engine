#pragma once
#include <GLFW/glfw3.h>


class SControls{
public:
	static SControls& getInstance() {
		static SControls instance;
		return instance;
	}

	bool init(int width, int height);
	GLFWwindow* getWindows();
	void computeMatrices();

private:
	SControls() = default;
	~SControls() = default;
	SControls(const SControls&) = delete;
	SControls& operator=(const SControls&) = delete;
	
	GLFWwindow* WINDOW;

	int WIDTH;
	int HEIGHT;
};

