////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the application layer of the transmitter.
//				This layer is in charge of reading the
//				information to be transmitted from a .txt file
////////////////////////////////////////////////////////////////

#include <fstream>
#include <string>
#include <iostream> //maybe not necessary if no in out to console
#include <list>
#include <bitset>

using namespace std;

////////////////////////////////////////////////////////////////
//	Description:It reads the file with the information to be
//				transmitted and stores the characters in
//				a list.
//
//	Return:		[out]list<char>: list of characters where
//								 the information will be stored
////////////////////////////////////////////////////////////////
list<char> ReadFile()
{
	ifstream inputFile;
	string line;
	list<char> characters;

	inputFile.open("ProjectFile.txt");

	if (inputFile.good())
	{
		char c;
		while (inputFile.get(c))
			characters.push_back(c);
	}
	else
		cout << "Could not read from ProjectFile.txt";

	return characters;
}

/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<char> l)
{
	for (list<char>::iterator it = l.begin(); it != l.end(); it++)
		cout << *it;
	cout << endl;
}
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<string> l)
{
	for (list<string>::iterator it = l.begin(); it != l.end(); it++)
		cout << *it << endl;
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