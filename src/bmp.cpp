/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:08:15 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/19 18:48:57 by thepaqui         ###   ########.fr       */
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

// Opens in binary mode
static std::ifstream	openIFile(const std::filesystem::path &filePath)
{
	std::ifstream	ifs;
	ifs.open(filePath, std::ifstream::ios_base::binary);
	if (!ifs.is_open() || ifs.fail())
	{
		std::cout << "ERROR WHILE OPENING " << filePath << std::endl;
		throw std::runtime_error("Could not open " + filePath.string());
	}
	if (isFileEmpty(ifs))
	{
		std::cout << filePath << " IS EMPTY" << std::endl;
		ifs.close();
		throw std::runtime_error(filePath.string() + " is empty");
	}
	return ifs;
}

// Reads n bytes from ifs into dst
static void	readNBytes(std::ifstream &ifs, char *dst, size_t n)
{
	ifs.read(dst, n);
}

static void	printFileHeader(t_FileHeader &fh)
{
	std::cout << std::hex
		<< "fileType =	0x"		<< fh.fileType		<< "\n"
		<< "fileSize =	0x"		<< fh.fileSize		<< "\n"
		<< "reserved1 =	0x"		<< fh.reserved1		<< "\n"
		<< "reserved2 =	0x"		<< fh.reserved2		<< "\n"
		<< "dataOffset =	0x"	<< fh.dataOffset	<< "\n"
		<< std::dec << std::endl;
}

static void	printInfoHeader(t_InfoHeader &ih)
{
	std::cout << std::hex
		<< "size =			0x"		<< ih.size				<< "\n"
		<< "width =			0x"		<< ih.width				<< "\n"
		<< "height =		0x"		<< ih.height			<< "\n"
		<< "planes =		0x"		<< ih.planes			<< "\n"
		<< "bpp =			0x"		<< ih.bpp				<< "\n"
		<< "compression =		0x"	<< ih.compression		<< "\n"
		<< "rawDataSize =		0x"	<< ih.rawDataSize		<< "\n"
		<< "xPixelsPerMeter =	0x"	<< ih.xPixelsPerMeter	<< "\n"
		<< "yPixelsPerMeter =	0x"	<< ih.yPixelsPerMeter	<< "\n"
		<< "colorsUsed =		0x"	<< ih.colorsUsed		<< "\n"
		<< "colorsImportant =	0x"	<< ih.colorsImportant	<< "\n"
		<< std::dec << std::endl;
}

static void	checkInfoHeader(const std::filesystem::path &filePath, t_InfoHeader &ih)
{
	if (ih.bpp != 24 && ih.bpp != 32)
	{
		std::cout << filePath << ": COLOR ENCODING UNSUPPORTED\nONLY BGR24 AND BGRA32 ARE SUPPORTED" << std::endl;
		throw std::runtime_error(filePath.string() + ": Color encoding unsupported");
	}
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
		throw std::runtime_error(filePath.string() + " inexistant or forbidden");
	}
	std::ifstream	bmp = openIFile(filePath);

	t_FileHeader	fileHeader;
	readNBytes(bmp, reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
	printFileHeader(fileHeader); // debug
	if (fileHeader.fileType != 0x4D42)
	{
		std::cout << filePath << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(filePath.string() + " is not a bmp file");
	}

	t_InfoHeader	infoHeader;
	readNBytes(bmp, reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
	printInfoHeader(infoHeader); // debug
	checkInfoHeader(filePath, infoHeader);

	// parse Info Header
	// parse Color Header
	// parse pixel data into BYTE* and return it

	return NULL;
}