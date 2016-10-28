#pragma once

void RunPhysicalLayerTests();

void ConvertToBinary_InputA_Returns01000001();

void ConvertToBinary_Inputa_Returns01100001();

void ConvertToBinary_Input0_Returns00110000();

void ConvertToBinary_Input$_Returns00100100();

void CalculateCRC_Input01000001_Return0000000110000110();

void CalculateCRC_InputFullFrameWith01000001Data_Returns0011111010111000();

void CalculateHammingCode_Input00000000_Return000000000000();

void CalculateHammingCode_Input10010001_Return001000110001();

void CalculateHammingCode_Input11111111_Return111011101111();

