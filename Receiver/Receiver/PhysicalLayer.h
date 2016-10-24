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

////////////////////////////////////////////////////////////////
//	Description:Starts the server connection, receives the
//				messages transmitted, calls methods for checking
//				the validity of the messages and printing out
//				the messages to the console
////////////////////////////////////////////////////////////////
void ReceiveMessages();

////////////////////////////////////////////////////////////////
//	Description:Separates a string of binary characters into
//				8 bit bitsets
//
//	Arguments:	[in]string: message
//
//	Return:		[out]list<bitset<8>>:list of bitsets representing				///////////
//									 the message
////////////////////////////////////////////////////////////////
list<bitset<12>> ConvertToBitsets(string message);

////////////////////////////////////////////////////////////////
//	Description:Checks that the message had no transmission
//				errors. This will be expanded in future milestones
//
//	Arguments:	[in]list<bitset<8>>:list of bitsets representing
//									 the message
//
//	Return:		[out]bool:indicates if the message is valid
//						  or not
//	Ret Value:	true if valid message, false if invalid
////////////////////////////////////////////////////////////////
bool IsMessageValid(list<bitset<12>> binaryCharacters);

////////////////////////////////////////////////////////////////
//	Description:Checks that the parity bit of the input bitset
//				is correct
//
//	Arguments:	[in]bitset<8>:binary bitset with a parity bit
//
//	Return:		[out]bool:indicates if the bitset has the 
//						  correct parity bit
//	Ret Value:	true if correct parity, false if incorrect
////////////////////////////////////////////////////////////////
bool CheckHammingParity(bitset<12> binaryChar);

////////////////////////////////////////////////////////////////
//	Description:Converts a list of bitsets into a list of
//				characters. It calls ConvertBinaryToChar for
//				the conversion of each specific bitset
//
//	Arguments:	[in]list<bitset<8>>:list of all the bitsets
//									 to be converted
//
//	Return:		[out]list<char>:list of all the characters
//								converted
////////////////////////////////////////////////////////////////
list<char> ConvertBinaryMessage(list< bitset<8>> binaryCharacters);

////////////////////////////////////////////////////////////////
//	Description:Converts a bitset into a character
//
//	Arguments:	[in]bitset<8>:binary char
//
//	Return:		[out]char: character obtained from the binary
////////////////////////////////////////////////////////////////
char ConvertBinaryToChar(bitset<8> binaryChar);
