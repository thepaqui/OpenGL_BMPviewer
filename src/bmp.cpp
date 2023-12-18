/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:08:15 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/18 18:39:30 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bmp.hpp"

inline bool	doesFileExist(const std::string &filename)
{
	std::ifstream	f(filename.c_str());
	return (f.good());
}

inline bool	isFileEmpty(std::ifstream &file)
{
	return (static_cast<int>(file.peek()) == std::ifstream::traits_type::eof());
}

static std::ifstream	openIFile(const std::filesystem::path &filePath)
{
	std::ifstream	ifs;
	ifs.open(filePath);
	if (!ifs.is_open() || ifs.fail())
	{
		std::cout << "ERROR WHILE OPENING " << filePath << std::endl;
		throw std::exception();
	}
	if (isFileEmpty(ifs))
	{
		std::cout << filePath << " IS EMPTY" << std::endl;
		ifs.close();
		throw std::exception();
	}
	return ifs;
}

// replace return type by BYTE* later!
unsigned char	*parseBMPFile(const std::filesystem::path &filePath, int *width, int *height, int *nbChannels)
{
	(void)width;
	(void)height;
	(void)nbChannels;

	if (!doesFileExist(filePath.string()))
	{
		std::cout << "NO ACCESS TO " << filePath << " (INEXISTANT OR FORBIDDEN)" << std::endl;
		throw std::exception();
	}
	std::ifstream	bmp = openIFile(filePath);

	// parse File Header
	// parse Info Header
	// parse Color Header
	// parse pixel data into BYTE* and return it

	return NULL;
}