////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the physical layer of the transmitter.
//				This layer is in charge of converting the
//				message to binary, applying CRC or Hamming
//				and transmitting the message
////////////////////////////////////////////////////////////////
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include "PhysicalLayer.h"
#include "Application.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <bitset>
#include <list>
#include <random> 
#include <array>

using namespace std;

/**************************************************************/
/****************************Common****************************/
/**************************************************************/

//////////////////////////////////////////////////////////////// 
//  Description: converts a list of characters into a list of 
//         bitsets containing a 7 bit binary representation 
//         of a character and a parity bit 
// 
//  Arguments:  [in]list<char>: list of characters 
// 
//  Return:    [out]list<bitset<8>>:list of bitsets. Each bitset 
//                   made up of 7 binary bits 
//                   representing a character 
//                   and a parity bit 
//////////////////////////////////////////////////////////////// 
list<bitset<8>> ConvertTextForTransmission(list<char> charList)
{
	list<bitset<8>> binaryInfo;

	//convert caracters to binary
	for (list<char>::iterator it = charList.begin(); it != charList.end(); it++)
		binaryInfo.push_back(IncludeParityBit(ConvertToBinary(*it)));


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
bitset<7> ConvertToBinary(char C)
{
	return bitset<7>(C);
}

//////////////////////////////////////////////////////////////// 
//  Description: appends a binary bit to the end of a 7 bit 
//         binary code representing a char 
// 
//  Arguments:  [in]bitset<7>: char represented in binary 
// 
//  Return:    [out]bitset<8>: bitset including the 7 bit 
//                representation of the char  
//                and the parity bit 
//////////////////////////////////////////////////////////////// 
bitset<8> IncludeParityBit(bitset<7> binaryChar)
{
	bitset<8> binaryCharWithParity;

	binaryCharWithParity[binaryCharWithParity.size() - 1] = IsOddParity(binaryChar);

	for (size_t i = 0; i < binaryChar.size(); i++)
		binaryCharWithParity[i] = binaryChar[i];

	return binaryCharWithParity;
}

//////////////////////////////////////////////////////////////// 
//  Description:Calculates whether the input binary character 
//        string has even or odd parity  
// 
//  Arguments:  [in]bitset<7>: char represented in binary 
// 
//  Return:    [out]bool: indicating if the parity is even or odd 
//  Ret Value:  true if odd parity, false if even parity 
//////////////////////////////////////////////////////////////// 
bool IsOddParity(bitset<7> binaryChar)
{
	if (binaryChar.count() % 2 == 0)
		return false; //contains an even number of 1 bits 
	else
		return true; //contains an odd number of 1 bits 
}

////////////////////////////////////////////////////////////////
//	Description:Starts the client connection and transmits all the
//				messages composed of binary characters (with
//				possible transmission errors given by numOfErrors)
//
//	**CRC Overload
//	Arguments:	[in]list<CRC>: list containing all the
//							   frames to be transmitted				/////////////////////////
//				[in]int:number of errors to be introduced during
//					    transmission
//
////////////////////////////////////////////////////////////////
void TransmitFrames(list<Frame> frames)
{
	char transmittedMessage[537];
	char accepted[1] = { '0' };
	char finalMessage[537] = "Done";

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

	cout << "Sending " << frames.size() << " Frames: " << endl;

	for (list<Frame>::iterator dataIt = frames.begin(); dataIt != frames.end(); dataIt++)
	{
		string  frame;

		////////////////////DO HDB3


		//frame = FrameToString(*dataIt);
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
		if (retryCount == 5)
		{
			recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval
			break;
		}
		
	}
	send(Connection, finalMessage, sizeof(finalMessage), NULL);
}