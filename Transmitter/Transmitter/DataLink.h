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
//	Description:generates a list of string with each entry
//				containing a string representing a message. Each
//				one containing two syn characters (22 in decimal),
//				a control character (number of characters in the
//				message) and up to 64 information characters			////////////////change
//
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in a binary bitset
//
//	Return:		[out]list<string>:list of strings containing
//								  all the framed messages
////////////////////////////////////////////////////////////////
list<frame/*string*/> FrameMessage(list<bitset<12>> binaryChacarcters);

////////////////////////////////////////////////////////////////
//	Description:Returns a string containing 22(decimal) in
//				binary as well as a parity bit appended at the
//				end of the string
//
//	Return:		[out]string: binary code for 22 with a parity
//							 bit
////////////////////////////////////////////////////////////////
/*string*/ bitset<8> IncludeSynChar();

////////////////////////////////////////////////////////////////
//	Description:separates the bitsets in groups of 64
//
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in a binary bitset
//
//	Return:		[out]list<string>:list of strings containing
//								  the bitsets separated in
//								  groups of 64
////////////////////////////////////////////////////////////////
/*list<string>*/ list<list<bitset<12>>> SeparateInBlocks(list<bitset<12>> *binaryChacarcters);

////////////////////////////////////////////////////////////////
//	Description:Returns a string containing information on the
//				length of the message (the number of characters
//				contained on each block)
//
//	Arguments:	[in]string:block containing all the bits to be
//						   transmitted
//
//	Return:		[out]string:string indicating the number of 
//							characters in the block. Composed of				//////////change
//							7 bits indicating the number and a
//							parity bit
////////////////////////////////////////////////////////////////
/*string*/ bitset<8> IncludeControlChar(/*string*/ list<bitset<12>> block);

////////////////////////////////////////////////////////////////
//	Description:counts how many 8bit characters are in a block
//
//	Arguments:	[in]string:block containing all the bits to be
//						   transmitted
//
//	Return:		[out]int:number of 8 bit characters contained
//						 in the block
////////////////////////////////////////////////////////////////
int CountCharsInBlock(/*string*/  list<bitset<12>> block);
