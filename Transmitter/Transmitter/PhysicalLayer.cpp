////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the physical layer of the transmitter.
//				This layer is in charge of converting the
//				message to binary and transmitting it
////////////////////////////////////////////////////////////////
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include "PhysicalLayer.h"
#include "Application.h"
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <bitset>
#include <list>
#include <random> 

using namespace std;

////////////////////////////////////////////////////////////////
//	Description: converts a list of characters into a list of
//				 bitsets containing a 7 bit binary representation
//				 of a character and a parity bit
//
//	Arguments:	[in]list<char>: list of characters
//
//	Return:		[out]list<bitset<8>>:list of bitsets. Each bitset
//									 made up of 7 binary bits
//									 representing a character
//									 and a parity bit
////////////////////////////////////////////////////////////////
list<bitset<8>> ConvertTextForTransmission(list<char> charList)
{
	list<bitset<8>> binaryInfo;

	//convert caracters to binary
	for (list<char>::iterator it = charList.begin(); it != charList.end(); it++)
		binaryInfo.push_back(ConvertToBinary(*it));

	return binaryInfo;
}

////////////////////////////////////////////////////////////////
//	Description:Converts a character into a 7 bit binary bitset
//
//	Arguments:	[in]char: character to be converted
//
//	Return:		[out]bitset<7>: binary code for the char stored
//								in a 7 bit bitset
////////////////////////////////////////////////////////////////
bitset<8> ConvertToBinary(char C)
{
	return bitset<8>(C);
}

////////////////////////////////////////////////////////////////////////////////////Describe
void ConnectSocket(SOCKET &Connection)
{
	//Startup
	WSAData wsaData;
	int sizeOfAddr;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN address;
	sizeOfAddr = sizeof(address);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(1111);
	address.sin_family = AF_INET;

	if (connect(Connection, (SOCKADDR*)&address, sizeOfAddr) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	//Connected
	cout << "Connected!" << endl;
}

////////////////////////////////////////////////////////////////
//	Description:Starts the client connection and transmits all the
//				messages composed of binary characters
//
//	Arguments:	[in]list<string>: list containing all the messages
//
////////////////////////////////////////////////////////////////
void TransmitFrames(list<HammingFrame> frames, int numOfErrors)				/////////hamming overload
{
	list<transmissionError> errorsIntroduced;
	list<HammingFrame> framesWithErrors;
	char transmittedMessage[805];
	char accepted[1] = { '0' };
	char finalMessage[805] = "Done";

	//Connect Socket
	WSAData wsaData;
	int sizeOfAddr;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN address;
	sizeOfAddr = sizeof(address);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(1111);
	address.sin_family = AF_INET;
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(Connection, (SOCKADDR*)&address, sizeOfAddr) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	//Connected
	cout << "Connected!" << endl;

	//Generate and send messages with errors
	if (numOfErrors > 0)
	{
		errorsIntroduced = GenerateTransmissionError(frames, framesWithErrors, numOfErrors);
		PrintList(errorsIntroduced);
		PrintList(framesWithErrors);

		cout << "Sending " << framesWithErrors.size() <<" Frames: " << endl;

		for (list<HammingFrame>::iterator dataIt = frames.begin(), errorIt = framesWithErrors.begin();
			dataIt != frames.end() && errorIt != framesWithErrors.end(); dataIt++, errorIt++)
		{
			string errorFrame, realFrame;
			
			errorFrame = FrameToString(*errorIt);
			errorFrame.copy(transmittedMessage, errorFrame.length(), 0);
			transmittedMessage[errorFrame.length()] = NULL;
			send(Connection, transmittedMessage, sizeof(transmittedMessage), NULL);

			int retryCount = 0;
			do
			{
				recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval

				if (accepted[0] == 1)
					cout << "--------------------Accepted Message-------------------" << endl;
				else
				{
					cout << "--Message Contained Errors and Could Not Be Corrected--" << endl;
					cout << "----------------Retransmitting Message-----------------" << endl;
					realFrame = FrameToString(*dataIt);
					realFrame.copy(transmittedMessage, realFrame.length(), 0);
					transmittedMessage[realFrame.length()] = NULL;
					send(Connection, transmittedMessage, sizeof(transmittedMessage), NULL);
				}
			} while (accepted[0] == 0 && retryCount < 5);
		}
	}
	//Send only messages without errors
	else
	{
		cout << "Sending " << frames.size() << " Frames: " << endl;

		for (list<HammingFrame>::iterator dataIt = frames.begin(); dataIt != frames.end(); dataIt++)
		{
			string  frame;

			frame = FrameToString(*dataIt);
			frame.copy(transmittedMessage, frame.length(), 0);
			transmittedMessage[frame.length()] = NULL;
			send(Connection, transmittedMessage, sizeof(transmittedMessage), NULL);

			int retryCount = 0;
			do
			{
				recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval

				if (accepted[0] == 1)
					cout << "----------------------Accepted Message---------------------" << endl;
				else
				{
					retryCount++;
					cout << "----Message Contained Errors and Could Not Be Corrected----" << endl;
					cout << "------------------Retransmitting Message-------------------" << endl;
					send(Connection, transmittedMessage, sizeof(transmittedMessage), NULL);
				}
			} while (accepted[0] == 0 && retryCount < 5);
		}
	}
	send(Connection, finalMessage, sizeof(finalMessage), NULL);
}

////////////////////////////////////////////////////////////////
//	Description:Changes the number of bits indicated
//				in the input parameter in a random position in
//				the message
//
//	Arguments:	[in]int (optional): Number of bits to change.			/////hamming overload
//									Default value: 0.
//
////////////////////////////////////////////////////////////////
list<transmissionError> GenerateTransmissionError(list<HammingFrame> frames, list<HammingFrame> &framesWithErrors, int numberOfBitsToChange)
{
	list<transmissionError> errors;

	//Copy the data into data with error
	CopyDataForErrorGeneration(frames, framesWithErrors);

	for (int i = 0; i < numberOfBitsToChange; i++)
	{
		int frameLocation, charLocation, bitLocation;
		transmissionError error;
		random_device rd;

		//Randomly generate a frame number
		frameLocation = abs(int(rd())) % framesWithErrors.size();
		list<HammingFrame>::iterator frameIt = next(framesWithErrors.begin(), frameLocation);

		//Randomly generate a character number
		charLocation = abs(int(rd())) % ((frameIt->data).size() + 3);
		
		//Change in Syn Char
		if (charLocation == 0)
		{
			//Randomly generate a bit number
			bitLocation = abs(int(rd())) % 12;
			frameIt->synChar1.flip(bitLocation);
		}
		//Change in Syn Char
		else if (charLocation == 1)
		{
			//Randomly generate a bit number
			bitLocation = abs(int(rd())) % 12;
			frameIt->synChar2.flip(bitLocation);
		}
		//Change in Control Char
		else if (charLocation == 2)
		{
			//Randomly generate a bit number
			bitLocation = abs(int(rd())) % 12;
			frameIt->controlChar.flip(bitLocation);
		}
		//Change in frame data
		else
		{
			//Randomly generate a bit number
			bitLocation = abs(int(rd())) % 12;
			list<bitset<12>>::iterator byteIt = next(frameIt->data.begin(), charLocation);
			byteIt->flip(bitLocation);
		}

		error.frameLocation = frameLocation + 1; /////////////everything is 1 indexed
		error.byteLocation = charLocation + 1;
		error.bitLocation = bitLocation + 1;

		errors.push_back(error);
	}

	return errors;
}

void CopyDataForErrorGeneration(list<HammingFrame> frames, list<HammingFrame> &framesWithErrors)		//hamming overload
{
	for (list<HammingFrame>::iterator it = frames.begin(); it != frames.end(); it++)
		framesWithErrors.push_back(*it);
}

list<bitset<12>> GenerateHammingForData(list<bitset<8>> data)
{
	list<bitset<12>> dataWithHamming;

	for (list<bitset<8>>::iterator it = data.begin(); it != data.end(); it++)
	{
			dataWithHamming.push_back(CalculateHammingCode(*it));
	}

	return dataWithHamming;
}

bitset<12> CalculateHammingCode(bitset<8> byteOfData)
{
	bitset<12> byteWithHamming;
	int parityCount = 0;
	int parityBitLocation = 0;
	int checkIndex = 0;

	//Copy bits in the right positions (p1 p2 b1 p4 b2 b3 b4 p8 b5 b6 b7 b8)
	for (size_t i = 0; i < 4; i++)
	{
		byteWithHamming[i] = byteOfData[i];
	}
	for (size_t i = 5; i < 8; i++)
	{
		byteWithHamming[i] = byteOfData[i - 1];
	}
	byteWithHamming[9] = byteOfData[7];

	//Calculate parity bit 1 (p1)
	parityBitLocation = byteWithHamming.size() - 1;
	checkIndex = parityBitLocation;
	do
	{
		if (byteWithHamming[checkIndex] == 1)
			parityCount++;
		checkIndex -= 2;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		byteWithHamming.set(parityBitLocation);

	//Calculate parity bit 2 (p2)
	parityCount = 0;
	parityBitLocation = byteWithHamming.size() - 2;
	checkIndex = parityBitLocation;
	do
	{
		if (byteWithHamming[checkIndex] == 1)
			parityCount++;
		if (byteWithHamming[checkIndex - 1] == 1 && checkIndex >= 0)
			parityCount++;
		checkIndex -= 4;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		byteWithHamming.set(parityBitLocation);

	//Calculate parity bit 4 (p4)
	parityCount = 0;
	parityBitLocation = byteWithHamming.size() - 4;
	checkIndex = parityBitLocation;
	do
	{
		if (byteWithHamming[checkIndex] == 1)
			parityCount++;
		if (checkIndex - 1 >= 0)
			if (byteWithHamming[checkIndex - 1])
				parityCount++;
		if (checkIndex - 2 >= 0)
			if (byteWithHamming[checkIndex - 2])
				parityCount++;
		if (checkIndex - 3 >= 0)
			if (byteWithHamming[checkIndex - 3])
				parityCount++;
		checkIndex -= 8;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		byteWithHamming.set(parityBitLocation);

	//Calculate parity bit 8 (p8)
	parityCount = 0;
	parityBitLocation = byteWithHamming.size() - 8;
	checkIndex = parityBitLocation;
	do
	{
		if (byteWithHamming[checkIndex] == 1)
			parityCount++;
		if (checkIndex - 1 >= 0)
			if (byteWithHamming[checkIndex - 1])
				parityCount++;
		if (checkIndex - 2 >= 0)
			if (byteWithHamming[checkIndex - 2])
				parityCount++;
		if (checkIndex - 3 >= 0)
			if (byteWithHamming[checkIndex - 3])
				parityCount++;
		if (checkIndex - 4 >= 0)
			if (byteWithHamming[checkIndex - 4])
				parityCount++;
		if (checkIndex - 5 >= 0)
			if (byteWithHamming[checkIndex - 5])
				parityCount++;
		if (checkIndex - 6 >= 0)
			if (byteWithHamming[checkIndex - 6])
				parityCount++;
		if (checkIndex - 7 >= 0)
			if (byteWithHamming[checkIndex - 7])
				parityCount++;
		checkIndex -= 16;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		byteWithHamming.set(parityBitLocation);

	return byteWithHamming;
}

void CalculateCRC(CRCFrame &frame)
{
	bitset<17> CRCANSI("11000000000000101");
	int sizeOfCRCCode = frame.data.size() + 3;
}