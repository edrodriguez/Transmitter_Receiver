#pragma once

void RunPhysicalLayerTests();

void ConvertBinaryToChar_Input10000110_ReturnsC();

void CheckParity_Input11111010_EvenParityAndParity0_ReturnsTrue();

void CheckParity_Input11001001_OddParityAndParity1_ReturnsTrue();

void CheckParity_Input11001000_OddParityAndParity0_ReturnsFalse();