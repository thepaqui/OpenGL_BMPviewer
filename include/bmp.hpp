/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:02:30 by thepaqui          #+#    #+#             */
/*   Updated: 2023/12/18 18:29:55 by thepaqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BMP_HPP
# define BMP_HPP
# include <iostream>
# include <fstream>
# include <filesystem>

typedef	__UINT8_TYPE__	BYTE;
typedef	__UINT16_TYPE__	WORD;
typedef	__UINT32_TYPE__	DWORD;

// This enforces tight packing of the structures
#pragma pack(push, 1)

struct BMPFileHeader
{
	WORD	fileType;	// Always BM (0x4D42) for BMP files
	DWORD	fileSize;	// In bytes
	WORD	reserved1;	// Reserved values, can be ignored here
	WORD	reserved2;	// Reserved values, can be ignored here
	DWORD	dataOffset;	// Offset at which the pixel data starts in file
};

#pragma pack(pop)

#endif
