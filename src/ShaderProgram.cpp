/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderProgram.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 22:28:49 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/17 00:55:29 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderProgram.hpp"

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->_id);
}

// Can throw ShaderCreationException, ShaderCompilationException,
// ShaderProgramCreationException, ShaderProgramLinkingException
// and FileReadingException.
ShaderProgram::ShaderProgram(const std::string &vertexShaderSourceFile, const std::string &fragmentShaderSourceFile)
{
	std::string		vertexShaderString;
	std::string		fragmentShaderString;

	std::ifstream	vertexShaderIF;
	std::ifstream	fragmentShaderIF;
	// Ensures ifstream objects throw exceptions if failbit or badbit is set
	vertexShaderIF.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderIF.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderIF.open(vertexShaderSourceFile);
		std::stringstream	vertexShaderStream;
		vertexShaderStream << vertexShaderIF.rdbuf();
		vertexShaderIF.close();
		vertexShaderString = vertexShaderStream.str();
	}
	catch (...)
	{
		std::cout << "READING VERTEX SHADER FROM " << vertexShaderSourceFile << " FAILED" << std::endl;
		throw FileReadingException();
	}

	try
	{
		fragmentShaderIF.open(fragmentShaderSourceFile);
		std::stringstream	fragmentShaderStream;
		fragmentShaderStream << fragmentShaderIF.rdbuf();
		fragmentShaderIF.close();
		fragmentShaderString = fragmentShaderStream.str();
	}
	catch (...)
	{
		std::cout << "READING FRAGMENT SHADER FROM " << fragmentShaderSourceFile << " FAILED" << std::endl;
		throw FileReadingException();
	}

	const GLchar	*vertexShaderSource = vertexShaderString.c_str();
	const GLchar	*fragmentShaderSource = fragmentShaderString.c_str();

	initialize(1, &vertexShaderSource, 1, &fragmentShaderSource);
}

// Can throw ShaderCreationException, ShaderCompilationException,
// ShaderProgramCreationException and ShaderProgramLinkingException
// The "Count" argument is the number of strings in the shader's source
ShaderProgram::ShaderProgram(GLsizei vertexCount, const GLchar **vertexShaderSource, GLsizei fragmentCount, const GLchar ** fragmentShaderSource)
{
	initialize(vertexCount, vertexShaderSource, fragmentCount, fragmentShaderSource);
}

void	ShaderProgram::initialize(GLsizei vertexCount, const GLchar **vertexShaderSource, GLsizei fragmentCount, const GLchar ** fragmentShaderSource)
{
	// VERTEX SHADER CREATION
	GLuint	vertexShader;
	try
	{
		vertexShader = compileShader(GL_VERTEX_SHADER, vertexCount, vertexShaderSource, NULL);
	}
	catch (ShaderCreationException &err)
	{
		std::cout << "VERTEX SHADER CREATION FAILED\n" << std::endl;
		throw ;
	}
	catch (CompilationException &err)
	{
		std::cout << "VERTEX SHADER COMPILATION FAILED\n" << std::endl;
		throw ;
	}

	// FRAGMENT SHADER CREATION
	GLuint	fragmentShader;
	try
	{
		fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCount, fragmentShaderSource, NULL);
	}
	catch (ShaderCreationException &err)
	{
		glDeleteShader(vertexShader);
		std::cout << "FRAGMENT SHADER CREATION FAILED\n" << std::endl;
		throw ;
	}
	catch (CompilationException &err)
	{
		glDeleteShader(vertexShader);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << std::endl;
		throw ;
	}

	// SHADER PROGRAM CREATION
	this->_id = glCreateProgram();
	if (!this->_id)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		std::cout << "SHADER PROGRAM CREATION FAILED\n" << std::endl;
		throw CreationException();
	}
	glAttachShader(this->_id, vertexShader);
	glAttachShader(this->_id, fragmentShader);
	try
	{
		link();
	}
	catch (LinkingException &err)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(this->_id);
		std::cout << "SHADER PROGRAM LINKING FAILED\n" << std::endl;
		throw ;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void	ShaderProgram::use()
{
	if (!this->_id)
		return ;
	glUseProgram(this->_id);
}

void	ShaderProgram::setBool(const std::string &name, const bool value) const
{
	if (!this->_id)
		return ;
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cout << "BOOL UNIFORM " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform1i(uni_id, value);
}

void	ShaderProgram::setInt(const std::string &name, const int value) const
{
	if (!this->_id)
		return ;
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cout << "INT UNIFORM " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform1i(uni_id, value);
}

void	ShaderProgram::setFloat(const std::string &name, const float value) const
{
	if (!this->_id)
		return ;
	GLint	uni_id = glGetUniformLocation(this->_id, name.c_str());
	if (uni_id == -1)
	{
		std::cout << "FLOAT UNIFORM " << name << " NOT FOUND\n" << std::endl;
		throw UniformNotFoundException();
	}
	glUniform1f(uni_id, value);
}

// Compiles a shader and returns its ID.
// Throws ShaderCreationException if creation fails.
// Throws ShaderCompilationException if compilation fails.
// Arguments are, in order:
//	- shader type
//	- number of strings passed as source code
//	- source code
//	- array of each string's length. If NULL, NULL-termination is assumed
GLuint	ShaderProgram::compileShader(GLenum type, GLsizei count, const GLchar **source, const GLint *length)
{
	// Shader object ID
	GLuint	shader = glCreateShader(type);
	if (!shader)
	{
		std::cout << "ERROR WHILE CREATING SHADER" << std::endl;
		throw ShaderCreationException();
	}

	// Links the shader with its source code
	glShaderSource(shader, count, source, length);

	// Shader compilation
	glCompileShader(shader);

	// Checking if the shader compilation was successful
	GLint	success;
	// Puts into success the compilation status of our shader
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar	infoLog[512];
		// Retrieves compilation log into infoLog
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		glDeleteShader(shader);
		std::cout << "ERROR WHILE COMPILING SHADER\n" << infoLog << std::endl;
		throw CompilationException();
	}

	return shader;
}

// This links the program with its attached shaders
// So you should still attach your shaders beforehand!
// In addition, this checks for errors during linking
// If a linking error occurs, throws a ShaderProgramLinkingException
// and does NOT delete the shader program
void	ShaderProgram::link()
{
	glLinkProgram(this->_id);

	GLint	success;
	glGetProgramiv(this->_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar	infoLog[512];
		glGetProgramInfoLog(this->_id, 512, NULL, infoLog);
		std::cout << "ERROR WHILE LINKING SHADER PROGRAM\n" << infoLog << std::endl;
		throw LinkingException();
	}
}
