#include "DataLink.h"
#include <list>
#include <bitset>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////
//	Description:Removes two syn characters and a control
//				character from the list of bitsets containing
//				the message
//
//	Arguments:	[in]list<bitset<8>>:list of bitsets including
//									the syn and control characters		/////////////////////
//
//	Return:		[out]list<bitset<8>>:list of bitsets without
//									the syn and control characters
////////////////////////////////////////////////////////////////
list<bitset<8>> DeFrame(list<bitset<12>> binaryMessage)
{
	//Erase syn characters and control characters
	binaryMessage.pop_front();
	binaryMessage.pop_front();
	binaryMessage.pop_front();

	list<bitset<8>> deFramedData;
	bitset<8> binaryChar;

	for (list<bitset<12>>::iterator it = binaryMessage.begin(); it != binaryMessage.end(); it++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			binaryChar[i] = (*it)[i];
		}
		for (size_t i = 5; i < 8; i++)
		{
			binaryChar[i - 1] = (*it)[i];
		}
		binaryChar[7] = (*it)[9];

		deFramedData.push_back(binaryChar);
	}

	return deFramedData;
}

////////////////////////////////////////////////////////////////
//	Description:Removes two syn characters and a control
//				character from the list of bitsets containing
//				the message
//
//	Arguments:	[in]list<bitset<8>>:list of bitsets including
//									the syn and control characters		/////////////////////
//
//	Return:		[out]list<bitset<8>>:list of bitsets without
//									the syn and control characters
////////////////////////////////////////////////////////////////
list<bitset<8>> DeFrame(list<bool> binaryMessage)
{
	size_t charSize = 8;
	list<bitset<8>> deFramedData;

	//Erase syn characters and control characters
	for (size_t i = 0; i < 3; i++)
		for(size_t j = 0; j < charSize; j++)
			binaryMessage.pop_front();

	//GetData
	int numChars = 0;
	while (numChars < (binaryMessage.size() - 16) && !binaryMessage.empty())
	{
		bitset<8> binaryChar;

		int numBits = charSize - 1;
		while (numBits >= 0 && !binaryMessage.empty())
		{
			binaryChar[numBits] = binaryMessage.front();
			binaryMessage.pop_front();
			numBits--;
		}
		deFramedData.push_back(binaryChar);
	}

	return deFramedData;
}