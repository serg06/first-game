// Normie shit

#include <iostream>
#include <windows.h>
#include <cstdlib>

// #define GLEW_STATIC // This breaks stuff...

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
	suprint("Start!");

	GLFWwindow* window = initWindow(); // create window

	Shader shader = Shader("C:/repo/first-game/src/shader/shader.vs", "C:/repo/first-game/src/shader/shader.fs");

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// Bind our vertex array object so that future VBO, EBO, glVertexAttribPointer or glEnableVertexAttribArray calls are stored inside of it.
	glBindVertexArray(VAO);

	// create vertices for our square made out of two triangles, and store it in a VBO
	GLfloat trianglesv[] = {
		-0.5f, -0.5f, 0.0f, // bottom left corner
		 0.5f, -0.5f, 0.0f, // bottom right corner
		-0.5f,  0.5f, 0.0f, // top left corner
		 0.5f,  0.5f, 0.0f  // top right corner
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglesv), trianglesv, GL_STATIC_DRAW);

	// create indices for VBO for our triangles and store in EBO
	GLuint trianglesi[] = {
		0, 1, 2,
		1, 2, 3
	};

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesi), trianglesi, GL_STATIC_DRAW);

	// set the attributes for our bound VAO's 0th attribute - location!
	glVertexAttribPointer(
		0, // 0th attribute 
		3, // size of our vertex attribute, which is a vec3 (does this mean 3D or 3 vertices?)
		GL_FLOAT, // type of vertex data
		GL_FALSE, // bool normalized: whether to map values out of 0-1 range to 0-1
		3 * sizeof(GLfloat), // the space between sets of vertex attributes. Let's say we had xyzcrap and we only needed to pass xyz, we'd do 7*float or w/e. 
							 // can also be set to 0 to let openGL auto-determine it (which it can if shit is tightly-packed (a.k.a. xyzxyzxyzxy..)
		(GLvoid*)0 // offset of where position data begins in buffer
	);

	// enable 0th attribute, a.k.a. location vertex
	glEnableVertexAttribArray(0);

	// unbind various objects from buffers (NOTE: must unbind EBO AFTER VAO, else it will unbind from the VAO)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLfloat timeValue, greenValue;
	GLint vertexColorLocation = glGetUniformLocation(shader.Program, "ourColor");
	shader.Use();

	// run program until close
	while (!glfwWindowShouldClose(window))
	{
		// check for events (like keys pressed!) and call callbacks appropriately
		glfwPollEvents();

		// set new color
		timeValue = glfwGetTime();
		greenValue = (sin(timeValue) / 2) + 0.5;
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// draw our triangle
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// put the rendered buffer on the screen, take the old buffer back to render the next frame
		glfwSwapBuffers(window);
	}
	
	// clean resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	suprint("End.");
	return 0;
}
