#include "PhysicalLayer_Test.h"
#include "PhysicalLayer.h"
#include <bitset>
#include <iostream>

void RunPhysicalLayerTests()
{
	ConvertToBinary_InputA_Returns1000001();
	ConvertToBinary_Inputa_Returns1100001();
	ConvertToBinary_Input0_Returns0110000();
	ConvertToBinary_Input$_Returns0100100();
	IsOddParity_Input1000001_ReturnFalse();
	IsOddParity_Input1100001_ReturnTrue();
}

void ConvertToBinary_InputA_Returns1000001()
{
	char input = 'A';
	bitset<7> expectedOutput(string("1000001"));
	bitset<7> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_InputA_Returns1000001 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_InputA_Returns1000001 FAILED" << endl;
}

void ConvertToBinary_Inputa_Returns1100001()
{
	char input = 'a';
	bitset<7> expectedOutput(string("1100001"));
	bitset<7> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_Inputa_Returns1100001 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_Inputa_Returns1100001 FAILED" << endl;
}

void ConvertToBinary_Input0_Returns0110000()
{
	char input = '0';
	bitset<7> expectedOutput(string("0110000"));
	bitset<7> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_Input0_Returns0110000 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_Input0_Returns0110000 FAILED" << endl;
}

void ConvertToBinary_Input$_Returns0100100()
{
	char input = '$';
	bitset<7> expectedOutput(string("0100100"));
	bitset<7> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_Input0_Returns0100100 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_Input0_Returns0100100 FAILED" << endl;
}

void IsOddParity_Input1000001_ReturnFalse()
{
	bitset<7>input(string("1000001"));
	bool expectedOutput = false;
	bool actualOutput;

	actualOutput = IsOddParity(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test IsOddParity_Input1000001_ReturnFalse PASSED" << endl;
	else
		std::cout << "Test IsOddParity_Input1000001_ReturnFalse FAILED" << endl;
}

void IsOddParity_Input1100001_ReturnTrue()
{
	bitset<7>input(string("1100001"));
	bool expectedOutput = true;
	bool actualOutput;

	actualOutput = IsOddParity(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test IsOddParity_Input1100001_ReturnFalse PASSED" << endl;
	else
		std::cout << "Test IsOddParity_Input1100001_ReturnFalse FAILED" << endl;
}