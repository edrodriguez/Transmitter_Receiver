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
		if (retryCount == 5)
		{
			recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval
			break;
		}
		
	}
	send(Connection, finalMessage, sizeof(finalMessage), NULL);
}

////////////////////////////////////////////////////////////////
//	Description:Changes the number of bits indicated
//				in the input parameter in a random position in
//				the message
//
//	**CRC Overload
//	Arguments:	[in]list<CRCFrame>: frames containing the
//									data without errors
//				[out]list<CRCFrame>&: frames containing the
//									 data with errors
//				[in]int*: Number of bits to change.
//
//	Return:		[out]list<TransmissionError>:list of the location
//										   of errors in the frame
////////////////////////////////////////////////////////////////
list<TransmissionError> GenerateTransmissionError(list<CRCFrame> frames,
												  list<CRCFrame> &framesWithErrors,
												  int numberOfBitsToChange)
{
	list<TransmissionError> errors;

	//Copy the data into data with error
	CopyFrames(frames, framesWithErrors);

	for (int i = 0; i < numberOfBitsToChange; i++)
	{
		int frameLocation, charLocation, bitLocation;
		TransmissionError error;
		random_device rd;

		//Randomly generate a frame number
		frameLocation = abs(int(rd())) % framesWithErrors.size();
		list<CRCFrame>::iterator frameIt = next(framesWithErrors.begin(), frameLocation);

		//Randomly generate a character number
		charLocation = abs(int(rd())) % ((frameIt->data).size() + 3);

		//Change in Syn Char
		if (charLocation == 0)
		{
			//Randomly generate a bit number
			bitLocation = abs(int(rd())) % frameIt->synChar1.size();
			frameIt->synChar1.flip(bitLocation);
		}
		//Change in Syn Char
		else if (charLocation == 1)
		{
			//Randomly generate a bit number
			bitLocation = abs(int(rd())) % frameIt->synChar2.size();
			frameIt->synChar2.flip(bitLocation);
		}
		//Change in Control Char
		else if (charLocation == 2)
		{
			//Randomly generate a bit number
			bitLocation = abs(int(rd())) % frameIt->controlChar.size();
			frameIt->controlChar.flip(bitLocation);
		}
		//Change in frame data
		else
		{
			//Randomly generate a bit number
			list<bitset<8>>::iterator bitIt = next(frameIt->data.begin(), charLocation - 3);
			bitLocation = abs(int(rd())) % bitIt->size();
			bitIt->flip(bitLocation);
		}

		error.frameLocation = frameLocation + 1;	//*Everything is displayed with starting index of
		error.charLocation = charLocation + 1;		//*1 to make it easier to look at to the user
		error.bitLocation = bitLocation + 1;

		errors.push_back(error);
	}

	return errors;
}

////////////////////////////////////////////////////////////////
//	Description:Copies the frames to a different list
//
//	**CRC Overload
//	Arguments:	[in]list<CRCFrame>: Original list
//				[out]list<CRCFrame>&: Copy of the frames
//
////////////////////////////////////////////////////////////////
void CopyFrames(list<CRCFrame> frames, list<CRCFrame> &framesWithErrors)
{
	for (list<CRCFrame>::iterator it = frames.begin(); it != frames.end(); it++)
		framesWithErrors.push_back(*it);
}

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
list<bitset<12>> GenerateHammingForData(list<bitset<8>> data)
{
	list<bitset<12>> dataWithHamming;

	for (list<bitset<8>>::iterator it = data.begin(); it != data.end(); it++)
	{
		dataWithHamming.push_back(CalculateHammingCode(*it));
	}

	return dataWithHamming;
}

////////////////////////////////////////////////////////////////
//	Description:Performs the calculation of 4 hamming parity bits
//
//	Arguments:	[in]bitset<8>: binary representation of a
//							   character
//
//	Return:		[out]bitset<12>: binary character with
//								 hamming parity bits
////////////////////////////////////////////////////////////////
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
void TransmitFrames(list<HammingFrame> frames, int numOfErrors)
{
	list<TransmissionError> errorsIntroduced;
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
		cout << "---------------------Generating Errors---------------------" << endl;
		errorsIntroduced = GenerateTransmissionError(frames, framesWithErrors, &numOfErrors);
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
					retryCount++;
					cout << "--Message Contained Errors and Could Not Be Corrected--" << endl;
					cout << "----------------Retransmitting Message-----------------" << endl;
					realFrame = FrameToString(*dataIt);
					realFrame.copy(transmittedMessage, realFrame.length(), 0);
					transmittedMessage[realFrame.length()] = NULL;
					send(Connection, transmittedMessage, sizeof(transmittedMessage), NULL);
				}
			} while (accepted[0] == 0 && retryCount < 5);
			if (retryCount == 5)
			{
				recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval
				break;
			}
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
			if (retryCount == 5)
			{
				recv(Connection, accepted, sizeof(accepted), NULL); //accept message of approval
				break;
			}
		}
	}
	send(Connection, finalMessage, sizeof(finalMessage), NULL);
}

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
//										  data with errors
//				[in]int*: Number of bits to change.
//
//	Return:		[out]list<TransmissionError>:list of the location
//										   of errors in the frame
////////////////////////////////////////////////////////////////
list<TransmissionError> GenerateTransmissionError(list<HammingFrame> frames,
												  list<HammingFrame> &framesWithErrors,
												  int *numberOfBitsToChange)
{
	list<TransmissionError> errors;
	list<int> errorFrames;
	list<int> errorChars;
	int totalNumOfErrors = 0;

	//Copy the data into data with error
	CopyFrames(frames, framesWithErrors);

	for (list<HammingFrame>::iterator frameIt = framesWithErrors.begin(); frameIt != framesWithErrors.end(); frameIt++)
	{
		list<int> charsUsed;
		bool positionFound  = false;
		int frameLocation = distance(framesWithErrors.begin(), frameIt);
		size_t changePerFrame = *numberOfBitsToChange;
		
		if (changePerFrame > frameIt->data.size() + 3)
		{
			cout << "**Number of Errors Indicated Per Frame Exceeds The Size of Frame " << (frameLocation + 1) << endl;
			changePerFrame = frameIt->data.size() + 3;
			cout << "**Adjusting Number to " << changePerFrame << endl;
		}

		totalNumOfErrors += changePerFrame;

		for (size_t i = 0; i < changePerFrame; i++)
		{
			positionFound = false;
			while (!positionFound)
			{
				TransmissionError error;
				random_device rd;
				int charLocation = abs(int(rd())) % ((frameIt->data).size() + 3);

				for (list<int>::iterator it = charsUsed.begin(); it != charsUsed.end(); it++)
				{
					if (charLocation == *it)
					{
						charLocation = -1;
						break;
					}

				}
				if (charLocation != -1)
				{
					positionFound = true;
					charsUsed.push_back(charLocation);

					int bitLocation;

					//Change in Syn Char
					if (charLocation == 0)
					{
						//Randomly generate a bit number
						bitLocation = abs(int(rd())) % frameIt->synChar1.size();
						frameIt->synChar1.flip(bitLocation);
					}
					//Change in Syn Char
					else if (charLocation == 1)
					{
						//Randomly generate a bit number
						bitLocation = abs(int(rd())) % frameIt->synChar2.size();
						frameIt->synChar2.flip(bitLocation);
					}
					//Change in Control Char
					else if (charLocation == 2)
					{
						//Randomly generate a bit number
						bitLocation = abs(int(rd())) % frameIt->controlChar.size();
						frameIt->controlChar.flip(bitLocation);
					}
					//Change in frame data
					else
					{
						//Randomly generate a bit number
						list<bitset<12>>::iterator bitIt = next(frameIt->data.begin(), charLocation - 3);
						bitLocation = abs(int(rd())) % bitIt->size();
						bitIt->flip(bitLocation);
					}

					error.frameLocation = frameLocation + 1; /////////////everything is 1 indexed
					error.charLocation = charLocation + 1;
					error.bitLocation = bitLocation + 1;

					errors.push_back(error);
				}
			}
		}
	}
	*numberOfBitsToChange = totalNumOfErrors;
	return errors;
}

////////////////////////////////////////////////////////////////
//	Description:Copies the frames to a different list
//
//	**Hamming Overload
//	Arguments:	[in]list<HammingFrame>: Original list
//				[out]list<HammingFrame>&: Copy of the frames
//
////////////////////////////////////////////////////////////////
void CopyFrames(list<HammingFrame> frames, list<HammingFrame> &framesWithErrors)
{
	for (list<HammingFrame>::iterator it = frames.begin(); it != frames.end(); it++)
		framesWithErrors.push_back(*it);
}