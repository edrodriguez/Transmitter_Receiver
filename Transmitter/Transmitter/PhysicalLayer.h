#pragma once
////////////////////////////////////////////////////////////////
//	Description:This file contains the declaration of functions
//				of the physical layer of the transmitter.
//				This layer is in charge of converting the
//				message to binary and transmitting it
////////////////////////////////////////////////////////////////
#include <string>
#include <list>
#include <bitset>

using namespace std;

struct frame
{
	bitset<8> synChar1;
	bitset<8> synChar2;
	bitset<8> controlChar;
	//list<bitset<8>> data;
	list<bitset<12>> data;
	string CRCCode;
};
struct message
{
	list<frame> frames;
};

struct transmissionError
{
	int frameLocation;
	int byteLocation;
	int bitLocation;
};

////////////////////////////////////////////////////////////////
//	Description: converts a list of characters into a list of
//				 bitsets containing a 7 bit binary representation
//				 of a character and a parity bit
//
//	Arguments:	[in]list<char>: list of characters
//
//	Return:		[out]list<bitset<8>>:list of bitsets. Each bitset
//									 made up of 7 binary bits
//									 representing a character
//									 and a parity bit
////////////////////////////////////////////////////////////////
list<bitset<8>> ConvertTextForTransmission(list<char> charList);

////////////////////////////////////////////////////////////////
//	Description:Converts a character into a 7 bit binary bitset
//
//	Arguments:	[in]char: character to be converted
//
//	Return:		[out]bitset<7>: binary code for the char stored
//								in a 7 bit bitset
////////////////////////////////////////////////////////////////
bitset<7> ConvertToBinary(char C);

////////////////////////////////////////////////////////////////
//	Description: appends a binary bit to the end of a 7 bit
//				 binary code representing a char
//
//	Arguments:	[in]bitset<7>: char represented in binary
//
//	Return:		[out]bitset<8>: bitset including the 7 bit
//								representation of the char 
//								and the parity bit
////////////////////////////////////////////////////////////////
bitset<8> IncludeParityBit(bitset<7> binaryChar);

////////////////////////////////////////////////////////////////
//	Description:Calculates whether the input binary character
//				string has even or odd parity 
//
//	Arguments:	[in]bitset<7>: char represented in binary
//
//	Return:		[out]bool: indicating if the parity is even or odd
//	Ret Value:	true if odd parity, false if even parity
////////////////////////////////////////////////////////////////
bool IsOddParity(bitset<7> binaryChar);

////////////////////////////////////////////////////////////////
//	Description:Starts the client connection and transmits all the
//				messages composed of binary characters
//
//	Arguments:	[in]list<string>: list containing all the messages		////////////change
//				[in]int:number of errors to be introduced during
//					transmission
//
////////////////////////////////////////////////////////////////
void TransmitMessages(/*list<string>*/ message message, int numOfErrors);

////////////////////////////////////////////////////////////////
//	Description:Changes the number of bits indicated
//				in the input parameter in a random position in
//				the message
//
//	Arguments:	[in]int (optional): Number of bits to change.		/////////////change
//									Default value: 0.
//
////////////////////////////////////////////////////////////////
list<transmissionError> GenerateTransmissionError(message &message, int numberOfBitsToChange);

list<bitset<12>> GenerateHamming(list<bitset<8>> data);

bitset<12> CalculateHammingCode(bitset<8> byteOfData);

void GenerateCRC(list<frame> &frames);

void CalculateCRC(frame &frame);