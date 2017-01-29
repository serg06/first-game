// only compile/link once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include <Util.h>

class Shader {
public:
	// Globally accessible program.
	// Feels weird without a getter.
	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		// Strings for shader code
		std::string vertexCode;
		std::string fragmentCode;
		// File stream for shader files
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		try {
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			
			// Back out if failed
			if (!vShaderFile.is_open())
				throw std::ifstream::failure("Unable to open vertex shader.");
			if (!fShaderFile.is_open())
				throw std::ifstream::failure("Unable to open fragment shader.");

			// Create string streams
			std::stringstream vShaderStream, fShaderStream;

			// Send file data to string streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close(); fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (std::ifstream::failure e) {
			suprint(e.what(), true);
		}

		// ensure strings null-terminated
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		GLuint vertex = createShader(GL_VERTEX_SHADER, vShaderCode);
		GLuint fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode);
		
		Program = createShaderProgram(vertex, fragment);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Use() {
		glUseProgram(Program);
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

		verifyShaderCompiled(shader, true, shaderMessage);
		return shader;
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

	GLuint createShaderProgram(GLuint vShader, GLuint fShader) {
		GLuint shaderProgram;
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vShader);
		glAttachShader(shaderProgram, fShader);
		glLinkProgram(shaderProgram);
		verifyProgramLinked(shaderProgram, true, "shader program");
		return shaderProgram;
	}

};

#endif