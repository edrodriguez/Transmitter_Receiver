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
//				messages transmitted, calls methods for checking		/////////////////////////////fix
//				the validity of the messages and printing out
//				the messages to the console
////////////////////////////////////////////////////////////////
void ReceiveHammingMessage()
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

		char M[805];
		list<char> finalMessage = { 'D','o','n','e' };
		list<char> message;
		int framesReceived = 0;

		while (transmitting)
		{
			recv(newConnection, M, sizeof(M), NULL); //Receive Message
			framesReceived++;

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
				list<bitset<12>> FrameWithHamming;
				FrameWithHamming = ConvertToBitsets(messageString);

				if (IsHammingValid(FrameWithHamming, framesReceived))
				{

					list<char> convertedFrame;
					list<bitset<8>> deFramedData;
					deFramedData = DeFrame(FrameWithHamming);
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
//	Description:Starts the server connection, receives the
//				messages transmitted, calls methods for checking		/////////////////////////////fix
//				the validity of the messages and printing out
//				the messages to the console
////////////////////////////////////////////////////////////////
void ReceiveCRCMessage()
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

		char M[553];
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
				list<bool> FrameWithCRC;
				FrameWithCRC = ConvertToBoolList(messageString);

				if (IsCRCValid(FrameWithCRC))
				{

					list<char> convertedMessage;
					list<bitset<8>> deFramedData;
					deFramedData = DeFrame(FrameWithCRC);
					convertedMessage = ConvertBinaryMessage(deFramedData);

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
//									 the message						//////////
////////////////////////////////////////////////////////////////
list<bitset<12>> ConvertToBitsets(string message)
{
	list<bitset<12>> binaryCharacters;

	//get syn chars
	for (int i = 0; i < 24; i += 12)
	{
		string synCharacter = message.substr(i, 12);
		bitset<12> binaryCharacter(synCharacter);
		binaryCharacters.push_back(binaryCharacter);
	}

	//get control char
	string controlCharacter = message.substr(24, 12);
	bitset<12> binaryCharacter(controlCharacter);
	binaryCharacters.push_back(binaryCharacter);

	for (size_t i = 36; i < message.size(); i += 12)
	{
		string character = message.substr(i, 12);

		//reverse character because it was received by LSB first
		//bitset<8> reversedBinaryCharacter(character);
		bitset<12> binaryCharacter(character);
		//for (size_t j = 0; j < reversedBinaryCharacter.size(); j++)
			//binaryCharacter[binaryCharacter.size() - 1 - j] = (reversedBinaryCharacter)[j];

		binaryCharacters.push_back(binaryCharacter);
	}

	return binaryCharacters;
}

////////////////////////////////////////////////////////////////
//	Description:Separates a string of binary characters into
//				8 bit bitsets and inverts the data characters
//				as they were transmitted with LSB first
//
//	Arguments:	[in]string: message
//
//	Return:		[out]list<bitset<8>>:list of bitsets representing
//									 the message						//////////Include reversing
////////////////////////////////////////////////////////////////
list<bool> ConvertToBoolList(string message)
{
	list<bool> binaryCharacters;
	const int ASCII_CONVERTER = 48;

	for (size_t i = 0; i < message.size(); i++)
		binaryCharacters.push_back(message[i] - ASCII_CONVERTER);

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
bool IsHammingValid(list<bitset<12>> &binaryCharacters, int framesReceived)
{
	int charLocation = 1;

	for (list<bitset<12>>::iterator it = binaryCharacters.begin(); it != binaryCharacters.end(); it++)
	{
		HammingErrorDetection hammingCheckResults;

		hammingCheckResults = CheckHammingParity(*it);

		if (!hammingCheckResults.isHammingCorrect)
		{
			cout << "---Received Message Contained Error, Atempting to Fix Character---" << endl;
			cout << "*Location of Error:" << endl;
			cout << "\t-Frame: " << framesReceived << endl;;
			cout << "\t-Character: " << charLocation << endl;;
			cout << "\t-Bit: " << (it->size() - hammingCheckResults.errorBit + 1) << endl;;
			//Attempt to fix code
			it->flip(it->size() - hammingCheckResults.errorBit);
			hammingCheckResults = CheckHammingParity(*it);
			if (!hammingCheckResults.isHammingCorrect)
			{
				cout << "----------Character Couldn't be Fixed, Requesting Resend----------" << endl;
				return false;
			}
			else
				cout << "-------------------------Fixed Character--------------------------" << endl;
		}
		charLocation++;
	}
	return true;
}
//////////////////////////////////////////////
bool IsCRCValid(list<bool> binaryCharacters)
{
	bitset<17> CRCANSI("11000000000000101");
	list<bool> D = binaryCharacters;
	list<bool> dividend;
	bool correctMessage = true;

	while (!D.empty())
	{
		while (dividend.size() < CRCANSI.size() && !D.empty())
		{
			dividend.push_back(*(D.begin()));
			D.pop_front();
		}

		if (dividend.size() == CRCANSI.size())
		{
			//Perform XOR Operation
			dividend = PerformXORWithCRCANSI(dividend, CRCANSI);
		}
	}

	//Erase leading 0s
	while (!dividend.empty() && *(dividend.begin()) != 1)
		dividend.pop_front();

	if (!dividend.empty())
		return false;

	return true;
}

list<bool> PerformXORWithCRCANSI(list<bool> l, bitset<17> b2)
{
	bitset<17> b1;
	bitset<17> result;
	list<bool> outResult;

	int index = b1.size() - 1;
	for (list<bool>::iterator it = l.begin(); it != l.end(); it++)
	{
		b1[index] = *it;
		index--;
	}

	//XOR
	result = b1^b2;

	for (size_t i = 0; i < result.size(); i++)
	{
		outResult.push_front(result[i]);
	}

	//Erase leading 0s
	while (!outResult.empty() && *(outResult.begin()) != 1)
		outResult.pop_front();

	return outResult;
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
HammingErrorDetection CheckHammingParity(bitset<12> binaryChar)
{
	int parityCount = 0;
	int parityBitLocation = 0;
	int checkIndex = 0;
	HammingErrorDetection error;
	error.errorBit = 0;
	error.isHammingCorrect = true;

	//Check parity bit 1 (p1)
	parityBitLocation = binaryChar.size() - 1;
	checkIndex = parityBitLocation;
	do
	{
		if (binaryChar[checkIndex] == 1)
			parityCount++;
		checkIndex -= 2;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		error.errorBit += 1;

	//Calculate parity bit 2 (p2)
	parityCount = 0;
	parityBitLocation = binaryChar.size() - 2;
	checkIndex = parityBitLocation;
	do
	{
		if (binaryChar[checkIndex] == 1)
			parityCount++;
		if (binaryChar[checkIndex - 1] == 1 && checkIndex >= 0)
			parityCount++;
		checkIndex -= 4;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		error.errorBit += 2;

	//Calculate parity bit 4 (p4)
	parityCount = 0;
	parityBitLocation = binaryChar.size() - 4;
	checkIndex = parityBitLocation;
	do
	{
		if (binaryChar[checkIndex] == 1)
			parityCount++;
		if (checkIndex - 1 >= 0)
			if (binaryChar[checkIndex - 1])
				parityCount++;
		if (checkIndex - 2 >= 0)
			if (binaryChar[checkIndex - 2])
				parityCount++;
		if (checkIndex - 3 >= 0)
			if (binaryChar[checkIndex - 3])
				parityCount++;
		checkIndex -= 8;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		error.errorBit += 4;

	//Calculate parity bit 8 (p8)
	parityCount = 0;
	parityBitLocation = binaryChar.size() - 8;
	checkIndex = parityBitLocation;
	do
	{
		if (binaryChar[checkIndex] == 1)
			parityCount++;
		if (checkIndex - 1 >= 0)
			if (binaryChar[checkIndex - 1])
				parityCount++;
		if (checkIndex - 2 >= 0)
			if (binaryChar[checkIndex - 2])
				parityCount++;
		if (checkIndex - 3 >= 0)
			if (binaryChar[checkIndex - 3])
				parityCount++;
		if (checkIndex - 4 >= 0)
			if (binaryChar[checkIndex - 4])
				parityCount++;
		if (checkIndex - 5 >= 0)
			if (binaryChar[checkIndex - 5])
				parityCount++;
		if (checkIndex - 6 >= 0)
			if (binaryChar[checkIndex - 6])
				parityCount++;
		if (checkIndex - 7 >= 0)
			if (binaryChar[checkIndex - 7])
				parityCount++;
		checkIndex -= 16;
	} while (checkIndex >= 0);
	if (parityCount % 2 == 1) //odd parity
		error.errorBit += 8;

	//If there is an error bit, the code is wrong
	if (error.errorBit != 0)
	{
		error.isHammingCorrect = false;
	}
	else
		error.isHammingCorrect = true;

	return error;
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
		convertedMessage.push_back(it->to_ulong());

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