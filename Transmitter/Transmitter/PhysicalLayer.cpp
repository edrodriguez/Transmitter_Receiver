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
	list<bitset<8>> binaryWithParity;

	//convert caracters to binary
	for (list<char>::iterator it = charList.begin(); it != charList.end(); it++)
		binaryInfo.push_back(ConvertToBinary(*it));

	return binaryWithParity;
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
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	list<transmissionError> errorsIntroduced;
	list<HammingFrame> framesWithErrors;

	//Generate Transmission errors
	if (numOfErrors > 0)
	{
		errorsIntroduced = GenerateTransmissionError(frames, framesWithErrors, numOfErrors);
		PrintList(errorsIntroduced);
	}

	char transmittedMessage[805];
	char accepted[1] = { '0' };
	char finalMessage[537] = "Done";

	for (list<HammingFrame>::iterator it = frames.begin(); it != frames.end(); it++)
	{
		///Convert frame into string
		///mes.copy(transmittedMessage, mes.length(), 0);
		///message[mes.length()] = NULL;

		//string mes = *it;
		//mes.copy(transmittedMessage, mes.length(), 0);
		//message[mes.length()] = NULL;


		cout << "Sending Frames: " << endl;
		if (numOfErrors != 0)
			;//send message with errors
		else
			;//send correct message
		//send(Connection, transmittedMessage, sizeof(message), NULL); //send message
		
		do
		{
			recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval

			if (accepted[0] == 1)
				cout << "--------------------Accepted Message-------------------" << endl;
			else
			{
				cout << "--Message Contained Errors and Could Not Be Corrected--" << endl;
				//send correct message
			}
		} while (accepted[0] == 0);
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

	return byteWithHamming;
}

void CalculateCRC(CRCFrame &frame)
{

}