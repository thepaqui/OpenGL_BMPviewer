/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 02:58:19 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/11 17:52:49 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

// This tells OpenGL that the window has been resized and to thus update
// the rendering viewport to reflect the new window dimensions
// A smaller viewport than the window is not always an error
// It just means OpenGL will only render inside the smaller viewport
// Leaving the remaining space unused but also uncleared so be careful!
void	processWindowResize(GLFWwindow *window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}
