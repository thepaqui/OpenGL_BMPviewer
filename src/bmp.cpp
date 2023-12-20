/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:08:15 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/20 18:42:23 by thepaqui         ###   ########.fr       */
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

static void	printColorHeader(t_ColorHeader &ch)
{
	char	*colorSpaceSTR = reinterpret_cast<char*>(&ch.colorSpaceType);
	std::cout << std::hex
		<< "redMask =		0x"		<< ch.redMask			<< "\n"
		<< "greenMask =		0x"		<< ch.greenMask			<< "\n"
		<< "blueMask =		0x"		<< ch.blueMask			<< "\n"
		<< "alphaMask =		0x"		<< ch.alphaMask			<< "\n"
		<< "colorSpaceType =	0x"	<< ch.colorSpaceType
		<< " " << colorSpaceSTR[0] << colorSpaceSTR[1]
		<< colorSpaceSTR[2] << colorSpaceSTR[3] << "\n"
		<< std::dec << std::endl;
}

inline static bool issRGB(t_ColorHeader &ch)
{
	return (ch.colorSpaceType == 0x73524742);
}

inline static bool isRealsRGBBitMask(t_ColorHeader &ch)
{
	return (ch.redMask == 0x00ff0000
		&& ch.greenMask == 0x0000ff00
		&& ch.blueMask == 0x000000ff
		&& ch.alphaMask == 0xff000000);
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
	if (bmp.eof())
	{
		std::cout << filePath << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(filePath.string() + " is not a bmp file");
	}
	printFileHeader(fileHeader); // debug
	if (fileHeader.fileType != 0x4D42)
	{
		std::cout << filePath << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(filePath.string() + " is not a bmp file");
	}

	t_InfoHeader	infoHeader;
	readNBytes(bmp, reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
	if (bmp.eof())
	{
		std::cout << filePath << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(filePath.string() + " is not a bmp file");
	}
	printInfoHeader(infoHeader); // debug
	if (infoHeader.bpp != 24 && infoHeader.bpp != 32)
	{
		std::cout << filePath << ": PIXEL ENCODING UNSUPPORTED\nONLY RGB24 AND RGBA32 ARE SUPPORTED" << std::endl;
		throw std::runtime_error(filePath.string() + ": Pixel encoding unsupported");
	}

	t_ColorHeader	colorHeader;
	if (infoHeader.bpp == 32)
	{
		if (infoHeader.size < (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader)))
		{
			std::cout << filePath << ": BAD COLOR HEADER (INEXISTANT OR WRONG FORMAT)" << std::endl;
			throw std::runtime_error(filePath.string() + " has no/wrong color header");
		}
		bmp.read((char*)&colorHeader, sizeof(colorHeader));
		if (bmp.eof())
		{
			std::cout << filePath << " IS NOT A BMP FILE" << std::endl;
			throw std::runtime_error(filePath.string() + " is not a bmp file");
		}
		printColorHeader(colorHeader); // debug
		if (!issRGB(colorHeader))
		{
			std::cout << filePath << ": COLOR ENCODING UNSUPPORTED\nONLY sRGB IS SUPPORTED FOR RGB32 ENCODING" << std::endl;
			throw std::runtime_error(filePath.string() + ": Color encoding unsupported");
		}
		if (!isRealsRGBBitMask(colorHeader))
		{
			std::cout << filePath << ": WRONG COLOR MASKS FOR sRGB ENCODING" << std::endl;
			throw std::runtime_error(filePath.string() + ": Wrong color masks for sRGB");
		}
	}

	// Go to start of actual data
	bmp.seekg(fileHeader.dataOffset, bmp.beg);

//	BYTE	*data = (BYTE*)calloc(infoHeader.rawDataSize, sizeof(BYTE));

	return NULL;
}