/*	數位影像處理主函式
	Develop by Jimmy HU <s103360021@gmail.com>
	This program is licensed under GNU General Public License v3.
*/

//-----include header files, 引入標頭檔-----
#include "MainImageProcessing.h"										//	引入標頭檔MainImageProcessing.h


//-----funtion implementation, 副程式實作-----
void MainImageProcessing(unsigned char* InputImage, unsigned int ImageSizeX, unsigned int ImageSizeY)
//	MainImageProcessing副程式
{																		//	進入MainImageProcessing副程式
	/*if (ImageSizeX == 0 || ImageSizeY == 0)								//	若ImageSizeX或ImageSizeY為0
	{																	//	進入if敘述
		return;															//	結束副程式
	}																	//	結束if敘述
	BMP24RGBIMAGE RGBImage1;											//	宣告RGBImage1用於BMP24RGB型態圖像資料處理
	RGBImage1.XSIZE = ImageSizeX;										//	傳遞XSIZE資訊
	RGBImage1.YSIZE = ImageSizeY;										//	傳遞YSIZE資訊
	RGBImage1.IMAGE_DATA = RGBQUADtoBMP24RGB((RGBQUAD*)InputImage, ImageSizeX, ImageSizeY);
	//	傳遞影像資料

	//-----生成影像檔名-----
	char* FileName = NULL;
	char* FileNameBMP = NULL;
	int Counter = 1;
	FileName = (char*)malloc(100 * sizeof(char));
	FileNameBMP = (char*)malloc(100 * sizeof(char));
	sprintf(FileName, "%s%d", "Output", Counter);
	sprintf(FileNameBMP, "%s%s", FileName, ".BMP");
	
	BmpWriteV1(ArrayToRAWImage(RGBImage1.IMAGE_DATA, RGBImage1.XSIZE, RGBImage1.YSIZE), RGBImage1.XSIZE, RGBImage1.YSIZE, FileName);
	//	呼叫BmpWrite副程式寫入BMP檔
	//ImageDataToTxt("ImageDataTxt.txt", RGBImage1.IMAGE_DATA, RGBImage1.XSIZE, RGBImage1.YSIZE);
	//Sleep(10000);
	
	free((void*)RGBImage1.IMAGE_DATA);
	RGBImage1.IMAGE_DATA = NULL;
	free((void*)FileName);
	FileName = NULL;
	free((void*)FileNameBMP);
	FileNameBMP = NULL;
	char GitCmd[] = "call C:\\Users\\Jimmy\\Documents\\GitHub\\Windows-classic-samples_Private\\CameraImageProcessing\\MFCaptureD3D\\GitCmd.bat /c/Users/Jimmy/Documents/GitHub/Windows-classic-samples_Private/CameraImageProcessing/MFCaptureD3D/ Upload_test_image Upload_test_image";
	system(GitCmd);
	//Sleep(30000);														//	暫止30秒*/
	return;																//	結束MainImageProcessing副程式
}																		//	結束MainImageProcessing副程式


void ViewBMPImage(const char *filename)									//	ViewBMPImage副程式
{																		//	進入ViewBMPImage副程式
	#ifdef _WIN32														//	若編譯環境為Windows系統
		if(filename==NULL)												//	若傳入之filename指標為NULL
		{																//	進入if敘述
			return;														//	結束副程式
		}																//	結束if敘述
		char fname_bmp[MAX_PATH];										//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
		sprintf(fname_bmp, "%s%s.BMP",FILE_ROOT_PATH ,filename);		//	建立欲開啟圖檔之完整檔名(最多MAX_PATH個字元)
		char RunCommand[MAX_PATH+100];									//	宣告執行開啟圖檔指令字串變數
		sprintf(RunCommand, "%s%s","\"C:\\WINDOWS\\system32\\rundll32.exe\" C:\\WINDOWS\\system32\\shimgvw.dll,ImageView_Fullscreen ",fname_bmp);
		#ifdef DebugMode												//	若有定義為DebugMode(除錯模式)
			printf("開啟圖檔指令：%s\n",RunCommand);					//	顯示開啟圖檔指令並換行
		#endif															//	結束ifdef敘述
		system(RunCommand);												//	執行system函數，開啟圖檔
	#endif																//	結束ifdef敘述
	#ifdef linux														//	若編譯環境為Linux系統
		if(filename==NULL)												//	若傳入之filename指標為NULL
		{																//	進入if敘述
			return;														//	結束副程式
		}																//	結束if敘述
		char fname_bmp[MAX_PATH];										//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
		sprintf(fname_bmp, "%s%s.BMP",FILE_ROOT_PATH ,filename);		//	建立欲開啟圖檔之完整檔名(最多MAX_PATH個字元)
		char RunCommand[MAX_PATH+100];									//	宣告執行開啟圖檔指令字串變數
		sprintf(RunCommand, "%s%s","eog ",fname_bmp);					//	開啟指定圖檔
		#ifdef DebugMode												//	若有定義為DebugMode(除錯模式)
			printf("開啟圖檔指令：%s\n",RunCommand);					//	顯示開啟圖檔指令並換行
		#endif															//	結束ifdef敘述
		system(RunCommand);												//	執行system函數，開啟圖檔
	#endif																//	結束ifdef敘述
	
}																		//	結束ViewBMPImage副程式

BMP24RGB *InitialIMGArray(const int xsize, const int ysize)				//	InitialIMGArray副程式
{																		//	進入InitialIMGArray副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData(二維)指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[LoopNumber1*xsize + LoopNumber2].R = 0;			//	填入R像素資料
			OutputData[LoopNumber1*xsize + LoopNumber2].G = 0;			//	填入G像素資料
			OutputData[LoopNumber1*xsize + LoopNumber2].B = 0;			//	填入B像素資料
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳生成結果
}																		//	結束InitialIMGArray副程式

BMP24RGB *RGBQUADtoBMP24RGB(const RGBQUAD* InputRGBQUADImage, const int xsize, const int ysize)
//	RGBQUADtoBMP24RGB副程式
{																		//	進入RGBQUADtoBMP24RGB副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData(二維)指標記憶體大小
	if (OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1, LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	for (LoopNumber1 = 0; LoopNumber1<ysize; LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for (LoopNumber2 = 0; LoopNumber2<xsize; LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[LoopNumber1*xsize + LoopNumber2].R = 			//	填入R像素資料
				InputRGBQUADImage[(ysize - LoopNumber1) * xsize + LoopNumber2].rgbRed;
			OutputData[LoopNumber1*xsize + LoopNumber2].G = 			//	填入G像素資料
				InputRGBQUADImage[(ysize - LoopNumber1) * xsize + LoopNumber2].rgbGreen;
			OutputData[LoopNumber1*xsize + LoopNumber2].B = 			//	填入B像素資料
				InputRGBQUADImage[(ysize - LoopNumber1) * xsize + LoopNumber2].rgbBlue;
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳轉換結果
}																		//	結束RGBQUADtoBMP24RGB副程式

RGBQUAD* BMP24RGBtoRGBQUAD(const BMP24RGB* InputBMP24RGBImage, const int xsize, const int ysize)
{
	RGBQUAD *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (RGBQUAD*)malloc(xsize * ysize * sizeof(RGBQUAD));		//	配置OutputData(二維)指標記憶體大小
	if (OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1, LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	for (LoopNumber1 = 0; LoopNumber1<ysize; LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for (LoopNumber2 = 0; LoopNumber2<xsize; LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[(ysize - LoopNumber1) * xsize + LoopNumber2].rgbRed = 
				InputBMP24RGBImage[LoopNumber1*xsize + LoopNumber2].R;	//	填入R像素資料
			OutputData[(ysize - LoopNumber1) * xsize + LoopNumber2].rgbGreen = 
				InputBMP24RGBImage[LoopNumber1*xsize + LoopNumber2].G;	//	填入G像素資料
			OutputData[(ysize - LoopNumber1) * xsize + LoopNumber2].rgbBlue = 
				InputBMP24RGBImage[LoopNumber1*xsize + LoopNumber2].B;	//	填入B像素資料
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳轉換結果
}

BMP24RGB *RAWImageToArray(const unsigned char *image, const int xsize, const int ysize)
//	BMP圖檔資料至陣列轉換副程式
{																		//	進入BMP圖檔資料至陣列轉換副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData(二維)指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	unsigned char FillingByte;											//	宣告FillingByte區域變數
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[LoopNumber1*xsize + LoopNumber2].R = 			//	填入R像素資料
			image[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 2];
			OutputData[LoopNumber1*xsize + LoopNumber2].G = 			//	填入G像素資料
			image[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 1];
			OutputData[LoopNumber1*xsize + LoopNumber2].B = 			//	填入B像素資料
			image[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 0];
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳轉換結果
}																		//	結束BMP圖檔資料至陣列轉換副程式

unsigned char *ArrayToRAWImage(const BMP24RGB* InputData,const int xsize,const int ysize)
//	ArrayToRAWImage副程式
{																		//	進入ArrayToRAWImage副程式
	unsigned char FillingByte;											//	宣告FillingByte區域變數
	FillingByte = BmpFillingByteCalc(xsize);							//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
	unsigned char *OutputData;											//	宣告OutputData記錄轉換完成資料
	OutputData = (unsigned char*)malloc((xsize * 3 + FillingByte) * ysize * sizeof(unsigned char));
	//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	//***初始化OutputData指標資料***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序初始化OutputData指標資料為0
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<(xsize * 3 + FillingByte);LoopNumber2++)
		//	以for迴圈依序初始化OutputData指標資料為0
		{																//	進入for迴圈
			OutputData[LoopNumber1 * (xsize * 3 + FillingByte) + LoopNumber2] = 0;
		}																//	結束for迴圈
	}																	//	結束for迴圈
	//***填入圖像資料至OutputData***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序處理圖像像素
		{																//	進入for迴圈
			OutputData[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 2]
			= InputData[LoopNumber1*xsize + LoopNumber2].R;				//	填入R像素資料
			OutputData[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 1]
			= InputData[LoopNumber1*xsize + LoopNumber2].G;				//	填入G像素資料
			OutputData[3*(LoopNumber1 * xsize + LoopNumber2) + LoopNumber1 * FillingByte + 0]
			= InputData[LoopNumber1*xsize + LoopNumber2].B;				//	填入B像素資料
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳轉換結果
}																		//	結束ArrayToRAWImage副程式

HSV *BMP24RGBToHSV(const BMP24RGB *InputData,const int xsize,const int ysize)
//	BMP24RGBToHSV副程式
{																		//	進入BMP24RGBToHSV副程式
	HSV *OutputData;													//	宣告OutputData為HSV指標型態變數
	OutputData = (HSV*)malloc(xsize * ysize * sizeof(HSV));				//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	//***OutputData指標初始化***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			OutputData[LoopNumber1 * xsize + LoopNumber2].H = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].S = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].V = 0;		//	初始化OutputData指標變數
		}																//	結束for迴圈
	}																	//	結束for迴圈
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			unsigned char Red,Green,Blue;								//	宣告Red,Green,Blue變數，記錄像素資料
			Red = InputData[LoopNumber1 * xsize + LoopNumber2].R;		//	填入Red資料
			Green = InputData[LoopNumber1 * xsize + LoopNumber2].G;		//	填入Green資料
			Blue = InputData[LoopNumber1 * xsize + LoopNumber2].B;		//	填入Blue資料
			//***排序RGB像素資料***
			unsigned char *SortArray;									//	宣告SortArray為unsigned char型態指標，供像素值RGB排序使用
			SortArray = (unsigned char *)malloc( 3 * sizeof(unsigned char) );
			SortArray[0] = Red;											//	將像素R資料填入SortArray
			SortArray[1] = Green;										//	將像素G資料填入SortArray
			SortArray[2] = Blue;										//	將像素B資料填入SortArray
			//printf("%d,%d,%d\n",SortArray[0],SortArray[1],SortArray[2]);//	顯示讀取資料
			SortArray = UCharBubbleSort(SortArray, 3, 0);				//	呼叫UCharBubbleSort排序副程式進行排序(由小至大)
			//printf("%d,%d,%d\n",SortArray[0],SortArray[1],SortArray[2]);//	顯示排序後資料
			unsigned char Max,Mid,Min;									//	宣告Max,Mid,Min變數，記錄排序完成像素資料
			Max = SortArray[2];											//	填入Max數值
			Mid = SortArray[1];											//	填入Mid數值
			Min = SortArray[0];											//	填入Min數值
			//system("pause");
			//***計算H***
			long double H1 = acos(0.5 * ((Red - Green) + (Red - Blue)) /
							 sqrt(((pow((Red - Green), 2.0)) + 
							 (Red - Blue) * (Green - Blue)))) * (180.0 / M_PI);
			if( Max == Min )											//	若Max=Min
			{															//	進入if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].H = 0.0;	//	設定H=0
			}															//	結束if敘述
			else if(Blue <= Green)										//	若Blue(藍色成分)小於等於Green(綠色成分)
			{															//	進入else if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].H = H1;
			}															//	結束else if敘述
			else														//	若Max不等於Min且Blue大於Green
			{															//	進入else敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].H = 360.0-H1;
			}															//	結束else敘述
			//***計算S***
			if(Max == 0)												//	若Max=0
			{															//	進入if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].S = 0.0;	//	填入S數值
			}															//	結束if敘述
			else														//	若Max不為0
			{															//	進入else敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].S =
				1.0 - ( (long double)Min / (long double)Max);			//	計算S數值
			}															//	結束else敘述
			//***計算V***
			OutputData[LoopNumber1 * xsize + LoopNumber2].V =
			SortArray[2];												//	計算V數值
			free(SortArray);											//	釋放SortArray空間
			
			#ifdef DebugMode											//	若有定義為DebugMode(除錯模式)
				printf("\n轉換結果：\n原始像素\t轉換後像素\n");
				printf("R=%d\t\tHue=\t\t",InputData[LoopNumber1 * xsize + LoopNumber2].R);
				ShowLongDouble(OutputData[LoopNumber1 * xsize + LoopNumber2].H);
				printf("\nG=%d\t\tSaturation=\t",InputData[LoopNumber1 * xsize + LoopNumber2].G);
				ShowLongDouble(OutputData[LoopNumber1 * xsize + LoopNumber2].S);
				printf("\nB=%d\t\tValue=\t\t",InputData[LoopNumber1 * xsize + LoopNumber2].B);
				ShowLongDouble(OutputData[LoopNumber1 * xsize + LoopNumber2].V);
				printf("\n");
				system("pause");
			#endif														//	結束ifdef敘述
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳OutputData，並結束副程式
}																		//	結束BMP24RGBToHSV副程式
BMP24RGB *HSVToBMP24RGB(const HSV *InputData,const int xsize,const int ysize)
//	HSVToBMP24RGB副程式
{																		//	進入HSVToBMP24RGB副程式
	BMP24RGB *OutputData;												//	宣告OutputData為HSV指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	//***OutputData指標初始化***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			OutputData[LoopNumber1 * xsize + LoopNumber2].R = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].G = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].B = 0;		//	初始化OutputData指標變數
		}																//	結束for迴圈
	}																	//	結束for迴圈
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			long double H,S,Max;										//	宣告H、S與Max長雙精度浮點數用以記錄當前像素資料
			H = InputData[LoopNumber1 * xsize + LoopNumber2].H;			//	填入H資料
			S = InputData[LoopNumber1 * xsize + LoopNumber2].S;			//	填入S資料
			Max = InputData[LoopNumber1 * xsize + LoopNumber2].V;		//	填入Max資料
			unsigned char hi = floor( H / 60.0);						//	計算hi
			long double f = (H / 60.0) - hi;							//	計算f
			long double Min,q,t;
			Min = Max * (1.0 - S);
			q = Max * (1.0 - f * S);
			t = Max * (1.0 - (1.0 - f) * S);
			if(hi == 0)													//	若hi=0
			{															//	進入if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].R = Max;	//	填入R資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].G = t;	//	填入G資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].B = Min;	//	填入B資料
			}															//	結束if敘述
			else if(hi == 1)											//	若hi=1
			{															//	進入else if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].R = q;	//	填入R資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].G = Max;	//	填入G資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].B = Min;	//	填入B資料
			}															//	結束else if敘述
			else if(hi == 2)											//	若hi=2
			{															//	進入else if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].R = Min;	//	填入R資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].G = Max;	//	填入G資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].B = t;	//	填入B資料
			}															//	結束else if敘述
			else if(hi == 3)											//	若hi=3
			{															//	進入else if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].R = Min;	//	填入R資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].G = q;	//	填入G資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].B = Max;	//	填入B資料
			}															//	結束else if敘述
			else if(hi == 4)											//	若hi=4
			{															//	進入else if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].R = t;	//	填入R資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].G = Min;	//	填入G資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].B = Max;	//	填入B資料
			}															//	結束else if敘述
			else if(hi == 5)											//	若hi=5
			{															//	進入else if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].R = Max;	//	填入R資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].G = Min;	//	填入G資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].B = q;	//	填入B資料
			}															//	結束else if敘述
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳運算結果
}																		//	結束HSVToBMP24RGB副程式
bool ImageDataToTxt(const char *file_name,const BMP24RGB *image,const int xsize,const int ysize)
//	ImageDataToTxt副程式，用於將圖像影像資料寫入txt檔
{																		//	進入ImageDataToTxt副程式
	char fname_txt[MAX_PATH];											//	宣告圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	sprintf(fname_txt, "%s.txt", file_name);							//	建立圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	if(FileWrite(fname_txt,"","w") == False)							//	嘗試建立檔案，若檔案建立失敗
	{																	//	進入if敘述
		return False;													//	回傳False並結束副程式返回
	}																	//	結束if敘述
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	char WriteStringTemp[50];											//	宣告WriteStringTemp字串，用於暫存寫入資料
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序寫入圖像資料
	{																	//	進入for迴圈
		sprintf(WriteStringTemp,"第%d個像素R=%d\t",loop_num,image[loop_num].R);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
		sprintf(WriteStringTemp,"第%d個像素G=%d\t",loop_num,image[loop_num].G);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
		sprintf(WriteStringTemp,"第%d個像素B=%d\n",loop_num,image[loop_num].B);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
	}																	//	結束for迴圈
	return True;														//	回傳True並結束副程式返回
}																		//	結束ImageDataToTxt副程式
bool HSVDataToTxt(const char *file_name,const HSV *image,const int xsize,const int ysize)
//	HSVDataToTxt副程式，用於將HSV型態影像資料寫入txt檔
{																		//	進入HSVDataToTxt副程式
	char fname_txt[MAX_PATH];											//	宣告圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	sprintf(fname_txt, "%s.txt", file_name);							//	建立圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	if(FileWrite(fname_txt,"","w") == False)							//	嘗試建立檔案，若檔案建立失敗
	{																	//	進入if敘述
		return False;													//	回傳False並結束副程式返回
	}																	//	結束if敘述
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	char WriteStringTemp[50];											//	宣告WriteStringTemp字串，用於暫存寫入資料
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序寫入圖像資料
	{																	//	進入for迴圈
		sprintf(WriteStringTemp,"第%d個像素H=%f\t",loop_num,(float)image[loop_num].H);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
		sprintf(WriteStringTemp,"第%d個像素S=%f\t",loop_num,(float)image[loop_num].S);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
		sprintf(WriteStringTemp,"第%d個像素V=%f\n",loop_num,(float)image[loop_num].V);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	結束if敘述
	}																	//	結束for迴圈
	return True;														//	回傳True並結束副程式返回
}																		//	結束HSVDataToTxt副程式
BMP24RGB *ImgDifference2(const BMP24RGB *InputData1,const BMP24RGB *InputData2,const int xsize,const int ysize)
//	ImgDifference2副程式
{																		//	進入ImgDifference2副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(HSV));		//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	//***OutputData指標初始化***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			OutputData[LoopNumber1 * xsize + LoopNumber2].R = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].G = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].B = 0;		//	初始化OutputData指標變數
		}																//	結束for迴圈
	}																	//	結束for迴圈
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			unsigned char Red1,Green1,Blue1;							//	宣告Red1,Green1,Blue1變數，記錄像素資料
			Red1 = InputData1[LoopNumber1 * xsize + LoopNumber2].R;		//	填入InputData1之Red資料
			Green1 = InputData1[LoopNumber1 * xsize + LoopNumber2].G;	//	填入InputData1之Green資料
			Blue1 = InputData1[LoopNumber1 * xsize + LoopNumber2].B;	//	填入InputData1之Blue資料
			unsigned char Red2,Green2,Blue2;							//	宣告Red2,Green2,Blue2變數，記錄像素資料
			Red2 = InputData2[LoopNumber1 * xsize + LoopNumber2].R;		//	填入InputData2之Red資料
			Green2 = InputData2[LoopNumber1 * xsize + LoopNumber2].G;	//	填入InputData2之Green資料
			Blue2 = InputData2[LoopNumber1 * xsize + LoopNumber2].B;	//	填入InputData2之Blue資料
			OutputData[LoopNumber1 * xsize + LoopNumber2].R = fabs(Red1 - Red2);
			//	將輸入資料InputData1與InputData2進行相減運算並取絕對值後填入OutputData
			OutputData[LoopNumber1 * xsize + LoopNumber2].G = fabs(Green1 - Green2);
			//	將輸入資料InputData1與InputData2進行相減運算並取絕對值後填入OutputData
			OutputData[LoopNumber1 * xsize + LoopNumber2].B = fabs(Blue1 - Blue2);
			//	將輸入資料InputData1與InputData2進行相減運算並取絕對值後填入OutputData
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳OutputData，並結束副程式
}																		//	結束ImgDifference2副程式
BMP24RGB *BMP24RGB2or(const BMP24RGB *InputData1,const BMP24RGB *InputData2,const int xsize,const int ysize)
//	BMP24RGB2or副程式
{																		//	進入BMP24RGB2or副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(HSV));		//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	結束if敘述
	int LoopNumber1,LoopNumber2;										//	宣告LoopNumber1與LoopNumber2，用於迴圈計算
	//***OutputData指標初始化***
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			OutputData[LoopNumber1 * xsize + LoopNumber2].R = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].G = 0;		//	初始化OutputData指標變數
			OutputData[LoopNumber1 * xsize + LoopNumber2].B = 0;		//	初始化OutputData指標變數
		}																//	結束for迴圈
	}																	//	結束for迴圈
	for(LoopNumber1 = 0;LoopNumber1<ysize;LoopNumber1++)				//	以for迴圈依序轉換圖像像素資料
	{																	//	進入for迴圈
		for(LoopNumber2 = 0;LoopNumber2<xsize;LoopNumber2++)			//	以for迴圈依序轉換圖像像素資料
		{																//	進入for迴圈
			unsigned char Red1,Green1,Blue1;							//	宣告Red1,Green1,Blue1變數，記錄像素資料
			Red1 = InputData1[LoopNumber1 * xsize + LoopNumber2].R;		//	填入InputData1之Red資料
			Green1 = InputData1[LoopNumber1 * xsize + LoopNumber2].G;	//	填入InputData1之Green資料
			Blue1 = InputData1[LoopNumber1 * xsize + LoopNumber2].B;	//	填入InputData1之Blue資料
			unsigned char Red2,Green2,Blue2;							//	宣告Red2,Green2,Blue2變數，記錄像素資料
			Red2 = InputData2[LoopNumber1 * xsize + LoopNumber2].R;		//	填入InputData2之Red資料
			Green2 = InputData2[LoopNumber1 * xsize + LoopNumber2].G;	//	填入InputData2之Green資料
			Blue2 = InputData2[LoopNumber1 * xsize + LoopNumber2].B;	//	填入InputData2之Blue資料
			OutputData[LoopNumber1 * xsize + LoopNumber2].R = Red1 | Red2;
			//	將輸入資料InputData1與InputData2進行OR運算後填入OutputData
			OutputData[LoopNumber1 * xsize + LoopNumber2].G = Green1 | Green2;
			//	將輸入資料InputData1與InputData2進行OR運算後填入OutputData
			OutputData[LoopNumber1 * xsize + LoopNumber2].B = Blue1 | Blue2;
			//	將輸入資料InputData1與InputData2進行OR運算後填入OutputData
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳OutputData，並結束副程式
}																		//	結束BMP24RGB2or副程式
BMP24RGB *BmpToGraylevel(const BMP24RGB *image,const int xsize,const int ysize)
//	BMP圖片資料轉灰階副程式
{																		//	進入BMP圖片資料轉灰階副程式
	BMP24RGB *GraylevelImage;											//	宣告GraylevelImage指標變數，記錄灰階轉換後圖像資料
	GraylevelImage = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));
	//	配置GraylevelImage指標記憶體大小
	if (GraylevelImage == NULL) 										//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素灰階值
	{																	//	進入for迴圈
		GraylevelImage[loop_num].R = (image[loop_num].R + image[loop_num].G + image[loop_num].B) / 3;
		//	計算灰階轉換像素值
		GraylevelImage[loop_num].G = (image[loop_num].R + image[loop_num].G + image[loop_num].B) / 3;
		//	計算灰階轉換像素值
		GraylevelImage[loop_num].B = (image[loop_num].R + image[loop_num].G + image[loop_num].B) / 3;
		//	計算灰階轉換像素值
	}																	//	結束for迴圈
    return GraylevelImage;												//	回傳灰階轉換結果
}																		//	結束BMP圖片資料轉灰階副程式
BMP24RGB *BmpConvolution33(const BMP24RGB *image,const int xsize,const int ysize,const long double Mask[])
//	BmpConvolution33副程式
{																		//	進入BmpConvolution33副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long double weighting = 0;											//	宣告並初始化weighting(權重)變數
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num < 3 * 3;loop_num = loop_num + 1)			//	以for迴圈計算權重累計值
	{																	//	進入for迴圈
		weighting = weighting + Mask[loop_num];							//	統計權重
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		//	(loop_num >= (xsize*(ysize-1)))		------->	|-－－－－－－－|
		//													|				|
		//	( (loop_num % xsize) == 0)			------->	|				|	<-------	( ((loop_num + 1) % xsize) == 0)
		//													|				|
		//	(loop_num < xsize)					------->	|-－－－－－－－|
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			OutputData[loop_num].R = (
				image[loop_num+xsize-1].R	* Mask[6] + image[loop_num+xsize].R	* Mask[7] + image[loop_num+xsize+1].R	* Mask[8] +
				image[loop_num-1].R 		* Mask[3] + image[loop_num].R		* Mask[4] + image[loop_num+1].R 		* Mask[5] +
				image[loop_num-xsize-1].R	* Mask[0] + image[loop_num-xsize].R	* Mask[1] + image[loop_num-xsize+1].R	* Mask[2]
				) / weighting;											//	計算Convolution
			OutputData[loop_num].G = (
				image[loop_num+xsize-1].G	* Mask[6] + image[loop_num+xsize].G	* Mask[7] + image[loop_num+xsize+1].G	* Mask[8] +
				image[loop_num-1].G 		* Mask[3] + image[loop_num].G		* Mask[4] + image[loop_num+1].G 		* Mask[5] +
				image[loop_num-xsize-1].G	* Mask[0] + image[loop_num-xsize].G	* Mask[1] + image[loop_num-xsize+1].G	* Mask[2]
				) / weighting;											//	計算Convolution
			OutputData[loop_num].B = (
				image[loop_num+xsize-1].B	* Mask[6] + image[loop_num+xsize].B	* Mask[7] + image[loop_num+xsize+1].B	* Mask[8] +
				image[loop_num-1].B 		* Mask[3] + image[loop_num].B		* Mask[4] + image[loop_num+1].B 		* Mask[5] +
				image[loop_num-xsize-1].B	* Mask[0] + image[loop_num-xsize].B	* Mask[1] + image[loop_num-xsize+1].B	* Mask[2]
				) / weighting;											//	計算Convolution
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束BmpConvolution33副程式
BMP24RGB *BmpConvolution55(const BMP24RGB *image,const int xsize,const int ysize,const long double Mask[])
//	BmpConvolution55副程式
{																		//	進入BmpConvolution55副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long double weighting = 0;											//	宣告並初始化weighting(權重)變數
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num < 5 * 5;loop_num = loop_num + 1)			//	以for迴圈計算權重累計值
	{																	//	進入for迴圈
		weighting = weighting + Mask[loop_num];							//	統計權重
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < 2 * xsize) || 
			( (loop_num % xsize) == 0) ||
			( (loop_num % xsize) == 1) ||
			( ((loop_num + 1) % xsize) == 0) ||
			( ((loop_num + 2) % xsize) == 0) ||
			(loop_num >= (xsize*(ysize-2))) )
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			OutputData[loop_num].R = (
				image[loop_num+2*xsize-2].R	* Mask[20] + image[loop_num+2*xsize-1].R	* Mask[21] + image[loop_num+2*xsize].R	* Mask[22] + image[loop_num+2*xsize+1].R	* Mask[23] + image[loop_num+2*xsize+2].R	* Mask[24] +
				image[loop_num+1*xsize-2].R	* Mask[15] + image[loop_num+1*xsize-1].R	* Mask[16] + image[loop_num+1*xsize].R	* Mask[17] + image[loop_num+1*xsize+1].R	* Mask[18] + image[loop_num+1*xsize+2].R	* Mask[19] +
				image[loop_num-0*xsize-2].R	* Mask[10] + image[loop_num-0*xsize-1].R	* Mask[11] + image[loop_num-0*xsize].R	* Mask[12] + image[loop_num-0*xsize+1].R	* Mask[13] + image[loop_num-0*xsize+2].R	* Mask[14] +
				image[loop_num-1*xsize-2].R	* Mask[ 5] + image[loop_num-1*xsize-1].R	* Mask[6]  + image[loop_num-1*xsize].R	* Mask[ 7] + image[loop_num-1*xsize+1].R	* Mask[ 8] + image[loop_num-1*xsize+2].R	* Mask[ 9] +
				image[loop_num-2*xsize-2].R	* Mask[ 0] + image[loop_num-2*xsize-1].R	* Mask[1]  + image[loop_num-2*xsize].R	* Mask[ 2] + image[loop_num-2*xsize+1].R	* Mask[ 3] + image[loop_num-2*xsize+2].R	* Mask[ 4]
				) / weighting;											//	計算Convolution
			OutputData[loop_num].G = (
				image[loop_num+2*xsize-2].G	* Mask[20] + image[loop_num+2*xsize-1].G	* Mask[21] + image[loop_num+2*xsize].G	* Mask[22] + image[loop_num+2*xsize+1].G	* Mask[23] + image[loop_num+2*xsize+2].G	* Mask[24] +
				image[loop_num+1*xsize-2].G	* Mask[15] + image[loop_num+1*xsize-1].G	* Mask[16] + image[loop_num+1*xsize].G	* Mask[17] + image[loop_num+1*xsize+1].G	* Mask[18] + image[loop_num+1*xsize+2].G	* Mask[19] +
				image[loop_num-0*xsize-2].G	* Mask[10] + image[loop_num-0*xsize-1].G	* Mask[11] + image[loop_num-0*xsize].G	* Mask[12] + image[loop_num-0*xsize+1].G	* Mask[13] + image[loop_num-0*xsize+2].G	* Mask[14] +
				image[loop_num-1*xsize-2].G	* Mask[ 5] + image[loop_num-1*xsize-1].G	* Mask[ 6]  + image[loop_num-1*xsize].G	* Mask[ 7] + image[loop_num-1*xsize+1].G	* Mask[ 8] + image[loop_num-1*xsize+2].G	* Mask[ 9] +
				image[loop_num-2*xsize-2].G	* Mask[ 0] + image[loop_num-2*xsize-1].G	* Mask[ 1]  + image[loop_num-2*xsize].G	* Mask[ 2] + image[loop_num-2*xsize+1].G	* Mask[ 3] + image[loop_num-2*xsize+2].G	* Mask[ 4]
				) / weighting;											//	計算Convolution
			OutputData[loop_num].B = (
				image[loop_num+2*xsize-2].B	* Mask[20] + image[loop_num+2*xsize-1].B	* Mask[21] + image[loop_num+2*xsize].B	* Mask[22] + image[loop_num+2*xsize+1].B	* Mask[23] + image[loop_num+2*xsize+2].B	* Mask[24] +
				image[loop_num+1*xsize-2].B	* Mask[15] + image[loop_num+1*xsize-1].B	* Mask[16] + image[loop_num+1*xsize].B	* Mask[17] + image[loop_num+1*xsize+1].B	* Mask[18] + image[loop_num+1*xsize+2].B	* Mask[19] +
				image[loop_num-0*xsize-2].B	* Mask[10] + image[loop_num-0*xsize-1].B	* Mask[11] + image[loop_num-0*xsize].B	* Mask[12] + image[loop_num-0*xsize+1].B	* Mask[13] + image[loop_num-0*xsize+2].B	* Mask[14] +
				image[loop_num-1*xsize-2].B	* Mask[ 5] + image[loop_num-1*xsize-1].B	* Mask[ 6] + image[loop_num-1*xsize].B	* Mask[ 7] + image[loop_num-1*xsize+1].B	* Mask[ 8] + image[loop_num-1*xsize+2].B	* Mask[ 9] +
				image[loop_num-2*xsize-2].B	* Mask[ 0] + image[loop_num-2*xsize-1].B	* Mask[ 1] + image[loop_num-2*xsize].B	* Mask[ 2] + image[loop_num-2*xsize+1].B	* Mask[ 3] + image[loop_num-2*xsize+2].B	* Mask[ 4]
				) / weighting;											//	計算Convolution
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束BmpConvolution55副程式
BMP24RGB *BmpConvolution77(const BMP24RGB *image,const int xsize,const int ysize,const long double Mask[])
//	BmpConvolution77副程式
{																		//	進入BmpConvolution77副程式
	char MaskSize = 7;													//	宣告MaskSize(Mask大小變數，記錄Mask邊長)為7
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long double weighting = 0;											//	宣告並初始化weighting(權重)變數
	long long int loop_num1;											//	宣告loop_num1區域變數供迴圈使用
	for(loop_num1 = 0; loop_num1 < MaskSize * MaskSize;loop_num1 = loop_num1 + 1)
	//	以for迴圈計算權重累計值
	{																	//	進入for迴圈
		weighting = weighting + Mask[loop_num1];						//	統計權重
	}																	//	結束for迴圈
	for(loop_num1 = 0; loop_num1 <(xsize * ysize); loop_num1++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num1 % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num1 / xsize);						//	計算PixelLocationY
		if( (PixelLocationX < floor(MaskSize/2)) || (PixelLocationX > (xsize - floor(MaskSize/2))) ||
			(PixelLocationY < floor(MaskSize/2)) || (PixelLocationY > (ysize - floor(MaskSize/2))) )
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num1].R = image[loop_num1].R;				//	邊界點不處理
			OutputData[loop_num1].G = image[loop_num1].G;				//	邊界點不處理
			OutputData[loop_num1].B = image[loop_num1].B;				//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			char loop_num2, loop_num3;									//	宣告loop_num2、loop_num3變數以進行迴圈計數
			double sum_R = 0;											//	宣告sum_R變數統計像素加權總和
			double sum_G = 0;											//	宣告sum_G變數統計像素加權總和
			double sum_B = 0;											//	宣告sum_B變數統計像素加權總和
			for(loop_num2 = -floor(MaskSize/2); loop_num2 <= floor(MaskSize/2); loop_num2 ++)
			//	以for迴圈計算Convolution
			{															//	進入for迴圈
				for(loop_num3 = -floor(MaskSize/2); loop_num3 <= floor(MaskSize/2); loop_num3 ++)
				//	以for迴圈計算Convolution
				{														//	進入for迴圈
					sum_R += image[loop_num1+loop_num2*xsize+loop_num3].R *
							 Mask[(int)((loop_num2 + floor(MaskSize/2)) * MaskSize + (loop_num3 + floor(MaskSize/2)))];
					//	統計像素加權總和
					sum_G += image[loop_num1+loop_num2*xsize+loop_num3].G *
							 Mask[(int)((loop_num2 + floor(MaskSize/2)) * MaskSize + (loop_num3 + floor(MaskSize/2)))];
					//	統計像素加權總和
					sum_B += image[loop_num1+loop_num2*xsize+loop_num3].B *
							 Mask[(int)((loop_num2 + floor(MaskSize/2)) * MaskSize + (loop_num3 + floor(MaskSize/2)))];
					//	統計像素加權總和
				}														//	結束for迴圈
			}															//	結束for迴圈
			OutputData[loop_num1].R = sum_R / weighting;				//	計算Convolution
			OutputData[loop_num1].G = sum_G / weighting;				//	計算Convolution
			OutputData[loop_num1].B = sum_B / weighting;				//	計算Convolution
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束BmpConvolution77副程式
BMP24RGB *BmpConvolution(int MaskSize, const BMP24RGB *image,const int xsize,const int ysize,const long double Mask[])
{																		//	進入BmpConvolution副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long double weighting = 0;											//	宣告並初始化weighting(權重)變數
	long long int loop_num1;											//	宣告loop_num1區域變數供迴圈使用
	for(loop_num1 = 0; loop_num1 < MaskSize * MaskSize;loop_num1 = loop_num1 + 1)
	//	以for迴圈計算權重累計值
	{																	//	進入for迴圈
		weighting = weighting + Mask[loop_num1];						//	統計權重
	}																	//	結束for迴圈
	for(loop_num1 = 0; loop_num1 <(xsize * ysize); loop_num1++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num1 % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num1 / xsize);						//	計算PixelLocationY
		if( (PixelLocationX < floor(MaskSize/2)) || (PixelLocationX > (xsize - floor(MaskSize/2))) ||
			(PixelLocationY < floor(MaskSize/2)) || (PixelLocationY > (ysize - floor(MaskSize/2))) )
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num1].R = image[loop_num1].R;				//	邊界點不處理
			OutputData[loop_num1].G = image[loop_num1].G;				//	邊界點不處理
			OutputData[loop_num1].B = image[loop_num1].B;				//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			char loop_num2, loop_num3;									//	宣告loop_num2、loop_num3變數以進行迴圈計數
			double sum_R = 0;											//	宣告sum_R變數統計像素加權總和
			double sum_G = 0;											//	宣告sum_G變數統計像素加權總和
			double sum_B = 0;											//	宣告sum_B變數統計像素加權總和
			for(loop_num2 = -floor(MaskSize/2); loop_num2 <= floor(MaskSize/2); loop_num2 ++)
			//	以for迴圈計算Convolution
			{															//	進入for迴圈
				for(loop_num3 = -floor(MaskSize/2); loop_num3 <= floor(MaskSize/2); loop_num3 ++)
				//	以for迴圈計算Convolution
				{														//	進入for迴圈
					sum_R += image[loop_num1+loop_num2*xsize+loop_num3].R *
							 Mask[(int)((loop_num2 + floor(MaskSize/2)) * MaskSize + (loop_num3 + floor(MaskSize/2)))];
					//	統計像素加權總和
					sum_G += image[loop_num1+loop_num2*xsize+loop_num3].G *
							 Mask[(int)((loop_num2 + floor(MaskSize/2)) * MaskSize + (loop_num3 + floor(MaskSize/2)))];
					//	統計像素加權總和
					sum_B += image[loop_num1+loop_num2*xsize+loop_num3].B *
							 Mask[(int)((loop_num2 + floor(MaskSize/2)) * MaskSize + (loop_num3 + floor(MaskSize/2)))];
					//	統計像素加權總和
				}														//	結束for迴圈
			}															//	結束for迴圈
			OutputData[loop_num1].R = sum_R / weighting;				//	計算Convolution
			OutputData[loop_num1].G = sum_G / weighting;				//	計算Convolution
			OutputData[loop_num1].B = sum_B / weighting;				//	計算Convolution
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束BmpConvolution副程式
BMP24RGB *ImageSmoothing33V1(const BMP24RGB *image,const int xsize,const int ysize)
//	ImageSmoothing33V1(BMP圖檔3*3Mask平滑濾波)副程式
{																		//	進入ImageSmoothing33V1副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			OutputData[loop_num].R = (
				image[loop_num+xsize-1].R	* 1 + image[loop_num+xsize].R	* 1 + image[loop_num+xsize+1].R	* 1 +
				image[loop_num-1].R 		* 1 + image[loop_num].R			* 1 + image[loop_num+1].R 		* 1 +
				image[loop_num-xsize-1].R	* 1 + image[loop_num-xsize].R	* 1 + image[loop_num-xsize+1].R	* 1
				) / 9;													//	平滑法計算
			OutputData[loop_num].G = (
				image[loop_num+xsize-1].G	* 1 + image[loop_num+xsize].G	* 1 + image[loop_num+xsize+1].G	* 1 +
				image[loop_num-1].G 		* 1 + image[loop_num].G			* 1 + image[loop_num+1].G 		* 1 +
				image[loop_num-xsize-1].G	* 1 + image[loop_num-xsize].G	* 1 + image[loop_num-xsize+1].G	* 1
				) / 9;													//	平滑法計算
			OutputData[loop_num].B = (
				image[loop_num+xsize-1].B	* 1 + image[loop_num+xsize].B	* 1 + image[loop_num+xsize+1].B	* 1 +
				image[loop_num-1].B 		* 1 + image[loop_num].B			* 1 + image[loop_num+1].B 		* 1 +
				image[loop_num-xsize-1].B	* 1 + image[loop_num-xsize].B	* 1 + image[loop_num-xsize+1].B	* 1
				) / 9;													//	平滑法計算
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束ImageSmoothing33V1副程式
BMP24RGB *ImageSmoothing33V2(const BMP24RGB *image,const int xsize,const int ysize)
//	ImageSmoothing33V2(BMP圖檔3*3Mask平滑濾波)副程式
{																		//	進入ImageSmoothing33V2副程式
	long double Mask[3 * 3];											//	宣告Mask陣列
	Mask[6] = (long double)1;	Mask[7] = (long double)1;	Mask[8] = (long double)1;
	//	給定Mask遮罩數值
	Mask[3] = (long double)1;	Mask[4] = (long double)1;	Mask[5] = (long double)1;
	//	給定Mask遮罩數值
	Mask[0] = (long double)1;	Mask[1] = (long double)1;	Mask[2] = (long double)1;
	//	給定Mask遮罩數值
    return BmpConvolution33(image, xsize, ysize, Mask);					//	呼叫BmpConvolution33副程式計算並回傳運算結果
}																		//	結束ImageSmoothing33V2副程式
BMP24RGB *ImageSmoothing55(const BMP24RGB *image,const int xsize,const int ysize)
//	ImageSmoothing55(BMP圖檔5*5Mask平滑濾波)副程式
{																		//	進入ImageSmoothing55副程式
	long double Mask[5 * 5];											//	宣告Mask陣列
	Mask[20] = (long double)1;	Mask[21] = (long double)1;	Mask[22] = (long double)1;	Mask[23] = (long double)1;	Mask[24] = (long double)1;
	Mask[15] = (long double)1;	Mask[16] = (long double)1;	Mask[17] = (long double)1;	Mask[18] = (long double)1;	Mask[19] = (long double)1;
	Mask[10] = (long double)1;	Mask[11] = (long double)1;	Mask[12] = (long double)1;	Mask[13] = (long double)1;	Mask[14] = (long double)1;
	Mask[ 5] = (long double)1;	Mask[ 6] = (long double)1;	Mask[ 7] = (long double)1;	Mask[ 8] = (long double)1;	Mask[ 9] = (long double)1;
	Mask[ 0] = (long double)1;	Mask[ 1] = (long double)1;	Mask[ 2] = (long double)1;	Mask[ 3] = (long double)1;	Mask[ 4] = (long double)1;
	//	給定Mask遮罩數值
    return BmpConvolution55(image, xsize, ysize, Mask);					//	呼叫BmpConvolution55副程式計算並回傳運算結果
}																		//	結束ImageSmoothing55副程式
BMP24RGB *ImageSmoothing77(const BMP24RGB *image,const int xsize,const int ysize)
//	ImageSmoothing77(BMP圖檔7*7Mask平滑濾波)副程式
{																		//	進入ImageSmoothing77副程式
	long double Mask[7 * 7];											//	宣告Mask陣列
	char loop_num = 0;													//	宣告loop_num變數用於迴圈計數，並設定初始值為0
	for(loop_num = 0; loop_num < 7 * 7; loop_num++)						//	以for迴圈給定Mask數值
	{																	//	進入for迴圈
		Mask[loop_num] = (long double)1;								//	給定Mask遮罩數值
	}																	//	結束for迴圈
    return BmpConvolution77(image, xsize, ysize, Mask);					//	呼叫BmpConvolution77副程式計算並回傳運算結果
}																		//	結束ImageSmoothing77副程式
BMP24RGB *ImageSmoothing(const int MaskSize, const BMP24RGB *image, const int xsize, const int ysize)
//	ImageSmoothing(BMP圖檔平滑濾波)副程式
{																		//	進入ImageSmoothing副程式
	long double *Mask;													//	宣告Mask指標
	Mask = (long double*)malloc(MaskSize * MaskSize * sizeof(long double));
	//	配置Mask指標記憶體大小
	int loop_num = 0;													//	宣告loop_num變數用於迴圈計數，並設定初始值為0
	for(loop_num = 0; loop_num < MaskSize * MaskSize; loop_num++)		//	以for迴圈給定Mask數值
	{																	//	進入for迴圈
		Mask[loop_num] = (long double)1;								//	給定Mask遮罩數值
	}																	//	結束for迴圈
    return BmpConvolution(MaskSize, image, xsize, ysize, Mask);			//	呼叫BmpConvolution副程式計算並回傳運算結果
}																		//	結束ImageSmoothing副程式
BMP24RGB *MedianFilter33(const BMP24RGB *image,const int xsize,const int ysize)
//	BMP圖檔3*3Mask中值濾波副程式
{																		//	進入MedianFilter33(BMP圖檔3*3中值濾波)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			unsigned char *SortArray;									//	宣告SortArray為unsigned char型態指標，供像素值排序使用
			SortArray = (unsigned char*)malloc(9 * sizeof(unsigned char));
			//---R像素排序---
			SortArray[0] = image[loop_num-xsize-1].R;					//	將像素資料填入SortArray陣列
			SortArray[1] = image[loop_num-xsize].R;						//	將像素資料填入SortArray陣列
			SortArray[2] = image[loop_num-xsize+1].R;					//	將像素資料填入SortArray陣列
			SortArray[3] = image[loop_num-1].R;							//	將像素資料填入SortArray陣列
			SortArray[4] = image[loop_num].R;							//	將像素資料填入SortArray陣列
			SortArray[5] = image[loop_num+1].R;							//	將像素資料填入SortArray陣列
			SortArray[6] = image[loop_num+xsize-1].R;					//	將像素資料填入SortArray陣列
			SortArray[7] = image[loop_num+xsize].R;						//	將像素資料填入SortArray陣列
			SortArray[8] = image[loop_num+xsize+1].R;					//	將像素資料填入SortArray陣列
			//qsort(SortArray, 9, sizeof(unsigned char), Compare);		//	呼叫qsort排序函數(定義於stdlib.h)
			SortArray = UCharBubbleSort(SortArray,9,0);
			OutputData[loop_num].R = SortArray[4];						//	將排序後中值填入像素資料
			//---G像素排序---
			SortArray[0] = image[loop_num-xsize-1].G;					//	將像素資料填入SortArray陣列
			SortArray[1] = image[loop_num-xsize].G;						//	將像素資料填入SortArray陣列
			SortArray[2] = image[loop_num-xsize+1].G;					//	將像素資料填入SortArray陣列
			SortArray[3] = image[loop_num-1].G;							//	將像素資料填入SortArray陣列
			SortArray[4] = image[loop_num].G;							//	將像素資料填入SortArray陣列
			SortArray[5] = image[loop_num+1].G;							//	將像素資料填入SortArray陣列
			SortArray[6] = image[loop_num+xsize-1].G;					//	將像素資料填入SortArray陣列
			SortArray[7] = image[loop_num+xsize].G;						//	將像素資料填入SortArray陣列
			SortArray[8] = image[loop_num+xsize+1].G;					//	將像素資料填入SortArray陣列
			//qsort(SortArray, 9, sizeof(unsigned char), Compare);		//	呼叫qsort排序函數(定義於stdlib.h)
			UCharBubbleSort(SortArray,9,0);
			OutputData[loop_num].G = SortArray[4];						//	將排序後中值填入像素資料
			//---B像素排序---
			SortArray[0] = image[loop_num-xsize-1].B;					//	將像素資料填入SortArray陣列
			SortArray[1] = image[loop_num-xsize].B;						//	將像素資料填入SortArray陣列
			SortArray[2] = image[loop_num-xsize+1].B;					//	將像素資料填入SortArray陣列
			SortArray[3] = image[loop_num-1].B;							//	將像素資料填入SortArray陣列
			SortArray[4] = image[loop_num].B;							//	將像素資料填入SortArray陣列
			SortArray[5] = image[loop_num+1].B;							//	將像素資料填入SortArray陣列
			SortArray[6] = image[loop_num+xsize-1].B;					//	將像素資料填入SortArray陣列
			SortArray[7] = image[loop_num+xsize].B;						//	將像素資料填入SortArray陣列
			SortArray[8] = image[loop_num+xsize+1].B;					//	將像素資料填入SortArray陣列
			//qsort(SortArray, 9, sizeof(unsigned char), Compare);		//	呼叫qsort排序函數(定義於stdlib.h)
			UCharBubbleSort(SortArray,9,0);
			OutputData[loop_num].B = SortArray[4];						//	將排序後中值填入像素資料
		}																//	結束else敘述		
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束MedianFilter33(BMP圖檔3*3中值濾波)副程式
BMP24RGB *GaussianBlur33V1(const BMP24RGB *image,const int xsize,const int ysize,const long double StandardDeviation)
//	GaussianBlur33V1(3*3高斯濾波)副程式
{																		//	進入GaussianBlur33V1副程式
	#ifdef DebugMode													//	若有定義為DebugMode(除錯模式)
		printf("StandardDeviation：");									//	顯示"StandardDeviation："字串
		ShowLongDouble(StandardDeviation);								//	顯示副程式傳入之StandardDeviation(除錯用)
	#endif																//	結束ifdef敘述
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long double Gaussian00 = NormalDistribution2D(0,0,StandardDeviation);
	//	呼叫NormalDistribution2D副程式計算高斯二維分布(x=0,y=0,常態分布的標準偏差σ=StandardDeviation)
	long double Gaussian01 = NormalDistribution2D(0,1,StandardDeviation);
	//	呼叫NormalDistribution2D副程式計算高斯二維分布(x=0,y=1,常態分布的標準偏差σ=StandardDeviation)，Gaussian01與Gaussian10相同，不重覆計算
	long double Gaussian11 = NormalDistribution2D(1,1,StandardDeviation);
	//	呼叫NormalDistribution2D副程式計算高斯二維分布(x=1,y=1,常態分布的標準偏差σ=StandardDeviation)
	long double weighting = Gaussian00 + 4 * Gaussian01 + 4 * Gaussian11;
	//	計算權重值
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			OutputData[loop_num].R = (
				image[loop_num+xsize-1].R	* Gaussian11 + image[loop_num+xsize].R	* Gaussian01 + image[loop_num+xsize+1].R	* Gaussian11 +
				image[loop_num-1].R 		* Gaussian01 + image[loop_num].R		* Gaussian00 + image[loop_num+1].R 			* Gaussian01 +
				image[loop_num-xsize-1].R	* Gaussian11 + image[loop_num-xsize].R	* Gaussian01 + image[loop_num-xsize+1].R	* Gaussian11
				) / weighting;											//	高斯濾波計算
			OutputData[loop_num].G = (
				image[loop_num+xsize-1].G	* Gaussian11 + image[loop_num+xsize].G	* Gaussian01 + image[loop_num+xsize+1].G	* Gaussian11 +
				image[loop_num-1].G 		* Gaussian01 + image[loop_num].G		* Gaussian00 + image[loop_num+1].G 			* Gaussian01 +
				image[loop_num-xsize-1].G	* Gaussian11 + image[loop_num-xsize].G	* Gaussian01 + image[loop_num-xsize+1].G	* Gaussian11
				) / weighting;											//	高斯濾波計算
			OutputData[loop_num].B = (
				image[loop_num+xsize-1].B	* Gaussian11 + image[loop_num+xsize].B	* Gaussian01 + image[loop_num+xsize+1].B	* Gaussian11 +
				image[loop_num-1].B 		* Gaussian01 + image[loop_num].B		* Gaussian00 + image[loop_num+1].B 			* Gaussian01 +
				image[loop_num-xsize-1].B	* Gaussian11 + image[loop_num-xsize].B	* Gaussian01 + image[loop_num-xsize+1].B	* Gaussian11
				) / weighting;											//	高斯濾波計算
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束GaussianBlur33V1副程式
BMP24RGB *GaussianBlur33V2(const BMP24RGB *image,const int xsize,const int ysize,const long double StandardDeviation)
//	GaussianBlur33V2(3*3高斯濾波)副程式
{																		//	進入GaussianBlur33V2副程式
	long double Mask[9];												//	宣告Mask陣列
	Mask[6] = NormalDistribution2D(-1,1,StandardDeviation);		Mask[7] = NormalDistribution2D(0,1,StandardDeviation);	Mask[8] = NormalDistribution2D(1,1,StandardDeviation);
	//	給定Mask遮罩數值
	Mask[3] = NormalDistribution2D(-1,0,StandardDeviation);		Mask[4] = NormalDistribution2D(0,0,StandardDeviation);	Mask[5] = NormalDistribution2D(1,0,StandardDeviation);
	//	給定Mask遮罩數值
	Mask[0] = NormalDistribution2D(-1,-1,StandardDeviation);	Mask[1] = NormalDistribution2D(0,-1,StandardDeviation);	Mask[2] = NormalDistribution2D(1,-1,StandardDeviation);
	//	給定Mask遮罩數值
    return BmpConvolution33(image, xsize, ysize, Mask);					//	呼叫BmpConvolution33副程式計算並回傳運算結果
}																		//	結束GaussianBlur33V2副程式
BMP24RGB *GaussianBlur(const int MaskSize, const BMP24RGB *image,const int xsize,const int ysize,const long double StandardDeviation)
//	GaussianBlur(高斯濾波)副程式
{																		//	進入GaussianBlur副程式
	long double *Mask;													//	宣告Mask指標
	Mask = (long double*)malloc(MaskSize * MaskSize * sizeof(long double));
	//	配置Mask指標記憶體大小
	int loop_num1 = 0;													//	宣告loop_num1變數用於迴圈計數，並設定初始值為0
	int loop_num2 = 0;													//	宣告loop_num2變數用於迴圈計數，並設定初始值為0
	for(loop_num1 = -floor(MaskSize/2); loop_num1 <= floor(MaskSize/2); loop_num1 ++)
	//	以for迴圈生成Mask權重
	{																	//	進入for迴圈
		for(loop_num2 = -floor(MaskSize/2); loop_num2 <= floor(MaskSize/2); loop_num2 ++)
		//	以for迴圈生成Mask權重
		{																//	進入for迴圈
			Mask[(int)((loop_num1 + floor(MaskSize/2)) * MaskSize + (loop_num2 + floor(MaskSize/2)))] = 
				NormalDistribution2D((long double) loop_num2, (long double) loop_num1, StandardDeviation);
			//	計算Mask權重
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return BmpConvolution(MaskSize, image, xsize, ysize, Mask);			//	呼叫BmpConvolution33副程式計算並回傳運算結果
}																		//	結束GaussianBlur副程式
BMP24RGB *GaussianFigure2D(const int xsize,const int ysize,const long double StandardDeviation)
//	GaussianFigure2D(二維高斯資料圖形生成)副程式
{																		//	進入GaussianFigure2D(二維高斯資料圖形生成)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	int Centerx,Centery;												//	宣告Centerx, Centery記錄圖像中心座標
	Centerx = xsize / 2;												//	計算Centerx數值為圖像寬度/2
	Centery = ysize / 2;												//	計算Centery數值為圖像寬度/2
	long double NormalizeNumber;										//	宣告NormalizeNumber(正規化參數)
	NormalizeNumber = (long double) 127.0 / NormalDistribution2D(0,0,StandardDeviation);
	//	計算正規化參數，用於將二維高斯圖像中心點正規化為255
	int loop_num1, loop_num2;											//	宣告loop_num1, loop_num2區域變數供迴圈使用
	for(loop_num1 = 0; loop_num1 < xsize; loop_num1 ++)					//	以迴圈依序處理像素
	{																	//	進入for迴圈
		for(loop_num2 = 0; loop_num2 < ysize; loop_num2 ++)				//	以迴圈依序處理像素
		{																//	進入for迴圈
			long double Data = 
			NormalDistribution2D(fabs(Centerx - loop_num1), fabs(Centery - loop_num2), StandardDeviation) * NormalizeNumber + 128;
			//	計算圖像像素值並正規化
			OutputData[ loop_num2 * xsize + loop_num1 ].R = Data;		//	寫入R資料
			OutputData[ loop_num2 * xsize + loop_num1 ].G = Data;		//	寫入G資料
			OutputData[ loop_num2 * xsize + loop_num1 ].B = Data;		//	寫入B資料
		}																//	結束for迴圈
	}																	//	結束for迴圈
	return OutputData;													//	回傳二維高斯圖像生成結果
}																		//	結束GaussianFigure2D(二維高斯資料圖形生成)副程式
long double NormalDistribution2D(long double xlocation,long double ylocation,long double StandardDeviation)
//	NormalDistribution2D(二維常態分布計算)副程式
{																		//	進入NormalDistribution2D副程式
	return pow(M_E,-(pow(xlocation,2) + pow(ylocation,2)) / (2 * pow(StandardDeviation,2)))/(2 * M_PI * pow(StandardDeviation,2));
}																		//	結束NormalDistribution2D副程式
BMP24RGB *ImageOCR(const BMP24RGB *image,const int xsize,const int ysize)
//	ImageOCR(影像OCR)副程式
{																		//	進入ImageOCR(影像OCR)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述
	//***初始化OutputData指標資料***
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		OutputData[loop_num].R = 128;									//	設定像素初始值
		OutputData[loop_num].G = 128;									//	設定像素初始值
		OutputData[loop_num].B = 128;									//	設定像素初始值
	}																	//	結束for迴圈
	bool XArray[1700];													//	宣告XArray布林變數陣列
	bool YArray[2340];													//	宣告YArray布林變數陣列
	//***初始化布林變數陣列***
	for(loop_num = 0; loop_num < 1700; loop_num++)						//	以for迴圈初始化XArray布林變數陣列
	{																	//	進入for迴圈
		XArray[loop_num] = false;										//	將false填入XArray布林變數陣列
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 2340; loop_num++)						//	以for迴圈初始化YArray布林變數陣列
	{																	//	進入for迴圈
		YArray[loop_num] = false;										//	將false填入YArray布林變數陣列
	}																	//	結束for迴圈
	//***水平掃描OCR圖像***
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序掃描圖像像素
	{																	//	進入for迴圈
		//---忽略邊界像素點(將index轉換為二維座標)---
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num / xsize);						//	計算PixelLocationY
		if( (PixelLocationX < 150) || (PixelLocationX > (xsize - 150)) || (PixelLocationY < 150) || (PixelLocationY > (ysize - 150)))
		//	檢測邊界點像素(像素點與邊界最短距離若小於150)
		{																//	進入if敘述
			
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if( (image[loop_num].R == 0) || (image[loop_num].G == 0) || (image[loop_num].B == 0) )
			//	若R、G、B像素值任一為0(可能為文字)
			{															//	進入if敘述
				YArray[PixelLocationY] = true;							//	將布林變數改為true
			}															//	結束if敘述
		}																//	結束else敘述
	}																	//	結束for迴圈	
	//***垂直掃描OCR圖像***
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序掃描圖像像素
	{																	//	進入for迴圈
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num / xsize);						//	計算PixelLocationY
		if( (PixelLocationX < 150) || (PixelLocationX > (xsize - 150)) || (PixelLocationY < 150) || (PixelLocationY > (ysize - 150)))
		//	檢測邊界點像素(像素點與邊界最短距離若小於150)
		{																//	進入if敘述
			
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if( (image[loop_num].R == 0) || (image[loop_num].G == 0) || (image[loop_num].B == 0) )
			//	若R、G、B像素值任一為0(可能為文字)
			{															//	進入if敘述
				YArray[PixelLocationY] = true;							//	將布林變數改為true
			}															//	結束if敘述
		}																//	結束else敘述
	}																	//	結束for迴圈	
	//---輸出掃描結果---
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		int PixelLocationX;												//	宣告PixelLocationX，記錄像素點位於圖像二維座標中之X方向位置
		int PixelLocationY;												//	宣告PixelLocationY，記錄像素點位於圖像二維座標中之Y方向位置
		PixelLocationX = loop_num % xsize;								//	計算PixelLocationX
		PixelLocationY = floor(loop_num / xsize);						//	計算PixelLocationY
		if( (XArray[PixelLocationX] == true) && (YArray[PixelLocationY] == true))
		{
			OutputData[loop_num].R = image[loop_num].R;								
			OutputData[loop_num].G = image[loop_num].G;								
			OutputData[loop_num].B = image[loop_num].B;								
		}
	}																	//	結束for迴圈
	return OutputData;
	#ifdef DebugMode													//	若有定義為DebugMode(除錯模式)
	//---顯示掃描結果---
	printf("OCR test\n");
	bool temp;
	int CountNum,CountNum2;
	/*temp = XArray[0];
	CountNum=0;
	CountNum2=0;
	printf("XArray:\n%d:",(int)temp);
	for(loop_num = 0; loop_num < 1700; loop_num++)						//	以for迴圈初始化XArray布林變數陣列
	{																	//	進入for迴圈
		if(XArray[loop_num] == temp)
		{
			CountNum++;
			CountNum2++;
		}
		else
		{
			temp = XArray[loop_num];
			printf("%d(%d)\n%d:",CountNum,CountNum2,(int)temp);
			CountNum=1;
		}
	}																	//	結束for迴圈
	printf("%d",CountNum);*/
	temp = YArray[0];
	CountNum=0;
	CountNum2=0;
	printf("\nYArray:\n%d:",(int)temp);
	for(loop_num = 0; loop_num < 2340; loop_num++)						//	以for迴圈初始化YArray布林變數陣列
	{																	//	進入for迴圈
		if(YArray[loop_num] == temp)
		{
			CountNum++;
			CountNum2++;
		}
		else
		{
			temp = YArray[loop_num];
			printf("%d(%d)\n%d:",CountNum,ysize-CountNum2,(int)temp);
			CountNum=1;
		}
	}																	//	結束for迴圈
	printf("%d",CountNum);
	#endif																//	結束ifdef敘述
}																		//	結束ImageOCR(影像OCR)副程式
BMP24RGB *BMP24RGBGradient(const BMP24RGB *image,const int xsize,const int ysize)
//	BMP24RGBGradient(BMP圖片資料梯度計算)副程式
{																		//	進入BMP24RGBGradient(BMP圖片資料梯度計算)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素梯度
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			//***計算R像素平面梯度***
			int Gx = 0, Gy = 0;											//	宣告梯度計算結果Gx、Gy，並給定初始值為0
			Gx = (
				image[loop_num+xsize-1].R	* (-1) + image[loop_num+xsize].R	* 0 + image[loop_num+xsize+1].R	* 1 +
				image[loop_num-1].R 		* (-1) + image[loop_num].R			* 0 + image[loop_num+1].R 		* 1 +
				image[loop_num-xsize-1].R	* (-1) + image[loop_num-xsize].R	* 0 + image[loop_num-xsize+1].R	* 1
				);														//	Gx計算
			Gy = (
				image[loop_num+xsize-1].R	* (-1) + image[loop_num+xsize].R	* (-1) + image[loop_num+xsize+1].R	* (-1) +
				image[loop_num-1].R 		*   0  + image[loop_num].R			*   0  + image[loop_num+1].R 		*   0  +
				image[loop_num-xsize-1].R	*   1  + image[loop_num-xsize].R	*   1  + image[loop_num-xsize+1].R	*   1
				);														//	Gy計算
			long double Magnitude, Direction;							//	宣告Magnitude與Direction變數記錄梯度大小與方向
			Magnitude = sqrt(pow(Gx, 2) + pow(Gy, 2));					//	計算Magnitude
			if((Gx > 0) && (Gy >= 0)) 									//	若Gx大於0且Gy大於等於0
			{															//	進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	結束if敘述
			else if(Gx < 0)												//	若Gx小於0										
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)180;
				//	計算梯度方向(Direction)－第二象限、第三象限
			}															//	結束else if敘述
			else if( (Gx > 0) && (Gy < 0) )								//	若Gx大於0且Gy小於0
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)360;
				//	計算梯度方向(Direction)－第四象限
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy > 0) )							//	若Gx為0且Gy大於0
			{															//	進入else if敘述
				Direction = (long double)90;							//	梯度為90度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy < 0) )							//	若Gx為0且Gy小於0
			{															//	進入else if敘述
				Direction = (long double)270;							//	梯度為270度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy == 0) )							//	若Gx為0且Gy等於0
			{															//	進入else if敘述
				Direction = 0;											//	梯度方向未定義
			}															//	結束else if敘述
			
			OutputData[loop_num].R = (Magnitude > 255)?255:(Magnitude < 0)?0:(unsigned char)Magnitude;
			//	將計算結果填入像素資料
			
			//***計算G像素平面梯度***
			Gx = (
				image[loop_num+xsize-1].G	* (-1) + image[loop_num+xsize].G	* 0 + image[loop_num+xsize+1].G	* 1 +
				image[loop_num-1].G 		* (-1) + image[loop_num].G			* 0 + image[loop_num+1].G 		* 1 +
				image[loop_num-xsize-1].G	* (-1) + image[loop_num-xsize].G	* 0 + image[loop_num-xsize+1].G	* 1
				);														//	Gx計算
			Gy = (
				image[loop_num+xsize-1].G	* (-1) + image[loop_num+xsize].G	* (-1) + image[loop_num+xsize+1].G	* (-1) +
				image[loop_num-1].G 		*   0  + image[loop_num].G			*   0  + image[loop_num+1].G 		*   0  +
				image[loop_num-xsize-1].G	*   1  + image[loop_num-xsize].G	*   1  + image[loop_num-xsize+1].G	*   1
				);														//	Gy計算
			Magnitude = sqrt(pow(Gx, 2) + pow(Gy, 2));					//	計算Magnitude
			if((Gx > 0) && (Gy >= 0)) 									//	若Gx大於0且Gy大於等於0
			{															//	進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	結束if敘述
			else if(Gx < 0)												//	若Gx小於0										
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)180;
				//	計算梯度方向(Direction)－第二象限、第三象限
			}															//	結束else if敘述
			else if( (Gx > 0) && (Gy < 0) )								//	若Gx大於0且Gy小於0
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)360;
				//	計算梯度方向(Direction)－第四象限
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy > 0) )							//	若Gx為0且Gy大於0
			{															//	進入else if敘述
				Direction = (long double)90;							//	梯度為90度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy < 0) )							//	若Gx為0且Gy小於0
			{															//	進入else if敘述
				Direction = (long double)270;							//	梯度為270度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy == 0) )							//	若Gx為0且Gy等於0
			{															//	進入else if敘述
				Direction = 0;											//	梯度方向未定義
			}															//	結束else if敘述
			
			OutputData[loop_num].G = (Magnitude > 255)?255:(Magnitude < 0)?0:(unsigned char)Magnitude;
			//	將計算結果填入像素資料
			
			//***計算B像素平面梯度***
			Gx = (
				image[loop_num+xsize-1].B	* (-1) + image[loop_num+xsize].B	* 0 + image[loop_num+xsize+1].B	* 1 +
				image[loop_num-1].B 		* (-1) + image[loop_num].B			* 0 + image[loop_num+1].B 		* 1 +
				image[loop_num-xsize-1].B	* (-1) + image[loop_num-xsize].B	* 0 + image[loop_num-xsize+1].B	* 1
				);														//	Gx計算
			Gy = (
				image[loop_num+xsize-1].B	* (-1) + image[loop_num+xsize].B	* (-1) + image[loop_num+xsize+1].B	* (-1) +
				image[loop_num-1].B 		*   0  + image[loop_num].B			*   0  + image[loop_num+1].B 		*   0  +
				image[loop_num-xsize-1].B	*   1  + image[loop_num-xsize].B	*   1  + image[loop_num-xsize+1].B	*   1
				);														//	Gy計算
			Magnitude = sqrt(pow(Gx, 2) + pow(Gy, 2));					//	計算Magnitude
			if((Gx > 0) && (Gy >= 0)) 									//	若Gx大於0且Gy大於等於0
			{															//	進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	結束if敘述
			else if(Gx < 0)												//	若Gx小於0										
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)180;
				//	計算梯度方向(Direction)－第二象限、第三象限
			}															//	結束else if敘述
			else if( (Gx > 0) && (Gy < 0) )								//	若Gx大於0且Gy小於0
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)360;
				//	計算梯度方向(Direction)－第四象限
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy > 0) )							//	若Gx為0且Gy大於0
			{															//	進入else if敘述
				Direction = (long double)90;							//	梯度為90度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy < 0) )							//	若Gx為0且Gy小於0
			{															//	進入else if敘述
				Direction = (long double)270;							//	梯度為270度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy == 0) )							//	若Gx為0且Gy等於0
			{															//	進入else if敘述
				Direction = 0;											//	梯度方向未定義
			}															//	結束else if敘述
			
			OutputData[loop_num].B = (Magnitude > 255)?255:(Magnitude < 0)?0:(unsigned char)Magnitude;
			//	將計算結果填入像素資料
			
		}																//	結束else敘述		
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束BMP24RGBGradient(BMP圖片資料梯度計算)副程式
BMP24RGB *BMP24RGBSobelEdge(const BMP24RGB *image,const int xsize,const int ysize)
//	BMP24RGBSobelEdge(索貝爾算子邊緣偵測)副程式
{																		//	進入BMP24RGBSobelEdge(索貝爾算子邊緣偵測)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素梯度
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			//***計算R像素平面梯度***
			int Gx = 0, Gy = 0;											//	宣告梯度計算結果Gx、Gy，並給定初始值為0
			Gx = (
				image[loop_num+xsize-1].R	* (-1) + image[loop_num+xsize].R	* 0 + image[loop_num+xsize+1].R	* 1 +
				image[loop_num-1].R 		* (-2) + image[loop_num].R			* 0 + image[loop_num+1].R 		* 2 +
				image[loop_num-xsize-1].R	* (-1) + image[loop_num-xsize].R	* 0 + image[loop_num-xsize+1].R	* 1
				);														//	Gx計算
			Gy = (
				image[loop_num+xsize-1].R	* (-1) + image[loop_num+xsize].R	* (-2) + image[loop_num+xsize+1].R	* (-1) +
				image[loop_num-1].R 		*   0  + image[loop_num].R			*   0  + image[loop_num+1].R 		*   0  +
				image[loop_num-xsize-1].R	*   1  + image[loop_num-xsize].R	*   2  + image[loop_num-xsize+1].R	*   1
				);														//	Gy計算
			long double Magnitude, Direction;							//	宣告Magnitude與Direction變數記錄梯度大小與方向
			Magnitude = sqrt(pow(Gx, 2) + pow(Gy, 2));					//	計算Magnitude
			if((Gx > 0) && (Gy >= 0)) 									//	若Gx大於0且Gy大於等於0
			{															//	進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	結束if敘述
			else if(Gx < 0)												//	若Gx小於0										
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)180;
				//	計算梯度方向(Direction)－第二象限、第三象限
			}															//	結束else if敘述
			else if( (Gx > 0) && (Gy < 0) )								//	若Gx大於0且Gy小於0
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)360;
				//	計算梯度方向(Direction)－第四象限
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy > 0) )							//	若Gx為0且Gy大於0
			{															//	進入else if敘述
				Direction = (long double)90;							//	梯度為90度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy < 0) )							//	若Gx為0且Gy小於0
			{															//	進入else if敘述
				Direction = (long double)270;							//	梯度為270度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy == 0) )							//	若Gx為0且Gy等於0
			{															//	進入else if敘述
				Direction = 0;											//	梯度方向未定義
			}															//	結束else if敘述
			
			OutputData[loop_num].R = (Magnitude > 255)?255:(Magnitude < 0)?0:(unsigned char)Magnitude;
			//	將計算結果填入像素資料
			
			//***計算G像素平面梯度***
			Gx = (
				image[loop_num+xsize-1].G	* (-1) + image[loop_num+xsize].G	* 0 + image[loop_num+xsize+1].G	* 1 +
				image[loop_num-1].G 		* (-2) + image[loop_num].G			* 0 + image[loop_num+1].G 		* 2 +
				image[loop_num-xsize-1].G	* (-1) + image[loop_num-xsize].G	* 0 + image[loop_num-xsize+1].G	* 1
				);														//	Gx計算
			Gy = (
				image[loop_num+xsize-1].G	* (-1) + image[loop_num+xsize].G	* (-2) + image[loop_num+xsize+1].G	* (-1) +
				image[loop_num-1].G 		*   0  + image[loop_num].G			*   0  + image[loop_num+1].G 		*   0  +
				image[loop_num-xsize-1].G	*   1  + image[loop_num-xsize].G	*   2  + image[loop_num-xsize+1].G	*   1
				);														//	Gy計算
			Magnitude = sqrt(pow(Gx, 2) + pow(Gy, 2));					//	計算Magnitude
			if((Gx > 0) && (Gy >= 0)) 									//	若Gx大於0且Gy大於等於0
			{															//	進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	結束if敘述
			else if(Gx < 0)												//	若Gx小於0										
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)180;
				//	計算梯度方向(Direction)－第二象限、第三象限
			}															//	結束else if敘述
			else if( (Gx > 0) && (Gy < 0) )								//	若Gx大於0且Gy小於0
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)360;
				//	計算梯度方向(Direction)－第四象限
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy > 0) )							//	若Gx為0且Gy大於0
			{															//	進入else if敘述
				Direction = (long double)90;							//	梯度為90度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy < 0) )							//	若Gx為0且Gy小於0
			{															//	進入else if敘述
				Direction = (long double)270;							//	梯度為270度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy == 0) )							//	若Gx為0且Gy等於0
			{															//	進入else if敘述
				Direction = 0;											//	梯度方向未定義
			}															//	結束else if敘述
			
			OutputData[loop_num].G = (Magnitude > 255)?255:(Magnitude < 0)?0:(unsigned char)Magnitude;
			//	將計算結果填入像素資料
			
			//***計算B像素平面梯度***
			Gx = (
				image[loop_num+xsize-1].B	* (-1) + image[loop_num+xsize].B	* 0 + image[loop_num+xsize+1].B	* 1 +
				image[loop_num-1].B 		* (-2) + image[loop_num].B			* 0 + image[loop_num+1].B 		* 2 +
				image[loop_num-xsize-1].B	* (-1) + image[loop_num-xsize].B	* 0 + image[loop_num-xsize+1].B	* 1
				);														//	Gx計算
			Gy = (
				image[loop_num+xsize-1].B	* (-1) + image[loop_num+xsize].B	* (-2) + image[loop_num+xsize+1].B	* (-1) +
				image[loop_num-1].B 		*   0  + image[loop_num].B			*   0  + image[loop_num+1].B 		*   0  +
				image[loop_num-xsize-1].B	*   1  + image[loop_num-xsize].B	*   2  + image[loop_num-xsize+1].B	*   1
				);														//	Gy計算
			Magnitude = sqrt(pow(Gx, 2) + pow(Gy, 2));					//	計算Magnitude
			if((Gx > 0) && (Gy >= 0)) 									//	若Gx大於0且Gy大於等於0
			{															//	進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	結束if敘述
			else if(Gx < 0)												//	若Gx小於0										
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)180;
				//	計算梯度方向(Direction)－第二象限、第三象限
			}															//	結束else if敘述
			else if( (Gx > 0) && (Gy < 0) )								//	若Gx大於0且Gy小於0
			{															//	進入else if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI) + (long double)360;
				//	計算梯度方向(Direction)－第四象限
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy > 0) )							//	若Gx為0且Gy大於0
			{															//	進入else if敘述
				Direction = (long double)90;							//	梯度為90度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy < 0) )							//	若Gx為0且Gy小於0
			{															//	進入else if敘述
				Direction = (long double)270;							//	梯度為270度
			}															//	結束else if敘述
			else if( (Gx == 0) && (Gy == 0) )							//	若Gx為0且Gy等於0
			{															//	進入else if敘述
				Direction = 0;											//	梯度方向未定義
			}															//	結束else if敘述
			
			OutputData[loop_num].B = (Magnitude > 255)?255:(Magnitude < 0)?0:(unsigned char)Magnitude;
			//	將計算結果填入像素資料
			
		}																//	結束else敘述		
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束BMP24RGBSobelEdge(索貝爾算子邊緣偵測)副程式
BMP24RGB *RGBHistogramEqualization(const BMP24RGB *image,const int xsize,const int ysize)
//	RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式
{																		//	進入RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述
	//***初始化OutputData指標資料***
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		OutputData[loop_num].R = 0;										//	設定像素初始值
		OutputData[loop_num].G = 0;										//	設定像素初始值
		OutputData[loop_num].B = 0;										//	設定像素初始值
	}																	//	結束for迴圈
	long long int CountPixel[256];										//	宣告CountPixel陣列統計各像素值出現次數
	//***以for迴圈依序初始化CountPixel統計值為0***
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈初始化CountPixel統計值為0
	{																	//	進入for迴圈
		CountPixel[loop_num] = 0;										//	初始化CountPixel統計值為0
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		CountPixel[image[loop_num].R] = CountPixel[image[loop_num].R] + 1;
		//	計算R像素值出現次數
	}																	//	結束for迴圈
	unsigned long long int PDF[256],CDF[256];							//	宣告PDF與CDF陣列，計算個別像素值機率與累計機率
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序計算PDF
	{																	//	進入for迴圈
		PDF[loop_num] = CountPixel[loop_num];							//	計算像素值PDF
	}																	//	結束for迴圈
	//system("pause");
	CDF[0] = PDF[0];													//	填入像素值為0的CDF
	for(loop_num = 1; loop_num < 256; loop_num++)						//	以for迴圈依序計算CDF
	{																	//	進入for迴圈
		CDF[loop_num] = CDF[loop_num - 1] + PDF[loop_num];				//	計算像素值CDF
	}																	//	結束for迴圈
	unsigned char ResultPixel[256];										//	宣告ResultPixel陣列記錄HistogramEqualization後像素值
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序HistogramEqualization後像素值
	{																	//	進入for迴圈
		ResultPixel[loop_num] = CDF[loop_num] * 255 / (xsize * ysize);	//	計算HistogramEqualization後像素值
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈輸出結果至OutputData
	{																	//	進入for迴圈
		OutputData[loop_num].R = ResultPixel[image[loop_num].R];		//	輸出結果至OutputData
	}																	//	結束for迴圈
	
	//***以for迴圈依序初始化CountPixel統計值為0***
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈初始化CountPixel統計值為0
	{																	//	進入for迴圈
		CountPixel[loop_num] = 0;										//	初始化CountPixel統計值為0
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		CountPixel[image[loop_num].G] = CountPixel[image[loop_num].G] + 1;
		//	計算G像素值出現次數
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序計算PDF
	{																	//	進入for迴圈
		PDF[loop_num] = CountPixel[loop_num];							//	計算像素值PDF
	}																	//	結束for迴圈
	CDF[0] = PDF[0];													//	填入像素值為0的CDF
	for(loop_num = 1; loop_num < 256; loop_num++)						//	以for迴圈依序計算CDF
	{																	//	進入for迴圈
		CDF[loop_num] = CDF[loop_num - 1] + PDF[loop_num];				//	計算像素值CDF
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序HistogramEqualization後像素值
	{																	//	進入for迴圈
		ResultPixel[loop_num] = CDF[loop_num] * 255 / (xsize * ysize);	//	計算HistogramEqualization後像素值
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈輸出結果至OutputData
	{																	//	進入for迴圈
		OutputData[loop_num].G = ResultPixel[image[loop_num].G];		//	輸出結果至OutputData
	}																	//	結束for迴圈
	
	//***以for迴圈依序初始化CountPixel統計值為0***
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈初始化CountPixel統計值為0
	{																	//	進入for迴圈
		CountPixel[loop_num] = 0;										//	初始化CountPixel統計值為0
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		CountPixel[image[loop_num].B] = CountPixel[image[loop_num].B] + 1;
		//	計算B像素值出現次數
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序計算PDF
	{																	//	進入for迴圈
		PDF[loop_num] = CountPixel[loop_num];							//	計算像素值PDF
	}																	//	結束for迴圈
	CDF[0] = PDF[0];													//	填入像素值為0的CDF
	for(loop_num = 1; loop_num < 256; loop_num++)						//	以for迴圈依序計算CDF
	{																	//	進入for迴圈
		CDF[loop_num] = CDF[loop_num - 1] + PDF[loop_num];				//	計算像素值CDF
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序HistogramEqualization後像素值
	{																	//	進入for迴圈
		ResultPixel[loop_num] = CDF[loop_num] * 255 / (xsize * ysize);	//	計算HistogramEqualization後像素值
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈輸出結果至OutputData
	{																	//	進入for迴圈
		OutputData[loop_num].B = ResultPixel[image[loop_num].B];		//	輸出結果至OutputData
	}																	//	結束for迴圈
	return OutputData;													//	回傳HistogramEqualization運算結果
}																		//	結束RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式
BMP24RGB *BMPHaarWavelet(const BMP24RGB *image,const int xsize,const int ysize, const char mode)
//	BMPHaarWavelet(BMP圖檔Haar小波濾波)副程式
{																		//	進入BMPHaarWavelet副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if(mode==HorizontalHighPass)								//	若mode為垂直高通模式
			{															//	進入if敘述
				OutputData[loop_num].R = abs(
					image[loop_num-1].R 	* (-1) + image[loop_num].R		* 0 + image[loop_num+1].R 	* 1
					);													//	垂直高通模式計算
				OutputData[loop_num].G = abs(
					image[loop_num-1].G 	* (-1) + image[loop_num].G		* 0 + image[loop_num+1].G 	* 1
					);													//	垂直高通模式計算
				OutputData[loop_num].B = abs(
					image[loop_num-1].B 	* (-1) + image[loop_num].B		* 0 + image[loop_num+1].B 	* 1
					);													//	垂直高通模式計算
			}															//	結束if敘述
			else if(mode==HorizontalLowPass)							//	若mode為垂直低通模式
			{															//	進入else if敘述
				OutputData[loop_num].R = (
					image[loop_num-1].R 	* 1 + image[loop_num].R		* 0 + image[loop_num+1].R 	* 1
					) / 2;												//	垂直低通模式計算
				OutputData[loop_num].G = (
					image[loop_num-1].G 	* 1 + image[loop_num].G		* 0 + image[loop_num+1].G 	* 1
					) / 2;												//	垂直低通模式計算
				OutputData[loop_num].B = (
					image[loop_num-1].B 	* 1 + image[loop_num].B		* 0 + image[loop_num+1].B 	* 1
					) / 2;												//	垂直低通模式計算
			}															//	結束else if敘述
			else if(mode==VerticalHighPass)								//	若mode為水平高通模式
			{															//	進入else if敘述
				OutputData[loop_num].R = abs(
								image[loop_num+xsize].R	* (-1) +
								image[loop_num].R		* 0    +
								image[loop_num-xsize].R	* 1
					);													//	水平高通模式計算
				OutputData[loop_num].G = abs(
								image[loop_num+xsize].G	* (-1) +
								image[loop_num].G		* 0    +
								image[loop_num-xsize].G	* 1
					);													//	水平高通模式計算
				OutputData[loop_num].B = abs(
								image[loop_num+xsize].B	* (-1) +
								image[loop_num].B		* 0    +
								image[loop_num-xsize].B	* 1
					);													//	水平高通模式計算
			}															//	結束else if敘述
			else if(mode==VerticalLowPass)								//	若mode為水平低通模式
			{															//	進入else if敘述
				OutputData[loop_num].R = (
								image[loop_num+xsize].R	* 1 +
								image[loop_num].R		* 0 +
								image[loop_num-xsize].R	* 1
					) / 2;												//	水平低通模式計算
				OutputData[loop_num].G = (
								image[loop_num+xsize].G	* 1 +
								image[loop_num].G		* 0 +
								image[loop_num-xsize].G	* 1
					) / 2;												//	水平低通模式計算
				OutputData[loop_num].B = (
								image[loop_num+xsize].B	* 1 +
								image[loop_num].B		* 0 +
								image[loop_num-xsize].B	* 1
					) / 2;												//	水平低通模式計算
			}															//	結束else if敘述
		}																//	結束else敘述
	}																	//	結束for迴圈
    return OutputData;													//	回傳運算結果
}																		//	結束BMPHaarWavelet副程式
BMP24RGB *BMPHaarWavelet2(const BMP24RGB *image,const int xsize,const int ysize, const char mode)
//	BMPHaarWavelet2(BMP圖檔二階Haar小波濾波)副程式
{																		//	進入BMPHaarWavelet2副程式
	BMP24RGB *OutputData;												//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	if(mode == HighHigh)												//	若模式為HighHigh
	{																	//	進入if敘述
		OutputData = BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalHighPass);
		//	呼叫BMPHaarWavelet副程式HorizontalHighPass模式，再呼叫BMPHaarWavelet副程式VerticalHighPass模式
	}																	//	結束if敘述
	else if(mode == HighLow)											//	若模式為HighLow
	{																	//	進入else if敘述
		OutputData = BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalLowPass);
		//	呼叫BMPHaarWavelet副程式HorizontalHighPass模式，再呼叫BMPHaarWavelet副程式VerticalLowPass模式
	}																	//	結束else if敘述
	else if(mode == LowHigh)											//	若模式為LowHigh
	{																	//	進入else if敘述
		OutputData = BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalLowPass),xsize,ysize,VerticalHighPass);
		//	呼叫BMPHaarWavelet副程式HorizontalLowPass模式，再呼叫BMPHaarWavelet副程式VerticalHighPass模式
	}																	//	結束else if敘述
	else if(mode == LowLow)												//	若模式為LowLow
	{																	//	進入else if敘述
		OutputData = BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalLowPass),xsize,ysize,VerticalLowPass);
		//	呼叫BMPHaarWavelet副程式HorizontalLowPass模式，再呼叫BMPHaarWavelet副程式VerticalLowPass模式
	}																	//	結束else if敘述
    else if(mode == (HighHigh|HighLow) )								//	若模式為HighHigh與HighLow混和
	{																	//	進入else if敘述
		OutputData = BMP24RGB2or(BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalHighPass),
								 BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalLowPass),
								 xsize,ysize);
		//	將HighHigh結果與HighLow結果混合(or運算)
	}																	//	結束else if敘述
	else if(mode == (HighHigh|LowHigh) )								//	若模式為HighHigh與LowHigh混和
	{																	//	進入else if敘述
		OutputData = BMP24RGB2or(BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalHighPass),
								 BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalLowPass),xsize,ysize,VerticalHighPass),
								 xsize,ysize);
		//	將HighHigh結果與LowHigh結果混合(or運算)
	}																	//	結束else if敘述
	else if(mode == (HighLow|LowHigh) )									//	若模式為HighLow與LowHigh混和
	{																	//	進入else if敘述
		OutputData = BMP24RGB2or(BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalLowPass),
								 BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalLowPass),xsize,ysize,VerticalHighPass),
								 xsize,ysize);
		//	將HighLow結果與LowHigh結果混合(or運算)
	}																	//	結束else if敘述
	else if(mode == (HighHigh|HighLow|LowHigh) )						//	若模式為HighHigh、HighLow與LowHigh混和
	{																	//	進入else if敘述
		OutputData = BMP24RGB2or(BMP24RGB2or(BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalHighPass),
								 BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalLowPass),
								 xsize,ysize),
								 BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalLowPass),xsize,ysize,VerticalHighPass),
								 xsize,ysize);
		//	將HighHigh結果、HighLow結果與LowHigh結果混合(or運算)
	}																	//	結束else if敘述
	return OutputData;													//	回傳運算結果
}																		//	結束BMPHaarWavelet2副程式
HSV *HSVHistogramEqualization(const HSV *image,const int xsize,const int ysize)
//	HSVHistogramEqualization(HSV影像直方圖等化)副程式
{																		//	進入HSVHistogramEqualization(HSV影像直方圖等化)副程式
	HSV *OutputData;													//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (HSV*)malloc(xsize * ysize * sizeof(HSV));				//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述
	//***初始化OutputData指標資料***
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		OutputData[loop_num].H = image[loop_num].H;						//	設定像素初始值
		OutputData[loop_num].S = image[loop_num].S;						//	設定像素初始值
		OutputData[loop_num].V = 0;										//	設定像素初始值
	}																	//	結束for迴圈
	long long int CountPixel[256];										//	宣告CountPixel陣列統計各像素值出現次數
	//***以for迴圈依序初始化CountPixel統計值為0***
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈初始化CountPixel統計值為0
	{																	//	進入for迴圈
		CountPixel[loop_num] = 0;										//	初始化CountPixel統計值為0
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		CountPixel[(int)(image[loop_num].V)] = CountPixel[(int)(image[loop_num].V)] + 1;
		//	計算Value值出現次數
	}																	//	結束for迴圈
	unsigned long long int PDF[256],CDF[256];							//	宣告PDF與CDF陣列，計算個別像素值機率與累計機率
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序計算PDF
	{																	//	進入for迴圈
		PDF[loop_num] = CountPixel[loop_num];							//	計算像素值PDF
	}																	//	結束for迴圈
	//system("pause");
	CDF[0] = PDF[0];													//	填入像素值為0的CDF
	for(loop_num = 1; loop_num < 256; loop_num++)						//	以for迴圈依序計算CDF
	{																	//	進入for迴圈
		CDF[loop_num] = CDF[loop_num - 1] + PDF[loop_num];				//	計算像素值CDF
	}																	//	結束for迴圈
	unsigned char ResultPixel[256];										//	宣告ResultPixel陣列記錄HistogramEqualization後像素值
	for(loop_num = 0; loop_num < 256; loop_num++)						//	以for迴圈依序HistogramEqualization後像素值
	{																	//	進入for迴圈
		ResultPixel[loop_num] = CDF[loop_num] * 255 / (xsize * ysize);	//	計算HistogramEqualization後像素值
	}																	//	結束for迴圈
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈輸出結果至OutputData
	{																	//	進入for迴圈
		OutputData[loop_num].V = ResultPixel[(int)(image[loop_num].V)];	//	輸出結果至OutputData
	}																	//	結束for迴圈
	return OutputData;													//	回傳HistogramEqualization運算結果
}																		//	結束HSVHistogramEqualization(HSV影像直方圖等化)副程式
BMP24RGB *HueToBMP24RGB(const HSV *image,const int xsize,const int ysize)
//	HueToBMP24RGB副程式
{																		//	進入HueToBMP24RGB副程式
	BMP24RGB *OutputImage;												//	宣告OutputImage指標變數，記錄輸出圖像資料
	OutputImage = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));
	//	配置OutputImage指標記憶體大小
	if (OutputImage == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int LoopNumber;											//	宣告LoopNumber區域變數供迴圈使用
	for(LoopNumber = 0; LoopNumber <(xsize * ysize); LoopNumber++)		//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		OutputImage[LoopNumber].R = image[LoopNumber].H * (long double)255 / (long double)360;
		//	填入影像資料
		OutputImage[LoopNumber].G = image[LoopNumber].H * (long double)255 / (long double)360;
		//	填入影像資料
		OutputImage[LoopNumber].B = image[LoopNumber].H * (long double)255 / (long double)360;
		//	填入影像資料
	}																	//	結束for迴圈
    return OutputImage;													//	回傳輸出圖像
}																		//	結束HueToBMP24RGB副程式
BMP24RGB *SaturationToBMP24RGB(const HSV *image,const int xsize,const int ysize)
//	SaturationToBMP24RGB副程式
{																		//	進入SaturationToBMP24RGB副程式
	BMP24RGB *OutputImage;												//	宣告OutputImage指標變數，記錄輸出圖像資料
	OutputImage = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));
	//	配置OutputImage指標記憶體大小
	if (OutputImage == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int LoopNumber;											//	宣告LoopNumber區域變數供迴圈使用
	for(LoopNumber = 0; LoopNumber <(xsize * ysize); LoopNumber++)		//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		OutputImage[LoopNumber].R = image[LoopNumber].S * (long double)255;
		//	填入影像資料
		OutputImage[LoopNumber].G = image[LoopNumber].S * (long double)255;
		//	填入影像資料
		OutputImage[LoopNumber].B = image[LoopNumber].S * (long double)255;
		//	填入影像資料
	}																	//	結束for迴圈
    return OutputImage;													//	回傳輸出圖像
}																		//	結束SaturationToBMP24RGB副程式
BMP24RGB *ValueToBMP24RGB(const HSV *image,const int xsize,const int ysize)
//	ValueToBMP24RGB副程式
{																		//	進入ValueToBMP24RGB副程式
	BMP24RGB *OutputImage;												//	宣告OutputImage指標變數，記錄輸出圖像資料
	OutputImage = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputImage指標記憶體大小
	if (OutputImage == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述 
	long long int LoopNumber;											//	宣告LoopNumber區域變數供迴圈使用
	for(LoopNumber = 0; LoopNumber <(xsize * ysize); LoopNumber++)		//	以for迴圈依序處理圖像像素
	{																	//	進入for迴圈
		OutputImage[LoopNumber].R = image[LoopNumber].V;				//	填入影像資料
		OutputImage[LoopNumber].G = image[LoopNumber].V;				//	填入影像資料
		OutputImage[LoopNumber].B = image[LoopNumber].V;				//	填入影像資料
	}																	//	結束for迴圈
    return OutputImage;													//	回傳輸出圖像
}																		//	結束ValueToBMP24RGB副程式
HSV *HSVSkin(const HSV *image,const int xsize,const int ysize)
//	HSVSkin(HSV影像皮膚過濾)副程式
{																		//	進入HSVSkin(HSV影像皮膚過濾)副程式
	HSV *OutputData;													//	宣告OutputData指標變數，記錄圖像資料運算結果
	OutputData = (HSV*)malloc(xsize * ysize * sizeof(HSV));				//	配置OutputData指標記憶體大小
	if (OutputData == NULL) 											//	若建立影像空間失敗 
	{																	//	進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	結束if敘述
	//***初始化OutputData指標資料***
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		OutputData[loop_num].H = 0;										//	設定像素初始值
		OutputData[loop_num].S = 0;										//	設定像素初始值
		OutputData[loop_num].V = 0;										//	設定像素初始值
	}																	//	結束for迴圈
	//***過濾皮膚像素點***
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序初始化OutputData圖像像素
	{																	//	進入for迴圈
		long double HData,SData;										//	宣告HData與SData長雙精度浮點數，記錄像素點之H與S資訊
		HData = image[loop_num].H;										//	填入HData
		SData = image[loop_num].S;										//	填入SData
		if( ( HData >= (long double)15.0 ) &&							//	若HData大於等於15
			( HData <= (long double)50.0 ) && 							//	且HData小於等於50
			( SData >= (long double)0.23 ) && 							//	且SData大於等於0.23
			( SData >= (long double)0.68 ) )							//	且SData小於等於0.68
		{																//	進入if敘述
			OutputData[loop_num].H = image[loop_num].H;					//	填入原像素值
			OutputData[loop_num].S = image[loop_num].S;					//	填入原像素值
			OutputData[loop_num].V = image[loop_num].V;					//	填入原像素值
		}																//	結束if敘述
		else															//	若像素色彩資訊不在範圍中
		{																//	進入else敘述
			OutputData[loop_num].H = image[loop_num].H;					//	填入原像素值
			OutputData[loop_num].S = image[loop_num].S;					//	填入原像素值
			OutputData[loop_num].V = image[loop_num].V * (long double)0.3;
			//	填入抑制Value
		}																//	結束else敘述
	}																	//	結束for迴圈
	return OutputData;													//	回傳過濾結果
}																		//	結束HSVSkin(HSV影像皮膚過濾)副程式
