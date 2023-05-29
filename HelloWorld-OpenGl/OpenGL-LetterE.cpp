// Edimar Valentin Kery <edimar.valentin@upr.edu> 

// This program draws the letter E on the window using GLFW, GLAD and OpenGl.


// The first thing you must do when working with a GLFW/OpenGl is to include GLAD.
// This is basically a gl loader that loads pointers to OpenGL functions at runtime.
// Is basically what binds OpenGl to this project.
// ________________________________________________
#include <glad.h>


// GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan 
// development on the desktop. It provides a simple API for creating 
// windows, contexts and surfaces, receiving input and events.
// ________________________________________________
#include <GLFW/glfw3.h>

// A standard library mostly used to write in the console
// ________________________________________________
#include <iostream>

#include "shader.h"

// Prototype declarations
// ________________________________________________
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Vertices
// ________________________________________________

// E
float vertices[] = {
	-0.5f,  0.5f, 0.0f, //0
	 0.5f,  0.5f, 0.0f, //1
	-0.3f,  0.3f, 0.0f, //2
	 0.5f,  0.3f, 0.0f, //3
	-0.3f,  0.1f, 0.0f, //4
	 0.5f,  0.1f, 0.0f, //5
	-0.3f, -0.1f, 0.0f, //6
	 0.5f, -0.1f, 0.0f, //7
	-0.3f, -0.3f, 0.0f, //8
	 0.5f, -0.3f, 0.0f, //9
	-0.5f, -0.5f, 0.0f, //10
	 0.5f, -0.5f, 0.0f  //11
};

unsigned int indices[] = {
	0, 1, 3,
	0, 2, 3,
	4, 6, 7,
	5, 4, 7,
	8, 10, 11,
	9, 8, 11,
	10, 8, 0,
	0, 2, 8
};

int main() {
	// This function initializes the GLFW library. Before most GLFW functions can 
	// be used, GLFW must be initialized, and before an application terminates GLFW should 
	// be terminated in order to free any resources allocated during or after initialization.
	// ____________________________________________
	glfwInit();

	// This function is used to set some hints before we create the window. Is basically an initial
	// setup of the context. Here we say the OpenGl context is 3.3. 
	// 3.0(major) + 0.3(minor) Core profile. 
	// ____________________________________________
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create and store the window object. 
	// Parameters: width, height, window's name, GLFWmonitor* monitor, GLFWwindow* share 
	// ____________________________________________
	GLFWwindow* window = glfwCreateWindow(800, 600, "E for Edimar", NULL, NULL);

	// This is C++ and we're on our own. We need to handle our own errors. 
	// If the windows couldn't be created, then terminate the program and free up the resources 
	// we allocated.
	// ____________________________________________
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread. 
	// A context must only be made current on a single thread at a time and each thread can have only a single 
	// current context at a time.
	// _____________________________________________
	glfwMakeContextCurrent(window);

	// All the functions we called so far belonged to the GLFW library and not OpenGL. Now we need to bind OpenGL to the
	// project and we're gonna do that using GLAD.
	// _____________________________________________
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// All OpenGl functions start with gl. First thing we want to do is tell OpenGL the size of our rendering window.
	// We'll do that using the glViewport function.
	// The first two parameters of glViewport set the location of the lower left corner of the window.
	// The other two set the width and height.
	// _____________________________________________
	glViewport(0, 0, 800, 600);

	// Tells GLFW that we have a custom function for handling window resizing.
	// _____________________________________________
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// OpenGL REQUIRES that all objects being drawn are stored inside a Vertex Array Object. It will not draw 
	// if is not a VAO
	// _____________________________________________
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// GRAPHICS PIPELINE: VERTEX INPUT
	// Vertex Buffer Object:
	// Space in GPU memory we're allocating to store our vertex data.
	// _____________________________________________
	unsigned int VBO; // OpenGL asks for the address of this variable and stores an id when it creates the buffer.
	glGenBuffers(1, &VBO);

	// OpenGL has many types of buffers and we want to specify this is a vertex buffer.
	// The right type would be a GL_ARRAY_BUFFER
	// _____________________________________________
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the vertex data to the newly created buffer
	// _____________________________________________
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element Buffer Object:
	// A buffer to store indices that allow us to performed indexed drawing
	// _____________________________________________
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader* shader = new Shader("vertex.glsl", "fragment.glsl");

	// _____________________________________________
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Uncomment this following line for Wireframe mode:
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// The render loop
	// _____________________________________________
	while (!glfwWindowShouldClose(window)) {

		// Handle inputs
		// _________________________________________
		processInput(window);


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float redValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shader->ID, "ourColor");
		shader->use();
		glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

		// Double buffer:
		// When an application draws in a single buffer the resulting image may
		// display flickering issues.This is because the resulting output image is not drawn in an
		// instant, but drawn pixel by pixel and usually from left to right and top to bottom.Because
		// this image is not displayed at an instant to the user while still being rendered to, the result
		// may contain artifacts.To circumvent these issues, windowing applications apply a double
		// buffer for rendering.The front buffer contains the final output image that is shown at
		// the screen, while all the rendering commands draw to the back buffer.As soon as all
		// the rendering commands are finished we swap the back buffer to the front buffer so the
		// image can be displayed without still being rendered to, removing all the aforementioned
		// artifacts.
		// _________________________________________
		glfwSwapBuffers(window);


		glfwPollEvents();
	}


	// De-allocate all resources once they've outlived their purpose:
	// -----------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Clear all of GLFW's resources that were allocated
	// ________________________________________
	glfwTerminate();

	return 0;
}

// Function for resizing the viewport when the user resizes the window
// _________________________________________________
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Function for handling inputs
// _________________________________________________
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}