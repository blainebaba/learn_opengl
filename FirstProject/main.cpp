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

int deviceWidth = 1800;
int deviceHeight = 1200;

Shader* shader;
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

// the basic plane
void drawPlane(glm::mat4 modelMat, Shader* shader) {
	shader->setMat4("modelMat", glm::value_ptr(modelMat));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawCube(glm::mat4 modelMat, Shader* shader) {
	// plane is on XY plane, center at origin, let's create a cube that center at origin

	glm::mat4 planeMat;
	// front face
	planeMat = glm::mat4(1);
	planeMat = glm::translate(planeMat, glm::vec3(0, 0, 1));
	drawPlane(modelMat*planeMat, shader);
	// back face
	planeMat = glm::mat4(1);
	planeMat = glm::rotate(planeMat, glm::radians(180.0f), glm::vec3(1, 0, 0));
	planeMat = glm::translate(planeMat, glm::vec3(0, 0, 1));
	drawPlane(modelMat*planeMat, shader);
	// up face
	planeMat = glm::mat4(1);
	planeMat = glm::rotate(planeMat, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	planeMat = glm::translate(planeMat, glm::vec3(0, 0, 1));
	drawPlane(modelMat*planeMat, shader);
	//// donw face
	planeMat = glm::mat4(1);
	planeMat = glm::rotate(planeMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
	planeMat = glm::translate(planeMat, glm::vec3(0, 0, 1));
	drawPlane(modelMat*planeMat, shader);
	//// left face
	planeMat = glm::mat4(1);
	planeMat = glm::rotate(planeMat, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	planeMat = glm::translate(planeMat, glm::vec3(0, 0, 1));
	drawPlane(modelMat*planeMat, shader);
	//// right face
	planeMat = glm::mat4(1);
	planeMat = glm::rotate(planeMat, glm::radians(90.0f), glm::vec3(0, 1, 0));
	planeMat = glm::translate(planeMat, glm::vec3(0, 0, 1));
	drawPlane(modelMat*planeMat, shader);
}


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	// shader
	shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

	// camera
	camera = new Camera(glm::radians(45.0f), deviceWidth, deviceHeight);

	// load texture
	int texture1, texture2;
	shader->loadTexture("container.jpg", "texture1");
	shader->loadTexture("awesomeface.png", "texture2");

	// opengl config
	glEnable(GL_DEPTH_TEST);

	// init model
	glm::mat4 modelMats[20];
	for (int i = 0; i < 20; i++) {
		glm::mat4 modelMat = glm::mat4(1);
		modelMats[i] = glm::translate(modelMat, glm::vec3(-10 + rand() % 20, -10 + rand() % 20, -10 - rand() % 20));
	}

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// define in object coordinate
		float vertices[] = {
			// location  // color // texture coords
			-1,1,0,		1,0,0,      0,1,
			1,1,0,		0,1,0,      1,1,
			-1,-1,0,	0,0,1,		0,0,
			1,-1,0,		1,1,0,		1,0,
		};
		unsigned int elements[] = {
			0, 2, 1,
			1, 2, 3,
		};

		// VAO: vertex array object, stores vertex config, including VBO
		// later config on VBO and vertex attributes are stored in this VAO
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO: vertex buffer objects, stores vertex values in a buffer
		unsigned int VBO;
		glGenBuffers(1, &VBO); // first arg: number of buffers to generate
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// EBO: element buffer object, stores vertex indexes
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

		// link vertex attributes
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
		glEnableVertexAttribArray(2);

		// end of this VAO
		glBindVertexArray(NULL);

		// pass newest projection matrix
		shader->setMat4("projectMat", glm::value_ptr(camera->getProjectMat()));
		shader->setMat4("viewMat", glm::value_ptr(camera->getViewMat()));

		// draw
		glBindVertexArray(VAO);
		shader->use();
		for (int i = 0; i < 20; i++) {
			drawCube(modelMats[i], shader);
		}

		// swap1
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

