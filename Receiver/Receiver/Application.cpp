////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the application layer of the receiver.
//				This layer is in charge of displaying the
//				message received
////////////////////////////////////////////////////////////////
#include "Application.h"
#include <list>
#include <bitset>
#include <iostream>
#include <string>

using namespace std;

/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<char> l)
{
	for (list<char>::iterator it = l.begin(); it != l.end(); it++)
		cout << *it;
}
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<string> l)
{
	for (list<string>::iterator it = l.begin(); it != l.end(); it++)
		cout << *it;
}
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<bitset<7>> l)
{
	for (list<bitset<7>>::iterator it = l.begin(); it != l.end(); it++)
		cout << *it << ' ';
}
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<bitset<8>> l)
{
	for (list<bitset<8>>::iterator it = l.begin(); it != l.end(); it++)
		cout << *it << ' ';
}