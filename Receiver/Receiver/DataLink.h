#pragma once
////////////////////////////////////////////////////////////////
//	Description:This file contains the declaration of functions
//				of the data link layer of the receiver.
//				This layer is in charge of deframing the message
//				received
////////////////////////////////////////////////////////////////
#include <list>
#include <bitset>

using namespace std;

////////////////////////////////////////////////////////////////
//	Description:Removes two syn characters and a control
//				character from the list of bitsets containing
//				the message
//
//	Arguments:	[in]list<bitset<8>>:list of bitsets including
//									the syn and control characters
//
//	Return:		[out]list<bitset<8>>:list of bitsets without
//									the syn and control characters
////////////////////////////////////////////////////////////////
list<bitset<8>> DeFrame(list<bitset<12>> binaryMessage);

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
list<bitset<8>> DeFrame(list<bool> binaryMessage);
