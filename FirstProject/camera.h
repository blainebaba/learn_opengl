#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
private:
	// mouse control
	float cameraHeading = 0;
	float cameraPitch = 0;
	float lastMouseX;
	float lastMouseY;
	bool lastMouseInitialized = false;

	// keyboard control
	float lastKeyboardEventTime = -1;
	float cameraMoveSpeed = 10;

	// camera position & direction
	glm::vec3 cameraPos = glm::vec3(0,0,0);
	glm::vec3 cameraDir = glm::vec3(0, 0, -1);
	glm::mat4 viewMat;
	bool viewMatChanged = true;

	// perspective projection
	float fov;
	float aspectRatio;
	glm::mat4 projectMat;
public:

	Camera(float fov, int windowWidth, int windowHeight ) {
		// init projection matrix
		this->fov = fov;
		aspectRatio = (float)windowWidth / (float)windowHeight;
		projectMat = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);

		// init view matrix
		viewMat = glm::lookAt(cameraPos, cameraPos + cameraDir, glm::vec3(0, 1, 0));
	};

	glm::mat4 getProjectMat() {
		return projectMat;
	}

	glm::mat4 getViewMat() {
		// compute view matrix at get method
		if (viewMatChanged) {
			viewMatChanged = false;
			viewMat = glm::lookAt(cameraPos, cameraPos + cameraDir, glm::vec3(0, 1, 0));
		}
		return viewMat;
	}

	glm::vec3 getViewPos() {
		return cameraPos;
	}

	glm::vec3 getViewDir() {
		return cameraDir;
	}

	void windowSizeChanged(int newWindowWidth, int newWindowHeight) {
		aspectRatio = (float)newWindowWidth / (float)newWindowHeight;
		projectMat = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);
	}

	void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
		if (!lastMouseInitialized) {
			lastMouseX = xpos;
			lastMouseY = ypos;
			lastMouseInitialized = true;
			return;
		}

		cameraHeading -= ((float)xpos - lastMouseX) * 0.001 * M_PI_2;
		cameraPitch -= ((float)ypos - lastMouseY) * 0.001 * M_PI_2;
		if (cameraPitch > M_PI_2) {
			cameraPitch = M_PI_2;
		}
		if (cameraPitch < -M_PI_2) {
			cameraPitch = -M_PI_2;
		}
		lastMouseX = xpos;
		lastMouseY = ypos;

		cameraDir = glm::vec3(-cos(cameraPitch) * sin(cameraHeading), sin(cameraPitch), -cos(cameraPitch) * cos(cameraHeading));
		viewMatChanged = true;
	}

	void keyboardCallBack(GLFWwindow* window) {
		float curTime = glfwGetTime();

		if (lastKeyboardEventTime < 0) {
			lastKeyboardEventTime = curTime;
			return;
		}

		// direction in view space
		glm::vec4 moveDi = glm::vec4(0);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			moveDi.z--;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			moveDi.z++;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			moveDi.x--;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			moveDi.x++;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			moveDi.y--;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			moveDi.y++;
		}
		if (glm::length(moveDi) > 0.1) {
			// convert to world space
			moveDi = glm::inverse(viewMat) * glm::normalize(moveDi);
			glm::vec3 moveDelta = moveDi * ((curTime - lastKeyboardEventTime) * cameraMoveSpeed);
			cameraPos += moveDelta;
			viewMatChanged = true;
		}

		lastKeyboardEventTime = curTime;
	}
};
