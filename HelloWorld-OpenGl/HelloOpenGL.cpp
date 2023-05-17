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

// Prototype declarations
// ________________________________________________
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Vertices
// ________________________________________________

// Triangle
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

// OpenGL Shading Language
// A language for creating our own shaders.
// ________________________________________________
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2, 1.0f);\n"
"}\0";


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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGl", NULL, NULL);
	
	// This is C++ and we're on our own. We need to handle our own errors. 
	// If the windows couldn't be created, then terminate the program and free up the resources 
	// we allocated.
	// ____________________________________________
	if(window == NULL){
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

	// GRAPHICS PIPELINE: 
	// VERTEX SHADER
	// OpenGL require us to define and compile our own shaders using the OpenGL Shading Language.
	// OpenGL has to dynamically compile this langauge from its source code.
	// _____________________________________________

	// Create a shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Compile the source code into the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// We can check if errors were generated while compiling the source code
	// for the shader. Good for error handling
	int success;
	char infoLog[512];

	// Fetches the status of post-compilation
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// If no success, get the error message
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	// GRAPHICS PIPELINE: 
	// VERTEX SHADER
	// Create shader object, load the source code and compile
	// ____________________________________________
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// GRAPHICS PIPELINE: 
	// Shader Program
	// The shader program is the linked version of multiple shaders combined.
	// _____________________________________________

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for post-link status

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}

	// Activate program
	// Every shader amd reneding call after this now use this object.
	glUseProgram(shaderProgram);

	// Once the shaders were linked into the program, they're no longer needed and can be deleted.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Linking Vertex Attributes
	// The GPU has the vertex data and the shaders but is doesn't know how our vertex data is formatted.
	// Our vertex data is formatted as an array of floats, where every float takes 4 bytes of space.
	// That means that every vertex, that has x, y, and z contains 12 bytes of space.
	// _____________________________________________
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO);

	// The render loop
	// _____________________________________________
	while (!glfwWindowShouldClose(window)) {

		// Handle inputs
		// _________________________________________
		processInput(window);


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

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