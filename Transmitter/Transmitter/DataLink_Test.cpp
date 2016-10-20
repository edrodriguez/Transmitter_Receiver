#include "DataLink_Test.h"
#include "DataLink.h"
#include <bitset>
#include <iostream>

void RunDataLinkTests()
{
	cout << "Running Data Link Tests:" << endl;
	CountCharsInBlock_Input1ElementList_Return1();
	CountCharsInBlock_Input64ElementList_Return64();
	IncludeControlChar_Input1ElementList_Return00000011();
	IncludeControlChar_Input64ElementList_Return10000001();
}

void CountCharsInBlock_Input1ElementList_Return1()
{
	list<bitset<12>> input;
	bitset<12> testBitset("101010101010");
	int expectedOutput = 1;
	int actualOutput;

	//Add 1 bitset to the list
	input.push_back(testBitset);

	actualOutput = CountCharsInBlock(input);

	if (actualOutput == expectedOutput)
		std::cout << "CountCharsInBlock_Input1ElementList_Return1 PASSED" << endl;
	else
		std::cout << "CountCharsInBlock_Input1ElementList_Return1 FAILED" << endl;

}

void CountCharsInBlock_Input64ElementList_Return64()
{
	list<bitset<12>> input;
	bitset<12> testBitset("101010101010");
	int expectedOutput = 64;
	int actualOutput;

	//Add 64 bitsets to the list
	for (int i = 0; i < 64; i++)
		input.push_back(testBitset);

	actualOutput = CountCharsInBlock(input);

	if (actualOutput == expectedOutput)
		std::cout << "CountCharsInBlock_Input64ElementList_Return64 PASSED" << endl;
	else
		std::cout << "CountCharsInBlock_Input64ElementList_Return64 FAILED" << endl;

}

void IncludeControlChar_Input1ElementList_Return00000011()
{
	list<bitset<12>> input;
	bitset<12> testBitset("101010101010");
	bitset<8> expectedOutput("00000011");
	bitset<8> actualOutput;

	input.push_back(testBitset);

	actualOutput = IncludeControlChar(input);

	if (actualOutput == expectedOutput)
		std::cout << "IncludeControlChar_Input1ElementList_Return00000011 PASSED" << endl;
	else
		std::cout << "IncludeControlChar_Input1ElementList_Return00000011 FAILED" << endl;

}

void IncludeControlChar_Input64ElementList_Return10000001()
{
	list<bitset<12>> input;
	bitset<12> testBitset("101010101010");
	bitset<8> expectedOutput("10000001");
	bitset<8> actualOutput;

	//Add 64 bitsets to the list
	for (int i = 0; i < 64; i++)
		input.push_back(testBitset);

	actualOutput = IncludeControlChar(input);

	if (actualOutput == expectedOutput)
		std::cout << "IncludeControlChar_Input64ElementList_Return10000001 PASSED" << endl;
	else
		std::cout << "IncludeControlChar_Input64ElementList_Return10000001 FAILED" << endl;

}