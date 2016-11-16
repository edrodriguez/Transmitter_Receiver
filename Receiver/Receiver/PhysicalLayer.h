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

////////////////////////////////////////////////////////////////
//	Description:Converts a string into a list of bools
//
//	Arguments:	[in]string: frame
//
//	Return:		[out]list<bool>:list of bools representing
//									 the frame					
////////////////////////////////////////////////////////////////
list<bool> ConvertToBoolList(string message);

/**************************************************************/
/***************************Hamming****************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Starts the server connection, receives the
//				messages transmitted, and prints out
//				the messages to the console
////////////////////////////////////////////////////////////////
void ReceiveMessage();

////////////////////////////////////////////////////////////////
//	Description:Separates a string of binary characters into
//				12 bit bitsets
//
//	Arguments:	[in]string: message
//
//	Return:		[out]list<bitset<12>>:list of bitsets representing
//									 the received frame
////////////////////////////////////////////////////////////////
list<bitset<8>> ConvertToBitsets(string frame);


//////////////////////////////////
list<char> ConvertToBinary(vector<char> message);