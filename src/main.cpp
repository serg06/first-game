#include <iostream>
#include <windows.h>

#define sec 1000

// GLEW
#define GLEW_STATIC
#define STATIC_GLEW
#include <GL/glew.h>
#include <GL/GL.h>

// GLFW
#include <GLFW/glfw3.h>

/* Change to 64-bit project using http://stackoverflow.com/a/4364020
 * Add include folder to project VC++ include directories
 * Add build/src/Debug folder to VC++ library directories
 * Add .lib file name to linker/input
 */

// stands for super print cuz fuck you
void suprint(char * info, int secs = 0) {
	std::cout << info << std::endl; Sleep(secs * 1000);
}

void startMessage() {
	suprint("BEGIN");
}

void endMessage() {
	suprint("FIN!");
}

void initGlfw() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	suprint("INITIALIZED GLFW");
}

GLFWwindow* createWindow() {
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		suprint("Failed to create GLFW window", 5*sec);
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	suprint("CREATED GLFW WINDOW");
	return window;
}

// initialize GLEW, the guys with the functions mapping to the correct shit
void initGlew() {
	// glewExperimental = GL_TRUE; // why tf this don't work
	GLenum a = glewInit();
	switch (a) {
	case GLEW_OK:
		suprint("GLEW INITIALIZED"); break;
	case GLEW_ERROR_NO_GL_VERSION:
		suprint("GLEW INIT ERROR: NO GL VERSION (did you start window?)", 5); exit(-1);
	case GLEW_ERROR_GL_VERSION_10_ONLY:
		suprint("GLEW INIT ERROR: GLEW_ERROR_GL_VERSION_10_ONLY", 5); exit(-1);
	case GLEW_ERROR_GLX_VERSION_11_ONLY:
		suprint("GLEW INIT ERROR: GLEW_ERROR_GLX_VERSION_11_ONLY", 5); exit(-1);
	default:
		printf("What the fuck... glewInit returned %i??\n", a); Sleep(5*sec); exit(-1);
	}
}

// tell openGL the size of our window
void initViewport(GLFWwindow* window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	suprint("VIEWPORT INITLALIZED");
}

// watch for key press and act accordingly
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// ESC: quit
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

GLFWwindow* initWindow() {
	initGlfw();
	GLFWwindow* window = createWindow();
	initGlew();
	initViewport(window); // tell openGl the size of our window
	
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1); // set vsync=on (good for cpu especially with that while loop damn)
	
	return window;
}

int main() {
	startMessage();
	
	GLFWwindow* window = initWindow();
	
	while (!glfwWindowShouldClose(window))
	{
		// check and call events (like key callbacks!)
		glfwPollEvents();

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// put the rendered buffer on the screen, take the old buffer back to render the next frame
		glfwSwapBuffers(window);
	}
	glfwTerminate();


	endMessage(); return 0;
}