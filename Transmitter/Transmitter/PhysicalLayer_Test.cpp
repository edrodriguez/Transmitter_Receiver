#include "PhysicalLayer_Test.h"
#include "PhysicalLayer.h"
#include <bitset>
#include <iostream>

void RunPhysicalLayerTests()
{
	cout << "Running Physical Layer Tests:" << endl;
	ConvertToBinary_InputA_Returns1000001();
	ConvertToBinary_Inputa_Returns1100001();
	ConvertToBinary_Input0_Returns0110000();
	ConvertToBinary_Input$_Returns0100100();
	BipolarAMI_Input1Element10101010_ReturnsP0N0P0N0();
	BipolarAMI_Input2Element10101010_ReturnsP0N0P0N0P0N0P0N0();
	HDB3_InputP0N0000000000P0_ReturnP0N000NP00P00N0();
	HDB3_InputPN00000000PN00000P0_ReturnPN000NP00PNPN00N0P0();
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
		std::cout << "Test ConvertToBinary_Input$_Returns0100100 PASSED" << endl;
	else
		std::cout << "Test ConvertToBinary_Input$_Returns0100100 FAILED" << endl;
}

void BipolarAMI_Input1Element10101010_ReturnsP0N0P0N0()
{
	list<char> input;
	list<char> expectedOutput;
	list<char> actualOutput;

	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');

	expectedOutput.push_back('+');
	expectedOutput.push_back('0');
	expectedOutput.push_back('-');
	expectedOutput.push_back('0');
	expectedOutput.push_back('+');
	expectedOutput.push_back('0');
	expectedOutput.push_back('-');
	expectedOutput.push_back('0');

	actualOutput = BipolarAMI(input, false);

	if (actualOutput == expectedOutput)
		std::cout << "Test BipolarAMI_Input1Element10101010_ReturnsP0N0P0N0 PASSED" << endl;
	else
		std::cout << "Test BipolarAMI_Input1Element10101010_ReturnsP0N0P0N0 FAILED" << endl;
}

void BipolarAMI_Input2Element10101010_ReturnsP0N0P0N0P0N0P0N0()
{
	list<char> input;
	list<char> expectedOutput;
	list<char> actualOutput;

	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');
	input.push_back('1');
	input.push_back('0');

	expectedOutput.push_back('+');
	expectedOutput.push_back('0');
	expectedOutput.push_back('-');
	expectedOutput.push_back('0');
	expectedOutput.push_back('+');
	expectedOutput.push_back('0');
	expectedOutput.push_back('-');
	expectedOutput.push_back('0');
	expectedOutput.push_back('+');
	expectedOutput.push_back('0');
	expectedOutput.push_back('-');
	expectedOutput.push_back('0');
	expectedOutput.push_back('+');
	expectedOutput.push_back('0');
	expectedOutput.push_back('-');
	expectedOutput.push_back('0');

	actualOutput = BipolarAMI(input, false);

	if (actualOutput == expectedOutput)
		std::cout << "Test BipolarAMI_Input2Element10101010_ReturnsP0N0P0N0P0N0P0N0 PASSED" << endl;
	else
		std::cout << "Test BipolarAMI_Input2Element10101010_ReturnsP0N0P0N0P0N0P0N0 FAILED" << endl;
}

void HDB3_InputP0N0000000000P0_ReturnP0N000NP00P00N0()
{
	list<char> input = { '+','0','-','0','0','0','0','0','0','0','0','0','0','+','0' };
	list<char> expectedOutput = { '+','0','-','0','0','0','-','+','0','0','+','0','0','-','0' };
	list<char> actualOutput;

	actualOutput = HDB3(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test HDB3_InputP0N0000000000P0_ReturnP0N000NP00P00N0 PASSED" << endl;
	else
		std::cout << "Test HDB3_InputP0N0000000000P0_ReturnP0N000NP00P00N0 FAILED" << endl;
}

void HDB3_InputPN00000000PN00000P0_ReturnPN000NP00PNPN00N0P0()
{
	list<char> input = { '+','-','0','0','0','0','0','0','0','0','+','-','0','0', '0', '0', '0', '+', '0'};
	list<char> expectedOutput = { '+','-','0','0','0','-','+','0','0','+','-','+','-','0', '0', '-', '0', '+', '0' };
	list<char> actualOutput;

	actualOutput = HDB3(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test HDB3_InputPN00000000PN00000P0_ReturnPN000NP00PNPN00N0P0 PASSED" << endl;
	else
		std::cout << "Test HDB3_InputPN00000000PN00000P0_ReturnPN000NP00PNPN00N0P0 FAILED" << endl;
}	