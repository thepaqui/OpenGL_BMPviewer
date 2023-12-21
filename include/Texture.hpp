/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 00:09:19 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/21 17:02:38 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP
# include "main.hpp"
# include <fstream>

// 16 bits / 2 bytes / unsigned
typedef	__UINT16_TYPE__	WORD;
// 32 bits / 4 bytes / unsigned
typedef	__UINT32_TYPE__	DWORD;
// 32 bits / 4 bytes / signed
typedef	__INT32_TYPE__	SDWORD;

// This enforces tight packing of the structures
#pragma pack(push, 1)

// Size = 14 bytes
typedef struct	BMPFileHeader
{
	WORD	fileType{0};	// Always BM (0x4D42) for BMP files
	DWORD	fileSize{0};	// In bytes
	WORD	reserved1{0};	// Reserved values, can be ignored here
	WORD	reserved2{0};	// Reserved values, can be ignored here
	DWORD	dataOffset{0};	// Offset at which the pixel data starts in file
}				t_FileHeader;

// Size = 40 bytes
// This header has multiple versions, I only take BITMAPINFOHEADER
// Supported by at least Windows NT, 3.1x
typedef struct	BMPInfoHeader
{
	DWORD	size{0};			// Size of this header (in bytes) (includes color header size if present)
	SDWORD	width{0};			// Width of BMP image data in pixels
	SDWORD	height{0};			// Height of BMP image data in pixels (if positive, data is Y-flipped)
	WORD	planes{1};			// Always 1
	WORD	bpp{0};				// Bits per pixel
	DWORD	compression{0};		// 0 (uncompresed RGB) or 3 (uncompressed RGBA). Anything else is compressed or unsupported.
	DWORD	rawDataSize{0};		// Size of image's raw data in bytes (can be 0 if uncompressed)
	SDWORD	xPixelsPerMeter{0};
	SDWORD	yPixelsPerMeter{0};
	DWORD	colorsUsed{0};		// Nb of color indexes in the color table. 0 means the max number of colors allowed by bpp
	DWORD	colorsImportant{0};	// Nb of colors used for displaying the bitmap. If 0 all colors are required
}				t_InfoHeader;

// Size = 20 bytes
// This header is only present for transparent images (RGBA32bit)
// RGB24 does not use this header (if it is there, it can be ignored)
typedef struct	BMPColorHeader
{
	DWORD	redMask{0x00ff0000};		// Bit mask for the red channel
	DWORD	greenMask{0x0000ff00};		// Bit mask for the green channel
	DWORD	blueMask{0x000000ff};		// Bit mask for the blue channel
	DWORD	alphaMask{0xff000000};		// Bit mask for the alpha channel
	DWORD	colorSpaceType{0x73524742};	// Default "sRGB" (0x73524742)
}				t_ColorHeader;

#pragma pack(pop)

class Texture
{
private	:
	GLuint			_id = 0;

	unsigned char*	_data = NULL;
	unsigned int	_width = 0;
	unsigned int	_height = 0;
	unsigned int	_bpp = 0;
	bool			_yFlipped = false;
	bool			_debug = false;

	// Parsing functions

	void	parseBMPFileHeader(const std::filesystem::path &fp,
		std::ifstream &bmp, t_FileHeader &fh);
	void	parseBMPInfoHeader(const std::filesystem::path &fp,
		std::ifstream &bmp, t_InfoHeader &ih);
	void	parseBMPColorHeader(const std::filesystem::path &fp,
		std::ifstream &bmp, t_ColorHeader &ch, const t_InfoHeader &ih);
	void	parseBMPData(const std::filesystem::path &fp, std::ifstream &bmp,
		const t_FileHeader &fh, const t_InfoHeader &ih);
	void	parseBMPFile(const std::filesystem::path &filePath);

	// Utility functions

	inline bool		doesFileExist(const std::string &filename) const;
	inline bool		isFileEmpty(std::ifstream &file) const;
	std::ifstream	openIFileBin(const std::filesystem::path &filePath) const;
	inline void		readNBytes(std::ifstream &ifs, char *dst, size_t n);
	inline bool		issRGB(const t_ColorHeader &ch) const noexcept;
	inline bool		isRealsRGBBitMask(const t_ColorHeader &ch) const noexcept;

	// Debug functions

	void	printFileHeader(const t_FileHeader &fh) const;
	void	printInfoHeader(const t_InfoHeader &ih) const;
	void	printColorHeader(const t_ColorHeader &ch) const;
	void	printData(const t_InfoHeader &ih, const size_t padByt,
		const size_t datByt) const;

public	:
	Texture(const std::filesystem::path &filePath);
	~Texture();

	void			use();

	unsigned char	*getData() const;
	unsigned int	getWidth() const;
	unsigned int	getHeight() const;
	unsigned int	getBPP() const;
};

#endif