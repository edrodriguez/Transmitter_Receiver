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

/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<char> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<string> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<bitset<7>> l);
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
void PrintList(list<HammingFrame> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<CRCFrame> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<transmissionError> l);
/////hamming
string FrameToString(HammingFrame frame);
/////crc
string FrameToString(CRCFrame frame);

