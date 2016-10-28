#pragma once

void RunPhysicalLayerTests();

void IsCRCValid_Input010000010000000110000110_ReturnTrue();

void IsCRCValid_Input110000010000000110000110_ReturnFalse();

void CheckHammingParity_Input100010010001_ReturnTrue();

void CheckHammingParity_Input110010010001_ReturnFalse();
