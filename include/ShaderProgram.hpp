/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderProgram.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:05:05 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/17 00:50:14 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERPROGRAM_HPP
# define SHADERPROGRAM_HPP
# include <exception>
# include <string>
# include <fstream>
# include <sstream>
# include <iostream>
# include "main.hpp"

// By standard, a shader's source code can be written in the following files
// But it does not really matter, you can make all of these plain text files
//	name_vert.glsl - a vertex shader
//	name_tesc.glsl - a tessellation control shader
//	name_tese.glsl - a tessellation evaluation shader
//	name_geom.glsl - a geometry shader
//	name_frag.glsl - a fragment shader
//	name_comp.glsl - a compute shader

class ShaderProgram
{
private	:
	GLuint	_id = 0;

	void	initialize(GLsizei vertexCount, const GLchar **vertexShaderSource, GLsizei fragmentCount, const GLchar ** fragmentShaderSource);
	GLuint	compileShader(GLenum type, GLsizei count, const GLchar **source, const GLint *length);
	void	link();

public	:
	ShaderProgram(const std::string &vertexShaderSourceFile, const std::string &fragmentShaderSourceFile);
	ShaderProgram(GLsizei vertexCount, const GLchar **vertexShaderSource, GLsizei fragmentCount, const GLchar ** fragmentShaderSource);
	~ShaderProgram();

	void	use();

	void	setBool(const std::string &name, const bool value) const;
	void	setInt(const std::string &name, const int value) const;
	void	setFloat(const std::string &name, const float value) const;

	/* Exceptions */

	class FileReadingException : public std::exception
	{ public : const char	*what() const noexcept
		{ return "Reading shader from file failed"; }
	};

	class ShaderCreationException : public std::exception
	{ public : const char	*what() const noexcept
		{ return "Shader creation failed"; }
	};

	class CompilationException : public std::exception
	{ public : const char	*what() const noexcept
		{ return "Shader compilation failed"; }
	};

	class CreationException : public std::exception
	{ public : const char	*what() const noexcept
		{ return "Shader program creation failed"; }
	};

	class LinkingException : public std::exception
	{ public : const char	*what() const noexcept
		{ return "Shader program linking failed"; }
	};

	class UniformNotFoundException : public std::exception
	{ public : const char	*what() const noexcept
		{ return "Uniform variable was not found"; }
	};
};

#endif
