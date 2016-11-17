////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the physical layer of the receiver.
//				This layer is in charge of receiving a message,
//				checking the validity of the message and
//				converting the binary received into characters
////////////////////////////////////////////////////////////////
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "PhysicalLayer.h"
#include "Application.h"
#include "DataLink.h"
#include <WinSock2.h>
#include <iostream>
#include <bitset>
#include <list>
#include <array>
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
list<char> ConvertBinaryMessage(list<bitset<8>> binaryCharacters)
{
	list<char> convertedMessage;

	for (list<bitset<8>>::iterator it = binaryCharacters.begin(); it != binaryCharacters.end(); it++)
		convertedMessage.push_back(ConvertBinaryToChar(*it));

	return convertedMessage;
}

//////////////////////////////////////////////////////////////// 
//  Description:Converts a bitset into a character 
// 
//  Arguments:  [in]bitset<8>:binary char 
// 
//  Return:    [out]char: character obtained from the binary 
//////////////////////////////////////////////////////////////// 
char ConvertBinaryToChar(bitset<8> binaryChar)
{
	bitset<7> charWithoutParityBit;

	for (size_t i = 0; i <= binaryChar.size() - 2; i++)
		charWithoutParityBit[i] = binaryChar[i];

	return char(charWithoutParityBit.to_ulong());
}

////////////////////////////////////////////////////////////////
//	Description:Converts a string into a list of bools
//
//	Arguments:	[in]string: frame
//
//	Return:		[out]list<bool>:list of bools representing
//									 the frame					
////////////////////////////////////////////////////////////////
list<bool> ConvertToBoolList(string message)
{
	list<bool> binaryCharacters;
	const int ASCII_CONVERTER = 48;

	for (size_t i = 0; i < message.size(); i++)
		binaryCharacters.push_back((message[i] - ASCII_CONVERTER) != 0);

	return binaryCharacters;
}

////////////////////////////////////////////////////////////////
//	Description:Starts the server connection, receives the
//				messages transmitted, decodes it, calls methods
//				for checking the validity of the messages, and
//				prints out the message to the console
////////////////////////////////////////////////////////////////
void ReceiveMessage()
{
	list<char> convertedMessage;

   //Startup
	WSAData wsaData;
	int		sizeOfAddr;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	SOCKADDR_IN address;
	sizeOfAddr = sizeof(address);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(1111);
	address.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&address, sizeof(address));
	listen(sListen, SOMAXCONN);


	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&address, &sizeOfAddr);

	if (newConnection == 0)
	{
		cout << "Failed to accept the client's connection." << endl;
	}
	else
	{
		cout << "Client Connected!" << endl;
		bool transmitting = true;
		char accepted[1] = { 1 };
		char rejected[1] = { 0 };

		char M[537];
		list<char> finalMessage = { 'D','o','n','e' };
		vector<char> message;

		while (transmitting)
		{
			recv(newConnection, M, sizeof(M), NULL); //Receive Message

			//Check if done receiving
			int matchCount = 0;
			for (list<char>::iterator it = finalMessage.begin(); it != finalMessage.end(); it++)
			{
				if (*it == M[matchCount])
					matchCount++;
				else
					break;

				if (matchCount == 4) //received message is "Done"
					transmitting = false;
			}

			if (transmitting)
			{
				//store incoming message in a list of characters
				for (int i = 0; i < sizeof(M); i++)
				{
					if (M[i] == NULL)
						break;
					else
						message.push_back(M[i]);
				}

				//Convert received message to binary
				list<char> binaryMessage = DecodeHDB3(message);

				string binaryMessageString;
				for (list<char>::iterator it = binaryMessage.begin(); it != binaryMessage.end(); it++)
					binaryMessageString.append(1u, *it);

				//convert the string into binary bitsets
				list<bitset<8>> frame;
				frame = ConvertToBitsets(binaryMessageString);

				if (IsMessageValid(frame))
				{

					list<char> convertedFrame;
					list<bitset<8>> deFramedData;
					deFramedData = DeFrame(frame);
					convertedFrame = ConvertBinaryMessage(deFramedData);

					for (list<char>::iterator it = convertedFrame.begin(); it != convertedFrame.end(); it++)
						convertedMessage.push_back(*it);

					//send confirmation
					send(newConnection, accepted, sizeof(accepted), NULL);
				}
				else //send rejection
					send(newConnection, rejected, sizeof(rejected), NULL);

				message.clear();
			}
		}
	}
	//print message
	cout << "Received Message:" << endl;
	PrintList(convertedMessage);
	cout << endl;
}

////////////////////////////////////////////////////////////////
//	Description:Separates a string of binary characters into
//				8 bit bitsets
//
//	Arguments:	[in]string: frame
//
//	Return:		[out]list<bitset<8>>:list of bitsets representing
//									 the received frame
////////////////////////////////////////////////////////////////
list<bitset<8>> ConvertToBitsets(string frame)
{
	list<bitset<8>> binaryCharacters;

	//get syn chars
	for (int i = 0; i < 16; i += 8)
	{
		string synCharacter = frame.substr(i, 8);
		bitset<8> binaryCharacter(synCharacter);
		binaryCharacters.push_back(binaryCharacter);
	}

	//get control char
	string controlCharacter = frame.substr(16, 8);
	bitset<8> binaryCharacter(controlCharacter);
	binaryCharacters.push_back(binaryCharacter);

	for (size_t i = 24; i < frame.size(); i += 8)
	{
		string character = frame.substr(i, 8);

		bitset<8> binaryCharacter(character);

		binaryCharacters.push_back(binaryCharacter);
	}

	return binaryCharacters;
}

////////////////////////////////////////////////////////////////
//	Description:Decodes a vector of characters with HDB3 encoding
//				and saves the result into a list of 1s and 0s
//
//	Arguments:	[in]vector<char>: message
//
//	Return:		[out]list<char>:list decoded 1s and 0s
////////////////////////////////////////////////////////////////
list<char> DecodeHDB3(vector<char> message)
{
	list<char> convertedMessage;
	size_t lastIndexConverted = -1;
	bool lastPulse = 0; //0 for negative pulse, 1 for positive pulse
	int countOf1s = 0;
	array<char,4> block;
	array<array<char, 4>, 4> patterns{ {
		{ '-', '0', '0', '-' },
		{ '+', '0', '0', '+' },
		{ '0', '0', '0', '+' },
		{ '0', '0', '0', '-' }
		}
	};

	for (size_t i = 0; i + 3 < message.size(); i++)
	{
		block[0] = message[i];
		block[1] = message[i+1];
		block[2] = message[i+2];
		block[3] = message[i+3];

		for (size_t j = 0; j < patterns.size(); j++)
		{
			if (block == patterns[j])
			{
				//000V check other factors because it is possible that it is only 3 zeros
				if (j == 2)
				{
					countOf1s = Count1s(message, i);
					if (countOf1s % 2 != 0)
						break;
					if (!lastPulse)
						break;
				}
				else if (j == 3)
				{
					countOf1s = Count1s(message, i);
					if (countOf1s % 2 != 0)
						break;
					if (lastPulse)
						break;
				}

				for (size_t k = lastIndexConverted + 1; k < i; k++)
				{
					if (message[k] == '+' || message[k] == '-')
						convertedMessage.push_back('1');
					
					else if (message[k] == '0')
						convertedMessage.push_back('0');
				}
				convertedMessage.push_back('0');
				convertedMessage.push_back('0');
				convertedMessage.push_back('0');
				convertedMessage.push_back('0');
				i += 3;
				lastIndexConverted = i;
				break;
			}
		}
		if (block[0] == '+')
			lastPulse = 1;
		else if (block[0] == '-')
			lastPulse = 0;
	}

	if (lastIndexConverted < message.size())
	{
		for (size_t k = lastIndexConverted + 1; k < message.size(); k++)
		{
			if (message[k] == '+' || message[k] == '-')
				convertedMessage.push_back('1');

			else if (message[k] == '0')
				convertedMessage.push_back('0');
		}
	}

	return convertedMessage;
}

////////////////////////////////////////////////////////////////
//	Description:counts the number of +s and -s in a list of chars
//				from the beginning of the list until finalIndex-1
//
//	Arguments:	[in]vector<char>: list to check
//				[in]int: Index to determine the termination of the
//						loop
//
//	Return:		[out]int: number of +s and -s
////////////////////////////////////////////////////////////////
int Count1s(vector<char> l, int finalIndex) {
	int countOf1s = 0;

	for(int i = 0; i < finalIndex; i++) {
		if (l[i] == '+' || l[i] == '-')
			countOf1s++;
	}

	return countOf1s;
}

//////////////////////////////////////////////////////////////// 
//  Description:Checks that the message had no transmission 
//        errors 
// 
//  Arguments:  [in]list<bitset<8>>:list of bitsets representing 
//                   the message 
// 
//  Return:    [out]bool:indicates if the message is valid 
//              or not 
//  Ret Value:  true if valid message, false if invalid 
//////////////////////////////////////////////////////////////// 
bool IsMessageValid(list<bitset<8>> binaryCharacters)
{
	for (list<bitset<8>>::iterator it = binaryCharacters.begin(); it != binaryCharacters.end(); it++)
		if (!CheckParity(*it))
			return false;
	return true;
}

//////////////////////////////////////////////////////////////// 
//  Description:Checks that the parity bit of the input bitset 
//        is correct 
// 
//  Arguments:  [in]bitset<8>:binary bitset with a parity bit 
// 
//  Return:    [out]bool:indicates if the bitset has the  
//              correct parity bit 
//  Ret Value:  true if correct parity, false if incorrect 
//////////////////////////////////////////////////////////////// 
bool CheckParity(bitset<8> binaryChar)
{
	bitset<7> charWithoutParityBit;

	//MSB is the parity bit 
	int parityBit = binaryChar[binaryChar.size() - 1];

	//remove remove parity bit 
	for (size_t i = 0; i <= binaryChar.size() - 2; i++)
		charWithoutParityBit[i] = binaryChar[i];

	//supposed odd parity 
	if (parityBit == 1)
	{
		//check remaining message for an odd number of 1s 
		if (charWithoutParityBit.count() % 2 == 1)
			return true;
	}
	//supposed even parity 
	else if (parityBit == 0)
	{
		//check remaining message for an even number of 1s 
		if (charWithoutParityBit.count() % 2 == 0)
			return true;
	}

	return false;
}
