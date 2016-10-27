#pragma once
////////////////////////////////////////////////////////////////
//	Description:This file contains the declaration of functions
//				of the physical layer of the transmitter.
//				This layer is in charge of converting the
//				message to binary and transmitting it
////////////////////////////////////////////////////////////////
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <list>
#include <vector>
#include <bitset>

using namespace std;

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

struct Message
{
	list<HammingFrame> HammingFrames;
	list<CRCFrame> CRCFrames;
};

struct TransmissionError
{
	int frameLocation;
	int charLocation;
	int bitLocation;
	Message messageWithErrors;
};

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


////////////////////////////////////////////////////////////////////////////////////Describe
void ConnectSocket(SOCKET &Connection);

////////////////////////////////////////////////////////////////
//	Description:Starts the client connection and transmits all the
//				messages composed of binary characters
//
//	Arguments:	[in]list<string>: list containing all the messages		////////////change
//				[in]int:number of errors to be introduced during
//					transmission
//
////////////////////////////////////////////////////////////////
void TransmitFrames(list<HammingFrame> frames, int numOfErrors);   /////////////////////hamming overload

////////////////////////////////////////////////////////////////
//	Description:Starts the client connection and transmits all the
//				messages composed of binary characters
//
//	Arguments:	[in]list<string>: list containing all the messages		////////////change
//				[in]int:number of errors to be introduced during
//					transmission
//
////////////////////////////////////////////////////////////////
void TransmitFrames(list<CRCFrame> frames, int numOfErrors);   /////////////////////crc overload

////////////////////////////////////////////////////////////////
//	Description:Changes the number of bits indicated
//				in the input parameter in a random position in
//				the message
//
//	Arguments:	[in]int (optional): Number of bits to change.		/////////////change
//									Default value: 0.
//
////////////////////////////////////////////////////////////////     ///////////hamming overload
list<TransmissionError> GenerateTransmissionError(list<HammingFrame> frames, list<HammingFrame> &framesWithErrors, int *numberOfBitsToChange);

//////////////////////////////crc overload
list<TransmissionError> GenerateTransmissionError(list<CRCFrame> frames, list<CRCFrame> &framesWithErrors, int numberOfBitsToChange);

void CopyDataForErrorGeneration(list<HammingFrame> frames, list<HammingFrame> &framesWithErrors);

//////////crc
void CopyDataForErrorGeneration(list<CRCFrame> frames, list<CRCFrame> &framesWithErrors);

list<bitset<12>> GenerateHammingForData(list<bitset<8>> data);

bitset<12> CalculateHammingCode(bitset<8> byteOfData);

void CalculateCRC(CRCFrame &frame);