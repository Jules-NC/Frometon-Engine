#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class SControls{
public:
	static SControls& getInstance() {
		static SControls instance;
		return instance;
	}

    bool init(int width, int height);
	GLFWwindow* getWindows();

	void computeMatrices();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();


private:
    SControls() = default;
	~SControls() = default;
	SControls(const SControls&) = delete;
	SControls& operator=(const SControls&) = delete;
	
	GLFWwindow* WINDOW;

    int WIDTH;
    int HEIGHT;

    bool lockView;
    bool firstTickOfUnLock;

    float lastLockTimestamp;
    float horizontalAngle;
    float verticalAngle;
    float fov;
    float cameraSpeed;

    glm::vec3 position;
    glm::vec3 direction;
};
