/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 01:14:54 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/17 15:00:41 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int	mainError(std::string msg, int err)
{
	if (err != 1)
		glfwTerminate();
	std::cout << "ERROR: " << msg << std::endl;
	return err;
}

int	main(void)
{
	// Initializing GLFW
	if (glfwInit() == GLFW_FALSE)
		return mainError("Could not initialize GLFW", 1);
	// Telling GLFW that we are using OpenGL version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Telling GLFW that we are using OpenGL in core-profile mode (i.e. modern OpenGL)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Creating a window with GLFW
	GLFWwindow	*window = glfwCreateWindow(WIN_WID, WIN_HEI, "thepaqui\'s scop", NULL, NULL);
	if (window == NULL)
		return mainError("Could not create GLFW window", 2);
	// Making the window's context the context of the current thread
	glfwMakeContextCurrent(window);

	// Initializing GLAD
	// glwfGetProcAddress loads all OpenGL function pointers (OS-specific)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return mainError("Could not initialize GLAD", 3);

	// Telling OpenGL the position and size of its viewport
	// The viewport can be smaller than the actual window size
	// Position (0,0) is the lower-left corner
	glViewport(0, 0, WIN_WID, WIN_HEI);

	// Telling GLFW which function to call every time our main window is resized
	glfwSetFramebufferSizeCallback(window, processWindowResize);

	// Render loop
	try
	{
		renderLoop(window);
	}
	catch (...)
	{
		return mainError("Main render loop threw an exception", 4);
	}

	glfwTerminate();
	return 0;
}
