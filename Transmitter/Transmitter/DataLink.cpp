////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the data link layer of the transmitter.
//				This layer is in charge of framing the message
//				to be transmitted
////////////////////////////////////////////////////////////////

#include "DataLink.h"
#include "PhysicalLayer.h"
#include <list>
#include <bitset>
#include <string>

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
bitset<8> IncludeSynChar()
{
	const string ACII_22 = "00010110";
	bitset<8> synChar(ACII_22);

	return synChar;
}

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
list<list<bitset<8>>> SeparateInBlocks(list<bitset<8>> *binaryChacarcters)
{
	list<list<bitset<8>>> blocks;
	list<bitset<8>> block;

	for (list<bitset<8>>::iterator it = binaryChacarcters->begin(); it != binaryChacarcters->end(); it++)
	{
		//If it's not the 64th character, append it to the same block
		if ((distance(binaryChacarcters->begin(), it) + 1) % 64 != 0)
		{
			block.push_back(*it);
		}
		//If it's the 64th character, add it to the block and then separate that block
		else
		{
			block.push_back(*it);
			blocks.push_back(block);
			block.clear();
		}
	}

	//Add any remaining characters to a block
	if (!block.empty())
		blocks.push_back(block);

	return blocks;
}

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
bitset<8> IncludeControlChar(list<bitset<8>> block)
{
	int sizeOfBlock = CountCharsInBlock(block);

	bitset<8> controlChar(sizeOfBlock);

	return controlChar;
}

////////////////////////////////////////////////////////////////
//	Description:counts how many 8bit characters are in a block
//
//	Arguments:	[in]string:block containing all the bits to be
//						   transmitted
//
//	Return:		[out]int:number of 8 bit characters contained
//						 in the block
////////////////////////////////////////////////////////////////
int CountCharsInBlock(list<bitset<8>> block)
{
	int numChars = block.size();

	return numChars;
}

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
void FrameMessage(list<bitset<8>> data, list<CRCFrame> &frames)
{
	list<list<bitset<8>>> blocks;

	blocks = SeparateInBlocks(&data);

	for (list<list<bitset<8>>>::iterator it = blocks.begin(); it != blocks.end(); it++)
	{
		CRCFrame frame;
		frame.synChar1 = IncludeSynChar();
		frame.synChar2 = IncludeSynChar();
		frame.controlChar = IncludeControlChar(*it);
		frame.data = *it;
		CalculateCRC(frame);
		frames.push_back(frame);
	}
}


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
void FrameMessage(list<bitset<8>> data, list<HammingFrame> &frames)
{
	list<list<bitset<8>>> blocks;

	blocks = SeparateInBlocks(&data);

	for (list<list<bitset<8>>>::iterator it = blocks.begin(); it != blocks.end(); it++)
	{
		HammingFrame frame;
		frame.synChar1 = CalculateHammingCode(IncludeSynChar());
		frame.synChar2 = CalculateHammingCode(IncludeSynChar());
		frame.controlChar = CalculateHammingCode(IncludeControlChar(*it));
		frame.data = GenerateHammingForData(*it);
		frames.push_back(frame);
	}
}
