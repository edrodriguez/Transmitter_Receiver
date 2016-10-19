#pragma once

void RunPhysicalLayerTests();

void ConvertToBinary_InputA_Returns1000001();

void ConvertToBinary_Inputa_Returns1100001();

void ConvertToBinary_Input0_Returns0110000();

void ConvertToBinary_Input$_Returns0100100();

void IsOddParity_Input1000001_ReturnFalse();

void IsOddParity_Input1100001_ReturnTrue();
