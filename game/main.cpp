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

void start() {
	std::cout << "BEGIN" << std::endl; Sleep(sec);
}

void end() {
	std::cout << "FIN" << std::endl; Sleep(sec);
}

void initGlfw() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void createWindow() {
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		Sleep(1000);
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
}

void initGlew() {
	// glewExperimental = GL_TRUE;
	GLenum a = glewInit();
	switch (a) {
	case GLEW_OK:
		std::cout << "GLEW INITIALIZED" << std::endl; Sleep(sec); break;
	case GLEW_ERROR_NO_GL_VERSION:
		std::cout << "GLEW INIT ERROR: NO GL VERSION (did you start window?)" << std::endl; Sleep(5 * sec); exit(-1);
	case GLEW_ERROR_GL_VERSION_10_ONLY:
		std::cout << "GLEW INIT ERROR: GLEW_ERROR_GL_VERSION_10_ONLY" << std::endl; Sleep(5 * sec); exit(-1);
	case GLEW_ERROR_GLX_VERSION_11_ONLY:
		std::cout << "GLEW INIT ERROR: GLEW_ERROR_GLX_VERSION_11_ONLY" << std::endl; Sleep(5 * sec); exit(-1);
	default:
		printf("What the fuck... glewInit returned %s??\n", a); Sleep(5*sec); exit(-1);
	}
}

int main() {
	start();
	
	initGlfw();
	createWindow();
	initGlew();

	end(); return 0;
}