/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 02:58:59 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/18 18:20:43 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include <iostream>
# include <filesystem>
# include <cstring>
# include <cctype>
# include <algorithm>

# define WIN_WID 800
# define WIN_HEI 600

/* Window */
void	processWindowResize(GLFWwindow *window, int width, int height);

/* User Input */
void	processInputs(GLFWwindow *window);

/* Textures */
unsigned char	*parseBMPFile(const std::filesystem::path &filePath, int *width, int *height, int *nbChannels);
//GLuint	parseTexture(std::filesystem::path &filePath, int *width, int *height, int *nbChannels);

/* Render */
void	renderLoop(GLFWwindow *window);

#endif
