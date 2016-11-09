#pragma once

void RunPhysicalLayerTests();

void ConvertToBinary_InputA_Returns1000001();

void ConvertToBinary_Inputa_Returns1100001();

void ConvertToBinary_Input0_Returns0110000();

void ConvertToBinary_Input$_Returns0100100();

void BipolarAMI_Input1Element10101010_ReturnsP0N0P0N0();

void BipolarAMI_Input2Element10101010_ReturnsP0N0P0N0P0N0P0N0();

void HDB3_Input101000000000010_ReturnP0N000NP00P00N0();


