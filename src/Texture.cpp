/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 00:17:15 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/21 03:27:44 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Texture.hpp"

Texture::Texture(const std::filesystem::path &filePath)
{
	parseBMPFile(filePath);
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Black
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLint	internalFormat = GL_RGBA8;
	GLenum	format = GL_BGRA;
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height,
					0, format, GL_UNSIGNED_BYTE, _data);

	glGenerateMipmap(GL_TEXTURE_2D);

	free(_data);
	_data = NULL;

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	free(_data);
	glDeleteTextures(1, &_id);
}

void	Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, _id);
}

unsigned char	*Texture::getData() const
{
	return _data;
}

unsigned int	Texture::getWidth() const
{
	return _width;
}

unsigned int	Texture::getHeight() const
{
	return _height;
}

unsigned int	Texture::getBPP() const
{
	return _bpp;
}

/* PRIVATE METHODS */

void	Texture::parseBMPFile(const std::filesystem::path &filePath)
{
	if (!doesFileExist(filePath.string()))
	{
		std::cout << "NO ACCESS TO " << filePath << " (INEXISTANT OR FORBIDDEN)" << std::endl;
		throw std::runtime_error(filePath.string() + " inexistant or forbidden");
	}
	std::ifstream	bmp = openIFileBin(filePath);

	t_FileHeader	fileHeader;
	readNBytes(bmp, reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
	if (bmp.eof())
	{
		std::cout << filePath << " IS NOT A BMP FILE" << std::endl;
		throw std::runtime_error(filePath.string() + " is not a bmp file");
	}
//	printFileHeader(fileHeader); // debug
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
//	printInfoHeader(infoHeader); // debug
	if (infoHeader.bpp != 24 && infoHeader.bpp != 32)
	{
		std::cout << filePath << ": PIXEL ENCODING UNSUPPORTED\nONLY RGB24 AND RGBA32 ARE SUPPORTED" << std::endl;
		throw std::runtime_error(filePath.string() + ": Pixel encoding unsupported");
	}
	if (infoHeader.height < 0)
	{
		infoHeader.height = -1 * infoHeader.height; // DO MORE HERE, THIS MEANS IMAGE IS BOTTOM FIRST
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
//		printColorHeader(colorHeader); // debug
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
	size_t	nbInfoBytes = infoHeader.width * (infoHeader.bpp / 8);
	size_t	dataBytes = infoHeader.width * 4 * infoHeader.height;
	size_t	nbPaddingBytes = (4 - (nbInfoBytes % 4)) % 4;
	_data = (unsigned char*)calloc(dataBytes, sizeof(unsigned char));
	if (!_data)
	{
		std::cout << "MEMORY ALLOCATION FAILED" << std::endl;
		throw std::bad_alloc();
	}
	if (infoHeader.bpp == 32)
		bmp.read((char*)_data, dataBytes);
	else
	{
		if (nbPaddingBytes == 0)
		{
			char	*pos = NULL;
			for (int y = 0; y < infoHeader.height; y++)
			{
				for (int x = 0; x < infoHeader.width; x++)
				{
					pos = (char*)(_data + (y * infoHeader.width * 4) + (x * 4));
					bmp.read(pos, 3);
					pos[3] = '\xff';
				}
			}
		}
		else
		{
			char	throwaway[nbPaddingBytes];
			char	*pos = NULL;
			for (int y = 0; y < infoHeader.height; y++)
			{
				for (int x = 0; x < infoHeader.width; x++)
				{
					pos = (char*)(_data + (y * infoHeader.width * 4) + (x * 4));
					bmp.read(pos, 3);
					pos[3] = '\xff';
				}
				bmp.read(throwaway, nbPaddingBytes);
			}
			if (bmp.eof())
			{
				free(_data);
				_data = NULL;
				std::cout << filePath << " HAS BAD DATA PADDING" << std::endl;
				throw std::runtime_error(filePath.string() + " has bad data padding");
			}
		}
	}
//	printData(infoHeader, nbInfoBytes, nbPaddingBytes, dataBytes); // debug

	_width = infoHeader.width;
	_height = infoHeader.height;
	_bpp = infoHeader.bpp;
}

inline bool	Texture::doesFileExist(const std::string &filename) const
{
	std::ifstream	f(filename.c_str());
	return (f.good());
}

inline bool	Texture::isFileEmpty(std::ifstream &file) const
{
	return (static_cast<int>(file.peek()) == std::ifstream::traits_type::eof());
}

// Opens file in binary mode
std::ifstream	Texture::openIFileBin(const std::filesystem::path &filePath) const
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
// ifs should be opened and valid beforehand
inline void	Texture::readNBytes(std::ifstream &ifs, char *dst, const size_t n)
{
	ifs.read(dst, n);
}

inline bool	Texture::issRGB(const t_ColorHeader &ch) const noexcept
{
	return (ch.colorSpaceType == 0x73524742);
}

inline bool	Texture::isRealsRGBBitMask(const t_ColorHeader &ch) const noexcept
{
	return (ch.redMask == 0x00ff0000
		&& ch.greenMask == 0x0000ff00
		&& ch.blueMask == 0x000000ff
		&& ch.alphaMask == 0xff000000);
}

void	Texture::printFileHeader(const t_FileHeader &fh) const
{
	std::cout << std::hex
		<< "fileType =	0x"		<< fh.fileType		<< "\n"
		<< "fileSize =	0x"		<< fh.fileSize		<< "\n"
		<< "reserved1 =	0x"		<< fh.reserved1		<< "\n"
		<< "reserved2 =	0x"		<< fh.reserved2		<< "\n"
		<< "dataOffset =	0x"	<< fh.dataOffset	<< "\n"
		<< std::dec << std::endl;
}

void	Texture::printInfoHeader(const t_InfoHeader &ih) const
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

void	Texture::printColorHeader(const t_ColorHeader &ch) const
{
	const char	*CSstr = reinterpret_cast<const char*>(&ch.colorSpaceType);
	std::cout << std::hex
		<< "redMask =		0x"		<< ch.redMask			<< "\n"
		<< "greenMask =		0x"		<< ch.greenMask			<< "\n"
		<< "blueMask =		0x"		<< ch.blueMask			<< "\n"
		<< "alphaMask =		0x"		<< ch.alphaMask			<< "\n"
		<< "colorSpaceType =	0x"	<< ch.colorSpaceType
		<< " " << CSstr[0] << CSstr[1] << CSstr[2] << CSstr[3] << "\n"
		<< std::dec << std::endl;
}

void	Texture::printData(const t_InfoHeader &ih, const size_t infByt,
	const size_t padByt, const size_t datByt) const
{
	std::cout << ih.height << " lines of " << infByt << " bytes = "
		<< datByt << " bytes" << std::endl;
	std::cout << padByt << " bytes of padding" << std::endl;
	size_t	i = 0;
	std::cout << "data =\n" << std::hex;
	for (int y = 0; y < ih.height; y++)
	{
		std::cout << "\t";
		for (int x = 0; x < (ih.bpp / 8) * ih.width; x++)
		{
			i = (y * ih.width * ih.bpp / 8) + x;
			std::cout << std::setfill('0') << std::setw(2) << +_data[i] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::dec << std::endl;
}
