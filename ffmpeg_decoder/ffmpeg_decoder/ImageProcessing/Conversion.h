#pragma once
//-----include header files, 引入標頭檔-----
#include <stdio.h>														//	include "stdio.h" header file, 引入標頭檔stdio.h
#include "DIPDefine\\DIPDefine.h"

//-----funtion declaration, 副程式宣告區-----
unsigned char Clip(int clr);
RGBQUAD ConvertYCrCbToRGB(
	int y,
	int cr,
	int cb
);
