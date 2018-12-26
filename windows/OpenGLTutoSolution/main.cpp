#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "src/common/GlobalConfig.h"
#include "src/headers/shader.h"

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
	}
}

int main(void) {

	if (!glfwInit()) {
		fprintf(stderr, "Failed to init GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	//Open a window and create OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Tutorial OpenGL", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Init GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Depth test
	/*glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);*/

	GLfloat data[] = {
		-0.5f, -0.5f, 0.0f,
		 //1.0f, 0.0f, 0.0f,

		 0.5f, -0.5f, 0.0f,
		 //0.0f, 1.0f, 0.0f,

		 0.0f, 0.5f, 0.0f
		 //0.0f, 0.0f, 1.0f,
	};

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	////COLOR
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));

	GLushort indices[] = { 0, 1, 2 };

	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	GLCheckError();

	GLuint programId = LoadShaders("shader/VertexShader.vertexshader", "shader/FragmentShader.fragmentshader");
	glUseProgram(programId);

	GLCheckError();

	do {
		glClear(GL_COLOR_BUFFER_BIT);

		//glViewport(0, 0, WIDTH, HEIGHT);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//POSITION
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		GLCheckError();

		GLClearError();
		glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_SHORT, nullptr);
		GLCheckError();

		glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);

		//Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteProgram(programId);

	glfwTerminate();

	return 0;
}
