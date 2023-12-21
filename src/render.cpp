/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 22:25:10 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/21 02:04:42 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "project.hpp"

static GLuint	windowVBO() noexcept
{
	float	vertices[] =
	{
		// Position [-1;1]		// Texture pos [0;1]
		-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
		-1.0f,	1.0f,	0.0f,	0.0f,	1.0f,
		1.0f,	1.0f,	0.0f,	1.0f,	1.0f
	};

	GLuint	VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VBO;
}

GLuint	windowEBO() noexcept
{
	unsigned int	indices[] =
	{
		0,	1,	2,
		1,	2,	3
	};

	GLuint	EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return EBO;
}

void	renderLoop(GLFWwindow *window, const std::filesystem::path &filePath)
{
	ShaderProgram	shaderProgram("src/shaders/simple_vert.glsl",
		"src/shaders/simple_frag.glsl");
	Texture			texture(filePath);

	shaderProgram.use();
	texture.use();
	GLuint	VAO = makeVAO();
	GLuint	VBO = windowVBO();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLuint	EBO = windowEBO();

	while(!glfwWindowShouldClose(window))
	{
		processInputs(window);
		if (glfwWindowShouldClose(window))
			break ;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
