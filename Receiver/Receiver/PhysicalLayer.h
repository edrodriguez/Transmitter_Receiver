#pragma once
////////////////////////////////////////////////////////////////
//	Description:This file contains the declaration of functions
//				of the physical layer of the receiver.
//				This layer is in charge of receiving a message,
//				checking the validity of the message and
//				converting the binary received into characters
////////////////////////////////////////////////////////////////
#include <bitset>
#include <list>

using namespace std;

/**************************************************************/
/***********************Structs for Data***********************/
/**************************************************************/
struct HammingErrorDetection
{
	bool isHammingCorrect;
	int errorBit;
};

/**************************************************************/
/****************************Common****************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Converts a list of bitsets into a list of
//				characters.
//
//	Arguments:	[in]list<bitset<8>>:list of all the bitsets
//									 to be converted
//
//	Return:		[out]list<char>:list of all the characters
//								converted
////////////////////////////////////////////////////////////////
list<char> ConvertBinaryMessage(list< bitset<8>> binaryCharacters);

/**************************************************************/
/*****************************CRC******************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Starts the server connection, receives the
//				messages transmitted, calls methods for checking
//				the validity of the messages and printing out
//				the messages to the console
////////////////////////////////////////////////////////////////
void ReceiveCRCMessage();

////////////////////////////////////////////////////////////////
//	Description:Converts a string into a list of bools
//
//	Arguments:	[in]string: frame
//
//	Return:		[out]list<bool>:list of bools representing
//									 the frame					
////////////////////////////////////////////////////////////////
list<bool> ConvertToBoolList(string message);

////////////////////////////////////////////////////////////////
//	Description:Checks that the message had no transmission
//				errors.
//
//	Arguments:	[in]list<bool>:list of bitsets representing
//							   the frame
//
//	Return:		[out]bool:indicates if the message is valid
//						  or not
//	Ret Value:	true if valid message, false if invalid
////////////////////////////////////////////////////////////////
bool IsCRCValid(list<bool> binaryCharacters);

/**************************************************************/
/***************************Hamming****************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Starts the server connection, receives the
//				messages transmitted, calls methods for checking
//				the validity of the messages and printing out
//				the messages to the console
////////////////////////////////////////////////////////////////
void ReceiveHammingMessage();

////////////////////////////////////////////////////////////////
//	Description:Separates a string of binary characters into
//				12 bit bitsets
//
//	Arguments:	[in]string: message
//
//	Return:		[out]list<bitset<12>>:list of bitsets representing
//									 the received frame
////////////////////////////////////////////////////////////////
list<bitset<12>> ConvertToBitsets(string frame);

////////////////////////////////////////////////////////////////
//	Description:Checks that the message had no transmission
//				errors. If there are errors, it attempts to
//				correct them.
//
//	Arguments:	[in]list<bitset<12>>&:list of bitsets representing
//									 the frame
//
//	Return:		[out]bool:indicates if the message is valid
//						  or not
//	Ret Value:	true if valid message, false if invalid
////////////////////////////////////////////////////////////////
bool IsHammingValid(list<bitset<12>> &binaryCharacters, int framesReceived);

////////////////////////////////////////////////////////////////
//	Description:Checks that the parity bit of the input bitset
//				is correct. It also stores the location of the
//				error bit
//
//	Arguments:	[in]bitset<12>:binary bitset with a parity bit
//
//	Return:		[out]HammingErrorDetection: bool indicating
//					 whether the parity is correct for that
//					 bitset and if it is incorrect it also
//					 contains the location of the error bit
////////////////////////////////////////////////////////////////
HammingErrorDetection CheckHammingParity(bitset<12> binaryChar);