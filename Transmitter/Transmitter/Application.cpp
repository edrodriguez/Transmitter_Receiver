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

/**************************************************************/
/****************************Common****************************/
/**************************************************************/

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
void PrintList(list<Frame> l)
{
	for (list<Frame>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << "Frame: ";
		cout << it->synChar1 << ' ' << it->synChar2 << ' ' << it->controlChar << ' ';
		PrintList(it->data);
	}
	
	cout << endl;
}

/////////////////////////////////////////////////
void CopyListForTransmission(list<char> l, char(&arr)[537])
{
	int i = 0;
	while (!l.empty() && i < sizeof(arr) / sizeof(*arr))
	{
		arr[i] = l.front();
		l.pop_front();
		i++;
	}
}

list<char> TurnFrameIntoList(Frame frame)
{
	list<char> l;

	//syn char1
	for (size_t i = frame.synChar1.size() - 1; i >= 0 && i < frame.synChar1.size(); i--)
	{
		l.push_back(frame.synChar1[i] + 48);
	}

	//syn char2
	for (size_t i = frame.synChar2.size() - 1; i >= 0 && i < frame.synChar2.size(); i--)
	{
		l.push_back(char(frame.synChar2[i]) + 48);
	}

	//syn char
	for (size_t i = frame.controlChar.size() - 1; i >= 0 && i < frame.controlChar.size(); i--)
	{
		l.push_back(char(frame.controlChar[i]) + 48);
	}

	for (list<bitset<8>>::iterator it = frame.data.begin(); it != frame.data.end(); it++)
	{
		for (size_t i = it->size() - 1; i >= 0 && i < it->size(); i--)
			l.push_back(char((*it)[i]) + 48);
	}

	return l;

}
