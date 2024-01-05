#pragma once

#include <glad/glad.h>
#include <iostream>

class FrameBuffer {
public:
	FrameBuffer(int width, int height) {
		this->width = width;
		this->height = height;
		initFrameBuffer();
	};

	void use() {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	};
	unsigned int getTexture() {
		return colorbuffer;
	};
	void onSizeChange(int width, int height) {
		// re-create framebuffer
		this->width = width;
		this->height = height;
		initFrameBuffer();
	};

private:
	unsigned int framebuffer;
	unsigned int colorbuffer;

	int width;
	int height;

	void initFrameBuffer();
};

void FrameBuffer::initFrameBuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &colorbuffer);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, colorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorbuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "frame buffer not ready" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
