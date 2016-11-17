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
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////////
//	Description:Starts the server connection, receives the
//				messages transmitted, decodes it, calls methods
//				for checking the validity of the messages, and
//				prints out the message to the console
////////////////////////////////////////////////////////////////
void ReceiveMessage();

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

//////////////////////////////////////////////////////////////// 
//  Description:Converts a bitset into a character 
// 
//  Arguments:  [in]bitset<8>:binary char 
// 
//  Return:    [out]char: character obtained from the binary 
//////////////////////////////////////////////////////////////// 
char ConvertBinaryToChar(bitset<8> binaryChar);

////////////////////////////////////////////////////////////////
//	Description:Separates a string of binary characters into
//				8 bit bitsets
//
//	Arguments:	[in]string: frame
//
//	Return:		[out]list<bitset<8>>:list of bitsets representing
//									 the received frame
////////////////////////////////////////////////////////////////
list<bitset<8>> ConvertToBitsets(string frame);

////////////////////////////////////////////////////////////////
//	Description:Decodes a vector of characters with HDB3 encoding
//				and saves the result into a list of 1s and 0s
//
//	Arguments:	[in]vector<char>: message
//
//	Return:		[out]list<char>:list decoded 1s and 0s
////////////////////////////////////////////////////////////////
list<char> DecodeHDB3(vector<char> message);

////////////////////////////////////////////////////////////////
//	Description:counts the number of +s and -s in a list of chars
//				from the beginning of the list until finalIndex-1
//
//	Arguments:	[in]vector<char>: list to check
//				[in]int: Index to determine the termination of the
//						loop
//
//	Return:		[out]int: number of +s and -s
////////////////////////////////////////////////////////////////
int Count1s(vector<char> l, int finalIndex);

//////////////////////////////////////////////////////////////// 
//  Description:Checks that the message had no transmission 
//        errors 
// 
//  Arguments:  [in]list<bitset<8>>:list of bitsets representing 
//                   the message 
// 
//  Return:    [out]bool:indicates if the message is valid 
//              or not 
//  Ret Value:  true if valid message, false if invalid 
////////////////////////////////////////////////////////////////
bool IsMessageValid(list<bitset<8>> binaryCharacters);

//////////////////////////////////////////////////////////////// 
//  Description:Checks that the parity bit of the input bitset 
//        is correct 
// 
//  Arguments:  [in]bitset<8>:binary bitset with a parity bit 
// 
//  Return:    [out]bool:indicates if the bitset has the  
//              correct parity bit 
//  Ret Value:  true if correct parity, false if incorrect 
//////////////////////////////////////////////////////////////// 
bool CheckParity(bitset<8> binaryChar);
