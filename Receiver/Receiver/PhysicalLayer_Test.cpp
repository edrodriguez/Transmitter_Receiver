#include "PhysicalLayer_Test.h"
#include "PhysicalLayer.h"
#include <bitset>
#include <iostream>
#include <vector>

void RunPhysicalLayerTests()
{
	DecodeHDB3_InputP0N000NP00P00N0_Return101000000000010();
	DecodeHDB3_InputP0N000N0PNP00PNPN00N00_Return1010000011000011000000();
}

void DecodeHDB3_InputP0N000NP00P00N0_Return101000000000010()
{
	vector<char> input{'+', '0', '-', '0', '0', '0', '-', '+', '0', '0', '+', '0', '0', '-', '0' };
	list<char> expectedOutput{ '1', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0' };
	list<char> actualOutput;

	actualOutput = DecodeHDB3(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test DecodeHDB3_InputP0N000NP00P00N0_Return101000000000010 PASSED" << endl;
	else
		std::cout << "Test DecodeHDB3_InputP0N000NP00P00N0_Return101000000000010 FAILED" << endl;
}

void DecodeHDB3_InputP0N000N0PNP00PNPN00N00_Return1010000011000011000000()
{
	vector<char> input       { '+', '0', '-', '0', '0', '0', '-', '0', '+', '-', '+', '0', '0', '+', '-', '+', '-', '0', '0', '-', '0', '0' };
	list<char> expectedOutput{ '1', '0', '1', '0', '0', '0', '0', '0', '1', '1', '0', '0', '0', '0', '1', '1', '0', '0', '0', '0', '0', '0' };
	list<char> actualOutput;

	actualOutput = DecodeHDB3(input);

	if (actualOutput == expectedOutput)
		std::cout << "Test DecodeHDB3_InputP0N000N0PNP00PNPN00N00_Return1010000011000011000000 PASSED" << endl;
	else
		std::cout << "Test DecodeHDB3_InputP0N000N0PNP00PNPN00N00_Return1010000011000011000000 FAILED" << endl;
}
