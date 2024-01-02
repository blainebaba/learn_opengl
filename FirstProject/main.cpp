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

// define in object coordinate
float vertices[] = {
	// location // color   // texture coords	// normal
	-1,1,0,		1,0,0,      0,1,				0,0,1,
	1,1,0,		0,1,0,      1,1,				0,0,1,			
	-1,-1,0,	0,0,1,		0,0,				0,0,1,
	1,-1,0,		1,1,0,		1,0,				0,0,1,
};

float lightVertices[] = {
	// location  
	-1,1,0,		
	1,1,0,		
	-1,-1,0,	
	1,-1,0,		
};


unsigned int elements[] = {
	0, 2, 1,
	1, 2, 3,
};


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

// the basic plane
void drawPlane(glm::mat4 modelMat, Shader* shader) {
	shader->use();
	shader->setMat4("modelMat", glm::value_ptr(modelMat));
	glm::vec4 norm = modelMat * glm::vec4(0, 1, 0, 0);
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

unsigned int initLightVAO() {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO); // first arg: number of buffers to generate
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// link vertex attributes
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// end of this VAO
	glBindVertexArray(NULL);

	return VAO;
}

unsigned int initObjectVAO() {
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
	int stride = 11;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// end of this VAO
	glBindVertexArray(NULL);

	return VAO;
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
	objShader.loadTexture("resources/container2.png", "material.diffuseMap");
	objShader.loadTexture("resources/container2_specular.png", "material.specularMap");
	objShader.setFloat("material.shininess", 32);
	unsigned int objVAO = initObjectVAO();
	glm::mat4 modelMats[20];
	for (int i = 0; i < 20; i++) {
		glm::mat4 modelMat = glm::mat4(1);
		modelMats[i] = glm::translate(modelMat, glm::vec3(-10 + rand() % 20, -10 + rand() % 20, -10 + rand() % 20));
	}

	// init lights
	// Shader lightShader = Shader("shaders/light_vertex.glsl", "shaders/light_fragment.glsl");
	// glm::vec3 lightPos = glm::vec3(0, 5, -5);
	// unsigned int lightVAO = initLightVAO();
	// glm::mat4 lightModelMatrix = glm::mat4(1);
	// lightModelMatrix = glm::translate(lightModelMatrix, lightPos);
	// lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.2, 0.2, 0.2));

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render commands
		glClearColor(0,0,0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// rotate light by time
		glm::mat4 lightRotateMatrix = glm::rotate(glm::mat4(1), (float)glfwGetTime(), glm::vec3(0, 1, 0));

		// pass newest projection matrix
		objShader.setMat4("projectMat", glm::value_ptr(camera->getProjectMat()));
		objShader.setMat4("viewMat", glm::value_ptr(camera->getViewMat()));
		// lightShader.setMat4("projectMat", glm::value_ptr(camera->getProjectMat()));
		// lightShader.setMat4("viewMat", glm::value_ptr(camera->getViewMat()));

		objShader.setVec3("viewPos", glm::value_ptr(camera->getViewPos()));
		// glm::vec3 rotatedLightPos = lightRotateMatrix * glm::vec4(lightPos, 1);
		objShader.setVec3("light.lightPos", glm::value_ptr(camera->getViewPos()));
		objShader.setVec3("light.lightDir", glm::value_ptr(camera->getViewDir()));


		objShader.setFloat("light.constant", 1);
		objShader.setFloat("light.linear", 0.8);
		objShader.setFloat("light.quadratic", 1.5);
		objShader.setFloat("light.cutoff", cos(M_PI_2 * 0.25));
		objShader.setFloat("light.cutoffStart", cos(M_PI_2 * 0.15));

		// draw objects
		glBindVertexArray(objVAO);
		for (int i = 0; i < 20; i++) {
			drawCube(modelMats[i], &objShader);
		}

		// draw light
		// glBindVertexArray(lightVAO);
		// drawCube(lightRotateMatrix * lightModelMatrix, &lightShader);

		// swap1
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

