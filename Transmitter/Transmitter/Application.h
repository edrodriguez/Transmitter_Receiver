#pragma once
////////////////////////////////////////////////////////////////
//	Description:This file contains the declaration of functions
//				of the application layer of the transmitter.
//				This layer is in charge of reading the
//				information to be transmitted from a .txt file
////////////////////////////////////////////////////////////////
#include "PhysicalLayer.h"
#include <list>

using namespace std;

////////////////////////////////////////////////////////////////
//	Description:It reads the file with the information to be
//				transmitted and stores the characters in
//				a list.
//
//	Return:		[out]list<char>: list of characters where
//								 the information will be stored
////////////////////////////////////////////////////////////////
list<char> ReadFile();

////////////////////////////////////////////////////////////////
//	Description:Transforms a frame struct with all the fields
//				separated into a list will all the data together
//
//  Arguments:  [in]Frame: frame separated per block
//
//	Return:		[out]list<char>: list containing all the
//						characters in the frame together
////////////////////////////////////////////////////////////////
list<char> TurnFrameIntoList(Frame frame);

////////////////////////////////////////////////////////////////
//	Description:Copies the contents of a list of characters into
//				an array of characters for transmission of data
//
//  Arguments:  [in]list<char>: list of characters to be copied
//
//	Return:		[out]char(&array)[537]: array for the characters
//						to be copied
////////////////////////////////////////////////////////////////
void CopyListForTransmission(list<char> l, char(&arr)[537]);

/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<char> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<bitset<8>> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<bitset<12>> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<Frame> l);