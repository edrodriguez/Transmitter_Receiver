////////////////////////////////////////////////////////////////
//	Description:This file contains the main entry to the
//				transmitter application. It calls fucntions in
//				all layers of the transmitter, in order to get
//				the information from an input file, convert it
//				to the proper binary format and transmit it
////////////////////////////////////////////////////////////////
#include <iostream>
#include <bitset>
#include <list>
#include <string>
#include "Application.h"
#include "DataLink.h"
#include "PhysicalLayer.h"
#include "PhysicalLayer_Test.h"
#include "DataLink_Test.h"

using namespace std;

int main(int argc, char* argv[])
{
	int numOfErrors = 0;

	//Parse the input parameters
	string mode = argv[1];
		
	for (size_t i = 0; i < mode.size(); i++)
		mode[i] = tolower(mode[i]);

	if (mode == "-test")
	{
		cout << "Running Tests for Transmitter" << endl;
		RunPhysicalLayerTests();
		RunDataLinkTests();
		return 0;
	}
	else if (mode == "-errornum")
	{
		numOfErrors = atoi(argv[2]);
	}

	// normal transmitter function
	list<char> infoFromFile;
	list<bitset<8>> textInBinary;
	list<bitset<12>> binaryDataWithHamming;
	list<transmissionError> errorsIntroduced;
	message message;

	//Read from File
	infoFromFile = ReadFile();
	cout << "-------------------Information From File-------------------" << endl;
	PrintList(infoFromFile);

	//Convert file text to binary
	textInBinary = ConvertTextForTransmission(infoFromFile);
	cout << "----------Binary Representation of each Character----------" << endl;
	PrintList(textInBinary);

	//calculate hamming
	binaryDataWithHamming = GenerateHamming(textInBinary);
	cout << "------------Binary Characters with Hamming Code------------" << endl;
	PrintList(binaryDataWithHamming);
	//Frame & CRC
	message.frames = FrameMessage(binaryDataWithHamming);
	cout << "-----------------Framed Messages After CRC----------------" << endl;
	PrintList(message.frames);
	//Generate Transmission errors
	if (numOfErrors > 0)
	{
		errorsIntroduced = GenerateTransmissionError(message, numOfErrors);
		PrintList(errorsIntroduced);
	}
	else
		PrintList(message.frames);
	//Transmit
	//TransmitMessages(message, numOfErrors);

	return 0;

}