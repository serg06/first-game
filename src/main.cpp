// Normie shit

#include <iostream>
#include <windows.h>
#include <cstdlib>

// #define GLEW_STATIC // This breaks stuff...

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <Util.h>
#include <Shader.h>

// initialize glfw, a c library for easy creation of windows
void initGlfw() {
	// initialize glfw library
	glfwInit();

	// set properties for the window which will be used when glfwCreateWindow is called
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // set profile (core or compatability)
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // set window to not resizeable

	suprint("INITIALIZED GLFW");
}

// create a window and make it our current OpenGL context
GLFWwindow* createWindow() {
	// create window AND ITS OWN OpenGL context
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		suprint("Failed to create GLFW window", true);
	}

	// make this window's context our current context so changes to our context will be made to this window's context
	glfwMakeContextCurrent(window);

	suprint("CREATED GLFW WINDOW");
	return window;
}

// initialize glew, a library that maps simple header functions to crazy OpenGL functions/extensions/whatnot
void initGlew() {
	glewExperimental = GL_TRUE;

	// maps simple header functions to OpenGL functions which are found at runtime
	GLenum a = glewInit();

	// check succesfulness + extra logging because I was bored
	switch (a) {
	case GLEW_OK:
		suprint("GLEW INITIALIZED"); break;
	case GLEW_ERROR_NO_GL_VERSION:
		suprint("GLEW INIT ERROR: NO GL VERSION (did you start window?)", true);
	case GLEW_ERROR_GL_VERSION_10_ONLY:
		suprint("GLEW INIT ERROR: GLEW_ERROR_GL_VERSION_10_ONLY", true);
	case GLEW_ERROR_GLX_VERSION_11_ONLY:
		suprint("GLEW INIT ERROR: GLEW_ERROR_GLX_VERSION_11_ONLY", true);
	default:
		suprint("What the fuck... glewInit returned %i??\n", true);
	}
}

// tell openGL the size of our window
void initViewport(GLFWwindow* window) {
	// get the window's dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	// tell opengl our viewable area (window's dimensions)
	glViewport(0, 0, width, height);

	suprint("VIEWPORT INITIALIZED");
}

// watch for key press and act accordingly
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// ESC: quit
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// fully initialize our window
GLFWwindow* initWindow() {
	initGlfw();
	GLFWwindow* window = createWindow();
	initGlew();

	initViewport(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1); // set vsync=on (good for cpu especially with that while loop damn. Why is this not in the tutorial!)
	
	return window;
}

int main() {
	// Set shit up
	suprint("Start!");
	GLFWwindow* window = initWindow();
	Shader shader = Shader("C:/repo/first-game/src/shader/shader.vs", "C:/repo/first-game/src/shader/shader.fs");

	// Vertices for our texture
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};

	// Bind VAO to record VBO/etc calls
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Bind crate texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Load crate image
	int width, height;
	unsigned char* image = SOIL_load_image("C:/repo/first-game/src/img/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	// load image into texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// clear trash
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Write triangles to buffer
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set up position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// set up color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// unbind stuff from buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// select shader
	shader.Use();

	// run program
	while (!glfwWindowShouldClose(window))
	{
		// check for events (like keys pressed!) and call callbacks appropriately
		glfwPollEvents();

		// draw our triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// put the rendered buffer on the screen, take the old buffer back to render the next frame
		glfwSwapBuffers(window);
	}
	
	// clean resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	suprint("End.");
	return 0;
}
