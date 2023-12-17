/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VAO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 01:16:09 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/17 01:06:26 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

// Creates and binds a VAO
GLuint	makeVAO() noexcept
{
	GLuint	VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return VAO;
}
