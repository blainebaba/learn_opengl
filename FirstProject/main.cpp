// chapter 5 exercise: craete two triangle use two fragment shader with different color
//
// Solution:
//   needs two shader program, but because vertex attributes are the same, we should be able to use the same VAO

// include order matters
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"void main(){\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource1 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main() {\n"
"  FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\0";

const char* fragmentShaderSource2 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main() {\n"
"  FragColor = vec4(0.3, 1.0, 0.3, 1.0);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void compileShader(unsigned int shader, const char* shaderSource) {
	int success;
	char infoLog[512];

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "shader compiling failed\n" << infoLog << std::endl;
	}
}

void linkShaderProgram(int program, int vertexShader, int fragmentShader) {
	int success;
	char infoLog[512];

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "shader program linking failed\n" << infoLog << std::endl;
	}
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

	// compile shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, vertexShaderSource);
	unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader1, fragmentShaderSource1);
	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader2, fragmentShaderSource2);

	// shader program
	unsigned int shaderProgram1 = glCreateProgram();
	linkShaderProgram(shaderProgram1, vertexShader, fragmentShader1);
	unsigned int shaderProgram2 = glCreateProgram();
	linkShaderProgram(shaderProgram2, vertexShader, fragmentShader2);

	// clean up shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float vertices[] = {
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// end of this VAO
		glBindVertexArray(NULL);

		// draw
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glUseProgram(shaderProgram2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3*sizeof(unsigned int)));

		// swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
