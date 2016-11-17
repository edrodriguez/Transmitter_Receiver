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

////////////////////////////////////////////////////////////////
//	Description:Generates a list of frames with each entry
//				containing a Frame struct representing a message.
//				Each one contains two syn characters (22 in decimal),
//				a control character (number of characters in the
//				message), up to 64 information characters. Each
//				character composed of 7 information bits and 1
//				parity bit
//
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in binary bitsets
//				[in/out]list<Frame>:list of Frames containing
//								 all the framed messages
////////////////////////////////////////////////////////////////
void FrameMessage(list<bitset<8>> data, list<Frame> &frame);

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
//	Description:Returns a string containing the ASCII Syn char
//				22(decimal) in binary							
//
//	Return:		[out]bitset<8>: 8 bit binary code for 22
////////////////////////////////////////////////////////////////
bitset<8> IncludeSynChar();

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
