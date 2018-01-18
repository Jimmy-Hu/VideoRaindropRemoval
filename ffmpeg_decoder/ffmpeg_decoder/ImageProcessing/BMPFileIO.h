/*	C語言影像處理函式庫
	Develop by Jimmy HU <s103360021@gmail.com>
	This program is licensed under GNU General Public License v3.
	BMPFileIO.h用於宣告BMP圖片檔案IO相關副程式
 */
#pragma once

//-----include header files, 引入標頭檔-----
#include "BasicFunctions.h"
#include "..\\ImageProcessing\\BMPFileIO.h"								//	引入標頭檔BMPFileIO.h
#include <math.h>														//	引入標頭檔math.h
#include <stdbool.h>													//	引入標頭檔stdbool.h
#include <stdio.h>														//	引入標頭檔stdio.h
#include <stdlib.h>														//	引入標頭檔stdlib.h
#include <string.h>														//	引入標頭檔string.h
#include "..\\unistd.h"													//	引入標頭檔unistd.h
#include "Conversion.h"													//	引入標頭檔Conversion.h

//-----funtion declaration, 副程式宣告區-----

/*  BmpReadFilesize副程式將輸入路徑之圖檔大小讀出並傳回
	副程式輸入為欲讀取大小之圖檔路徑(const char *型態字串，為欲讀取之檔名, FilenameExtension副檔名資訊)
	副程式輸出為圖檔大小(unsigned long型態)
 */
unsigned long BmpReadFilesize(const char *, const bool);				//	宣告BMP圖檔大小(Byte)讀取副程式BmpReadFilesize

/*  BmpReadXSize副程式將輸入路徑之圖檔xsize(寬度)讀出並傳回
	副程式輸入為欲讀取寬度大小之圖檔路徑(const char *型態，為欲讀取之檔名,FilenameExtension副檔名資訊)
	副程式輸出為圖檔寬度(unsigned long型態)
 */
unsigned long BmpReadXSize(const char *, const bool);					//	宣告BMP圖檔xsize(寬度)讀取副程式BmpReadXSize

/*  BmpReadYSize副程式將輸入路徑之圖檔ysize(高度)讀出並傳回
	副程式輸入為欲讀取高度大小之圖檔路徑(const char *型態，為欲讀取之檔名,FilenameExtension副檔名資訊)
	副程式輸出為圖檔高度(unsigned long型態)
 */
unsigned long BmpReadYSize(const char *, const bool);					//	宣告BMP圖檔ysize(高度)讀取副程式BmpReadYSize
/*	BmpRead副程式用於讀取BMP圖檔
	副程式輸入為(欲存放原始圖檔資料之unsigned char型態指標,欲讀取圖檔之寬度,欲讀取圖檔之高度,欲讀取之圖檔路徑,FilenameExtension副檔名資訊)
	副程式輸出：若順利讀取圖檔則傳回0，反之傳回-1
 */
char BmpRead(unsigned char*,const int,const int, const char *, const bool);
//	宣告BmpRead(BMP圖檔讀取)副程式
/*	BmpFileRead副程式整合BmpFillingByteCalc(BMP圖檔填補位元計算)副程式、
	BmpReadFilesize(BMP圖檔檔案大小讀取)副程式、
	BmpReadXSize(BMP圖檔檔案寬度讀取)副程式、
	BmpReadYSize(BMP圖檔檔案高度讀取)副程式與
	BmpRead(BMP圖檔檔案讀取)副程式之功能，用於讀取BMP圖檔
	副程式輸入為(欲讀取之圖檔路徑)，如："test.bmp"
	副程式輸出為BMPIMAGE影像資料結構(該影像資料結構包含檔名(FILENAME)、
	圖像寬度(XSIZE)、圖像高度(YSIZE)、填補位元(FILLINGBYTE)與圖像資料(IMAGE_DATA))
 */
BMPIMAGE BmpFileRead(const char *, const bool);							//	宣告BmpFileRead副程式
/*	BmpWriteV1副程式用於寫入BMP圖檔
	副程式輸入為(欲寫入圖檔之unsigned char型態指標資料,欲寫入圖檔之寬度,欲寫入圖檔之高度,欲寫入之圖檔路徑)
	副程式輸出：若順利寫入圖檔則傳回0，反之傳回-1
 */
int BmpWriteV1(const unsigned char*,const int,const int,const char*); 	//	宣告BmpWriteV1(BMP圖檔寫入)副程式
/*	BmpWriteV2副程式用於寫入BMP圖檔
	本副程式將BmpWriteV1副程式之輸入參數結構化，以BMPIMAGE結構圖檔資料傳入，為使用介面上的改進
	實作上仍呼叫BmpWriteV1副程式進行圖檔寫入
	副程式輸入為(欲寫入之BMPIMAGE結構圖檔資料)
	副程式輸出：若順利寫入圖檔則傳回0，反之傳回-1
 */
int BmpWriteV2(const BMPIMAGE); 										//	宣告BmpWriteV2(BMP圖檔寫入)副程式
