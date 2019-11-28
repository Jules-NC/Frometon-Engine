#pragma once
#include <GLFW/glfw3.h>

#ifndef GLM_I
    #define GLM_I
    #include <glm/glm.hpp>
#endif


class SControls{
public:
	static SControls& getInstance() {
		static SControls instance;
		return instance;
	}

    bool init(int width, int height);
	GLFWwindow* getWindows();

    void computeInputs();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    float getCameraSpeed();
    void setCameraSpeed(float f);
    glm::vec3 getPosition();

private:
    SControls() = default;
	~SControls() = default;
	SControls(const SControls&) = delete;
	SControls& operator=(const SControls&) = delete;
	
    //void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    GLFWwindow* WINDOW;

    int width;
    int height;

    bool lockView;
    bool firstTickOfUnLock;

    float lastLockTimestamp;
    float horizontalAngle;
    float verticalAngle;
    float fov;
    float cameraSpeed;
    float mouseSpeed;

    glm::vec3 position;
    glm::vec3 direction;

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
};
