////////////////////////////////////////////////////////////////
//	Description:This file contains the main entry to the
//				transmitter application. It calls fucntions in
//				all layers of the transmitter, in order to get
//				the information from an input file, convert it
//				to the proper binary format, apply CRC or
//				Hamming and transmit the message
////////////////////////////////////////////////////////////////
#include "Application.h"
#include "DataLink.h"
#include "PhysicalLayer.h"
#include "PhysicalLayer_Test.h"
#include "DataLink_Test.h"
#include <iostream>
#include <bitset>
#include <list>
#include <string>

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
			RunDataLinkTests();
			return 0;
		}
		else
		{
			// normal transmitter function
			list<char> infoFromFile;
			list<bitset<8>> binaryData;

			//Read from File
			infoFromFile = ReadFile();
			cout << "-------------------Information From File-------------------" << endl;
			PrintList(infoFromFile);

			//Convert file text to binary
			binaryData = ConvertTextForTransmission(infoFromFile);
			cout << "----------Binary Representation of Each Character----------" << endl;
			PrintList(binaryData);
			cout << endl;

			list<Frame> frames;
			//frame and crc calculation
			cout << "---------------------Frames---------------------" << endl;
			FrameMessage(binaryData, frames);			
			PrintList(frames);

			cout << "----------Transmitter Message Using HDB3 Encoding----------" << endl;
			TransmitFrames(frames);
		}
	}
	else
		cout << "Please Indicate a Mode of Operation" << endl;

	return 0;

}