#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// wrapper of a shader program
class Shader {
private:
	int textureUnit = 0;
public:
	unsigned int ID;

	// init shader from file
	Shader(const char* vertexPath, const char* fragmentPath);
	// use this shader
	void use();
	
	// set uniform attributes
	void setBool(const std::string &name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, const float* value);
	void setVec3(const std::string& name, const float* values);
	int loadTexture(const char* imgPath, const std::string& texNameInShader);
};

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// 1. read source code from files
	std::string vShaderCodeString;
	std::string  fShaderCodeString;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderCodeString = vShaderStream.str();
		fShaderCodeString = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "read shader souce code fail" << std::endl;
	}
	const char* vShaderCode = vShaderCodeString.c_str();
	const char* fShaderCode = fShaderCodeString.c_str();

	// 2. compile and link shader
	int success;
	char infoLog[512];

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "shader compiling failed\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "shader compiling failed\n" << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "shader program linking failed\n" << infoLog << std::endl;
	}

	// delete shaders after link
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) {
	int location = glGetUniformLocation(ID, name.c_str());
	glUseProgram(ID);
	glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	int location = glGetUniformLocation(ID, name.c_str());
	glUseProgram(ID);
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) {
	int location = glGetUniformLocation(ID, name.c_str());
	glUseProgram(ID);
	glUniform1f(location, value);
}

void Shader::setMat4(const std::string& name, const float* values) {
	int location = glGetUniformLocation(ID, name.c_str());
	glUseProgram(ID);
	glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

void Shader::setVec3(const std::string& name, const float* values) {
	int location = glGetUniformLocation(ID, name.c_str());
	glUseProgram(ID);
	glUniform3fv(location, 1, values);
}


int Shader::loadTexture(const char* imgPath, const std::string& texNameInShader) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);

	unsigned int texture;
	glGenTextures(1, &texture);
	// bind object, set target for following operation
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imgPath, &width, &height, &nChannel, 0);

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

	// not thread safe
	setInt(texNameInShader, textureUnit);
	textureUnit++;

	return 0;
}

