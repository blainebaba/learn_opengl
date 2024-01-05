// include order matters
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "framebuffer.h"
#include "Quad.h"

int deviceWidth = 1800;
int deviceHeight = 1200;

Camera* camera;

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	deviceWidth = w;
	deviceHeight = h;
	glViewport(0, 0, w, h);
	camera->windowSizeChanged(w, h);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	camera->keyboardCallBack(window);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	camera->mouseCallBack(window, xpos, ypos);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	// create window
	GLFWwindow* window = glfwCreateWindow(deviceWidth, deviceHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initiate GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// setup opengl viewport
	glViewport(0, 0, deviceWidth, deviceHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);

	srand((int)glfwGetTime());

	/////////////////////
	// end of boiler plate
	/////////////////////

	// opengl config
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// camera
	camera = new Camera(glm::radians(45.0f), deviceWidth, deviceHeight);

	// init objects
	Shader objShader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
	Model model = Model("resources/models/backpack/backpack.obj");
	glm::vec4 lightPos = glm::vec4(0, 0, 20, 1);

	// prepare framebuffer
	FrameBuffer framebuffer = FrameBuffer(deviceWidth, deviceHeight);
	Shader quadShader = Shader("shaders/quad_vertex.glsl","shaders/quad_fragment.glsl");
	Quad quad = Quad();


	// render loop
	float previousTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		float curTime = glfwGetTime();
		float deltaTime = curTime - previousTime;
		previousTime = curTime;

		// input
		processInput(window);

		// render to frame buffer
		framebuffer.use();
		glEnable(GL_DEPTH_TEST);

		// render commands
		glClearColor(0.2,0.2,0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// pass newest projection matrix
		objShader.setMat4("projectMat", glm::value_ptr(camera->getProjectMat()));
		objShader.setMat4("viewMat", glm::value_ptr(camera->getViewMat()));
		objShader.setVec3("viewPos", glm::value_ptr(camera->getViewPos()));
		glm::mat4 lightRotateMat = glm::rotate(glm::mat4(1), glm::radians(90.0f * curTime), glm::vec3(0, 1, 0));
		objShader.setVec3("lightPos", glm::value_ptr(glm::vec3(lightRotateMat * lightPos)));

		// draw objects
		model.draw(objShader);

		// draw quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		quad.draw(quadShader, framebuffer.getTexture());
		
		// swap1
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

