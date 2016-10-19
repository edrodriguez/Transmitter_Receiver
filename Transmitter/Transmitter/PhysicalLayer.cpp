////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the physical layer of the transmitter.
//				This layer is in charge of converting the
//				message to binary and transmitting it
////////////////////////////////////////////////////////////////
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include "PhysicalLayer.h"
#include <string>
#include <bitset>
#include <list>

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
	list<bitset<7>> binaryInfo;
	list<bitset<8>> binaryWithParity;

	//convert caracters to binary
	for (list<char>::iterator it = charList.begin(); it != charList.end(); it++)
		binaryInfo.push_back(ConvertToBinary(*it));

	//Include parity bits
	for (list<bitset<7>>::iterator it = binaryInfo.begin(); it != binaryInfo.end(); it++)
		binaryWithParity.push_back(IncludeParityBit(*it));

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
bitset<7> ConvertToBinary(char C)
{
	return bitset<7>(C);
}

////////////////////////////////////////////////////////////////
//	Description: appends a binary bit to the end of a 7 bit
//				 binary code representing a char
//
//	Arguments:	[in]bitset<7>: char represented in binary
//
//	Return:		[out]bitset<8>: bitset including the 7 bit
//								representation of the char 
//								and the parity bit
////////////////////////////////////////////////////////////////
bitset<8> IncludeParityBit(bitset<7> binaryChar)
{
	bitset<8> binaryCharWithParity;

	binaryCharWithParity[0] = IsOddParity(binaryChar);

	for (size_t i = 0; i < binaryChar.size(); i++)
		binaryCharWithParity[i + 1] = binaryChar[i];

	return binaryCharWithParity;
}

////////////////////////////////////////////////////////////////
//	Description:Calculates whether the input binary character
//				string has even or odd parity 
//
//	Arguments:	[in]bitset<7>: char represented in binary
//
//	Return:		[out]bool: indicating if the parity is even or odd
//	Ret Value:	true if odd parity, false if even parity
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
//				messages composed of binary characters
//
//	Arguments:	[in]list<string>: list containing all the messages
//
////////////////////////////////////////////////////////////////
void TransmitMessages(list<string> messages)
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
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(Connection, (SOCKADDR*)&address, sizeOfAddr) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	//Client connected. Transmit messages
	cout << "Connected!" << endl;

	char message[537];
	char accepted[1];
	char finalMessage[537] = "Done";

	for (list<string>::iterator it = messages.begin(); it != messages.end(); it++)
	{
		string mes = *it;
		mes.copy(message, mes.length(), 0);
		message[mes.length()] = NULL;

		do
		{
			cout << "Sending Message: " << *it << endl;
			send(Connection, message, sizeof(message), NULL); //send message
			recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval
			if (accepted[0] == 1)
				cout << "--------------------Accepted Message-------------------" << endl;
			else
				cout << "--------------------Rejected Message-------------------" << endl;
		} while (accepted[0] == 0);
	}
	send(Connection, finalMessage, sizeof(finalMessage), NULL);
}

void GenerateTransmissionError(int numberOfBitsToChange = 2)
{

}