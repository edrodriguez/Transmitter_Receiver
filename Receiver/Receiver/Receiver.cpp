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
	//Check if the application will be run in test mode
	if (argv[1] != nullptr)
	{
		string testMode = argv[1];

		for (size_t i = 0; i < testMode.size(); i++)
			testMode[i] = tolower(testMode[i]);

		if (testMode == "test")
		{
			cout << "Running Tests for Receiver" << endl;
			RunPhysicalLayerTests();
			return 0;
		}
	}
	// normal transmitter function
	else
	{
		ReceiveMessages();
	}
	return 0;
}