/*	C語言影像處理函式庫
Develop by Jimmy HU <s103360021@gmail.com>
This program is licensed under GNU General Public License v3.
BMPFileIO.cpp用於實作BMP圖片檔案IO相關副程式
*/

//-----include header files, 引入標頭檔-----
#include "BMPFileIO.h"

//----BMP圖檔大小(Byte)讀取副程式----
unsigned long BmpReadFilesize(const char *filename, const bool FilenameExtension)
//	BmpReadFilesize副程式
{																		//	進入BMP圖檔大小(Byte)讀取副程式
	//***檢查副程式傳入檔名***
	if(filename == NULL)												//	若輸入filename為NULL
	{																	//	進入if敘述
		printf("檔案路徑輸入為NULL\n");									//	顯示"檔案路徑輸入為NULL"並換行
		return 0;														//	回傳數值0，並結束副程式
	}																	//	結束if敘述
	//***參考FilenameExtension變數控制檔名處理方式***
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	printf("正在讀取下列檔案大小(Byte)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	unsigned long BmpRead_file_size;									//	宣告讀取BMP圖片檔案大小(Byte)變數，型態為unsigned long 
	BmpRead_file_size= header[2] + (header[3] << 8) + ( header[4] << 16) + ( header[5] << 24);
	//	計算輸入BMP圖片檔案大小(Byte，位元組) 
	fclose(fp);															//	關閉檔案
	return BmpRead_file_size;											//	將計算出檔案大小數值(Byte)傳回 
}																		//	結束BMP圖檔大小(Byte)讀取副程式 
//----BMP圖檔xsize(寬度)讀取副程式----
unsigned long BmpReadXSize(const char *filename, const bool FilenameExtension)
//	BMP圖檔xsize(寬度)讀取副程式
{																		//	進入BMP圖檔xsize(寬度)讀取副程式
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	printf("正在讀取下列檔案xsize(寬度)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	unsigned long BmpReadXSize;											//	宣告讀取BMP圖片檔案的x(寬度)大小(像素)變數，型態為unsigned long 
	BmpReadXSize= header[18] + (header[19] << 8) + ( header[20] << 16) + ( header[21] << 24);
	//	計算輸入BMP圖片檔案x(寬度)大小(像素) 
	fclose(fp);															//	關閉檔案
	return BmpReadXSize;												//	將計算出BMP圖片檔案x(寬度)大小(像素)數值傳回 
}																		//	結束BMP圖檔xsize(寬度)讀取副程式 

//----BMP圖檔ysize(高度)讀取副程式----
unsigned long BmpReadYSize(const char *filename, const bool FilenameExtension)
//	BMP圖檔ysize(高度)讀取副程式
{																		//	進入BMP圖檔ysize(高度)讀取副程式
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	printf("正在讀取下列檔案ysize(高度)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	unsigned long BmpReadYSize;											//	宣告讀取BMP圖片檔案的y(高度)大小(像素)變數，型態為unsigned long 
	BmpReadYSize= header[22] + (header[23] << 8) + ( header[24] << 16) + ( header[25] << 24);
	//	計算輸入BMP圖片檔案y(高度)大小(像素) 
	fclose(fp);															//	關閉檔案
	return BmpReadYSize;												//	將計算出BMP圖片檔案y(高度)大小(像素)數值傳回 
}																		//	結束BMP圖檔ysize(高度)讀取副程式 
//----BMP圖檔讀取副程式---- 
char BmpRead(unsigned char *image,const int xsize,const int ysize, const char *filename, const bool FilenameExtension)
{																		//	進入BMP圖檔讀取副程式
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	unsigned char FillingByte;											//	宣告填充位元組變數
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	printf("正在讀取檔案：%s\n",fname_bmp);								//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	fread(image, sizeof(unsigned char), (size_t)(long)(xsize * 3 + FillingByte)*ysize, fp);
	//	讀取圖檔資料 
	fclose(fp);															//	關閉檔案 
	return 0;															//	傳回0並結束副程式 
}																		//	結束BMP圖檔讀取副程式
BMPIMAGE BmpFileRead(const char *filename, const bool FilenameExtension)
//	BmpFileRead副程式
{																		//	進入BmpFileRead副程式
	BMPIMAGE OutputData;												//	宣告輸出資料空間
	strcpy(OutputData.FILENAME, "");									//	初始化OutputData
	OutputData.XSIZE = 0;												//	初始化OutputData
	OutputData.YSIZE = 0;												//	初始化OutputData
	OutputData.IMAGE_DATA = NULL;										//	初始化OutputData
	//***檢查副程式傳入檔名***
	if(filename == NULL)												//	若輸入filename為NULL
	{																	//	進入if敘述
		printf("檔案路徑輸入為NULL\n");									//	顯示"檔案路徑輸入為NULL"並換行
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	結束if敘述
	//***副檔名填補***
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	//***確認檔案存在及可讀性***
	if (FileExistCheck(fname_bmp) == false)								//	若檔案不存在 
	{																	//	進入if敘述 
		printf("檔案%s不存在！\n",fname_bmp);							//	顯示錯誤訊息 
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	結束if敘述	 
	if (FileReadPermissionCheck(fname_bmp) == false)					//	若檔案不可讀取
	{																	//	進入if敘述 
		printf("讀取檔案錯誤！\n");										//	顯示錯誤訊息
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	結束if敘述
	//***填入圖檔資訊至OutputData***
	strcpy(OutputData.FILENAME, fname_bmp);								//	填入檔案路徑(含副檔名)
	OutputData.XSIZE = (unsigned int)BmpReadXSize(OutputData.FILENAME,true);
	//	讀取輸入BMP圖檔寬度(路徑已包含副檔名) 
	OutputData.YSIZE = (unsigned int)BmpReadYSize(OutputData.FILENAME,true);
	//	讀取輸入BMP圖檔高度(路徑已包含副檔名) 
	if( (OutputData.XSIZE == -1) || (OutputData.YSIZE == -1) )			//	若XSIZE或YSIZE為-1(代表讀取檔案失敗)	
	{																	//	進入if敘述 
		printf("讀取圖檔大小資訊失敗!");								//	顯示"讀取圖檔大小資訊失敗!"
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	結束if敘述
	else																//	若XSIZE與YSIZE皆不為-1(正常讀取檔案)
	{																	//	進入else敘述 
		printf("輸入圖檔寬度：%d\n",OutputData.XSIZE);					//	顯示輸入圖檔寬度數值 
		printf("輸入圖檔高度：%d\n",OutputData.YSIZE);					//	顯示輸入圖檔高度數值 
		printf("輸入影像大小(Byte)：%d\n",(size_t)OutputData.XSIZE * OutputData.YSIZE * 3);
		//	顯示輸入影像大小數值(Byte) 
		OutputData.FILLINGBYTE = BmpFillingByteCalc(OutputData.XSIZE);	//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
		OutputData.IMAGE_DATA = (unsigned char*)malloc((OutputData.XSIZE * 3 + OutputData.FILLINGBYTE) * OutputData.YSIZE * sizeof(unsigned char));
		//	計算並建立影像大小空間 
		if (OutputData.IMAGE_DATA == NULL) 								//	若建立影像空間失敗 
		{																//	進入if敘述 
			printf("記憶體分配錯誤!");									//	顯示"記憶體分配錯誤!" 
			return OutputData;											//	回傳OutputData，並結束副程式
		}																//	結束if敘述 
		else															//	若未發生錯誤 
		{																//	進入else敘述 
			int loop_num = 0;											//	宣告區域變數loop_num供迴圈使用 
			for(loop_num=0;loop_num<((OutputData.XSIZE * 3 + OutputData.FILLINGBYTE) * OutputData.YSIZE);loop_num++)
			//	以for迴圈初始化每個像素 
			{															//	進入for迴圈 
				OutputData.IMAGE_DATA[loop_num]=255;						//	填入預設像素色彩數值 
			}															//	結束for迴圈 
			BmpRead(OutputData.IMAGE_DATA, OutputData.XSIZE, OutputData.YSIZE, OutputData.FILENAME,true);
			//	讀取圖檔資料
		}																//	結束else敘述
	}																	//	結束else敘述
	return OutputData;													//	回傳讀取資料
}																		//	結束BmpFileRead副程式
//----BMP圖檔寫入副程式---- 
/*	BmpWriteV1副程式程式執行BMP圖檔寫入，header陣列為BMP圖檔之檔頭 
 *
 *
 */
 
int BmpWriteV1(const unsigned char *image,const int xsize,const int ysize,const char *filename) 
{																		//	進入BmpWriteV1(BMP圖檔寫入)副程式 
	unsigned char FillingByte;											//	宣告填充位元組變數(FillingByte)
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	unsigned char header[54] = 											//	宣告檔頭設定header陣列(共54Bytes)
	{																	//	header陣列初始值設定 
	0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,									//	header陣列初始值設定
	54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 		//	header陣列初始值設定
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 		//	header陣列初始值設定
	0, 0, 0, 0															//	header陣列初始值設定
	};																	//	header陣列初始值設定
	unsigned long file_size = (long)xsize * (long)ysize * 3 + 54;		//	宣告file_size變數，設定檔案大小 
	unsigned long width, height;										//	宣告width與height為unsigned long型態變數，用於計算圖像檔案寬度、高度
	char fname_bmp[MAX_PATH];											//	宣告變數用以記錄輸出圖片之完整檔名(最多MAX_PATH個字元) 
	header[2] = (unsigned char)(file_size &0x000000ff);					//	計算檔頭資訊(圖像檔案大小)
	header[3] = (file_size >> 8) & 0x000000ff;							//	計算檔頭資訊(圖像檔案大小)
	header[4] = (file_size >> 16) & 0x000000ff;							//	計算檔頭資訊(圖像檔案大小)
	header[5] = (file_size >> 24) & 0x000000ff;							//	計算檔頭資訊(圖像檔案大小)
	
	width = xsize;														//	將圖像寬度資訊填入width變數
	header[18] = width & 0x000000ff;									//	計算檔頭資訊(圖像寬度)
	header[19] = (width >> 8) &0x000000ff;								//	計算檔頭資訊(圖像寬度)
	header[20] = (width >> 16) &0x000000ff;								//	計算檔頭資訊(圖像寬度)
	header[21] = (width >> 24) &0x000000ff;								//	計算檔頭資訊(圖像寬度)
	 
	height = ysize;														//	將圖像高度資訊填入height變數
	header[22] = height &0x000000ff;									//	計算檔頭資訊(圖像高度)
	header[23] = (height >> 8) &0x000000ff;								//	計算檔頭資訊(圖像高度)
	header[24] = (height >> 16) &0x000000ff;							//	計算檔頭資訊(圖像高度)
	header[25] = (height >> 24) &0x000000ff;							//	計算檔頭資訊(圖像高度)
	sprintf(fname_bmp, "%s.bmp", filename);								//	產生圖檔完整檔名路徑
	FILE *fp;															//	宣告檔案指標fp區域變數，用以記錄寫入圖檔路徑 
	if (!(fp = fopen(fname_bmp, "wb"))) 								//	嘗試以二進位讀檔方式開啟圖檔，若無法成功開啟
	{																	//	進入if敘述
		return -1;														//	回傳-1，並結束副程式
	}																	//	結束if敘述
	
	fwrite(header, sizeof(unsigned char), 54, fp);						//	寫入BMP圖檔檔頭資訊(54Bytes)
	fwrite(image, sizeof(unsigned char), (size_t)(long)(xsize * 3 + FillingByte)*ysize, fp);
	//	寫入BMP圖檔影像資料
	fclose(fp);															//	關閉檔案指標
	free((void*)image);
	image = NULL;
	return 0;															//	傳回0並結束副程式
}																		//	結束BmpWriteV1(BMP圖檔寫入)副程式
int BmpWriteV2(const BMPIMAGE OutputFile)								//	BmpWriteV2副程式
{																		//	進入BmpWriteV2副程式
	return BmpWriteV1(OutputFile.IMAGE_DATA, OutputFile.XSIZE, OutputFile.YSIZE, OutputFile.FILENAME);
	//	將OutputFile結構解析成各項參數傳入BmpWriteV1副程式
}																		//	結束BmpWriteV2副程式