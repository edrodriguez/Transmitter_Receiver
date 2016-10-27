#pragma once
////////////////////////////////////////////////////////////////
//	Description:This file contains the declaration of functions
//				of the data link layer of the transmitter.
//				This layer is in charge of framing the message
//				to be transmitted
////////////////////////////////////////////////////////////////

#include "PhysicalLayer.h"
#include <string>
#include <list>
#include <bitset>

using namespace std;

/**************************************************************/
/****************************Common****************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Returns a string containing the ASCII Syn char
//				22(decimal) in binary							
//
//	Return:		[out]bitset<8>: 8 bit binary code for 22
////////////////////////////////////////////////////////////////
bitset<8> IncludeSynChar();

////////////////////////////////////////////////////////////////
//	Description:separates the bitsets in groups of 64
//
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in a binary bitset
//
//	Return:		[out]list<list<bitset<8>>: A list of 
//						lists of bitsets containing the bitsets
//						separated in groups of 64
////////////////////////////////////////////////////////////////
list<list<bitset<8>>> SeparateInBlocks(list<bitset<8>> *binaryChacarcters);

////////////////////////////////////////////////////////////////
//	Description:Returns a bitset containing information on the
//				length of the message (the number of characters
//				contained on each block)
//
//	Arguments:	[in]list<bitset<8>>:block containing all the
//						   bits to be transmitted
//
//	Return:		[out]bitset<8>:bitset indicating the number of
//							characters in the block.
////////////////////////////////////////////////////////////////
bitset<8> IncludeControlChar(list<bitset<8>> block);

////////////////////////////////////////////////////////////////
//	Description:counts how many 8bit characters are in a block
//
//	Arguments:	[in]string:block containing all the bits to be
//						   transmitted
//
//	Return:		[out]int:number of 8 bit characters contained
//						 in the block
////////////////////////////////////////////////////////////////
int CountCharsInBlock(list<bitset<8>> block);

/**************************************************************/
/*****************************CRC******************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Generates a list a frame with each entry
//				containing a CRCFrame representing a message. Each
//				one containing two syn characters (22 in decimal),
//				a control character (number of characters in the
//				message), up to 64 information characters and
//				a CRC code at the end for error detection
//
//	**CRC Overload
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in binary bitsets
//				[out]list<CRCFrame>:list of CRCFrames containing
//								  all the framed messages
////////////////////////////////////////////////////////////////
void FrameMessage(list<bitset<8>> data, list<CRCFrame> &frame);

/**************************************************************/
/***************************Hamming****************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Generates a list a frame with each entry
//				containing a CRCFrame representing a message. Each
//				one containing two syn characters (22 in decimal),
//				a control character (number of characters in the
//				message), up to 64 information characters. Each
//				character composed of 8 information bits and 4
//				hamming parity bits
//
//	**Hamming Overload
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in binary bitsets
//				[out]list<HammingFrrame>:list of CRCFrames containing
//								         all the framed messages
////////////////////////////////////////////////////////////////
void FrameMessage(list<bitset<8>> data, list<HammingFrame> &frame);
