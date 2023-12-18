/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 01:14:54 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/18 18:11:59 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

static void	strToLower(std::string &s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

static bool	stringCompareCI(const std::string &s1, const std::string &s2)
{
	std::string	ls1(s1);
	std::string	ls2(s2);
	strToLower(ls1);
	strToLower(ls2);
	return (ls1.compare(ls2) == 0);
}

static int	mainError(std::string msg, int err) noexcept
{
	if (err != 1)
		glfwTerminate();
	std::cout << "ERROR: " << msg << std::endl;
	return err;
}

int	main(int ac, char **av)
{
	ac--;
	if (ac != 1)
	{
		std::cout << "Usage:\n$> ./bmp_view path_to_bmp_file" << std::endl;
		return 0;
	}

	std::filesystem::path	bmp_path(av[1]);
	{
		auto ext = bmp_path.extension().string();
		if (!stringCompareCI(ext, ".bmp") && !stringCompareCI(ext, ".bitmap"))
		{
			std::cout << "Argument should be a path to a .bmp file" << std::endl;
			return 0;
		}
	}

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

	unsigned char	*texture = NULL;
	int				width=0, height=0, nbChannels=0;
	try
	{
		texture = parseBMPFile(bmp_path, &width, &height, &nbChannels);
	}
	catch(...)
	{
		return mainError("Texture parsing threw an exception", 4);
	}
	(void)texture;
	(void)width;
	(void)height;
	(void)nbChannels;

	// Render loop
	try
	{
		renderLoop(window);
	}
	catch (...)
	{
		return mainError("Main render loop threw an exception", 5);
	}

	glfwTerminate();
	return 0;
}
