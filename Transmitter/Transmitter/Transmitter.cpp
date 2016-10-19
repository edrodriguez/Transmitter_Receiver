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

using namespace std;

int main(int argc, char* argv[])
{
	//Check if the application will be run in test mode
	if (argv[1] != nullptr)
	{
		string testMode = argv[1];
		
		for (size_t i = 0; i < testMode.size(); i++)
			testMode[i] = tolower(testMode[i]);

		if (testMode == "test")
		{
			cout << "Running Tests for Transmitter" << endl;
			RunPhysicalLayerTests();
			return 0;
		}
	}
	// normal transmitter function
	else
	{
		list<char> infoFromFile;
		list<bitset<8>> textInBinary;
		list<string> messages;

		infoFromFile = ReadFile();
		cout << "Information From File:" << endl;
		PrintList(infoFromFile);
		textInBinary = ConvertTextForTransmission(infoFromFile);
		messages = Frame(textInBinary);
		TransmitMessages(messages);

		return 0;
	}
}