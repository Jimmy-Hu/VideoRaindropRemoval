/*	C語言影像處理函式庫
	Develop by Jimmy HU <s103360021@gmail.com>
	This program is licensed under GNU General Public License v3.
	BasicFunctions.cpp用於實作基礎副程式
*/
#include "BasicFunctions.h"

unsigned char BmpFillingByteCalc(const unsigned int xsize)				//	BmpFillingByteCalc(BMP圖檔填補位元計算)副程式
{																		//	進入BmpFillingByteCalc副程式
	unsigned char FillingByte;											//	宣告FillingByte記錄填補位元組計算結果
	FillingByte = ( xsize % 4);											//	計算填補位元組數量
	return FillingByte;													//	傳回計算結果
}																		//	結束BmpFillingByteCalc副程式
bool FileExistCheck(char *file_name)									//	FileExistCheck(檔案存在檢查)副程式
{																		//	進入FileExistCheck(檔案存在檢查)副程式
	if( access( file_name, F_OK ) != -1 )								//	若檔案存在
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不存在
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束FileExistCheck(檔案存在檢查)副程式
bool FileReadPermissionCheck(const char *file_name)						//	FileReadPermissionCheck(檔案讀取權限檢查)副程式
{																		//	進入FileReadPermissionCheck(檔案讀取權限檢查)副程式
	if( access( file_name, R_OK ) != -1 )								//	若檔案可讀取
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不可讀取
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束FileReadPermissionCheck(檔案讀取權限檢查)副程式
bool FileWritePermissionCheck(const char *file_name)					//	FileWritePermissionCheck(檔案寫入權限檢查)副程式
{																		//	進入FileWritePermissionCheck(檔案寫入權限檢查)副程式
	if( access( file_name, W_OK ) != -1 )								//	若檔案可讀取
	{																	//	進入if敘述
    	return True;													//	傳回True
	} 																	//	結束if敘述
	else 																//	若檔案不可讀取
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束FileWritePermissionCheck(檔案寫入權限檢查)副程式

bool FileWrite(const char *file_name,const char *input_str,const char *mode)
//	FileWrite(檔案寫入)副程式，執行文字檔案寫入
//	file_name為欲寫入檔案檔名
// 	input_str為欲寫入檔案之指標形式字串資料
//	mode為寫入檔案模式設定，可傳入"w"或"a"，"w"為新增/覆蓋模式，"a"為擴充模式
//	回傳結果：若檔案寫入成功回傳True，若寫入失敗回傳False
{																		//	進入FileWrite(檔案寫入)副程式
	FILE *file_point;													//	宣告一file_point指標，控制檔案讀寫
	if (strcmp(mode, "w") != 0 && strcmp(mode, "a") != 0 && strcmp(mode, "w+") != 0 && strcmp(mode, "a+") != 0)
	//	若mode參數不為"w"亦不為"a"亦不為"w+"亦不為"a+"
	{																	//	進入if敘述
		printf("FileWrite:mode ERROR!\n");								//	顯示"FileWrite:mode ERROR!"並換行
		return False;													//	回傳False並結束副程式返回
	}																	//	結束if敘述
	if( strcmp(mode, "a") == 0 || strcmp(mode, "a+") == 0 )				//	若mode參數為"a"或"a+"(擴充模式)
	{																	//	進入if敘述
		if (FileWritePermissionCheck(file_name) == False)				//	若檔案無法寫入
		{																//	進入if敘述
			printf("FileWrite:permission ERROR!\n");					//	顯示"FileWrite:permission ERROR!"並換行
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
	}																	//	結束if敘述
	file_point = fopen(file_name, mode);								//	以mode模式打開檔案
	//	fprintf或fputs語法二擇一使用
	fprintf(file_point, input_str);										//	以fprintf語法寫入檔案
	//fputs(input_str, file_point);										//	以fputs語法寫入檔案
	fclose(file_point); 												//	關閉檔案
	return True;														//	回傳True並結束副程式返回
}																		//	結束FileWrite(檔案寫入)副程式

unsigned char *UCharBubbleSort(	const unsigned char *InputData,
								const unsigned long long int InputDataNum,
								const bool Mode)
//	UCharBubbleSort副程式
{																		//	進入UCharBubbleSort副程式
	unsigned char *OutputData;											//	宣告OutputData記錄輸出資料
	OutputData = (unsigned char*)malloc( InputDataNum * sizeof(unsigned char) );
	//	配置OutputData輸出資料指標記憶體空間
	unsigned long long int LoopNumber1,LoopNumber2;						//	宣告LoopNumber1與LoopNumber2，用於迴圈執行次數計算
	for(LoopNumber1=0;LoopNumber1 < InputDataNum;LoopNumber1++)			//	以迴圈依序
	{																	//	進入for迴圈
		OutputData[LoopNumber1] = InputData[LoopNumber1];				//	複製資料至輸出指標記憶體空間
	}																	//	結束for迴圈
	for(LoopNumber2 = 1;LoopNumber2 < InputDataNum;LoopNumber2++)		//	以迴圈依序比較數值
	{																	//	進入for迴圈
		for(LoopNumber1=0;LoopNumber1 < InputDataNum - LoopNumber2;LoopNumber1++)
		//	以迴圈依序比較數值
		{																//	進入for迴圈
			if( Mode == 0 )												//	若模式為由小排至大
			{															//	進入if敘述
				if(OutputData[LoopNumber1] > OutputData[LoopNumber1 + 1])
				//	若較大的數值在前
				{														//	進入if敘述
					unsigned char TempNumber;							//	宣告TempNumber暫存變數(用於數值交換)
					TempNumber = OutputData[LoopNumber1];				//	數值交換
					OutputData[LoopNumber1] = OutputData[LoopNumber1 + 1];
					//	數值交換
					OutputData[LoopNumber1 + 1] = TempNumber;			//	數值交換
				}														//	結束if敘述
			}															//	結束if敘述
			else if( Mode == 1 )										//	若模式為由大排至小
			{															//	進入else if敘述
				if(OutputData[LoopNumber1] < OutputData[LoopNumber1 + 1])
				//	若較小的數值在前
				{														//	進入if敘述
					unsigned char TempNumber;							//	宣告TempNumber暫存變數(用於數值交換)
					TempNumber = OutputData[LoopNumber1];				//	數值交換
					OutputData[LoopNumber1] = OutputData[LoopNumber1 + 1];
					//	數值交換
					OutputData[LoopNumber1 + 1] = TempNumber;			//	數值交換
				}														//	結束if敘述
			}															//	結束else if敘述
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳輸出資料
}																		//	結束UCharBubbleSort副程式

int Compare(const void *data1,const void *data2)						//	Compare副程式
{																		//	進入Compare副程式
	int *ptr1 = (int*)data1;											//	宣告*ptr1整數指標變數
	int *ptr2 = (int*)data2;											//	宣告*ptr2整數指標變數
	if(*ptr1 < *ptr2)													//	若*ptr1數值較小
	{																	//	進入if敘述
		return -1;														//	傳回-1，並結束副程式
	}																	//	結束if敘述
	else if(*ptr1 > *ptr2)												//	若*ptr1數值較大
	{																	//	進入else if敘述
		return 1;														//	傳回1，並結束副程式
	}																	//	結束else if敘述
	else																//	若*ptr1與*ptr2數值相等
	{																	//	進入else敘述
		return 0;														//	傳回0，並結束副程式
	}																	//	結束else敘述	
}																		//	結束Compare副程式
unsigned long long int CountCharPointStr(const char *Input_string, const bool Detail)	
//	CountCharPointStr(計算字元指標長度)副程式，計算字元指標(字串)長度
{																		//	進入CountCharPointStr副程式
	unsigned long long int count_num = 0;								//	宣告count_num區域變數，並設定初始值為0
	while (Input_string[count_num] != '\0')								//	若非字串結尾
	{																	//	進入while敘述
		if(Detail == True)												//	若Detail參數為True
		{																//	進入if敘述
			printf("第%d個字元為：%c\n",count_num + 1,Input_string[count_num]);
			//	顯示計數細節
		}																//	結束if敘述
		count_num++;													//	累加count_num變數
    }																	//	結束while敘述
    return count_num;													//	回傳count_num(字元指標長度)變數
}																		//	結束CountCharPointStr副程式
void Show_char_point_str(const char *Input_string)						//	Show_char_point_str(顯示字元指標)副程式
/*	顯示字串內容
	引用副程式：
		CountCharPointStr(計算字元指標長度)副程式
*/
{																		//	進入Show_char_point_str副程式
	unsigned long long int loop_num = 0;								//	宣告loop_num區域變數供迴圈使用，並設定初始值為0
	for(loop_num = 0; loop_num < CountCharPointStr(Input_string, False); loop_num++)
	{																	//	進入for迴圈
		printf("%c",Input_string[loop_num]);							//	依序顯示字元
	}																	//	結束for迴圈
}																		//	結束Show_char_point_str副程式
void ShowUCharPointStr(const unsigned char *InputData,unsigned long long int InputDataLen)
//	ShowUCharPointStr(顯示無號字元指標)副程式
{																		//	進入ShowUCharPointStr副程式
	unsigned long long int LoopNumber = 0;								//	宣告LoopNumber變數，用於迴圈計數
	for(LoopNumber=0;LoopNumber<InputDataLen;LoopNumber++)				//	以for迴圈依序顯示每項元素
	{																	//	進入for迴圈
		printf("%u,",InputData[LoopNumber]);							//	顯示無號字元指標內容
	}																	//	結束for迴圈
}																		//	結束ShowUCharPointStr副程式
void ShowLongDouble(const long double InputNumber)						//	ShowLongDouble副程式
{																		//	進入ShowLongDouble副程式
	int TimesNumber = 308;												//	宣告TimesNumber為整數(int)變數，並初始化為308
	long double DisplayNumber;											//	宣告DisplayNumber變數，用於記錄顯示數值
	//***處理正負號***
	if(InputNumber < 0)													//	若InputNumber輸入數值為負數
	{																	//	進入if敘述
		DisplayNumber = (long double)0.0 - InputNumber;					//	填入DisplayNumber數值
		printf("-");													//	顯示負號"-"
	}																	//	結束if敘述
	else																//	若InputNumber輸入數值不為負數
	{																	//	進入else敘述
		DisplayNumber = InputNumber;									//	填入DisplayNumber數值
	}																	//	結束else敘述
	//***去除前導0***
	char DisplayChar='\0';												//	宣告DisplayChar為字元(char)型態，並初始化為空字元
	DisplayChar = floor(DisplayNumber/(long double)pow(10,TimesNumber)) - pow(10,1) * floor(DisplayNumber/(long double)pow(10,(TimesNumber + 1)));
	//	取出顯示數值
	while((DisplayChar == 0) && (TimesNumber > 0))						//	若數值為0且TimesNumber(次方)大於0(不為個位數)
	{																	//	進入while敘述
		TimesNumber = TimesNumber - 1;									//	遞減TimesNumber變數
		DisplayChar = floor(DisplayNumber/(long double)pow(10,TimesNumber)) - pow(10,1) * floor(DisplayNumber/(long double)pow(10,(TimesNumber + 1)));
		//	取出顯示數值
	}																	//	結束while敘述
	//***顯示數值***
	int DisplayStartTimes = TimesNumber;								//	宣告DisplayStartTimes為整數(int)變數，用以記錄起始顯示數字次方
	int DisplayDigit;													//	宣告DisplayDigit為整數(int)變數，用以記錄顯示位數
	if(DisplayStartTimes > 14)											//	若DisplayStartTimes(起始顯示數字次方)大於14(即欲顯示數值InputNumber大於10^14)
	{																	//	進入if敘述
		DisplayDigit = DisplayStartTimes;								//	給定DisplayDigit顯示位數為起始顯示數字次方數
	}																	//	結束if敘述
	else																//	若欲顯示數值InputNumber不大於10^14
	{																	//	進入else敘述
		DisplayDigit = 14;												//	給定DisplayDigit顯示位數為14位(有效位數)
	}																	//	結束else敘述
	while(TimesNumber >= DisplayStartTimes - DisplayDigit)				//	
	{																	//	進入while敘述
		if(TimesNumber == -1)											//	若TimesNumber(次方)為-1時
		{																//	進入if敘述
			printf(".");												//	顯示小數點
		}																//	結束if敘述
		DisplayChar = floor(DisplayNumber/(long double)pow(10,TimesNumber)) - pow(10,1) * floor(DisplayNumber/(long double)pow(10,(TimesNumber + 1)));
		//	取出顯示數值
		if( (DisplayChar >= 0) && (DisplayChar <= 9) )					//	若取出之數值介於0~9之間
		{																//	進入if敘述
			printf("%d",DisplayChar);									//	顯示數值
		}																//	結束if敘述
		else															//	若取出數值錯誤
		{																//	進入else敘述
			break;														//	結束while迴圈
		}																//	結束else敘述
		TimesNumber = TimesNumber - 1;									//	遞減TimesNumber變數
	}																	//	結束while敘述
}																		//	結束ShowLongDouble副程式