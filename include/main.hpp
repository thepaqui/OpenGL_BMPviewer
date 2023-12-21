/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 02:58:59 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/21 01:42:24 by thepaqui         ###   ########.fr       */
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

/* Render */
void	renderLoop(GLFWwindow *window, const std::filesystem::path &filePath);

#endif
