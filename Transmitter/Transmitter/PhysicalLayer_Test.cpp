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

}

//void ConvertToBinary_InputA_Returns1000001()
//{
//	char input = 'A';
//	bitset<7> expectedOutput(string("1000001"));
//	bitset<7> actualOutput;
//
//	actualOutput = ConvertToBinary(input);
//
//	if (actualOutput == expectedOutput)
//		std::cout << "Test ConvertToBinary_InputA_Returns1000001 PASSED" << endl;
//	else
//		std::cout << "Test ConvertToBinary_InputA_Returns1000001 FAILED" << endl;
//}
//
//void ConvertToBinary_Inputa_Returns1100001()
//{
//	char input = 'a';
//	bitset<7> expectedOutput(string("1100001"));
//	bitset<7> actualOutput;
//
//	actualOutput = ConvertToBinary(input);
//
//	if (actualOutput == expectedOutput)
//		std::cout << "Test ConvertToBinary_Inputa_Returns1100001 PASSED" << endl;
//	else
//		std::cout << "Test ConvertToBinary_Inputa_Returns1100001 FAILED" << endl;
//}
//
//void ConvertToBinary_Input0_Returns0110000()
//{
//	char input = '0';
//	bitset<7> expectedOutput(string("0110000"));
//	bitset<7> actualOutput;
//
//	actualOutput = ConvertToBinary(input);
//
//	if (actualOutput == expectedOutput)
//		std::cout << "Test ConvertToBinary_Input0_Returns0110000 PASSED" << endl;
//	else
//		std::cout << "Test ConvertToBinary_Input0_Returns0110000 FAILED" << endl;
//}
//
//void ConvertToBinary_Input$_Returns0100100()
//{
//	char input = '$';
//	bitset<7> expectedOutput(string("0100100"));
//	bitset<7> actualOutput;
//
//	actualOutput = ConvertToBinary(input);
//
//	if (actualOutput == expectedOutput)
//		std::cout << "Test ConvertToBinary_Input0_Returns0100100 PASSED" << endl;
//	else
//		std::cout << "Test ConvertToBinary_Input0_Returns0100100 FAILED" << endl;
//}
//
//void IsOddParity_Input1000001_ReturnFalse()
//{
//	bitset<7>input(string("1000001"));
//	bool expectedOutput = false;
//	bool actualOutput;
//
//	actualOutput = IsOddParity(input);
//
//	if (actualOutput == expectedOutput)
//		std::cout << "Test IsOddParity_Input1000001_ReturnFalse PASSED" << endl;
//	else
//		std::cout << "Test IsOddParity_Input1000001_ReturnFalse FAILED" << endl;
//}
//
//void IsOddParity_Input1100001_ReturnTrue()
//{
//	//TO-DO Change string() not necessary
//	bitset<7>input(string("1100001"));
//	bool expectedOutput = true;
//	bool actualOutput;
//
//	actualOutput = IsOddParity(input);
//
//	if (actualOutput == expectedOutput)
//		std::cout << "Test IsOddParity_Input1100001_ReturnFalse PASSED" << endl;
//	else
//		std::cout << "Test IsOddParity_Input1100001_ReturnFalse FAILED" << endl;
//}
//
//void GenerateTransmissionError_Input000000000001_ReturnChangeInOneBit()
//{
//	string input = "000000000001";
//	string expectedOutput = "000000000011";
//	string actualOutput;
//
//	//GenerateTransmissionError(input, 1);
//
//	actualOutput = input;
//
//	cout << input << endl;
//}

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