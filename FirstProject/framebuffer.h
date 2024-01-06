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
	glBindTexture(GL_TEXTURE_2D, colorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorbuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "frame buffer not ready" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
