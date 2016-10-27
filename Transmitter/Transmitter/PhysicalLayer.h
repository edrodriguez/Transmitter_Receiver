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
struct CRCFrame
{
	bitset<8> synChar1;
	bitset<8> synChar2;
	bitset<8> controlChar;
	list<bitset<8>> data;
	list<bool> CRCCode;
};

struct HammingFrame
{
	bitset<12> synChar1;
	bitset<12> synChar2;
	bitset<12> controlChar;
	list<bitset<12>> data;
};

struct TransmissionError
{
	int frameLocation;
	int charLocation;
	int bitLocation;
};

/**************************************************************/
/****************************Common****************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description: converts a list of characters into a list of
//				 bitsets containing an 8 bit binary representation
//				 of a character and a parity bit
//
//	Arguments:	[in]list<char>: list of characters
//
//	Return:		[out]list<bitset<8>>:list of bitsets
////////////////////////////////////////////////////////////////
list<bitset<8>> ConvertTextForTransmission(list<char> charList);

////////////////////////////////////////////////////////////////
//	Description:Converts a character into an 8 bit binary bitset
//
//	Arguments:	[in]char: character to be converted
//
//	Return:		[out]bitset<8>: binary code for the char stored
//								in a 8 bit bitset
////////////////////////////////////////////////////////////////
bitset<8> ConvertToBinary(char C);

/**************************************************************/
/*****************************CRC******************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:Calculates the CRC code for the frame using a
//				shift register simulation approach
//
//	Arguments:	[in]CRCFrame&: Frame to be used for calculation
//
////////////////////////////////////////////////////////////////
void CalculateCRC(CRCFrame &frame);

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
void TransmitFrames(list<CRCFrame> frames, int numOfErrors);

////////////////////////////////////////////////////////////////
//	Description:Changes the number of bits indicated
//				in the input parameter in a random position in
//				the message
//
//	**CRC Overload
//	Arguments:	[in]list<CRCFrame>: frames containing the
//									data without errors
//				[out]list<CRCFrame>&: frames containing the
//									  data with errors
//				[in]int*: Number of bits to change.
//
//	Return:		[out]list<TransmissionError>:list of the location
//										   of errors in the frame
////////////////////////////////////////////////////////////////
list<TransmissionError> GenerateTransmissionError(list<CRCFrame> frames,
												  list<CRCFrame> &framesWithErrors,
												  int numberOfBitsToChange);

////////////////////////////////////////////////////////////////
//	Description:Copies the frames to a different list
//
//	**CRC Overload
//	Arguments:	[in]list<CRCFrame>: Original list
//				[out]list<CRCFrame>&: Copy of the frames
//
////////////////////////////////////////////////////////////////
void CopyFrames(list<CRCFrame> frames, list<CRCFrame> &framesWithErrors);

/**************************************************************/
/***************************Hamming****************************/
/**************************************************************/

////////////////////////////////////////////////////////////////
//	Description:iterates through the data list to generate the
//				hamming code for each character by calling
//				CalculateHammingCode on each one of them
//
//	Arguments:	[in]list<bitset<8>>: list containing the binary
//									 representation of each
//									 character read from the file
//
//	Return:		[out]list<bitset<12>>: binary characters with
//									   hamming parity bits
////////////////////////////////////////////////////////////////
list<bitset<12>> GenerateHammingForData(list<bitset<8>> data);

////////////////////////////////////////////////////////////////
//	Description:Performs the calculation of 4 hamming parity bits
//
//	Arguments:	[in]bitset<8>: binary representation of a
//							   character
//
//	Return:		[out]bitset<12>: binary character with
//								 hamming parity bits
////////////////////////////////////////////////////////////////
bitset<12> CalculateHammingCode(bitset<8> byteOfData);

////////////////////////////////////////////////////////////////
//	Description:Starts the client connection and transmits all the
//				messages composed of binary characters (with
//				possible transmission errors given by numOfErrors)
//
//	**Hamming Overload
//	Arguments:	[in]list<HammingFrame>: list containing all the
//										frames to be transmitted
//				[in]int:number of errors to be introduced during
//					    transmission
//
////////////////////////////////////////////////////////////////
void TransmitFrames(list<HammingFrame> frames, int numOfErrors); 

////////////////////////////////////////////////////////////////
//	Description:Changes the number of bits indicated
//				in the input parameter in a random position in
//				the message.
//	**Note: This function will make sure 2 errors are not
//			generated in the same character
//
//	**Hamming Overload
//	Arguments:	[in]list<HammingFrame>: frames containing the
//										data without errors
//				[out]list<HammingFrame>&: frames containing the
//										data with errors
//				[in]int*: Number of bits to change.
//
//	Return:		[out]list<TransmissionError>:list of the location
//										   of errors in the frame
////////////////////////////////////////////////////////////////
list<TransmissionError> GenerateTransmissionError(list<HammingFrame> frames,
												  list<HammingFrame> &framesWithErrors,
												  int *numberOfBitsToChange);

////////////////////////////////////////////////////////////////
//	Description:Copies the frames to a different list
//
//	**Hamming Overload
//	Arguments:	[in]list<HammingFrame>: Original list
//				[out]list<HammingFrame>&: Copy of the frames
//
////////////////////////////////////////////////////////////////
void CopyFrames(list<HammingFrame> frames, list<HammingFrame> &framesWithErrors);