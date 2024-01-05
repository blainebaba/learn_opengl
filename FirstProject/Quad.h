#pragma once

#include "shader.h"

class Quad {
public:
	Quad() {
		initQuad();
	}

	void draw(Shader& shader, unsigned int texture);
private:
	unsigned int VAO;

	void initQuad();
};

void Quad::draw(Shader& shader, unsigned int texture) {
	shader.use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Quad::initQuad() {
	float vertices[] = {
		// pos      // texCoord
		-1, 1,  0,  0, 1,
		-1, -1, 0,  0, 0, 
		1,  1,  0,  1, 1,
		
		1,  1,  0,  1, 1,
		-1, -1, 0,  0, 0,
		1,  -1, 0,  1, 0,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
}
