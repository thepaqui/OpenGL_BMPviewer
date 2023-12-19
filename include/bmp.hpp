/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:02:30 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/19 19:16:19 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BMP_HPP
# define BMP_HPP
# include <iostream>
# include <fstream>
# include <filesystem>
# include <stdexcept>

// 8 bits / unsigned
typedef	__UINT8_TYPE__	BYTE;
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
	DWORD	size{0};			// Size of this header (in bytes) (includes color header and bit mask info size if present)
	SDWORD	width{0};			// width of BMP image data in pixels
	SDWORD	height{0};			// height of BMP image data in pixels (if positive, data starts with bottom line of the image)
	WORD	planes{1};			// Always 1
	WORD	bpp{0};				// Bits per pixel
	DWORD	compression{0};		// 0 (uncompresed BGR) or 3 (uncompressed BGRA). Anything else is compressed or unsupported.
	DWORD	rawDataSize{0};		// Size of image's raw data in bytes (can be 0 if uncompressed)
	SDWORD	xPixelsPerMeter{0};
	SDWORD	yPixelsPerMeter{0};
	DWORD	colorsUsed{0};		// No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	DWORD	colorsImportant{0};	// No. of colors used for displaying the bitmap. If 0 all colors are required
}				t_InfoHeader;

// Size = 84 bytes
// This header is only present for transparent images (BGRA32bit)
// BGR24 does not use this or bit ma
typedef struct	BMPColorHeader
{
	DWORD	redMask{0x00ff0000};		// Bit mask for the red channel
	DWORD	greenMask{0x0000ff00};		// Bit mask for the green channel
	DWORD	blueMask{0x000000ff};		// Bit mask for the blue channel
	DWORD	alphaMask{0xff000000};		// Bit mask for the alpha channel
	DWORD	colorSpaceType{0x73524742};	// Default "sBGR" (0x73524742)
	DWORD	unused[16]{0};				// Unused data for sBGR color space
}				t_ColorHeader;

#pragma pack(pop)

#endif
