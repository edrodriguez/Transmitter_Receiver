#include "PhysicalLayer_Test.h"
#include "PhysicalLayer.h"
#include <bitset>
#include <iostream>

void RunPhysicalLayerTests()
{
	ConvertBinaryToChar_Input10000110_ReturnsC();
	CheckParity_Input11111010_EvenParityAndParity0_ReturnsTrue();
	CheckParity_Input11001001_OddParityAndParity1_ReturnsTrue();
	CheckParity_Input11001000_OddParityAndParity0_ReturnsFalse();
}

void ConvertBinaryToChar_Input10000110_ReturnsC()
{
	bitset<8> input(string("10000110"));
	char expectedOutput = 'C';
	char actualOutput;

	actualOutput = ConvertBinaryToChar(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertBinaryToChar_Input10000110_Returnsb PASSED" << endl;
	else
		std::cout << "Test ConvertBinaryToChar_Input10000110_Returnsb FAILED" << endl;
}

void CheckParity_Input11111010_EvenParityAndParity0_ReturnsTrue()
{/*
	bitset<8> input(std::string("11111010"));
	bool expectedOutput = true;
	bool actualOutput;

	actualOutput = CheckParity(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test CheckParity_Input11111010_EvenParity_ReturnsTrue PASSED" << endl;
	else
		std::cout << "Test CheckParity_Input11111010_EvenParity_ReturnsTrue FAILED" << endl;
	*/
}

void CheckParity_Input11001001_OddParityAndParity1_ReturnsTrue()
{
	/*
	bitset<8> input(std::string("11001001"));
	bool expectedOutput = true;
	bool actualOutput;

	actualOutput = CheckParity(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test CheckParity_Input11001001_OddParityAndParity1_ReturnsTrue PASSED" << endl;
	else
		std::cout << "Test CheckParity_Input11001001_OddParityAndParity1_ReturnsTrue FAILED" << endl;
		*/
}

void CheckParity_Input11001000_OddParityAndParity0_ReturnsFalse()
{
	//bitset<8> input(std::string("11001000"));
	//bool expectedOutput = false;
	//bool actualOutput;

	//actualOutput = CheckParity(input);

	//if (actualOutput == expectedOutput)
	//	std::cout << "Test CheckParity_Input11001000_OddParityAndParity0_ReturnsFalse PASSED" << endl;
	//else
	//	std::cout << "Test CheckParity_Input11001000_OddParityAndParity0_ReturnsFalse FAILED" << endl;
}
