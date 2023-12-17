/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 22:25:10 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/17 15:04:57 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "project.hpp"

static GLuint	windowVBO() noexcept
{
	float	vertices[] =
	{
		-1.0f,	-1.0f,	0.0f,
		1.0f,	-1.0f,	0.0f,
		-1.0f,	1.0f,	0.0f,
		-1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,	0.0f,
		1.0f,	1.0f,	0.0f
	};

	GLuint	VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	return VBO;
}

void	renderLoop(GLFWwindow *window)
{
	ShaderProgram	shaderProgram("src/shaders/simple_vert.glsl",
		"src/shaders/simple_frag.glsl");

	shaderProgram.use();
	GLuint	VAO = makeVAO();
	GLuint	VBO = windowVBO();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while(!glfwWindowShouldClose(window))
	{
		processInputs(window);
		if (glfwWindowShouldClose(window))
			break ;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
