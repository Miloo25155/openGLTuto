// Include standard headers
#include <stdio.h>
#include <stdlib.h>

//Include custom headers
#include "Global.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW (inputs etc...)
#include <GLFW/glfw3.h>
GLFWwindow* window;

//Include GLM (maths...)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include <shader.hpp>

int main( void )
{
    //Initialize GLFW
    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    //Open a window and create OpenGL context
    window = glfwCreateWindow(Global::WIN_WIDTH, Global::WIN_HEIGHT, "Tutorial OpenGL", NULL, NULL);
    if(window == NULL){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
    }

    glfwMakeContextCurrent(window);

    //Init GLEW
    if(glewInit() != GLEW_OK){
        fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
    }

    //To capture the keys being pressed
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //refresh the window with dark blue
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


    GLuint VertexArrayId;
    glGenVertexArrays(1, &VertexArrayId);
    glBindVertexArray(VertexArrayId);

    //Create and compile shaders
    GLuint programId = LoadShaders("Shaders/SimpleTransform.vertexshader", "Shaders/SingleColor.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixId = glGetUniformLocation(programId, "MVP");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)(Global::WIN_WIDTH/Global::WIN_HEIGHT), 0.1f, 100.0f);
    // Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    //Camera matrix
    glm::mat4 View = glm::lookAt(
                        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                        glm::vec3(0,0,0), // and looks at the origin
                        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                    );

    // Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    do{
        //Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        //User shaders
        glUseProgram(programId);

        // Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &MVP[0][0]);


        //1rst attribute buffer: vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,         // stride
            (void*)0  // array buffer offset
        );

        //Draw
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        //Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &VertexArrayId);
    glDeleteProgram(programId);

    // Close OpenGL window and terminate GLFW
	glfwTerminate();


	return 0;
}
