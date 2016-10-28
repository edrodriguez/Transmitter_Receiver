#include "PhysicalLayer_Test.h"
#include "PhysicalLayer.h"
#include <bitset>
#include <iostream>

void RunPhysicalLayerTests()
{
	IsCRCValid_Input010000010000000110000110_ReturnTrue();
	IsCRCValid_Input110000010000000110000110_ReturnFalse();
	CheckHammingParity_Input100010010001_ReturnTrue();
	CheckHammingParity_Input110010010001_ReturnFalse();
}

void IsCRCValid_Input010000010000000110000110_ReturnTrue()
{
	list<bool> input{ 0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0 };
	bool expectedOutput = true;

	bool actualOutput = IsCRCValid(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test IsCRCValid_Input010000010000000110000110_ReturnTrue PASSED" << endl;
	else
		std::cout << "Test IsCRCValid_Input010000010000000110000110_ReturnTrue FAILED" << endl;
}

void IsCRCValid_Input110000010000000110000110_ReturnFalse()
{
	list<bool> input{ 1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0 };
	bool expectedOutput = false;

	bool actualOutput = IsCRCValid(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test IsCRCValid_Input110000010000000110000110_ReturnFalse PASSED" << endl;
	else
		std::cout << "Test IsCRCValid_Input110000010000000110000110_ReturnFalse FAILED" << endl;
}

void CheckHammingParity_Input100010010001_ReturnTrue()
{
	bitset<12> input("100010010001");
	HammingErrorDetection expectedOutput, actualOutput;

	expectedOutput.isHammingCorrect = true;
	actualOutput = CheckHammingParity(input);

	if (actualOutput.isHammingCorrect == expectedOutput.isHammingCorrect)
		std::cout << "Test CheckHammingParity_Input100010010001_ReturnTrue PASSED" << endl;
	else
		std::cout << "Test CheckHammingParity_Input100010010001_ReturnTrue FAILED" << endl;
}

void CheckHammingParity_Input110010010001_ReturnFalse()
{
	bitset<12> input("110010010001");
	HammingErrorDetection expectedOutput, actualOutput;

	expectedOutput.isHammingCorrect = false;
	expectedOutput.errorBit = 2;
	actualOutput = CheckHammingParity(input);

	if (actualOutput.isHammingCorrect == expectedOutput.isHammingCorrect && actualOutput.errorBit == expectedOutput.errorBit)
		std::cout << "Test CheckHammingParity_Input110010010001_ReturnFalse PASSED" << endl;
	else
		std::cout << "Test CheckHammingParity_Input110010010001_ReturnFalse FAILED" << endl;
}
