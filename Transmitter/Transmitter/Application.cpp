////////////////////////////////////////////////////////////////
//	Description:This file contains the definition of functions
//				of the application layer of the transmitter.
//				This layer is in charge of reading the
//				information to be transmitted from a .txt file
////////////////////////////////////////////////////////////////

#include "PhysicalLayer.h"
#include <fstream>
#include <string>
#include <iostream>
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
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<bitset<12>> l)
{
	for (list<bitset<12>>::iterator it = l.begin(); it != l.end(); it++)
		cout << *it << ' ';
	cout << endl;
}
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<HammingFrame> l)
{
	for (list<HammingFrame>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << it->synChar1 << ' ' << it->synChar2 << ' ' << it->controlChar << ' ';
		PrintList(it->data);
	}
	
	cout << endl;
}
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<CRCFrame> l)
{
	for (list<CRCFrame>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << it->synChar1 << ' ' << it->synChar2 << ' ' << it->controlChar << ' ';
		PrintList(it->data);
		cout << ' ';
		for (list<bool>::iterator i = it->CRCCode.begin(); i != it->CRCCode.end(); i++)
			cout << *i;
	}
	cout << endl;
}
/////////////////////
////Auxiliary method for printing list
////////////////////
void PrintList(list<transmissionError> l)
{
	cout << "--------------------" << l.size() << " Errors Introduced--------------------" << endl;

	for (list<transmissionError>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << "*Location of Error:" << endl;
		cout << "\t-Frame: " << it->frameLocation << endl;;
		cout << "\t-Character: " << it->charLocation << endl;;
		cout << "\t-Bit: " << it->bitLocation << endl;;
	}
	cout << endl;
}

///////////////////////////////////////////////////////////description
string FrameToString(HammingFrame frame)
{
	string frameString = frame.synChar1.to_string() + frame.synChar2.to_string() +
		frame.controlChar.to_string();

	string dataString = "";

	for (list<bitset<12>>::iterator it = frame.data.begin(); it != frame.data.end(); it++)
		dataString.append(it->to_string());

	frameString.append(dataString);

	return frameString;
}

string FrameToString(CRCFrame frame)
{
	string frameString = frame.synChar1.to_string() + frame.synChar2.to_string() +
		frame.controlChar.to_string();

	string dataString = "";

	for (list<bitset<8>>::iterator it = frame.data.begin(); it != frame.data.end(); it++)
		dataString.append(it->to_string());

	frameString.append(dataString);

	for (list<bool>::iterator it = frame.CRCCode.begin(); it != frame.CRCCode.end(); it++)
		frameString.append(to_string(*it));

	return frameString;
}