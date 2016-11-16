#include "DataLink.h"
#include <list>
#include <bitset>
#include <string>

using namespace std;

//////////////////////////////////////////////////////////////// 
//  Description:Removes two syn characters and a control 
//        character from the list of bitsets containing 
//        the message 
// 
//  Arguments:  [in]list<bitset<8>>:list of bitsets including 
//                  the syn and control characters 
// 
//  Return:    [out]list<bitset<8>>:list of bitsets without 
//                  the syn and control characters 
////////////////////////////////////////////////////////////////
list<bitset<8>> DeFrame(list<bitset<8>> binaryMessage)
{
	//Erase syn characters and control characters 
	binaryMessage.pop_front();
	binaryMessage.pop_front();
	binaryMessage.pop_front();

	return binaryMessage;
}