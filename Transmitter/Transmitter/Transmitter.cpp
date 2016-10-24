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
	//Run the appropriate mode of operation
	if (argv[1] != nullptr)
	{
		string mode = argv[1];

		for (size_t i = 0; i < mode.size(); i++)
			mode[i] = tolower(mode[i]);

		if (mode == "-test")
		{
			cout << "Running Tests for Transmitter" << endl;
			RunPhysicalLayerTests();
			//RunDataLinkTests();
			return 0;
		}
		else
		{
			// normal transmitter function
			list<char> infoFromFile;
			list<bitset<8>> binaryData;
			int numOfErrors = 0;

			//Get number of errors if any
			if (argv[2] != nullptr)
			{
				string errorMode = argv[2];
				if (errorMode == "-errornum")
					if (argv[3] != nullptr)
						numOfErrors = atoi(argv[3]);
			}

			//Read from File
			infoFromFile = ReadFile();
			cout << "-------------------Information From File-------------------" << endl;
			PrintList(infoFromFile);

			//Convert file text to binary
			binaryData = ConvertTextForTransmission(infoFromFile);
			cout << "----------Binary Representation of each Character----------" << endl;
			PrintList(binaryData);

			if (mode == "-crc")
			{
				list<CRCFrame> CRCFrames;
				//frame and crc calculation
				FrameMessage(binaryData, CRCFrames);

				cout << "---------------------Frames With CRC---------------------" << endl;
				//PrintList(CRCFrames);

				//Reverse characters
				//TransmitMessages(message, mode, numOfErrors);
			}
			else if (mode == "-hamming")
			{
				list<HammingFrame> HammingFrames;
				//frame and hamming calculation
				FrameMessage(binaryData, HammingFrames);

				cout << "-------------------Frames With Hamming-------------------" << endl;
				PrintList(HammingFrames);

				//Reverse characters
				TransmitFrames(HammingFrames, numOfErrors);
			}
		}
	}
	else
		cout << "Please Indicate a Mode of Operation" << endl;

	return 0;

}