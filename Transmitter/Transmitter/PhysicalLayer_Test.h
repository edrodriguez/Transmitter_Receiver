#pragma once

void RunPhysicalLayerTests();

void ConvertToBinary_InputA_Returns1000001();

void ConvertToBinary_Inputa_Returns1100001();

void ConvertToBinary_Input0_Returns0110000();

void ConvertToBinary_Input$_Returns0100100();

void BipolarAMI_Input1Element10101010_ReturnsP0N0P0N0();

void BipolarAMI_Input2Element10101010_ReturnsP0N0P0N0P0N0P0N0();

void HDB3_InputP0N0000000000P0_ReturnP0N000NP00P00N0();

void HDB3_InputPN00000000PN00000P0_ReturnPN000NP00PNPN00N0P0();


