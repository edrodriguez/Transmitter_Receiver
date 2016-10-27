#include "PhysicalLayer_Test.h"
#include "PhysicalLayer.h"
#include <bitset>
#include <iostream>

void RunPhysicalLayerTests()
{
	cout << "Running Physical Layer Tests:" << endl;
	//ConvertToBinary_InputA_Returns1000001();
	//ConvertToBinary_Inputa_Returns1100001();
	//ConvertToBinary_Input0_Returns0110000();
	//ConvertToBinary_Input$_Returns0100100();
	//IsOddParity_Input1000001_ReturnFalse();
	//IsOddParity_Input1100001_ReturnTrue();
	CalculateHammingCode_Input00000000_Return000000000000();
	CalculateHammingCode_Input10010001_Return001000110001();
	CalculateHammingCode_Input11111111_Return111011101111();
	CalculateCRC_Input0000000_Return0000001111000000();
}

void ConvertToBinary_InputA_Returns01000001()
{
	char input = 'A';
	bitset<8> expectedOutput(string("01000001"));
	bitset<8> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_InputA_Returns01000001 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_InputA_Returns01000001 FAILED" << endl;
}

void ConvertToBinary_Inputa_Returns01100001()
{
	char input = 'a';
	bitset<8> expectedOutput(string("01100001"));
	bitset<8> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_Inputa_Returns01100001 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_Inputa_Returns01100001 FAILED" << endl;
}

void ConvertToBinary_Input0_Returns00110000()
{
	char input = '0';
	bitset<8> expectedOutput(string("00110000"));
	bitset<8> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_Input0_Returns00110000 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_Input0_Returns00110000 FAILED" << endl;
}

void ConvertToBinary_Input$_Returns00100100()
{
	char input = '$';
	bitset<8> expectedOutput(string("00100100"));
	bitset<8> actualOutput;

	actualOutput = ConvertToBinary(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test ConvertToBinary_Input$_Returns00100100 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_Input$_Returns00100100 FAILED" << endl;
}

void CalculateHammingCode_Input00000000_Return000000000000()
{
	bitset<8> input("00000000");
	bitset<12> expectedOutput("000000000000");
	bitset<12> actualOutput;
	
	actualOutput = CalculateHammingCode(input);
	
	if (actualOutput == expectedOutput)
		std::cout << "Test CalculateHammingCode_Input00000000_Return000000000000 PASSED" << endl;
	else
		std::cout << "Test CalculateHammingCode_Input00000000_Return000000000000 FAILED" << endl;
}

void CalculateHammingCode_Input10010001_Return001000110001()
{
	bitset<8> input("10010001");
	bitset<12> expectedOutput("001000110001");
	bitset<12> actualOutput;

	actualOutput = CalculateHammingCode(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test CalculateHammingCode_Input10010001_Return001000110001 PASSED" << endl;
	else
		std::cout << "Test CalculateHammingCode_Input10010001_Return001000110001 FAILED" << endl;
}

void CalculateHammingCode_Input11111111_Return111011101111()
{
	bitset<8> input("11111111");
	bitset<12> expectedOutput("111011101111");
	bitset<12> actualOutput;

	actualOutput = CalculateHammingCode(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test CalculateHammingCode_Input11111111_Return111011101111 PASSED" << endl;
	else
		std::cout << "Test CalculateHammingCode_Input11111111_Return111011101111 FAILED" << endl;
}

void CalculateCRC_Input0000000_Return0000001111000000()
{
	CRCFrame input;
	input.data.push_back(bitset<8>("10100000"));
	list<bool> expectedOutput{ 0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0 };

	CalculateCRC(input);

	if (input.CRCCode == expectedOutput)
		std::cout << "Test CalculateCRC_Input0000000_Return0000001111000000 PASSED" << endl;
	else
		std::cout << "Test CalculateCRC_Input0000000_Return0000001111000000 FAILED" << endl;
}