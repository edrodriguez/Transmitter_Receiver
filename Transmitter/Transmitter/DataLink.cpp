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

////////////////////////////////////////////////////////////////
//	Description:generates a list of string with each entry
//				containing a string representing a message. Each
//				one containing two syn characters (22 in decimal),
//				a control character (number of characters in the
//				message) and up to 64 information characters			////////////////to be changed
//
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in a binary bitset
//
//	Return:		[out]list<string>:list of strings containing
//								  all the framed messages
////////////////////////////////////////////////////////////////
list<frame> FrameMessage(list<bitset<12>> binaryChacarcters)
{
	list<frame> frames;
	list<list<bitset<12>>> blocks;

	blocks = SeparateInBlocks(&binaryChacarcters);

	for (list<list<bitset<12>>>::iterator it = blocks.begin(); it != blocks.end(); it++)
	{
		frame frame;
		frame.synChar1 = IncludeSynChar();
		frame.synChar2 = IncludeSynChar();
		frame.controlChar = IncludeControlChar(*it);
		frame.data = *it;
		frames.push_back(frame);
	}

	return frames;
}

////////////////////////////////////////////////////////////////
//	Description:Returns a string containing 22(decimal) in
//				binary as well as a parity bit appended at the
//				end of the string													/////////change
//
//	Return:		[out]string: binary code for 22 with a parity
//							 bit
////////////////////////////////////////////////////////////////
/*string*/ bitset<8> IncludeSynChar()
{
	const string ACII_22 = "0010110";
	bitset<7> synChar(ACII_22);
	
	//maybe not necessary
	bitset<8> synCharWithParity = IncludeParityBit(synChar);

	return synCharWithParity/*.to_string()*/;
}

////////////////////////////////////////////////////////////////
//	Description:separates the bitsets in groups of 64
//
//	Arguments:	[in]list<bitset<8>>:list of characters expressed
//									in a binary bitset
//
//	Return:		[out]list<string>:list of strings containing
//								  the bitsets separated in				//////////////change
//								  groups of 64
////////////////////////////////////////////////////////////////
/*list<string>*/ list<list<bitset<12>>> SeparateInBlocks(list<bitset<12>> *binaryChacarcters)
{
	list<list<bitset<12>>> blocks;
	//list<string> blocks;
	//string block;
	list<bitset<12>> block;

	for (list<bitset<12>>::iterator it = binaryChacarcters->begin(); it != binaryChacarcters->end(); it++)
	{
		//reverse character for transmission of LSB first
		bitset<12> reversedCharacter;
		for (size_t i = 0; i < it->size(); i++)
			reversedCharacter[reversedCharacter.size() - 1 - i] = (*it)[i];

		//If it's not the 64th character, append it to the same block
		if ((distance(binaryChacarcters->begin(), it) + 1) % 64 != 0)
		{
			block.push_back(reversedCharacter);
		}
		//If it's the 64th character, add it to the block and then separate that block
		else
		{
			block.push_back(reversedCharacter);
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
//	Description:Returns a string containing information on the
//				length of the message (the number of characters
//				contained on each block)
//
//	Arguments:	[in]string:block containing all the bits to be
//						   transmitted
//
//	Return:		[out]string:string indicating the number of				///////////change
//							characters in the block. Composed of
//							7 bits indicating the number and a
//							parity bit
////////////////////////////////////////////////////////////////
/*string*/bitset<8> IncludeControlChar(/*string*/ list<bitset<12>> block)
{
	int sizeOfBlock = CountCharsInBlock(block);

	bitset<7> controlChar = bitset<7>(sizeOfBlock);

	//maybe not necessary
	bitset<8> controlCharWithParity = IncludeParityBit(controlChar);

	return controlCharWithParity/*.to_string()*/;
}

////////////////////////////////////////////////////////////////
//	Description:counts how many 8bit characters are in a block
//
//	Arguments:	[in]string:block containing all the bits to be
//						   transmitted								///////change
//
//	Return:		[out]int:number of 8 bit characters contained
//						 in the block
////////////////////////////////////////////////////////////////
int CountCharsInBlock(/*string*/ list<bitset<12>> block)
{
	int numChars = block.size();

	return numChars;
}