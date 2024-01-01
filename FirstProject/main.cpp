// include order matters
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int textureUnit = 0;

int loadTexture(const char* imagePath, Shader* shader, int& textureId) {
	unsigned int texture;
	glGenTextures(1, &texture);
	// bind object, set target for following operation
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath, &width, &height, &nChannel, 0);

	if (data) {
		if (nChannel == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (nChannel == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "unknown image format" << std::endl;
			return -1;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "fail to load image" << std::endl;
		return -1;
	}

	textureId = texture;

	return 0;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// shader
	Shader shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

	// load texture
	int texture1, texture2;
	loadTexture("container.jpg", &shader, texture1);
	loadTexture("awesomeface.png", &shader, texture2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float vertices[] = {
			// location  // color // texture coords
			-0.5,0.5,0,		1,0,0,      0,1,
			0.5,0.5,0,		0,1,0,      1,1,
			-0.5,-0.5,0,	0,0,1,		0,0,
			0.5,-0.5,0,		1,1,0,		1,0,
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

		// create rotation matrix rotate by time
		glm::mat4 transform = glm::mat4(1);
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0, 0, 1));
		shader.setMat4("transform", glm::value_ptr(transform));

		// draw
		glBindVertexArray(VAO);
		shader.use();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap1
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

