/*	C語言影像處理函式庫
	Develop by Jimmy HU <s103360021@gmail.com>
	This program is licensed under GNU General Public License v3.
	BasicFunctions.h用於宣告基礎副程式
 */
#pragma once
#include "DIPDefine\\DIPDefine.h"										//	include "ImageProcessing\\DIPDefine.h" header file, 引入標頭檔"ImageProcessing\\DIPDefine.h"
#include <math.h>														//	引入標頭檔math.h
#include <stdbool.h>													//	引入標頭檔stdbool.h
#include <stdio.h>														//	引入標頭檔stdio.h
#include <string.h>														//	引入標頭檔string.h
#include "..\\unistd.h"													//	引入標頭檔unistd.h


/*	BmpFillingByteCalc(BMP圖檔資料區填補位元計算)副程式
 */
unsigned char BmpFillingByteCalc(const unsigned int);					//	宣告BmpFillingByteCalc(計算填充位元組大小)副程式
bool FileExistCheck(char *);											//	宣告FileExistCheck(檔案存在檢查)副程式
bool FileReadPermissionCheck(const char *);								//	宣告FileReadPermissionCheck(檔案讀取權限檢查)副程式
bool FileWritePermissionCheck(const char *);							//	宣告FileWritePermissionCheck(檔案寫入權限檢查)副程式
bool FileWrite(const char *,const char *,const char *);					//	宣告FileWrite(檔案寫入)副程式，執行文字檔案寫入
/*	UCharBubbleSort副程式用於排序unsigned char數值陣列
	副程式輸入為(欲排序之unsigned char數值指標(該指標含結束字元),欲排序數值個數,排序方式)
	在此排序方式輸入0為由小至大排序；輸入1為由大至小排序
	副程式輸出為排序完成之unsigned char數值指標
 */
unsigned char *UCharBubbleSort(const unsigned char *,const unsigned long long int,const bool);
int Compare(const void *,const void *);									//	宣告Compare副程式(供qsort排序使用)
/**	CountCharPointStr副程式
	該副程式用於計算字串指標長度；
	第一項參數為欲計算長度之字串；
	第二項參數為是否顯示計算細節之bool變數，若輸入Ture則顯示計算細節，反之則無
	如：CountCharPointStr("ABC", False)，可得長度為3
	如：CountCharPointStr("123456789a", True)，可顯示
	第1個字元為：1
	第2個字元為：2
	第3個字元為：3
	第4個字元為：4
	第5個字元為：5
	第6個字元為：6
	第7個字元為：7
	第8個字元為：8
	第9個字元為：9
	第10個字元為：a	
	並得長度為10
**/
unsigned long long int CountCharPointStr(const char *, const bool);
void Show_char_point_str(const char *);									//	宣告Show_char_point_str(顯示字元指標)副程式
/*	ShowUCharPointStr副程式用於顯示無號字元指標內容
	副程式輸入為(欲顯示內容之無號字元指標,無號字元指標長度)
	副程式輸出為void
 */
void ShowUCharPointStr(const unsigned char *,unsigned long long int);	//	宣告ShowUCharPointStr(顯示無號字元指標)副程式
/*	ShowLongDouble副程式用於顯示long double(長浮點數)數值
	雖long double(長浮點數)可記錄±1.7×10^(-308)~±1.7×10^308範圍內的數值
	由於ShowLongDouble副程式處理與運算上數值寬度為64位元
	因此可處理之InputNumber最大上限為2^64-1
	副程式輸入為欲顯示之long double型態數值
	副程式輸出為void
 */
void ShowLongDouble(const long double InputNumber);						//	宣告ShowLongDouble副程式
/*	InitialIMGArrayTest副程式用於測試InitialIMGArray副程式
	本測試副程式之輸入、輸出皆為void，所有參數皆設計於測試副程式中，
	以觀察使用副程式所需宣告與語法
	副程式輸入為void
	副程式輸出為void
 */