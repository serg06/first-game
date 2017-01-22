// Normie shit

#include <iostream>
#include <windows.h>
#include <cstdlib>

#define sec 1000


// ON TO THE FUN

// #define GLEW_STATIC // THIS BREAKS STUFF HARDCORE ?? !!
#include <GL/glew.h>
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
void suprint(char * info, bool error = false) {
	std::cout << info << std::endl;
	if (error) system("pause");
}

// bit of console logging so I know the program got here
void startMessage() {
	suprint("BEGIN");
}

// bit of console logging so I know the program got here
void endMessage() {
	suprint("FIN!");
}

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
		suprint("Failed to create GLFW window", true);
		glfwTerminate();
		exit(-1);
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
		suprint("GLEW INIT ERROR: NO GL VERSION (did you start window?)", true); exit(-1);
	case GLEW_ERROR_GL_VERSION_10_ONLY:
		suprint("GLEW INIT ERROR: GLEW_ERROR_GL_VERSION_10_ONLY", true); exit(-1);
	case GLEW_ERROR_GLX_VERSION_11_ONLY:
		suprint("GLEW INIT ERROR: GLEW_ERROR_GLX_VERSION_11_ONLY", true); exit(-1);
	default:
		suprint("What the fuck... glewInit returned %i??\n", true); exit(-1);
	}
}

// tell openGL the size of our window
void initViewport(GLFWwindow* window) {
	// get the window's dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	// tell opengl our viewable area (window's dimensions)
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

// update window viewable area with newest info
void updateScreen(GLFWwindow* window, GLuint VAO, GLuint EBO) {
	// check for events (like keys pressed!) and call callbacks appropriately
	glfwPollEvents();

	// set color to place when color buffer is cleared & clears it
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our triangle
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// put the rendered buffer on the screen, take the old buffer back to render the next frame
	glfwSwapBuffers(window);
}

void verifyShaderCompiled(GLuint shader, bool notifyOnSuccess = false, char* shaderName = NULL) {
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
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

void verifyProgramLinked(GLuint program, bool notifyOnSuccess = false, char* programName = NULL) {
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	else if (notifyOnSuccess)
	{
		std::cout << "PROGRAM LINKED SUCCESSFULLY";
		if (programName != NULL) {
			printf(": \"%s\"", programName);
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

GLuint createShaderProgram(GLuint vShader, GLuint fShader) {
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glLinkProgram(shaderProgram);
	verifyProgramLinked(shaderProgram, true, "shader program");
	return shaderProgram;
}

/* // init data, no longer doing it like this tho

void createTrianglesVBO(GLuint * VBO) {
	// create our [2d in 3d] triangle vertices
	GLfloat trianglev[] = {
		-0.5f, -0.5f, 0.0f, // bottom left corner
		0.5f,  -0.5f, 0.0f, // bottom right coner
		0.0f,   0.5f, 0.0f  // middle top corner
	};

	// create a buffer object for our triangle vertices
	glGenBuffers(1, VBO);

	// bind it to the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	// write the triangle vertices to [the array buffer which is bound to] our VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglev), trianglev, GL_STATIC_DRAW);
}

GLuint createTriangleVAO(GLuint * VBO) {
	// Create Vertex Array Object (data neded to draw an object)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// Bind our vertex array object so that future VBO, EBO, glVertexAttribPointer or glEnableVertexAttribArray calls are stored inside of it.
	glBindVertexArray(VAO);

	createTrianglesVBO(VBO);

	// set the attributes for our bound VAO's 0th attribute
	glVertexAttribPointer(
		0, // our vertex shader has location=0, which I don't quite understand
		3, // size of our vertex attribute, which is a vec3 (does this mean 3D or 3 vertices?)
		GL_FLOAT, // type of vertex data
		GL_FALSE, // bool normalized: whether to map values out of 0-1 range to 0-1
		3 * sizeof(GLfloat), // the space between sets of vertex attributes. Let's say we had xyzcrap and we only needed to pass xyz, we'd do 7*float or w/e. 
							 // can also be set to 0 to let openGL auto-determine it (which it can if shit is tightly-packed (a.k.a. xyzxyzxyzxy..)
		(GLvoid*)0 // offset of where position data begins in buffer
	);

	// enable 0th vertex attribute (ok...)
	glEnableVertexAttribArray(0);

	// unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// unbind the VAO
	glBindVertexArray(0);

	return VAO;
}
*/


// create triangle
int main() {

	startMessage();

	GLFWwindow* window = initWindow(); // create window

	GLuint vertexShader = createVertexShader(); // create vertex shader object
	GLuint fragmentShader = createFragmentShader(); // create fragment shader object
	
	GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader); // create full shader program using those two shader objects
	
	//no longer need shader objects, I assume we delete them because they are on limited vram
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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

	// set the attributes for our bound VAO's 0th attribute
	glVertexAttribPointer(
		0, // our vertex shader has location=0, which I don't quite understand
		3, // size of our vertex attribute, which is a vec3 (does this mean 3D or 3 vertices?)
		GL_FLOAT, // type of vertex data
		GL_FALSE, // bool normalized: whether to map values out of 0-1 range to 0-1
		3 * sizeof(GLfloat), // the space between sets of vertex attributes. Let's say we had xyzcrap and we only needed to pass xyz, we'd do 7*float or w/e. 
							 // can also be set to 0 to let openGL auto-determine it (which it can if shit is tightly-packed (a.k.a. xyzxyzxyzxy..)
		(GLvoid*)0 // offset of where position data begins in buffer
	);

	// enable 0th vertex attribute (ok...)
	glEnableVertexAttribArray(0);

	// unbind various objects from buffers (NOTE: must unbind EBO AFTER VAO, else it will unbind from the VAO)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(shaderProgram);

	// run program until close
	while (!glfwWindowShouldClose(window))
	{
		updateScreen(window, VAO, EBO);
	}
	
	// clean resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	endMessage(); return 0;
}
