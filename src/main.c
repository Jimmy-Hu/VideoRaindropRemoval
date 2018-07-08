/*	數位影像處理程式(Digital Image Processing program)
	Develop by Jimmy Hu
	This program is licensed under GNU General Public License v3.
	版本資訊可參考VersionInformation.txt檔案
 */
//-----include header files, 引入標頭檔-----
#include <math.h>														//	引入標頭檔math.h
#include <stdbool.h>													//	引入標頭檔stdbool.h
#include <stdio.h>														//	引入標頭檔stdio.h
#include <stdlib.h>														//	引入標頭檔stdlib.h
#include <string.h>														//	引入標頭檔string.h
#include <unistd.h>														//	引入標頭檔unistd.h
//-----Global definition, 全域定義區-----
#define MAX_PATH 256													//	定義檔案路徑最長長度為256字元
#define FILE_ROOT_PATH "..\\Images"
//	定義檔案根目錄路徑位置(用於開啟圖檔)
#define True true														//	定義True為true
#define False false														//	定義False為false
//#define DebugMode														//	定義程式為DebugMode
//-----全域結構、資料型態宣告區-----
typedef struct BMP24RGB													//	宣告24位元BMP圖檔像素RGB資料結構
{																		//	進入BMP24RGB資料結構
	unsigned char R;													//	宣告R成分變數
	unsigned char G;													//	宣告G成分變數
	unsigned char B;													//	宣告B成分變數
}BMP24RGB;																//	結束BMP24RGB資料結構
typedef struct HSV														//	宣告HSV資料結構
{																		//	進入HSV資料結構
	long double H;														//	宣告H成分變數(H為色相Hue，值域為0～360)
	long double S;														//	宣告S成分變數(S為飽和度Saturation，值域為0～1)
	long double V;														//	宣告V成分變數(V為明度Value，值域為0～255)
}HSV;																	//	結束HSV資料結構
/*	BMPIMAGE結構建立BMP影像物件，該物件包含：
		●檔名(FILENAME)，長度最長為MAX_PATH
		●圖像寬度(XSIZE)
		●圖像高度(YSIZE)
		●填補位元(FILLINGBYTE)，配合BMP圖像資料格式
		●圖像資料(IMAGE_DATA)
 */
typedef struct BMPIMAGE													//	宣告BMPIMAGE資料結構
{																		//	進入BMPIMAGE資料結構
	char FILENAME[MAX_PATH];											//	宣告輸入讀取檔案檔名變數FILENAME
	/*	IMAGE_DATA影像指標可使用陣列方式存取，在一張影像中：
　　　 __________________
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |                |
　　　 |________________|
　　　  ↑為第一像素 
　　　 	IMAGE_DATA[0]代表第一像素的藍色(B)，由淡到濃分成8位元，0代表沒有藍色，255代表全藍 
　　　 	IMAGE_DATA[1]代表第一像素的綠色(G)，由淡到濃分成8位元，0代表沒有綠色，255代表全綠
　　　 	IMAGE_DATA[2]代表第一像素的紅色(R)，由淡到濃分成8位元，0代表沒有紅色，255代表全紅
　　　 	IMAGE_DATA[3]代表第二像素的藍色(B)，由淡到濃分成8位元，0代表沒有藍色，255代表全藍 
		IMAGE_DATA[4]代表第二像素的綠色(G)，由淡到濃分成8位元，0代表沒有綠色，255代表全綠
		IMAGE_DATA[5]代表第二像素的紅色(R)，由淡到濃分成8位元，0代表沒有紅色，255代表全紅
		但由於BMP檔案格式可能存在填補位元(當影像寬度不為4的倍數時)，IMAGE_DATA陣列的index對應至圖像像素資料有可能不連續，
		以RAWImageToArray將IMAGE_DATA陣列轉換至BMP24RGB型態二維陣列，與二維圖像完全對應
	*/
	unsigned int XSIZE;													//	宣告X軸像素變數
	unsigned int YSIZE;													//	宣告Y軸像素變數
	unsigned char FILLINGBYTE;											//	宣告填充位元組大小
	unsigned char *IMAGE_DATA;											//	宣告影像資料指標*IMAGE_DATA
}BMPIMAGE;																//	結束BMPIMAGE資料結構
typedef struct BMP24RGBIMAGE											//	宣告BMP24RGBIMAGE資料結構
{																		//	進入BMP24RGBIMAGE資料結構
	unsigned int XSIZE;													//	宣告X軸像素變數
	unsigned int YSIZE;													//	宣告Y軸像素變數
	BMP24RGB *IMAGE_DATA;												//	宣告影像資料指標*IMAGE_DATA
}BMP24RGBIMAGE;															//	結束BMP24RGBIMAGE資料結構
typedef struct HSVIMAGE													//	宣告HSVIMAGE資料結構
{																		//	進入HSVIMAGE資料結構
	unsigned int XSIZE;													//	宣告X軸像素變數
	unsigned int YSIZE;													//	宣告Y軸像素變數
	HSV *IMAGE_DATA;													//	宣告影像資料指標*IMAGE_DATA
}HSVIMAGE;																//	結束HSVIMAGE資料結構
/*	HaarWaveletMode集合中宣告Haar濾波模式，說明如下：
		HorizontalHighPass	－	水平方向高通濾波
		HorizontalLowPass	－	水平方向低通濾波
		VerticalHighPass	－	垂直方向高通濾波
		VerticalLowPass		－	垂直方向低通濾波
	各模式計算細節記錄於BMPHaarWavelet副程式宣告處說明
 */
enum HaarWaveletMode													//	宣告HaarWaveletMode集合(用於HaarWavelet副程式)
{																		//	進入HaarWaveletMode集合(用於HaarWavelet副程式)
	HorizontalHighPass,													//	定義Haar小波轉換模式HorizontalHighPass
	HorizontalLowPass,													//	定義Haar小波轉換模式HorizontalLowPass
	VerticalHighPass,													//	定義Haar小波轉換模式VerticalHighPass
	VerticalLowPass,													//	定義Haar小波轉換模式VerticalLowPass
};																		//	結束HaarWaveletMode集合(用於HaarWavelet副程式)
/*	HaarWavelet2Mode集合中宣告二階Haar濾波模式，命名格式說明如下：
		[水平方向濾波方式]+[垂直方向濾波方式]
	即：
		HighHigh	－	
		水平方向高通濾波(呼叫HaarWavelet副程式HorizontalHighPass模式)、垂直方向高通濾波(呼叫HaarWavelet副程式VerticalHighPass模式)
		HighLow		－	
		水平方向高通濾波(呼叫HaarWavelet副程式HorizontalHighPass模式)、垂直方向低通濾波(呼叫HaarWavelet副程式VerticalLowPass模式)
		LowHigh		－	
		水平方向低通濾波(呼叫HaarWavelet副程式HorizontalLowPass模式)、垂直方向高通濾波(呼叫HaarWavelet副程式VerticalHighPass模式)
		LowLow		－	
		水平方向低通濾波(呼叫HaarWavelet副程式HorizontalLowPass模式)、垂直方向低通濾波(呼叫HaarWavelet副程式VerticalLowPass模式)
	並給定二進位冪次方數值，以便選定多重模式(如：HighHigh|HighLow、HighLow|LowHigh)
 */
enum HaarWavelet2Mode													//	宣告HaarWavelet2Mode集合(用於HaarWavelet2副程式)
{																		//	進入HaarWavelet2Mode集合(用於HaarWavelet2副程式)
	HighHigh = 1,														//	定義二階Haar小波轉換模式HighHigh
	HighLow = 2,														//	定義二階Haar小波轉換模式HighLow
	LowHigh = 4,														//	定義二階Haar小波轉換模式LowHigh
	LowLow = 8,															//	定義二階Haar小波轉換模式LowLow
};																		//	結束HaarWavelet2Mode集合(用於HaarWavelet2副程式)
/*	SIFTOctave資料結構
	該資料結構設計為提供SIFT(Scale-invariant feature transform)運算建立同一Octave(影像大小相同，層與層間使用之
	Gaussian模糊摺積的標準偏差σ為σ0*k)
 */
typedef struct SIFTOctave												//	宣告SIFTOctave資料結構
{																		//	進入SIFTOctave資料結構
	unsigned int XSIZE;													//	宣告X軸像素變數
	unsigned int YSIZE;													//	宣告Y軸像素變數
	BMP24RGB *IMAGE_DATA;												//	宣告影像資料指標*IMAGE_DATA
}SIFTOctave;															//	結束SIFTOctave資料結構
/*	
*/

//-----全域變數宣告區----- 
//int ErrorCode = 0;														//	宣告錯誤狀態紀錄變數ErrorCode
//-----副程式宣告區----- 
/*  BmpReadFilesize副程式將輸入路徑之圖檔大小讀出並傳回
	副程式輸入為欲讀取大小之圖檔路徑(const char *型態字串，為欲讀取之檔名,FilenameExtension副檔名資訊)
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
char BmpRead(unsigned char*, const int, const int, const char *, const bool);
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
/*	FreeBMPIMAGE副程式用於釋放BMPIMAGE物件記憶體空間
	副程式輸入為(BMPIMAGE物件)
	副程式輸出為void
 */
void FreeBMPIMAGE(BMPIMAGE InputBMPIMAGE);								//	宣告FreeBMPIMAGE副程式
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
/*	ViewBMPImage副程式用於呼叫圖片檢視器開啟圖檔
	副程式輸入為欲開啟檢視之圖檔絕對路徑(配合FILE_ROOT_PATH形成絕對路徑)
	副程式輸入為void(無)
 */
void ViewBMPImage(const char *);										//	宣告ViewBMPImage(BMP圖片檢視)副程式(以Windows圖片檢視器開啟)
/*	InitialIMGArray副程式用於生成BMP24RGB指標變數，並將其資料初始化為0
	副程式輸入為(欲生成BMP24RGB指標變數之圖像寬度,欲生成BMP24RGB指標變數之圖像高度)
	副程式輸出為生成之BMP24RGB指標變數
 */
BMP24RGB *InitialIMGArray(const int, const int);						//	宣告InitialIMGArray副程式
/*	RAWImageToArray副程式將來自BMP圖檔之圖像資料轉換至RGB型態二維陣列，
	轉換後的一個BMP24RGB型態二維陣列代表一張影像，其中：
	__________________
	|                |
	|                |
	|                |
	|                |
	|                |
	|                |
	|________________|
	 ↑為第一像素
	 
 */
BMP24RGB *RAWImageToArray(const unsigned char*, const int, const int);	//	宣告RAWImageToArray(BMP圖檔資料至陣列轉換)副程式
/*	ArrayToRAWImage副程式用於將RGB型態圖像二維陣列轉換至符合BMP圖檔格式之圖像資料(含填補位元)
	副程式輸入為(RGB型態圖像二維陣列,圖像寬度,圖像高度)
	副程式輸出為無號字元指標(unsigned char *)型態，符合BMP圖檔格式之圖像資料(含填補位元)
 */
unsigned char *ArrayToRAWImage(const BMP24RGB*,const int,const int);	//	宣告陣列至BMP圖檔資料轉換副程式

/*	BMP24RGBToHSV副程式用於將BMP24RGB型態之RGB像素資料轉換至HSV色彩空間
	副程式輸入為(欲轉換至HSV之BMP24RGB型態圖像資料,欲轉換至HSV之BMP24RGB型態圖像寬度,欲轉換至HSV之BMP24RGB型態圖像高度)
	副程式輸出為由BMP24RGB色彩空間轉換至HSV完成之影像資料
 */
HSV *BMP24RGBToHSV(const BMP24RGB*,const int,const int);				//	宣告BMP24RGB型態至HSV轉換副程式
/*	HSVToBMP24RGB副程式用於將HSV型態之像素資料轉換至BMP24RGB色彩空間
	副程式輸入為(欲轉換至BMP24RGB之HSV型態圖像資料,欲轉換至BMP24RGB之HSV型態圖像寬度,欲轉換至BMP24RGB之HSV型態圖像高度)
	副程式輸出為由HSV色彩空間轉換至BMP24RGB完成之影像資料
 */
BMP24RGB *HSVToBMP24RGB(const HSV*,const int,const int);				//	宣告BMP24RGB型態至HSV轉換副程式
/*	ImageDataToTxt副程式用於將圖像影像資料寫入txt檔
	副程式輸入為(欲寫入之txt檔案路徑,欲寫入之圖像影像資料,欲寫入之圖像影像寬度,欲寫入之圖像影像高度)
	副程式輸出：若順利寫入檔案則傳回true，反之傳回false
	寫入檔案資料舉例如下：
		第0個像素R=175	第0個像素G=255	第0個像素B=92
		第1個像素R=176	第1個像素G=255	第1個像素B=92
		第2個像素R=177	第2個像素G=255	第2個像素B=93
		第3個像素R=178	第3個像素G=255	第3個像素B=93
		第4個像素R=179	第4個像素G=255	第4個像素B=94
		第5個像素R=180	第5個像素G=255	第5個像素B=95
		第6個像素R=181	第6個像素G=255	第6個像素B=95
		第7個像素R=181	第7個像素G=255	第7個像素B=96
		第8個像素R=182	第8個像素G=255	第8個像素B=96
		第9個像素R=183	第9個像素G=255	第9個像素B=97
	下一代副程式預計
 */
bool ImageDataToTxt(const char*,const BMP24RGB*,const int,const int);	//	宣告ImageDataToTxt(圖像影像資料寫入txt檔)副程式
/*	HSVDataToTxt副程式用於將HSV型態影像資料寫入txt檔
	寫入檔案資料舉例如下：
		第0個像素H=89.391197	第0個像素S=0.639216	第0個像素V=255.000000
		第1個像素H=88.985390	第1個像素S=0.639216	第1個像素V=255.000000
		第2個像素H=88.775009	第2個像素S=0.635294	第2個像素V=255.000000
		第3個像素H=88.366875	第3個像素S=0.635294	第3個像素V=255.000000
		第4個像素H=88.151466	第4個像素S=0.631373	第4個像素V=255.000000
		第5個像素H=87.933411	第5個像素S=0.627451	第5個像素V=255.000000
		第6個像素H=87.520569	第6個像素S=0.627451	第6個像素V=255.000000
		第7個像素H=87.712685	第7個像素S=0.623529	第7個像素V=255.000000
		第8個像素H=87.297371	第8個像素S=0.623529	第8個像素V=255.000000
		第9個像素H=87.071434	第9個像素S=0.619608	第9個像素V=255.000000
	副程式輸入為(欲寫入之txt檔案路徑,欲寫入之HSV型態影像資料,欲寫入之圖像影像寬度,欲寫入之圖像影像高度)
	副程式輸出：若順利寫入檔案則傳回true，反之傳回false	
 */
bool HSVDataToTxt(const char*, const HSV*, const int, const int);		//	宣告HSVDataToTxt(HSV型態影像資料寫入txt檔)副程式
/*	ImgDifference2副程式用於執行兩BMP24RGB型態圖像像素之差(相減)運算
	運算方法為令：
	A為一5*5圖像，像素資料如下：
		-				-
		|	10	20	30	|
		|	40	10	20	|
		|	70	40	50	|
		-				-
	
	B為一5*5圖像，像素資料如下：
		-				-
		|	5	10	15	|
		|	20	5	10	|
		|	35	20	25	|
		-				-
	
	則圖像A-B可呼叫ImgDifference2，呼叫方法如：ImgDifference2(A, B, 5, 5);
	運算結果為：
		-				-
		|	5	10	15	|
		|	20	5	10	|
		|	35	20	25	|
		-				-
	為考慮相減為負數之情況，進行減法運算後取絕對值
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
 */
BMP24RGB *ImgDifference2(const BMP24RGB*,const BMP24RGB*,const int,const int);
/*	BMP24RGB2or副程式用於執行兩BMP24RGB型態圖像像素之OR運算
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	副程式輸入為(欲進行or運算之來源影像1, 欲進行or運算之來源影像2, 來源影像寬度, 來源影像高度)
	副程式輸出為執行像素or運算後之結果
 */
BMP24RGB *BMP24RGB2or(const BMP24RGB*,const BMP24RGB*,const int,const int);
//	宣告BMP24RGB2or(兩BMP24RGB型態圖像資料or運算)副程式
/*	BmpToGraylevel副程式用於將BMP24RGB型態圖像轉為灰階圖像(Graylevel Image)
	運算方法為令：
		R = (R + G + B) / 3
		G = (R + G + B) / 3
		B = (R + G + B) / 3
	副程式輸入為(欲轉換至灰階之BMP24RGB型態圖像資料, 欲轉換至灰階之圖像寬度, 欲轉換至灰階之圖像高度)
	副程式輸出為轉換為灰階後之BMP24RGB型態圖像資料
 */
BMP24RGB *BmpToGraylevel(const BMP24RGB*, const int, const int);		//	宣告BMP圖片資料轉灰階副程式
/*	BmpConvolution33用於計算給定3*3遮罩與輸入圖像進行摺積(Convolution)，
	但精確上來說，摺積過程中須將遮罩翻轉，但本副程式未進行翻轉，僅進行相乘
	舉例來說：
		若傳入之3*3遮罩為
			--		--
			| 1 1 1	 |
			| 1 1 1	 |
			| 1 1 1	 |
			--		--
		一圖像3*3區塊像素值如下：
		-				-
		|	10	20	30	|
		|	40	10	20	|
		|	70	40	50	|
		-				-
		計算摺積=(10*1+20*1+30*1+40*1+10*1+20*1+70*1+40*1+50*1)/9(除以遮罩權重總和)=290/9=32.2222
		
		3*3遮罩傳入方式為一陣列：
			假設陣列名稱為Mask，數值設定舉例如下：
			--							--
			| Mask[6] Mask[7] Mask[8]	 |
			| Mask[3] Mask[4] Mask[5]	 |
			| Mask[0] Mask[1] Mask[2]	 |
			--							--
			由左下角開始編號為0，由左至右，由下至上遞增
		
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	副程式輸入為(欲進行Convolution運算之BMP24RGB型態影像, 欲進行Convolution運算之BMP24RGB型態圖像之寬度, 欲進行Convolution運算之BMP24RGB型態圖像之高度, Convolution之Mask)
	副程式輸出為執行Convolution運算後之結果
 */
BMP24RGB *BmpConvolution33(const BMP24RGB*,const int,const int,const long double[]);
//	宣告BmpConvolution33(圖像3*3遮罩摺積)副程式
/*	BmpConvolution55用於計算給定5*5遮罩與輸入圖像進行摺積(Convolution)，
	但精確上來說，摺積過程中須將遮罩翻轉，但本副程式未進行翻轉，僅進行相乘
	舉例來說：
		若傳入之5*5遮罩為
			--			--
			| 1 1 1 1 1	 |
			| 1 1 1	1 1	 |
			| 1 1 1	1 1	 |
			| 1 1 1	1 1	 |
			| 1 1 1	1 1	 |
			--			--
		一圖像5*5區塊像素值如下：
		--						--
		|	99	96	82	24	78	 |
		|	9	20	3	28	94	 |
		|	33	35	40	52	66	 |
		|	37	34	27	26	69	 |
		|	29	1	67	31	47	 |
		--						--
		計算摺積=(	99 * 1 + 96 * 1 + 82 * 1 + 24 * 1 + 78 * 1 + 
					9 * 1 + 20 * 1 + 3 * 1 + 28 * 1 + 94 * 1 + 
					33 * 1 + 35 * 1 + 40 * 1 + 52 * 1 + 66 * 1 + 
					37 * 1 + 34 * 1 + 27 * 1 + 26 * 1 + 69 * 1 + 
					29 * 1 + 1 * 1 + 67 * 1 + 31 * 1 + 47)/25(除以遮罩權重總和)=1127 / 25 = 45.08
		
		5*5遮罩傳入方式為一陣列：
			假設陣列名稱為Mask，數值設定舉例如下：
			--												--
			| Mask[20] Mask[21] Mask[22] Mask[23] Mask[24]	 |
			| Mask[15] Mask[16] Mask[17] Mask[18] Mask[19]	 |
			| Mask[10] Mask[11] Mask[12] Mask[13] Mask[14]	 |
			| Mask[ 5] Mask[ 6] Mask[ 7] Mask[ 8] Mask[ 9]	 |
			| Mask[ 0] Mask[ 1] Mask[ 2] Mask[ 3] Mask[ 4]	 |
			--												--
			由左下角開始編號為0，由左至右，由下至上遞增
			
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	副程式輸入為(欲進行Convolution運算之BMP24RGB型態影像, 欲進行Convolution運算之BMP24RGB型態圖像之寬度, 欲進行Convolution運算之BMP24RGB型態圖像之高度, Convolution之Mask)
	副程式輸出為執行Convolution運算後之結果
 */
BMP24RGB *BmpConvolution55(const BMP24RGB*,const int,const int,const long double[]);
//	宣告BmpConvolution55(圖像5*5遮罩摺積)副程式
/*	BmpConvolution77用於計算給定7*7遮罩與輸入圖像進行摺積(Convolution)，
	但精確上來說，摺積過程中須將遮罩翻轉，但本副程式未進行翻轉，僅進行相乘
	舉例來說：
		若傳入之7*7遮罩為
			--			   --
			| 1 1 1 1 1 1 1	|
			| 1 1 1	1 1 1 1	|
			| 1 1 1	1 1 1 1	|
			| 1 1 1	1 1 1 1	|
			| 1 1 1	1 1 1 1	|
			| 1 1 1	1 1 1 1	|
			| 1 1 1	1 1 1 1	|
			--			   --
		一圖像7*7區塊像素值如下：
		--							   --
		|	99	96	82	24	78	20	84	|
		|	9	20	3	28	94	54	43	|
		|	33	35	40	52	66	73	12	|
		|	37	34	27	26	69	41	93	|
		|	29	1	67	31	47	55	34	|
		|	50	109	200	197	140	29	22	|
		|	184	13	238	97	170	253	239	|
		--							   --
		計算摺積=(	99 * 1 + 96 * 1 + 82 * 1 + 24 * 1 + 78 * 1 + 20 * 1 + 84 * 1 +
					9 * 1 + 20 * 1 + 3 * 1 + 28 * 1 + 94 * 1 + 54 * 1 + 43 * 1 +
					33 * 1 + 35 * 1 + 40 * 1 + 52 * 1 + 66 * 1 + 73 * 1 + 12 * 1 + 
					37 * 1 + 34 * 1 + 27 * 1 + 26 * 1 + 69 * 1 + 41 * 1 + 93 * 1 + 
					29 * 1 + 1 * 1 + 67 * 1 + 31 * 1 + 47 * 1 + 55 * 1 + 34 * 1 + 
					50 * 1 + 109 * 1 + 200 * 1 + 197 * 1 + 140 * 1 + 29 * 1 + 22 * 1 + 
					184 * 1 + 13 * 1 + 238 * 1 + 97 * 1 + 170 * 1 + 253 * 1 + 239
					)/( 7 * 7 )(除以遮罩權重總和) =
					3577 / 49 = 73
		
		7*7遮罩傳入方式為一陣列：
			假設陣列名稱為Mask，數值設定舉例如下：
			--																	--
			|	Mask[42] Mask[43] Mask[44] Mask[45] Mask[46] Mask[47] Mask[48]	 |
			|	Mask[35] Mask[36] Mask[37] Mask[38] Mask[39] Mask[40] Mask[41] 	 |
			|	Mask[28] Mask[29] Mask[30] Mask[31] Mask[32] Mask[33] Mask[34]	 |
			|	Mask[21] Mask[22] Mask[23] Mask[24] Mask[25] Mask[26] Mask[27]	 |
			|	Mask[14] Mask[15] Mask[16] Mask[17] Mask[18] Mask[19] Mask[20]	 |
			|	Mask[ 7] Mask[ 8] Mask[ 9] Mask[10] Mask[11] Mask[12] Mask[13]	 |
			|	Mask[ 0] Mask[ 1] Mask[ 2] Mask[ 3] Mask[ 4] Mask[ 5] Mask[ 6]	 |
			--																	--
			由左下角開始編號為0，由左至右，由下至上遞增
	
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	副程式輸入為(欲進行Convolution運算之BMP24RGB型態影像, 欲進行Convolution運算之BMP24RGB型態圖像之寬度, 欲進行Convolution運算之BMP24RGB型態圖像之高度, Convolution之Mask)
	副程式輸出為執行Convolution運算後之結果
 */
BMP24RGB *BmpConvolution77(const BMP24RGB*,const int,const int,const long double[]);
//	宣告BmpConvolution77(圖像7*7遮罩摺積)副程式
/*	BmpConvolution用於計算給定指定大小遮罩與輸入圖像進行摺積(Convolution)，
	但精確上來說，摺積過程中須將遮罩翻轉，但本副程式未進行翻轉，僅進行相乘。
	遮罩傳入方式為一陣列，由左下角開始編號為0，由左至右，由下至上遞增。
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同。
	本副程式輸入參數較多，依序說明如下：
	第一項參數為Convolution遮罩大小，若欲進行摺積之遮罩為7*7，則該參數傳入7
	第二項參數為欲進行Convolution運算之BMP24RGB型態影像
	第三項參數為欲進行Convolution運算之BMP24RGB型態圖像之寬度
	第四項參數為欲進行Convolution運算之BMP24RGB型態圖像之高度
	第五項參數為Convolution之Mask資料，需與第一項參數配合，若第一項參數傳入7，則該處Mask為一7*7大小之陣列
	副程式輸入為(Convolution遮罩大小, 欲進行Convolution運算之BMP24RGB型態影像, 欲進行Convolution運算之BMP24RGB型態圖像之寬度, 欲進行Convolution運算之BMP24RGB型態圖像之高度, Convolution之Mask)
	副程式輸出為執行Convolution運算後之結果
 */
BMP24RGB *BmpConvolution(const int, const BMP24RGB*,const int,const int,const long double[]);
//	宣告BmpConvolution(圖像摺積)副程式
/*	ImageSmoothing33V1(ImageSmoothing33 Version 1)副程式用於計算BMP24RGB型態圖像之3*3Mask平滑濾波
	運算時使用之3*3 Mask如下：
		-				-
		|	1/9	1/9	1/9	|
		|	1/9	1/9	1/9	|
		|	1/9	1/9	1/9	|
		-				-
	舉例而言：
	一圖像3*3區塊像素值如下：
		-				-
		|	10	20	30	|
		|	40	10	20	|
		|	70	40	50	|
		-				-
	計算平滑濾波得之像素值為10/9+20/9+30/9+40/9+10/9+20/9+70/9+40/9+50/9=290/9=32.2222
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	副程式輸入為(欲進行平滑濾波之BMP24RGB型態圖像資料,欲進行平滑濾波之圖像寬度,欲進行平滑濾波之圖像高度)
	副程式輸出為進行平滑濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *ImageSmoothing33V1(const BMP24RGB*,const int,const int);		//	宣告ImageSmoothing33V1(BMP圖檔3*3Mask平滑濾波 Version 1)副程式
/*	ImageSmoothing33V2(ImageSmoothing33 Version 2)副程式用於計算BMP24RGB型態圖像之3*3Mask平滑濾波
	運算使用BmpConvolution33副程式實現
	副程式輸入為(欲進行平滑濾波之BMP24RGB型態圖像資料,欲進行平滑濾波之圖像寬度,欲進行平滑濾波之圖像高度)
	副程式輸出為進行平滑濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *ImageSmoothing33V2(const BMP24RGB*,const int,const int);		//	宣告ImageSmoothing33V2(BMP圖檔3*3Mask平滑濾波 Version 2)副程式
/*	ImageSmoothing55副程式用於計算BMP24RGB型態圖像之5*5Mask平滑濾波
	運算使用BmpConvolution55副程式實現
	副程式輸入為(欲進行平滑濾波之BMP24RGB型態圖像資料,欲進行平滑濾波之圖像寬度,欲進行平滑濾波之圖像高度)
	副程式輸出為進行平滑濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *ImageSmoothing55(const BMP24RGB*,const int,const int);		//	宣告ImageSmoothing55(BMP圖檔5*5Mask平滑濾波)副程式
/*	ImageSmoothing77副程式用於計算BMP24RGB型態圖像之7*7Mask平滑濾波
	運算使用BmpConvolution77副程式實現
	副程式輸入為(欲進行平滑濾波之BMP24RGB型態圖像資料,欲進行平滑濾波之圖像寬度,欲進行平滑濾波之圖像高度)
	副程式輸出為進行平滑濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *ImageSmoothing77(const BMP24RGB*,const int,const int);		//	宣告ImageSmoothing77(BMP圖檔7*7Mask平滑濾波)副程式
/*	ImageSmoothing副程式用於計算BMP24RGB型態圖像之指定大小Mask平滑濾波
	運算使用BmpConvolution副程式實現
	第一項傳入參數為欲進行平滑濾波之Mask大小，若欲進行平滑濾波之遮罩為7*7，則該參數傳入7
	副程式輸入為(欲進行平滑濾波之Mask大小,欲進行平滑濾波之BMP24RGB型態圖像資料,欲進行平滑濾波之圖像寬度,欲進行平滑濾波之圖像高度)
	副程式輸出為進行平滑濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *ImageSmoothing(const int, const BMP24RGB*,const int,const int);
//	宣告ImageSmoothing(BMP圖檔平滑濾波)副程式
/*	MedianFilter33副程式用於計算BMP24RGB型態圖像之3*3Mask中值濾波
	副程式輸入為(欲進行中值濾波之BMP24RGB型態圖像資料,欲進行中值濾波之圖像寬度,欲進行中值濾波之圖像高度)
	副程式輸出為進行中值濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *MedianFilter33(const BMP24RGB*,const int,const int);			//	宣告MedianFilter33(BMP圖檔3*3中值濾波)副程式
/*	GaussianBlur33V1(3*3高斯濾波 Version 1)副程式用於計算BMP24RGB型態圖像資料之高斯模糊
	參考自維基百科：https://zh.wikipedia.org/wiki/%E9%AB%98%E6%96%AF%E6%A8%A1%E7%B3%8A
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	副程式輸入為(欲進行高斯濾波之BMP24RGB型態圖像資料,欲進行高斯濾波之圖像寬度,欲進行高斯濾波之圖像高度,常態分布的標準偏差σ)
	副程式輸出為進行高斯濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *GaussianBlur33V1(const BMP24RGB*,const int,const int,const long double);
//	宣告GaussianBlur33V1(3*3高斯濾波 Version 1)副程式
/*	GaussianBlur33V2(3*3高斯濾波 Version 2)副程式用於計算BMP24RGB型態圖像資料之高斯模糊
	參考自維基百科：https://zh.wikipedia.org/wiki/%E9%AB%98%E6%96%AF%E6%A8%A1%E7%B3%8A
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	運算使用BmpConvolution33副程式實現
	副程式輸入為(欲進行高斯濾波之BMP24RGB型態圖像資料,欲進行高斯濾波之圖像寬度,欲進行高斯濾波之圖像高度,常態分布的標準偏差σ)
	副程式輸出為進行高斯濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *GaussianBlur33V2(const BMP24RGB*, const int, const int, const long double);
//	宣告GaussianBlur33V2(3*3高斯濾波 Version 2)副程式
/*	GaussianBlur副程式用於計算任意大小之高斯濾波
	參考自維基百科：https://zh.wikipedia.org/wiki/%E9%AB%98%E6%96%AF%E6%A8%A1%E7%B3%8A
	本副程式將圖像像素之R、G、B成分分別處理，演算法相同
	運算使用BmpConvolution副程式實現
	本副程式輸入參數較多，依序說明如下：
	第一項參數為高斯濾波Convolution遮罩大小，若欲進行摺積之遮罩為7*7，則該參數傳入7
	第二項參數為欲進行高斯濾波之BMP24RGB型態圖像資料
	第三項參數為欲進行高斯濾波之圖像寬度
	第四項參數為欲進行高斯濾波之圖像高度
	第五項參數為常態分布的標準偏差σ
	副程式輸入為(高斯濾波遮罩大小,欲進行高斯濾波之BMP24RGB型態圖像資料,欲進行高斯濾波之圖像寬度,欲進行高斯濾波之圖像高度,常態分布的標準偏差σ)
	副程式輸出為進行高斯濾波後之BMP24RGB型態圖像資料
 */
BMP24RGB *GaussianBlur(const int, const BMP24RGB*, const int, const int, const long double);
//	宣告GaussianBlur(高斯濾波)副程式
/*	GaussianFigure2D副程式用於產生二維高斯資料圖形
	二維高斯圖像中心為峰值，逐漸向外遞減，因此該圖像中心為白色，以同心圓向外漸灰
	二維高斯計算使用NormalDistribution2D副程式
	副程式輸入為(欲生成二維高斯圖像寬度, 欲生成二維高斯圖像高度, 高斯分布的標準偏差σ)
	副程式輸出為二維高斯之BMP24RGB型態圖像資料
 */
BMP24RGB *GaussianFigure2D(const int, const int, const long double);	//	宣告GaussianFigure2D(二維高斯圖像)生成副程式
/*	NormalDistribution2D(二維常態分布計算)副程式用於計算二維常態分布數值
	公式如下：
		pow(M_E,-(pow(xlocation,2) + pow(ylocation,2)) / (2 * pow(StandardDeviation,2)))/(2 * M_PI * pow(StandardDeviation,2))
	公式中xlocation、ylocation為二維座標，StandardDeviation為常態分布的標準偏差σ
	副程式輸入為(xlocation,ylocation,StandardDeviation)
	副程式輸出為二維常態分布計算結果
 */
long double NormalDistribution2D(long double, long double, long double);//	宣告NormalDistribution2D(二維常態分布計算)副程式
BMP24RGB *ImageOCR(const BMP24RGB*,const int,const int);				//	宣告ImageOCR(影像OCR)副程式
/*	BMP24RGBGradient副程式用於計算BMP24RGB型態圖像資料之梯度
	運算方法為
		Step1：計算出Gx(X方向梯度)與Gy(Y方向梯度)，計算方法舉例如下：
		
		一圖像3*3區塊像素值如下：
		-				-
		|	10	20	30	|
		|	40	10	20	|
		|	70	40	50	|
		-				-
		
		則：
		Gx = 	-				-	-				-
				|	10	20	30	|	|	-1	0	1	|
				|	40	10	20	| * |	-1	0	1	| = 10 * (-1) + 40 * (-1) + 70 * (-1) + 
				|	70	40	50	|	|	-1	0	1	|	20 *   0  + 10 *   0  + 40 *   0  + 
				-				-	-				-	30 *   1  + 20 *   1  + 50 *   1  = -20
				
		Gy = 	-				-	-				-
				|	10	20	30	|	|	-1	-1	-1	|
				|	40	10	20	| * |	 0	 0	 0	| = 10 * (-1) + 20 * (-1) + 30 * (-1) + 
				|	70	40	50	|	|	 1	 1	 1	|	40 *   0  + 10 *   0  + 20 *   0  + 
				-				-	-				-	70 *   1  + 40 *   1  + 50 *   1  = 100
		
		Step2：計算梯度大小(Magnitude)與方向(Direction)
		藉由Gx與Gy計算梯度大小(Magnitude)與方向(Direction)，在此方向(Direction)為角度(deg)
		大小(Magnitude)之值域為[-765,765]
		方向(Direction)經atan函數計算之值域為[-90°,90°]，但實際上需考慮Gx與Gx之正負號以決定方向，
		若(Gx>0)且(Gy>0)，則方向(Direction)範圍介於0~90度
			ex：當Gx=1，Gy=1時，atan(1) = 0.785398163 rad = 45 deg，方向(Direction) = 45 deg
			當(Gx>0)且(Gy>0)時，方向(Direction) = atan( Gy / Gx )(deg)
		若(Gx<0)且(Gy>0)，則方向(Direction)範圍介於90~180度
			ex：當Gx=-1，Gy=1時，atan(-1) = -0.785398163 rad = -45 deg，但方向(Direction)應為135 deg
			當(Gx<0)且(Gy>0)時，方向(Direction) = atan( Gy / Gx )(deg) + 180°
		若(Gx<0)且(Gy<0)，則方向(Direction)範圍介於180~270度
			ex：當Gx=-1，Gy=-1時，atan(1) = 0.785398163 rad = 45 deg，但方向(Direction)應為225 deg
			當(Gx<0)且(Gy<0)時，方向(Direction) = atan( Gy / Gx )(deg) + 180°
		若(Gx>0)且(Gy<0)，則方向(Direction)範圍介於270~360度
			ex：當Gx=1，Gy=-1時，atan(-1) = -0.785398163 rad = -45 deg，但方向(Direction)應為315 deg
			當(Gx>0)且(Gy<0)時，方向(Direction) = atan( Gy / Gx )(deg) + 360°
		上述四項角度特性可歸納為：
		若(Gx>0)，則方向(Direction)範圍介於-90~90度(第一、四象限，右半平面)		第二象限|第一象限
		若(Gx<0)，則方向(Direction)範圍介於90~270度(第二、三象限，左半平面)		-----------------
																				第三象限|第四象限
		
		因此使用atan函數計算時，若(Gx<0)則計算結果需+180度，則方向(Direction)之值域為[-90,270]，
		此時左半平面之角度為正確，但右半平面之角度值域為[-90,90]，當右半平面之角度<0時，需將該角度+360
		
		故得：
		Magnitude = ( Gx ^ 2 + Gy ^ 2 ) ^ 0.5										//	計算梯度大小(Magnitude)
		Direction = atan( Gy / Gx ) * (180 / PI) 		if (Gx > 0) and (Gy >= 0)	//	計算梯度方向(Direction)－第一象限
					atan( Gy / Gx ) * (180 / PI) + 180°	if Gx < 0					//	計算梯度方向(Direction)－第二象限、第三象限
					atan( Gy / Gx ) * (180 / PI) + 360°	if (Gx > 0) and (Gy < 0)	//	計算梯度方向(Direction)－第四象限
							 90							if (Gx = 0) and (Gy > 0)	//	90°情況
							270							if (Gx = 0) and (Gy < 0)	//	270°情況
							0(Nan?)						if (Gx = 0) and (Gy = 0)	//	當Gx為0且Gy亦為0時，梯度方向無法定義

		本副程式將圖像像素之R、G、B成分分別處理，演算法相同
		
	副程式輸入為(欲計算梯度之BMP24RGB型態圖像資料,欲計算梯度之圖像寬度,欲計算梯度之圖像高度)
	副程式輸出為計算梯度後之BMP24RGB型態圖像資料
 */
BMP24RGB *BMP24RGBGradient(const BMP24RGB*,const int,const int);		//	宣告BMP24RGBGradient(BMP圖片資料梯度計算)副程式
/*	BMP24RGBSobelEdge副程式用於Sobel(索貝爾算子)邊緣偵測
	運算方法為
		Step1：計算出Gx(橫向邊緣檢測)與Gy(縱向邊緣檢測)，計算方法舉例如下：
		
		一圖像3*3區塊像素值如下：
		--				--
		|	10	20	30	 |
		|	40	10	20	 |
		|	70	40	50	 |
		--				--
		
		則：
		Gx = 	-				-	-				-
				|	10	20	30	|	|	-1	0	1	|
				|	40	10	20	| * |	-2	0	2	| = 10 * (-1) + 40 * (-2) + 70 * (-1) + 
				|	70	40	50	|	|	-1	0	1	|	20 *   0  + 10 *   0  + 40 *   0  + 
				-				-	-				-	30 *   1  + 20 *   2  + 50 *   1  = -40
				
		Gy = 	-				-	-				-
				|	10	20	30	|	|	-1	-2	-1	|
				|	40	10	20	| * |	 0	 0	 0	| = 10 * (-1) + 20 * (-2) + 30 * (-1) + 
				|	70	40	50	|	|	 1	 2	 1	|	40 *   0  + 10 *   0  + 20 *   0  + 
				-				-	-				-	70 *   1  + 40 *   2  + 50 *   1  = 120
		
		Step2：計算梯度大小(Magnitude)與方向(Direction)
		藉由Gx與Gy計算梯度大小(Magnitude)與方向(Direction)，在此方向(Direction)為角度(deg)
		大小(Magnitude)之值域為[-765,765]
		方向(Direction)經atan函數計算之值域為[-90°,90°]，但實際上需考慮Gx與Gx之正負號以決定方向，
		若(Gx>0)且(Gy>0)，則方向(Direction)範圍介於0~90度
			ex：當Gx=1，Gy=1時，atan(1) = 0.785398163 rad = 45 deg，方向(Direction) = 45 deg
			當(Gx>0)且(Gy>0)時，方向(Direction) = atan( Gy / Gx )(deg)
		若(Gx<0)且(Gy>0)，則方向(Direction)範圍介於90~180度
			ex：當Gx=-1，Gy=1時，atan(-1) = -0.785398163 rad = -45 deg，但方向(Direction)應為135 deg
			當(Gx<0)且(Gy>0)時，方向(Direction) = atan( Gy / Gx )(deg) + 180°
		若(Gx<0)且(Gy<0)，則方向(Direction)範圍介於180~270度
			ex：當Gx=-1，Gy=-1時，atan(1) = 0.785398163 rad = 45 deg，但方向(Direction)應為225 deg
			當(Gx<0)且(Gy<0)時，方向(Direction) = atan( Gy / Gx )(deg) + 180°
		若(Gx>0)且(Gy<0)，則方向(Direction)範圍介於270~360度
			ex：當Gx=1，Gy=-1時，atan(-1) = -0.785398163 rad = -45 deg，但方向(Direction)應為315 deg
			當(Gx>0)且(Gy<0)時，方向(Direction) = atan( Gy / Gx )(deg) + 360°
		上述四項角度特性可歸納為：
		若(Gx>0)，則方向(Direction)範圍介於-90~90度(第一、四象限，右半平面)		第二象限|第一象限
		若(Gx<0)，則方向(Direction)範圍介於90~270度(第二、三象限，左半平面)		-----------------
																				第三象限|第四象限
		
		因此使用atan函數計算時，若(Gx<0)則計算結果需+180度，則方向(Direction)之值域為[-90,270]，
		此時左半平面之角度為正確，但右半平面之角度值域為[-90,90]，當右半平面之角度<0時，需將該角度+360
		
		故得：
		Magnitude = ( Gx ^ 2 + Gy ^ 2 ) ^ 0.5										//	計算梯度大小(Magnitude)
		Direction = atan( Gy / Gx ) * (180 / PI) 		if (Gx > 0) and (Gy >= 0)	//	計算梯度方向(Direction)－第一象限
					atan( Gy / Gx ) * (180 / PI) + 180°	if Gx < 0					//	計算梯度方向(Direction)－第二象限、第三象限
					atan( Gy / Gx ) * (180 / PI) + 360°	if (Gx > 0) and (Gy < 0)	//	計算梯度方向(Direction)－第四象限
							 90							if (Gx = 0) and (Gy > 0)	//	90°情況
							270							if (Gx = 0) and (Gy < 0)	//	270°情況
							0(Nan?)						if (Gx = 0) and (Gy = 0)	//	當Gx為0且Gy亦為0時，梯度方向無法定義
		
		本副程式將圖像像素之R、G、B成分分別處理，演算法相同
		
		參考自維基百科：https://zh.wikipedia.org/wiki/%E7%B4%A2%E8%B2%9D%E7%88%BE%E7%AE%97%E5%AD%90
 */
BMP24RGB *BMP24RGBSobelEdge(const BMP24RGB*,const int,const int);		//	宣告BMP24RGBSobelEdge(BMP圖片資料Sobel邊緣偵測)副程式
/*	RGBHistogramEqualization副程式用於對BMP24RGB型態影像進行Histogram Equalization(直方圖等化)
	運算方法分別對R、G、B進行像素值統計、累積，分別Histogram Equalization(直方圖等化)，但該方法可能導致圖像顏色改變
	副程式輸入為(欲進行直方圖等化之BMP24RGB型態影像資料,影像寬度,影像高度)
	副程式輸出為直方圖等化後之BMP24RGB型態影像資料
 */
BMP24RGB *RGBHistogramEqualization(const BMP24RGB*,const int,const int);//	宣告RGBHistogramEqualization(RGB灰階影像直方圖等化)副程式
/*	BMPHaarWavelet副程式用於對BMP24RGB型態影像進行哈爾小波轉換
	運算方法分別對R、G、B進行哈爾小波轉換，具模式設定參數(Mode)，配合HaarWaveletMode集合宣告：
	HaarWaveletMode集合宣告HorizontalHighPass(水平高通濾波)、HorizontalLowPass(水平低通濾波)、VerticalHighPass(垂直高通濾波)與VerticalLowPass(垂直低通濾波)
	四種模式，分別說明如下：
		- HorizontalHighPass(水平高通濾波)模式
		計算水平像素間的差值，若水平像素間差異愈大，則輸出像素值愈大，使用遮罩為
						--				--
						|	-1,	0,	1	 |
						--				--
		故經過該模式輸出圖片將保留垂直紋理
		- HorizontalLowPass(水平低通濾波)模式
		計算水平像素平均值，使用遮罩為
						--					--
						|	0.5,	0,	0.5	 |
						--					--
		具有水平方向模糊之效果
		- VerticalHighPass(垂直高通濾波)
		計算垂直像素間的差值，若垂直像素間差異愈大，則輸出像素值愈大，使用遮罩為
						--		--
						|	-1	 |
						|	0	 |
						|	1	 |
						--		--
		故經過該模式輸出圖片將保留水平紋理
		- VerticalLowPass(垂直低通濾波)
		計算垂直像素平均值，使用遮罩為
						--		--
						|	0.5	 |
						|	0	 |
						|	0.5	 |
						--		--
		具有垂直方向模糊之效果
	副程式輸入為(欲進行Haar Wavelet之BMP24RGB型態影像資料,影像寬度,影像高度,Haar小波轉換模式)
	副程式輸出為Haar小波轉換後之BMP24RGB型態影像資料
 */
BMP24RGB *BMPHaarWavelet(const BMP24RGB*,const int,const int, const char);
/*	BMPHaarWavelet2副程式用於對BMP24RGB型態影像進行二階哈爾(Haar)小波轉換
	運算方法說明如下：
		由於圖像為二維結構，Haar小波濾波方向可分為水平濾波與垂直濾波，
		該副程式執行二階哈爾(Haar)小波轉換時
 */
BMP24RGB *BMPHaarWavelet2(const BMP24RGB*,const int,const int, const char);
/*	HSVHistogramEqualization副程式用於對HSV型態影像進行Histogram Equalization(直方圖等化)
	運算方法僅對於HSV色彩空間之Value進行Histogram Equalization(直方圖等化)
	副程式輸入為(欲進行直方圖等化之HSV型態影像資料,影像寬度,影像高度)
	副程式輸出為直方圖等化後之HSV型態影像資料
 */
HSV *HSVHistogramEqualization(const HSV*,const int,const int);			//	宣告HSVHistogramEqualization(HSV影像直方圖等化)副程式

/*	HueToBMP24RGB副程式用於將HSV型態影像資料中之Hue資料取出填入BMP24RGB圖像
	由於本程式中使用HSV色彩空間之Hue值域為0～360，但BMP24RGB型態之R、G、B之值域為0～255
	故令：
		R=Hue * 255 / 360；G=Hue * 255 / 360；B=Hue * 255 / 360
	即可完成將Hue填入至BMP24RGB型態
	副程式輸入為(HSV型態影像資料,HSV型態影像寬度,HSV型態影像高度)
	副程式輸出為取自HSV型態影像之Hue填入得BMP24RGB圖像資料
 */
BMP24RGB *HueToBMP24RGB(const HSV*,const int,const int);				//	宣告HueToBMP24RGB副程式
/*	SaturationToBMP24RGB副程式用於將HSV型態影像資料中之Saturation資料取出填入BMP24RGB圖像
	由於本程式中使用HSV色彩空間之Saturation值域為0～1，但BMP24RGB型態之R、G、B之值域為0～255
	故令：
		R=Saturation * 255；G=Saturation * 255；B=Saturation * 255
	即可完成將Saturation填入至BMP24RGB型態
	副程式輸入為(HSV型態影像資料,HSV型態影像寬度,HSV型態影像高度)
	副程式輸出為取自HSV型態影像之Saturation填入得BMP24RGB圖像資料
 */
BMP24RGB *SaturationToBMP24RGB(const HSV*,const int,const int);			//	宣告SaturationToBMP24RGB副程式
/*	ValueToBMP24RGB副程式用於將HSV型態影像資料中之Value資料取出填入BMP24RGB圖像
	由於本程式中使用HSV色彩空間之Value值域為0～255，BMP24RGB型態之R、G、B之值域亦為0～255
	故直接令：
		R=Value；G=Value；B=Value
	即可完成將Value填入至BMP24RGB型態
	副程式輸入為(HSV型態影像資料,HSV型態影像寬度,HSV型態影像高度)
	副程式輸出為取自HSV型態影像之Value填入得BMP24RGB圖像資料
 */
BMP24RGB *ValueToBMP24RGB(const HSV*,const int,const int);				//	宣告ValueToBMP24RGB副程式
/*	HSVSkin副程式用於透過HSV色彩空間資訊中，由給定之H、S、V之範圍過濾出皮膚資訊
	在此副程式中設定H範圍為15～50；S範圍為0.23～0.68；凡像素色彩資訊在此範圍中皆保留原像素資訊，
	在範圍外則抑制像素明度(Value)資訊(輸出像素之Value=原像素Value*0.3)
	副程式輸入為(HSV型態影像資料,HSV型態影像寬度,HSV型態影像高度)
	副程式輸出為過濾皮膚資訊之HSV型態影像
 */
HSV *HSVSkin(const HSV*,const int,const int);							//	宣告HSVSkin副程式
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
void InitialIMGArrayTest(void);											//	宣告InitialIMGArrayTest副程式
/*	BmpReadFilesizeTest副程式用於測試BmpReadFilesize副程式
	本測試副程式之輸入、輸出皆為void，所有參數皆設計於測試副程式中，
	以觀察使用副程式所需宣告與語法
	本副程式包含BmpReadFilesize副程式兩種用法－輸入檔案路徑包含副檔名與不含副檔名
	副程式輸入為void
	副程式輸出為void
 */
void BmpReadFilesizeTest(void);											//	宣告BmpReadFilesizeTest副程式
/*	BmpReadXSizeTest副程式用於測試BmpReadXSize副程式
	本測試副程式之輸入、輸出皆為void，所有參數皆設計於測試副程式中，
	以觀察使用副程式所需宣告與語法
	副程式輸入為void
	副程式輸出為void
 */
void BmpReadXSizeTest(void);											//	宣告BmpReadXSizeTest副程式
/*	BmpReadYSizeTest副程式用於測試BmpReadYSize副程式
	本測試副程式之輸入、輸出皆為void，所有參數皆設計於測試副程式中，
	以觀察使用副程式所需宣告與語法
	副程式輸入為void
	副程式輸出為void
 */
void BmpReadYSizeTest(void);											//	宣告BmpReadYSizeTest副程式
/*	BmpReadTest副程式用於測試BmpRead副程式
	本測試副程式之輸入、輸出皆為void，所有參數皆設計於測試副程式中，
	以觀察使用副程式所需宣告與語法
	副程式輸入為void
	副程式輸出為void
 */
void BmpReadTest(void);													//	宣告BmpReadTest副程式
/*	BmpWriteV2Test副程式用於測試BmpWriteV2副程式
	本測試副程式之輸入、輸出皆為void，所有參數皆設計於測試副程式中，
	以觀察使用副程式所需宣告與語法
	副程式輸入為void
	副程式輸出為void
 */
void BmpWriteV2Test(void);												//	宣告BmpWriteV2Test副程式
//----主程式---- 
int main(int argc, char** argv)											//	主程式 
{																		//	進入主程式 
	
	printf("請輸入BMP圖檔(ex:test)：");									//	顯示訊息"請輸入BMP圖檔(ex:test)："
	char *FilenameString;												//	宣告FilenameString字元指標用以記錄使用者輸入欲讀取之圖檔
	FilenameString = (char*)malloc( MAX_PATH * sizeof(char) );			//	配置FilenameString字元指標記憶體大小
	scanf("%s",FilenameString);											//	使用者輸入圖檔名稱 
	BMPIMAGE BMPImage1;													//	宣告BMP影像1(BMPImage1)
	BMPImage1 = BmpFileRead(FilenameString,false);						//	呼叫BmpFileRead副程式讀取BMP圖檔
	free(FilenameString);												//	釋放FilenameString字元指標記憶體空間
	
	if(BMPImage1.IMAGE_DATA == NULL)									//	若讀取BMP圖檔發生錯誤
	{																	//	if statement start, 進入if敘述
		printf("圖檔物件錯誤!");										//	顯示"圖檔物件錯誤!" 
		return -1;														//	傳回-1，並結束程式 
	}																	//	if statement end, 結束if敘述
	
	
	//***原始圖像資料轉換至BMP24RGB指標資料結構***
	BMP24RGBIMAGE RGBImage1;											//	宣告RGBImage1用於BMP24RGB型態圖像資料處理
	RGBImage1.XSIZE = BMPImage1.XSIZE;									//	傳遞XSIZE資訊
	RGBImage1.YSIZE = BMPImage1.YSIZE;									//	傳遞YSIZE資訊
	RGBImage1.IMAGE_DATA = (BMP24RGB*)malloc(RGBImage1.XSIZE * RGBImage1.YSIZE * sizeof(BMP24RGB));
	//	配置AnalysisData(二維)指標記憶體大小
	if (RGBImage1.IMAGE_DATA == NULL) 									//	若建立影像空間失敗 
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return -1;														//	傳回-1，並結束程式 
	}																	//	if statement end, 結束if敘述 
	RGBImage1.IMAGE_DATA = RAWImageToArray(BMPImage1.IMAGE_DATA, BMPImage1.XSIZE, BMPImage1.YSIZE);
	//	將BMP圖檔原始資料轉換為BMP24RGB型態二維陣列
	
	
	BmpWriteV1(ArrayToRAWImage(ImageSmoothing33V2(RGBImage1.IMAGE_DATA,RGBImage1.XSIZE, RGBImage1.YSIZE), RGBImage1.XSIZE, RGBImage1.YSIZE), BMPImage1.XSIZE, BMPImage1.YSIZE, "BMPImageSmoothing33");
	
	
	return 0;															//	傳回0 
}																		//	結束主程式 
//----BMP圖檔大小(Byte)讀取副程式----
unsigned long BmpReadFilesize(const char *filename, const bool FilenameExtension)
//	BmpReadFilesize副程式
{																		//	進入BMP圖檔大小(Byte)讀取副程式
	//***檢查副程式傳入檔名***
	if(filename == NULL)												//	若輸入filename為NULL
	{																	//	if statement start, 進入if敘述
		printf("檔案路徑輸入為NULL\n");									//	顯示"檔案路徑輸入為NULL"並換行
		return 0;														//	回傳數值0，並結束副程式
	}																	//	if statement end, 結束if敘述
	//***參考FilenameExtension變數控制檔名處理方式***
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	if statement start, 進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	if statement end, 結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	printf("正在讀取下列檔案大小(Byte)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	if statement start, 進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	if statement end, 結束if敘述	 
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
	{																	//	if statement start, 進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	if statement end, 結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	printf("正在讀取下列檔案xsize(寬度)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	if statement start, 進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	if statement end, 結束if敘述	 
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
	{																	//	if statement start, 進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	if statement end, 結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	printf("正在讀取下列檔案ysize(高度)：%s\n",fname_bmp);				//	顯示程式執行狀態
	FILE *fp;															//	宣告檔案指標fp區域變數
	fp = fopen(fname_bmp, "rb");										//	以rb(二進位讀取)模式開啟檔案
	if (fp==NULL)														//	若開啟檔案失敗 
	{																	//	if statement start, 進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	if statement end, 結束if敘述	 
	unsigned char header[54]; 											//	宣告檔頭設定header陣列 
	fread(header, sizeof(unsigned char), 54, fp);						//	讀取檔頭設定
	unsigned long BmpReadYSize;											//	宣告讀取BMP圖片檔案的y(高度)大小(像素)變數，型態為unsigned long 
	BmpReadYSize= header[22] + (header[23] << 8) + ( header[24] << 16) + ( header[25] << 24);
	//	計算輸入BMP圖片檔案y(高度)大小(像素) 
	fclose(fp);															//	關閉檔案
	return BmpReadYSize;												//	將計算出BMP圖片檔案y(高度)大小(像素)數值傳回 
}																		//	結束BMP圖檔ysize(高度)讀取副程式 
//----BMP file read functions, BMP圖檔讀取副程式---- 
char BmpRead(unsigned char *image,const int xsize,const int ysize, const char *filename, const bool FilenameExtension)
{																		//	進入BMP圖檔讀取副程式
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	if statement start, 進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("讀取檔案失敗！\n");										//	顯示錯誤訊息 
		return -1;														//	傳回-1，並結束副程式 
	}																	//	if statement end, 結束if敘述	 
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
	{																	//	if statement start, 進入if敘述
		printf("檔案路徑輸入為NULL\n");									//	顯示"檔案路徑輸入為NULL"並換行
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	if statement end, 結束if敘述
	//***副檔名填補***
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	if statement start, 進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	if statement end, 結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	//***確認檔案存在及可讀性***
	if (FileExistCheck(fname_bmp) == false)								//	若檔案不存在 
	{																	//	if statement start, 進入if敘述 
		printf("檔案%s不存在！\n",fname_bmp);							//	顯示錯誤訊息 
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	if statement end, 結束if敘述	 
	if (FileReadPermissionCheck(fname_bmp) == false)					//	若檔案不可讀取
	{																	//	if statement start, 進入if敘述 
		printf("讀取檔案錯誤！\n");										//	顯示錯誤訊息
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	if statement end, 結束if敘述
	//***填入圖檔資訊至OutputData***
	strcpy(OutputData.FILENAME, fname_bmp);								//	填入檔案路徑(含副檔名)
	OutputData.XSIZE = (unsigned int)BmpReadXSize(OutputData.FILENAME,true);
	//	讀取輸入BMP圖檔寬度(路徑已包含副檔名) 
	OutputData.YSIZE = (unsigned int)BmpReadYSize(OutputData.FILENAME,true);
	//	讀取輸入BMP圖檔高度(路徑已包含副檔名) 
	if( (OutputData.XSIZE == -1) || (OutputData.YSIZE == -1) )			//	若XSIZE或YSIZE為-1(代表讀取檔案失敗)	
	{																	//	if statement start, 進入if敘述 
		printf("讀取圖檔大小資訊失敗!");								//	顯示"讀取圖檔大小資訊失敗!"
		return OutputData;												//	回傳OutputData，並結束副程式
	}																	//	if statement end, 結束if敘述
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
		{																//	if statement start, 進入if敘述 
			printf("記憶體分配錯誤!");									//	顯示"記憶體分配錯誤!" 
			return OutputData;											//	回傳OutputData，並結束副程式
		}																//	if statement end, 結束if敘述 
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

void FreeBMPIMAGE(BMPIMAGE InputBMPIMAGE)								//	FreeBMPIMAGE function, FreeBMPIMAGE副程式
{																		//	FreeBMPIMAGE function start, 進入FreeBMPIMAGE副程式
	free((void *)InputBMPIMAGE.IMAGE_DATA);								//	釋放BMPIMAGE物件記憶體空間
}																		//	FreeBMPIMAGE function end, 結束FreeBMPIMAGE副程式
//----BMP圖檔寫入副程式---- 
/*	BmpWriteV1副程式程式執行BMP圖檔寫入，header陣列為BMP圖檔之檔頭 
 *
 *
 */
 
int BmpWriteV1(const unsigned char *image, const int xsize, const int ysize, const char *filename) 
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
	{																	//	if statement start, 進入if敘述
		return -1;														//	回傳-1，並結束副程式
	}																	//	if statement end, 結束if敘述
	
	fwrite(header, sizeof(unsigned char), 54, fp);						//	寫入BMP圖檔檔頭資訊(54Bytes)
	fwrite(image, sizeof(unsigned char), (size_t)(long)(xsize * 3 + FillingByte)*ysize, fp);
	//	寫入BMP圖檔影像資料
	fclose(fp);															//	關閉檔案指標
	return 0;															//	傳回0並結束副程式
}																		//	結束BmpWriteV1(BMP圖檔寫入)副程式
int BmpWriteV2(const BMPIMAGE OutputFile)								//	BmpWriteV2副程式
{																		//	進入BmpWriteV2副程式
	return BmpWriteV1(OutputFile.IMAGE_DATA, OutputFile.XSIZE, OutputFile.YSIZE, OutputFile.FILENAME);
	//	將OutputFile結構解析成各項參數傳入BmpWriteV1副程式
}																		//	結束BmpWriteV2副程式
void ViewBMPImage(const char *filename)									//	ViewBMPImage副程式
{																		//	進入ViewBMPImage副程式
	#ifdef _WIN32														//	若編譯環境為Windows系統
		if(filename==NULL)												//	若傳入之filename指標為NULL
		{																//	if statement start, 進入if敘述
			return;														//	結束副程式
		}																//	if statement end, 結束if敘述
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
		{																//	if statement start, 進入if敘述
			return;														//	結束副程式
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	if statement end, 結束if敘述
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
BMP24RGB *RAWImageToArray(const unsigned char *image, const int xsize, const int ysize)
//	BMP圖檔資料至陣列轉換副程式
{																		//	進入BMP圖檔資料至陣列轉換副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(BMP24RGB));	//	配置OutputData(二維)指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	if statement start, 進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].H = 0.0;	//	設定H=0
			}															//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].S = 0.0;	//	填入S數值
			}															//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				OutputData[LoopNumber1 * xsize + LoopNumber2].R = Max;	//	填入R資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].G = t;	//	填入G資料
				OutputData[LoopNumber1 * xsize + LoopNumber2].B = Min;	//	填入B資料
			}															//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		return False;													//	回傳False並結束副程式返回
	}																	//	if statement end, 結束if敘述
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	char WriteStringTemp[50];											//	宣告WriteStringTemp字串，用於暫存寫入資料
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序寫入圖像資料
	{																	//	進入for迴圈
		sprintf(WriteStringTemp,"第%d個像素R=%d\t",loop_num,image[loop_num].R);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	if statement start, 進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	if statement end, 結束if敘述
		sprintf(WriteStringTemp,"第%d個像素G=%d\t",loop_num,image[loop_num].G);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	if statement start, 進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	if statement end, 結束if敘述
		sprintf(WriteStringTemp,"第%d個像素B=%d\n",loop_num,image[loop_num].B);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	if statement start, 進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	if statement end, 結束if敘述
	}																	//	結束for迴圈
	return True;														//	回傳True並結束副程式返回
}																		//	結束ImageDataToTxt副程式
bool HSVDataToTxt(const char *file_name,const HSV *image,const int xsize,const int ysize)
//	HSVDataToTxt副程式，用於將HSV型態影像資料寫入txt檔
{																		//	進入HSVDataToTxt副程式
	char fname_txt[MAX_PATH];											//	宣告圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	sprintf(fname_txt, "%s.txt", file_name);							//	建立圖片資料記錄檔之完整檔名(最多MAX_PATH個字元)
	if(FileWrite(fname_txt,"","w") == False)							//	嘗試建立檔案，若檔案建立失敗
	{																	//	if statement start, 進入if敘述
		return False;													//	回傳False並結束副程式返回
	}																	//	if statement end, 結束if敘述
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	char WriteStringTemp[50];											//	宣告WriteStringTemp字串，用於暫存寫入資料
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序寫入圖像資料
	{																	//	進入for迴圈
		sprintf(WriteStringTemp,"第%d個像素H=%f\t",loop_num,(float)image[loop_num].H);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	if statement start, 進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	if statement end, 結束if敘述
		sprintf(WriteStringTemp,"第%d個像素S=%f\t",loop_num,(float)image[loop_num].S);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	if statement start, 進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	if statement end, 結束if敘述
		sprintf(WriteStringTemp,"第%d個像素V=%f\n",loop_num,(float)image[loop_num].V);
		//	生成寫入字串資料
		if(FileWrite(fname_txt,WriteStringTemp,"a")==False)				//	嘗試寫入資料
		{																//	if statement start, 進入if敘述
			return False;												//	回傳False並結束副程式返回
		}																//	if statement end, 結束if敘述
	}																	//	結束for迴圈
	return True;														//	回傳True並結束副程式返回
}																		//	結束HSVDataToTxt副程式
BMP24RGB *ImgDifference2(const BMP24RGB *InputData1,const BMP24RGB *InputData2,const int xsize,const int ysize)
//	ImgDifference2副程式
{																		//	進入ImgDifference2副程式
	BMP24RGB *OutputData;												//	宣告OutputData為BMP24RGB指標型態變數
	OutputData = (BMP24RGB*)malloc(xsize * ysize * sizeof(HSV));		//	配置OutputData指標記憶體大小
	if(OutputData == NULL)												//	若Output為空指標
	{																	//	if statement start, 進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤"
		return NULL;													//	回傳NULL，並結束副程式
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
		{																//	if statement start, 進入if敘述
			OutputData[loop_num1].R = image[loop_num1].R;				//	邊界點不處理
			OutputData[loop_num1].G = image[loop_num1].G;				//	邊界點不處理
			OutputData[loop_num1].B = image[loop_num1].B;				//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
		{																//	if statement start, 進入if敘述
			OutputData[loop_num1].R = image[loop_num1].R;				//	邊界點不處理
			OutputData[loop_num1].G = image[loop_num1].G;				//	邊界點不處理
			OutputData[loop_num1].B = image[loop_num1].B;				//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述
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
		{																//	if statement start, 進入if敘述
			
		}																//	if statement end, 結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if( (image[loop_num].R == 0) || (image[loop_num].G == 0) || (image[loop_num].B == 0) )
			//	若R、G、B像素值任一為0(可能為文字)
			{															//	if statement start, 進入if敘述
				YArray[PixelLocationY] = true;							//	將布林變數改為true
			}															//	if statement end, 結束if敘述
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
		{																//	if statement start, 進入if敘述
			
		}																//	if statement end, 結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if( (image[loop_num].R == 0) || (image[loop_num].G == 0) || (image[loop_num].B == 0) )
			//	若R、G、B像素值任一為0(可能為文字)
			{															//	if statement start, 進入if敘述
				YArray[PixelLocationY] = true;							//	將布林變數改為true
			}															//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素梯度
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素梯度
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				Direction = atan((long double)Gy / (long double)Gx) * ( 180 / M_PI);
				//	計算梯度方向(Direction)－第一象限
			}															//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
	long long int loop_num;												//	宣告loop_num區域變數供迴圈使用
	for(loop_num = 0; loop_num <(xsize * ysize); loop_num++)			//	以for迴圈依序計算圖像像素
	{																	//	進入for迴圈
		if( (loop_num < xsize) || ( (loop_num % xsize) == 0) || ( ((loop_num + 1) % xsize) == 0) || (loop_num >= (xsize*(ysize-1))))
		//	檢測邊界點像素
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].R = image[loop_num].R;					//	邊界點不處理
			OutputData[loop_num].G = image[loop_num].G;					//	邊界點不處理
			OutputData[loop_num].B = image[loop_num].B;					//	邊界點不處理
		}																//	if statement end, 結束if敘述
		else															//	若非邊界點像素
		{																//	進入else敘述
			if(mode==HorizontalHighPass)								//	若mode為垂直高通模式
			{															//	if statement start, 進入if敘述
				OutputData[loop_num].R = abs(
					image[loop_num-1].R 	* (-1) + image[loop_num].R		* 0 + image[loop_num+1].R 	* 1
					);													//	垂直高通模式計算
				OutputData[loop_num].G = abs(
					image[loop_num-1].G 	* (-1) + image[loop_num].G		* 0 + image[loop_num+1].G 	* 1
					);													//	垂直高通模式計算
				OutputData[loop_num].B = abs(
					image[loop_num-1].B 	* (-1) + image[loop_num].B		* 0 + image[loop_num+1].B 	* 1
					);													//	垂直高通模式計算
			}															//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
	if(mode == HighHigh)												//	若模式為HighHigh
	{																	//	if statement start, 進入if敘述
		OutputData = BMPHaarWavelet(BMPHaarWavelet(image,xsize,ysize,HorizontalHighPass),xsize,ysize,VerticalHighPass);
		//	呼叫BMPHaarWavelet副程式HorizontalHighPass模式，再呼叫BMPHaarWavelet副程式VerticalHighPass模式
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述 
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
	{																	//	if statement start, 進入if敘述 
		printf("記憶體分配錯誤!");										//	顯示"記憶體分配錯誤!" 
		return NULL;													//	傳回NULL，並結束程式 
	}																	//	if statement end, 結束if敘述
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
		{																//	if statement start, 進入if敘述
			OutputData[loop_num].H = image[loop_num].H;					//	填入原像素值
			OutputData[loop_num].S = image[loop_num].S;					//	填入原像素值
			OutputData[loop_num].V = image[loop_num].V;					//	填入原像素值
		}																//	if statement end, 結束if敘述
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

unsigned char BmpFillingByteCalc(const unsigned int xsize)				//	BmpFillingByteCalc(BMP圖檔填補位元計算)副程式
{																		//	進入BmpFillingByteCalc副程式
	unsigned char FillingByte;											//	宣告FillingByte記錄填補位元組計算結果
	FillingByte = ( xsize % 4);											//	計算填補位元組數量
	return FillingByte;													//	傳回計算結果
}																		//	結束BmpFillingByteCalc副程式
bool FileExistCheck(char *file_name)									//	FileExistCheck(檔案存在檢查)副程式
{																		//	進入FileExistCheck(檔案存在檢查)副程式
	if( access( file_name, F_OK ) != -1 )								//	若檔案存在
	{																	//	if statement start, 進入if敘述
    	return True;													//	傳回True
	} 																	//	if statement end, 結束if敘述
	else 																//	若檔案不存在
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束FileExistCheck(檔案存在檢查)副程式
bool FileReadPermissionCheck(const char *file_name)						//	FileReadPermissionCheck(檔案讀取權限檢查)副程式
{																		//	進入FileReadPermissionCheck(檔案讀取權限檢查)副程式
	if( access( file_name, R_OK ) != -1 )								//	若檔案可讀取
	{																	//	if statement start, 進入if敘述
    	return True;													//	傳回True
	} 																	//	if statement end, 結束if敘述
	else 																//	若檔案不可讀取
	{																	//	進入else敘述
	    return False;													//	回傳false
	}																	//	結束else敘述
} 																		//	結束FileReadPermissionCheck(檔案讀取權限檢查)副程式
bool FileWritePermissionCheck(const char *file_name)					//	FileWritePermissionCheck(檔案寫入權限檢查)副程式
{																		//	進入FileWritePermissionCheck(檔案寫入權限檢查)副程式
	if( access( file_name, W_OK ) != -1 )								//	若檔案可讀取
	{																	//	if statement start, 進入if敘述
    	return True;													//	傳回True
	} 																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		printf("FileWrite:mode ERROR!\n");								//	顯示"FileWrite:mode ERROR!"並換行
		return False;													//	回傳False並結束副程式返回
	}																	//	if statement end, 結束if敘述
	if( strcmp(mode, "a") == 0 || strcmp(mode, "a+") == 0 )				//	若mode參數為"a"或"a+"(擴充模式)
	{																	//	if statement start, 進入if敘述
		if (FileWritePermissionCheck(file_name) == False)				//	若檔案無法寫入
		{																//	if statement start, 進入if敘述
			printf("FileWrite:permission ERROR!\n");					//	顯示"FileWrite:permission ERROR!"並換行
			return False;												//	回傳False並結束副程式返回
		}																//	if statement end, 結束if敘述
	}																	//	if statement end, 結束if敘述
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
			{															//	if statement start, 進入if敘述
				if(OutputData[LoopNumber1] > OutputData[LoopNumber1 + 1])
				//	若較大的數值在前
				{														//	if statement start, 進入if敘述
					unsigned char TempNumber;							//	宣告TempNumber暫存變數(用於數值交換)
					TempNumber = OutputData[LoopNumber1];				//	數值交換
					OutputData[LoopNumber1] = OutputData[LoopNumber1 + 1];
					//	數值交換
					OutputData[LoopNumber1 + 1] = TempNumber;			//	數值交換
				}														//	if statement end, 結束if敘述
			}															//	if statement end, 結束if敘述
			else if( Mode == 1 )										//	若模式為由大排至小
			{															//	進入else if敘述
				if(OutputData[LoopNumber1] < OutputData[LoopNumber1 + 1])
				//	若較小的數值在前
				{														//	if statement start, 進入if敘述
					unsigned char TempNumber;							//	宣告TempNumber暫存變數(用於數值交換)
					TempNumber = OutputData[LoopNumber1];				//	數值交換
					OutputData[LoopNumber1] = OutputData[LoopNumber1 + 1];
					//	數值交換
					OutputData[LoopNumber1 + 1] = TempNumber;			//	數值交換
				}														//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		return -1;														//	傳回-1，並結束副程式
	}																	//	if statement end, 結束if敘述
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
		{																//	if statement start, 進入if敘述
			printf("第%d個字元為：%c\n",count_num + 1,Input_string[count_num]);
			//	顯示計數細節
		}																//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		DisplayNumber = (long double)0.0 - InputNumber;					//	填入DisplayNumber數值
		printf("-");													//	顯示負號"-"
	}																	//	if statement end, 結束if敘述
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
	{																	//	if statement start, 進入if敘述
		DisplayDigit = DisplayStartTimes;								//	給定DisplayDigit顯示位數為起始顯示數字次方數
	}																	//	if statement end, 結束if敘述
	else																//	若欲顯示數值InputNumber不大於10^14
	{																	//	進入else敘述
		DisplayDigit = 14;												//	給定DisplayDigit顯示位數為14位(有效位數)
	}																	//	結束else敘述
	while(TimesNumber >= DisplayStartTimes - DisplayDigit)				//	
	{																	//	進入while敘述
		if(TimesNumber == -1)											//	若TimesNumber(次方)為-1時
		{																//	if statement start, 進入if敘述
			printf(".");												//	顯示小數點
		}																//	if statement end, 結束if敘述
		DisplayChar = floor(DisplayNumber/(long double)pow(10,TimesNumber)) - pow(10,1) * floor(DisplayNumber/(long double)pow(10,(TimesNumber + 1)));
		//	取出顯示數值
		if( (DisplayChar >= 0) && (DisplayChar <= 9) )					//	若取出之數值介於0~9之間
		{																//	if statement start, 進入if敘述
			printf("%d",DisplayChar);									//	顯示數值
		}																//	if statement end, 結束if敘述
		else															//	若取出數值錯誤
		{																//	進入else敘述
			break;														//	結束while迴圈
		}																//	結束else敘述
		TimesNumber = TimesNumber - 1;									//	遞減TimesNumber變數
	}																	//	結束while敘述
}																		//	結束ShowLongDouble副程式

void InitialIMGArrayTest(void)											//	InitialIMGArrayTest副程式
{																		//	進入InitialIMGArrayTest副程式
	BmpWriteV1(ArrayToRAWImage(InitialIMGArray(10,10),10,10),10,10,"TestIMG");
	//	初始影像建立測試(測試InitialIMGArray副程式功能)
}																		//	結束InitialIMGArrayTest副程式
void BmpReadFilesizeTest(void)											//	BmpReadFilesizeTest副程式
{																		//	進入BmpReadFilesizeTest副程式
	//***正常呼叫方式***
	printf("BmpReadFilesize副程式測試(無副檔名)：%d\n", BmpReadFilesize("LennaTestBMP",false));
	//	呼叫BmpReadFilesize副程式取出"LennaTestBMP"BMP圖檔之大小
	printf("BmpReadFilesize副程式測試(含副檔名)：%d\n", BmpReadFilesize("LennaTestBMP.bmp",true));
	//	呼叫BmpReadFilesize副程式取出"LennaTestBMP"BMP圖檔之大小
	//***例外狀況測試***
	printf("BmpReadFilesize副程式例外狀況測試1：%d\n", BmpReadFilesize("LennaTestBMP.bmp",(int)123));
	//	以例外狀況呼叫副程式
}																		//	結束BmpReadFilesizeTest副程式
void BmpReadXSizeTest(void)												//	BmpReadXSizeTest副程式
{																		//	進入BmpReadXSizeTest副程式
	BMPIMAGE BMPImage1;													//	宣告BMP影像1(BMPImage1)
	printf("請輸入BMP圖檔：");											//	顯示訊息"請輸入BMP圖檔："
	scanf("%s",&BMPImage1.FILENAME);									//	使用者輸入圖檔名稱 
	bool FileCheck;														//	宣告FileCheck布林變數，用以記錄檔案是否存在(若檔案存在為true)
	FileCheck = FileExistCheck(BMPImage1.FILENAME);						//	呼叫FileExistCheck副程式檢查檔案是否存在
	if(FileCheck == false)												//	若檔案不存在
	{																	//	if statement start, 進入if敘述
		printf("圖檔不存在!");											//	顯示"圖檔不存在!"
		return;															//	程式結束 
	}																	//	if statement end, 結束if敘述
	BMPImage1.XSIZE = (unsigned int)BmpReadXSize(BMPImage1.FILENAME,false);
	//	讀取輸入BMP圖檔寬度 
	if(BMPImage1.XSIZE == -1)											//	若XSIZE為-1(代表讀取檔案失敗)	
	{																	//	if statement start, 進入if敘述 
		printf("讀取圖檔大小資訊失敗!");								//	顯示"讀取圖檔大小資訊失敗!"
		return;															//	程式結束 
	}																	//	if statement end, 結束if敘述 
	else																//	若XSIZE不為-1(正常讀取檔案)
	{																	//	進入else敘述
		printf("輸入圖檔寬度：%d\n",BMPImage1.XSIZE);					//	顯示輸入圖檔寬度數值 
		return;															//	程式結束 
	}																	//	結束else敘述 
}																		//	結束BmpReadXSizeTest副程式

void BmpReadYSizeTest(void)												//	BmpReadYSizeTest副程式
{																		//	進入BmpReadYSizeTest副程式
	BMPIMAGE BMPImage1;													//	宣告BMP影像1(BMPImage1)
	printf("請輸入BMP圖檔：");											//	顯示訊息"請輸入BMP圖檔："
	scanf("%s",&BMPImage1.FILENAME);									//	使用者輸入圖檔名稱 
	bool FileCheck;														//	宣告FileCheck布林變數，用以記錄檔案是否存在(若檔案存在為true)
	FileCheck = FileExistCheck(BMPImage1.FILENAME);						//	呼叫FileExistCheck副程式檢查檔案是否存在
	if(FileCheck == false)												//	若檔案不存在
	{																	//	if statement start, 進入if敘述
		printf("圖檔不存在!");											//	顯示"圖檔不存在!"
		return;															//	程式結束 
	}																	//	if statement end, 結束if敘述
	BMPImage1.YSIZE = (unsigned int)BmpReadYSize(BMPImage1.FILENAME,false);
	//	讀取輸入BMP圖檔高度 
	if(BMPImage1.YSIZE == -1)											//	若YSIZE為-1(代表讀取檔案失敗)	
	{																	//	if statement start, 進入if敘述 
		printf("讀取圖檔大小資訊失敗!");								//	顯示"讀取圖檔大小資訊失敗!"
		return;															//	程式結束 
	}																	//	if statement end, 結束if敘述 
	else																//	若YSIZE不為-1(正常讀取檔案)
	{																	//	進入else敘述
		printf("輸入圖檔高度：%d\n",BMPImage1.YSIZE);					//	顯示輸入圖檔高度數值 
		return;															//	程式結束 
	}																	//	結束else敘述 
}																		//	結束BmpReadYSizeTest副程式

void BmpReadTest(void)													//	BmpReadTest副程式
{																		//	進入BmpReadTest副程式
	char filename[MAX_PATH]="";											//	宣告filename字串
	printf("請輸入BMP圖檔(ex：test)：");								//	顯示訊息"請輸入BMP圖檔(ex：test)："
	scanf("%s",filename);												//	使用者輸入圖檔名稱 
	bool FilenameExtension = false;										//	設定使用者輸入圖檔名稱不含副檔名
	//***檢查輸入檔名是否空白***
	if(filename == NULL)												//	若輸入filename為NULL
	{																	//	if statement start, 進入if敘述
		printf("檔案路徑輸入為NULL\n");									//	顯示"檔案路徑輸入為NULL"並換行
		return;															//	結束副程式
	}																	//	if statement end, 結束if敘述
	//***副檔名填補***
	char fname_bmp[MAX_PATH];											//	宣告檔案名稱fname_bmp陣列變數(最多MAX_PATH個字元) 
	if(FilenameExtension == false)										//	若輸入參數fname_bmp不具副檔名
	{																	//	if statement start, 進入if敘述
		sprintf(fname_bmp, "%s.bmp", filename);							//	產生完整檔案路徑並存放至fname_bmp陣列
	}																	//	if statement end, 結束if敘述
	else																//	若輸入參數fname_bmp已包含副檔名
	{																	//	進入else敘述
		strcpy(fname_bmp,filename);										//	直接填入檔名路徑
	}																	//	結束else敘述
	//***確認檔案存在及可讀性***
	if (FileExistCheck(fname_bmp) == false)								//	若檔案不存在 
	{																	//	if statement start, 進入if敘述 
		printf("檔案不存在！\n");										//	顯示錯誤訊息 
		return;															//	結束副程式
	}																	//	if statement end, 結束if敘述	 
	if (FileReadPermissionCheck(fname_bmp) == false)					//	若檔案不可讀取
	{																	//	if statement start, 進入if敘述 
		printf("讀取檔案錯誤！\n");										//	顯示錯誤訊息
		return;															//	結束副程式
	}																	//	if statement end, 結束if敘述
	unsigned int XSIZE,YSIZE;											//	宣告XSIZE與YSIZE變數記錄圖檔寬度與高度
	XSIZE = (unsigned int)BmpReadXSize(fname_bmp,true);					//	讀取輸入BMP圖檔寬度(路徑已包含副檔名) 
	YSIZE = (unsigned int)BmpReadYSize(fname_bmp,true);					//	讀取輸入BMP圖檔高度(路徑已包含副檔名) 
	if( (XSIZE == -1) || (YSIZE == -1) )								//	若XSIZE或YSIZE為-1(代表讀取檔案失敗)	
	{																	//	if statement start, 進入if敘述 
		printf("讀取圖檔大小資訊失敗!");								//	顯示"讀取圖檔大小資訊失敗!"
		return;															//	結束副程式
	}																	//	if statement end, 結束if敘述
	else																//	若XSIZE與YSIZE皆不為-1(正常讀取檔案)
	{																	//	進入else敘述 
		printf("輸入圖檔寬度：%d\n",XSIZE);								//	顯示輸入圖檔寬度數值 
		printf("輸入圖檔高度：%d\n",YSIZE);								//	顯示輸入圖檔高度數值 
		printf("輸入影像大小(Byte)：%d\n",(size_t)XSIZE * YSIZE * 3);	//	顯示輸入影像大小數值(Byte) 
		char FILLINGBYTE;												//	宣告FILLINGBYTE記錄圖檔填補位元
		FILLINGBYTE = BmpFillingByteCalc(XSIZE);						//	呼叫BmpFillingByteCalc副程式計算填充之位元組數量
		unsigned char *IMAGE_DATA;										//	宣告IMAGE_DATA無號字元指標用以記錄圖像資料
		IMAGE_DATA = (unsigned char*)malloc((XSIZE * 3 + FILLINGBYTE) * YSIZE * sizeof(unsigned char));
		//	計算並建立影像大小空間 
		if (IMAGE_DATA == NULL) 										//	若建立影像空間失敗 
		{																//	if statement start, 進入if敘述 
			printf("記憶體分配錯誤!");									//	顯示"記憶體分配錯誤!" 
			return;														//	結束副程式
		}																//	if statement end, 結束if敘述 
		else															//	若未發生錯誤 
		{																//	進入else敘述 
			int loop_num = 0;											//	宣告區域變數loop_num供迴圈使用 
			for(loop_num=0;loop_num<((XSIZE * 3 + FILLINGBYTE) * YSIZE);loop_num++)
			//	以for迴圈初始化每個像素 
			{															//	進入for迴圈 
				IMAGE_DATA[loop_num]=255;								//	填入預設像素色彩數值 
			}															//	結束for迴圈 
			BmpRead(IMAGE_DATA, XSIZE, YSIZE, fname_bmp,true);			//	讀取圖檔資料
		}																//	結束else敘述
	}																	//	結束else敘述
	
}																		//	結束BmpReadTest副程式
void BmpWriteV2Test(void)												//	BmpWriteV2Test副程式
{																		//	進入BmpWriteV2Test副程式
	printf("請輸入BMP圖檔(ex:test)：");									//	顯示訊息"請輸入BMP圖檔(ex:test)："
	char *FilenameString;												//	宣告FilenameString字元指標用以記錄使用者輸入欲讀取之圖檔
	FilenameString = (char*)malloc( MAX_PATH * sizeof(char) );			//	配置FilenameString字元指標記憶體大小
	scanf("%s",FilenameString);											//	使用者輸入圖檔名稱 
	BMPIMAGE BMPImage1;													//	宣告BMP影像1(BMPImage1)
	BMPImage1 = BmpFileRead(FilenameString,false);						//	呼叫BmpFileRead副程式讀取BMP圖檔
	free(FilenameString);												//	釋放FilenameString字元指標記憶體空間
	printf("%s\n",BMPImage1.FILENAME);									//	顯示讀取圖檔之檔名
	
	if(BMPImage1.IMAGE_DATA == NULL)									//	若讀取BMP圖檔發生錯誤
	{																	//	if statement start, 進入if敘述
		printf("圖檔物件錯誤!");										//	顯示"圖檔物件錯誤!" 
		return;															//	結束程式 
	}																	//	if statement end, 結束if敘述
	
	//***BmpWriteV2副程式測試***
	sprintf(BMPImage1.FILENAME, "%s", "BmpWriteV2Test");				//	設定寫入圖檔名稱
	BmpWriteV2(BMPImage1);												//	將圖檔寫入至檔案
}																		//	結束BmpWriteV2Test副程式
