////////////////////////////////////////////////////////////////
//	Description:This file contains the main entry to the
//				receiver application. It calls fucntions in
//				all layers of the receiver in order to get
//				the information from the transmitter, convert it
//				into characters and display it to the console
////////////////////////////////////////////////////////////////

#include "Application.h"
#include "DataLink.h"
#include "PhysicalLayer.h"
#include "PhysicalLayer_Test.h"
#include <iostream>
#include <bitset>

int main(int argc, const char* argv[])
{
	//Check the mode that the application will be running
	if (argv[1] != nullptr)
	{
		string mode = argv[1];

		for (size_t i = 0; i < mode.size(); i++)
			mode[i] = tolower(mode[i]);

		if (mode == "-test")
		{
			cout << "Running Tests for Receiver" << endl;
			RunPhysicalLayerTests();
			return 0;
		}
		else if (mode == "-crc")
			ReceiveCRCMessage();
		else if (mode == "-hamming")
			ReceiveHammingMessage();
		else
			cout << "Invalid Mode of Operation" << endl;
	}
	else
		cout << "Please Indicate a Mode of Operation" << endl;

	return 0;
}