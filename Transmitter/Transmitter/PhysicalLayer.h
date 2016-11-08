#pragma once
////////////////////////////////////////////////////////////////
//	Description:This file contains the declaration of functions
//				of the physical layer of the transmitter.
//				This layer is in charge of converting the
//				message to binary, applying CRC or Hamming
//				and transmitting the message
////////////////////////////////////////////////////////////////
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <list>
#include <bitset>

using namespace std;

/**************************************************************/
/***********************Structs for Data***********************/
/**************************************************************/

struct Frame
{
	bitset<8> synChar1;
	bitset<8> synChar2;
	bitset<8> controlChar;
	list<bitset<8>> data;
};

/**************************************************************/
/****************************Common****************************/
/**************************************************************/

//////////////////////////////////////////////////////////////// 
//  Description: converts a list of characters into a list of 
//         bitsets containing a 7 bit binary representation 
//         of a character and a parity bit 
// 
//  Arguments:  [in]list<char>: list of characters 
// 
//  Return:    [out]list<bitset<8>>:list of bitsets. Each bitset 
//                   made up of 7 binary bits 
//                   representing a character 
//                   and a parity bit 
//////////////////////////////////////////////////////////////// 
list<bitset<8>> ConvertTextForTransmission(list<char> charList);

////////////////////////////////////////////////////////////////
//	Description:Converts a character into an 7 bit binary bitset
//
//	Arguments:	[in]char: character to be converted
//
//	Return:		[out]bitset<7>: binary code for the char stored
//								in a 8 bit bitset
////////////////////////////////////////////////////////////////
bitset<7> ConvertToBinary(char C);

//////////////////////////////////////////////////////////////// 
//  Description: appends a binary bit to the end of a 7 bit 
//         binary code representing a char 
// 
//  Arguments:  [in]bitset<7>: char represented in binary 
// 
//  Return:    [out]bitset<8>: bitset including the 7 bit 
//                representation of the char  
//                and the parity bit 
//////////////////////////////////////////////////////////////// 
bitset<8> IncludeParityBit(bitset<7> binaryChar);

//////////////////////////////////////////////////////////////// 
//  Description:Calculates whether the input binary character 
//        string has even or odd parity  
// 
//  Arguments:  [in]bitset<7>: char represented in binary 
// 
//  Return:    [out]bool: indicating if the parity is even or odd 
//  Ret Value:  true if odd parity, false if even parity 
//////////////////////////////////////////////////////////////// 
bool IsOddParity(bitset<7> binaryChar);

////////////////////////////////////////////////////////////////
//	Description:Starts the client connection and transmits all the
//				messages composed of binary characters (with
//				possible transmission errors given by numOfErrors)
//
//	**CRC Overload
//	Arguments:	[in]list<CRC>: list containing all the
//							   frames to be transmitted
//				[in]int:number of errors to be introduced during
//					    transmission
//
////////////////////////////////////////////////////////////////
void TransmitFrames(list<Frame> frames);

list<char> BipolarAMI(Frame frame);