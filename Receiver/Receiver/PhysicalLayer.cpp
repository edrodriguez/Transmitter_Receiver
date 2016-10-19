////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the physical layer of the receiver.
//				This layer is in charge of receiving a message,
//				checking the validity of the message and
//				converting the binary received into characters
////////////////////////////////////////////////////////////////
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <bitset>
#include <list>
#include "PhysicalLayer.h"
#include "Application.h"
#include "DataLink.h"

using namespace std;

////////////////////////////////////////////////////////////////
//	Description:Starts the server connection, receives the
//				messages transmitted, calls methods for checking
//				the validity of the messages and printing out
//				the messages to the console
////////////////////////////////////////////////////////////////
void ReceiveMessages()
{
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
		list<char> message;

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
				//convert message into a string
				string messageString;
				for (list<char>::iterator it = message.begin(); it != message.end(); it++)
					messageString.append(1u, *it);

				//convert the string into binary bitsets
				list<bitset<8>> binaryCharacters;
				binaryCharacters = ConvertToBitsets(messageString);

				if (IsMessageValid(binaryCharacters))
				{

					list<char> convertedMessage;
					binaryCharacters = DeFrame(binaryCharacters);
					convertedMessage = ConvertBinaryMessage(binaryCharacters);

					//print message
					PrintList(convertedMessage);

					//send confirmation
					send(newConnection, accepted, sizeof(accepted), NULL);
				}
				else //send rejection
					send(newConnection, rejected, sizeof(rejected), NULL);

				message.clear();
			}
		}
	}
	cout << endl;
}

////////////////////////////////////////////////////////////////
//	Description:Separates a string of binary characters into
//				8 bit bitsets and inverts the data characters
//				as they were transmitted with LSB first
//
//	Arguments:	[in]string: message
//
//	Return:		[out]list<bitset<8>>:list of bitsets representing
//									 the message
////////////////////////////////////////////////////////////////
list<bitset<8>> ConvertToBitsets(string message)
{
	list<bitset<8>> binaryCharacters;

	//get syn chars
	for (int i = 0; i < 16; i += 8)
	{
		string synCharacter = message.substr(i, 8);
		bitset<8> binaryCharacter(synCharacter);
		binaryCharacters.push_back(binaryCharacter);
	}

	//get control char
	string controlCharacter = message.substr(16, 8);
	bitset<8> binaryCharacter(controlCharacter);
	binaryCharacters.push_back(binaryCharacter);

	for (size_t i = 24; i < message.size(); i += 8)
	{
		string character = message.substr(i, 8);

		//reverse character because it was received by LSB first
		bitset<8> reversedBinaryCharacter(character);
		bitset<8> binaryCharacter;
		for (size_t j = 0; j < reversedBinaryCharacter.size(); j++)
			binaryCharacter[binaryCharacter.size() - 1 - j] = (reversedBinaryCharacter)[j];

		binaryCharacters.push_back(binaryCharacter);
	}

	return binaryCharacters;
}

////////////////////////////////////////////////////////////////
//	Description:Checks that the message had no transmission
//				errors. This will be expanded in future milestones
//
//	Arguments:	[in]list<bitset<8>>:list of bitsets representing
//									 the message
//
//	Return:		[out]bool:indicates if the message is valid
//						  or not
//	Ret Value:	true if valid message, false if invalid
////////////////////////////////////////////////////////////////
bool IsMessageValid(list<bitset<8>> binaryCharacters)
{
	for (list<bitset<8>>::iterator it = binaryCharacters.begin(); it != binaryCharacters.end(); it++)
		if (!CheckParity(*it))
			return false;
	return true;
}


////////////////////////////////////////////////////////////////
//	Description:Checks that the parity bit of the input bitset
//				is correct
//
//	Arguments:	[in]bitset<8>:binary bitset with a parity bit
//
//	Return:		[out]bool:indicates if the bitset has the 
//						  correct parity bit
//	Ret Value:	true if correct parity, false if incorrect
////////////////////////////////////////////////////////////////
bool CheckParity(bitset<8> binaryChar)
{
	bitset<7> charWithoutParityBit;

	//LSB is the parity bit
	int parityBit = binaryChar[0];
	
	//remove remove parity bit
	for (size_t i = 1; i <= binaryChar.size() - 1; i++)
		charWithoutParityBit[i - 1] = binaryChar[i];

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

////////////////////////////////////////////////////////////////
//	Description:Converts a list of bitsets into a list of
//				characters. It calls ConvertBinaryToChar for
//				the conversion of each specific bitset
//
//	Arguments:	[in]list<bitset<8>>:list of all the bitsets
//									 to be converted
//
//	Return:		[out]list<char>:list of all the characters
//								converted
////////////////////////////////////////////////////////////////
list<char> ConvertBinaryMessage(list< bitset<8>> binaryCharacters)
{
	list<char> convertedMessage;

	for (list<bitset<8>>::iterator it = binaryCharacters.begin(); it != binaryCharacters.end(); it++)
		convertedMessage.push_back(ConvertBinaryToChar(*it));

	return convertedMessage;
}

////////////////////////////////////////////////////////////////
//	Description:Converts a bitset into a character
//
//	Arguments:	[in]bitset<8>:binary char
//
//	Return:		[out]char: character obtained from the binary
////////////////////////////////////////////////////////////////
char ConvertBinaryToChar(bitset<8> binaryChar)
{
	bitset<7> charWithoutParityBit;

	for (size_t i = 1; i <= binaryChar.size() - 1; i++)
		charWithoutParityBit[i - 1] = binaryChar[i];

	return char(charWithoutParityBit.to_ulong());
}