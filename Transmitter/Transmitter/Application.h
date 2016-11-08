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

/**************************************************************/
/****************************Common****************************/
/**************************************************************/

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
void PrintList(list<bitset<8>> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<bitset<12>> l);
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<TransmissionError> l);

/**************************************************************/
/*****************************CRC******************************/
/**************************************************************/
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<CRCFrame> l);

////////////////////////////////////////////////////////////////
//	Description:Converts the frame into a string
//
//	**CRC Overload
//	Arguments:	[in]CRCFrame: frame to be converted
//
//	Return:		[out]string: string with the information of the
//							 frame
////////////////////////////////////////////////////////////////
string FrameToString(CRCFrame frame);

/**************************************************************/
/***************************Hamming****************************/
/**************************************************************/
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<Frame> l);

////////////////////////////////////////////////////////////////
//	Description:Converts the frame into a string
//
//	**Hamming Overload
//	Arguments:	[in]HammingFrame: frame to be converted
//
//	Return:		[out]string: string with the information of the
//							 frame
////////////////////////////////////////////////////////////////
string FrameToString(HammingFrame frame);

