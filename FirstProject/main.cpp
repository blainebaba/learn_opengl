// include order matters

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "cube.h"
#include "quad.h"

using namespace std;
using namespace glm;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 1800, height = 1200;
	GLFWwindow* window = glfwCreateWindow(width, height, "Ch26_Framebuffer", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// framebuffer
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	// GL_FRAMEBUFFER = GL_DRAW_BUFFER + GL_READ_BUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint texture;
	glGenTextures(1, &texture);
	// states for object initialized when it first bind
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// The initial value of GL_TEXTURE_MIN_FILTER is GL_NEAREST_MIPMAP_LINEAR.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "framebuffer not completed 0x" << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER) << endl;
	}

	// quad
	Quad quad;

	// init
	Shader shader = Shader("shaders/vs.glsl", "shaders/fs.glsl");
	Cube cube;
	glm::mat4 projectMat = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 viewMat = glm::lookAt(vec3(0, 0, 4), vec3(0,0,0), vec3(0, 1, 0));


	while (!glfwWindowShouldClose(window)) {

		// first pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.2, 0.2, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setMat4("projectMat", projectMat);
		shader.setMat4("viewMat", viewMat);
		cube.draw();

		// second pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		quad.draw(depthMap);

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteFramebuffers(1, &framebuffer);
	glfwTerminate();
}