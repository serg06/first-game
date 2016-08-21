	// Normie shit

#include <iostream>
#include <windows.h>
#include <cstdlib>

#define sec 1000

	// Special shit

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// our vertex shader code to be compiled at runtime
const GLchar * vertexShaderSource = "\
#version 330 core\n\
\n\
layout(location = 0) in vec3 position;\n\
\n\
void main()\n\
{\n\
	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
}\0";

// our fragment shader source code!
const GLchar * fragmentShaderSource = "\n\
#version 330 core\n\
\n\
out vec4 color;\n\
\n\
void main()\n\
{\n\
	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
}";

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
	glfwSetKeyCallback(window, key_callback); // assign the window our key callback function
	glfwSwapInterval(1); // set vsync=on (good for cpu especially with that while loop damn)
	
	return window;
}

// run the game until quit
void updateScreen(GLFWwindow* window) {
	// check and call events (like key callbacks!)
	glfwPollEvents();

	// render
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// put the rendered buffer on the screen, take the old buffer back to render the next frame
	glfwSwapBuffers(window);
}

// NOTE: THESE COMMENTS ARE PROBABLY VERY WRONG
GLuint createTriangleVBO() {
	// vertex for 2d triangle in 3d space
	GLfloat trianglev[] = {
		-0.5f, -0.5f, 0.0f, // bottom left corner
		0.5f,  -0.5f, 0.0f, // bottom right coner
		0.0f,   0.5f, 0.0f  // middle top corner
	};

	// generate buffer object whose memory will be used to store our vertex buffer
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// bind our buffer object memory (VBO) to the GL_ARRAY_BUFFER
	// any calls to GL_ARRAY_BUFFER will modify VBO, our buffer memory
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// now we can call glBufferData to copy our vertex into the buffer's memory,
	// telling it the amount of memory to copy from the object
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglev), trianglev, GL_STATIC_DRAW);
	
	return VBO;
}

void verifyShaderCompiled(GLuint shader, bool notifyOnSuccess = false, char* shaderName = NULL) {
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else if (notifyOnSuccess)
	{
		std::cout << "SHADER COMPILED SUCCESSFULLY";
		if (shaderName != NULL) {
			printf(": \"%s\"", shaderName);
		}
		std::cout << std::endl;
	}
}

GLuint createShader(GLuint shaderType, const GLchar * sourceCode) {
	// create shader
	GLuint shader;
	shader = glCreateShader(shaderType);

	// attach the shader source code to the shader and compile it
	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	// make sure shader compiled correctly, as IDE will not tell me on its own
	char * shaderMessage;

	switch (shaderType) {
	case GL_VERTEX_SHADER:
		shaderMessage = "vertex shader"; break;
	case GL_FRAGMENT_SHADER:
		shaderMessage = "fragment shader"; break;
	default:
		shaderMessage = "unknown shader"; break;
	}

	verifyShaderCompiled(shader, true, "vertex shader");
	return shader;
}

GLuint createVertexShader() {
	return createShader(GL_VERTEX_SHADER, vertexShaderSource);
}

GLuint createFragmentShader() {
	return createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
}

// create triangle
int main() {
	startMessage();
	GLFWwindow* window = initWindow();

		// VERTEX INPUT

	// generate buffer object whose memory will be used to store our vertex buffer
	GLuint VBO = createTriangleVBO();

	// create two required shader
	GLuint vertexShader = createVertexShader();
	GLuint fragmentShader = createFragmentShader();

	// TODO: Create shader program

	while (!glfwWindowShouldClose(window))
	{
		updateScreen(window);
	}
	glfwTerminate();


	endMessage(); return 0;
}