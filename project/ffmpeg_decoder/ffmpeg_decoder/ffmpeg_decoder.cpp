#include "ffmpeg_decoder.h"
#include "stdafx.h"
#define __STD_CONSTANT_MACROS

extern "C"
{
	
	#include <libavutil/opt.h>
	#include "libavcodec/avcodec.h"     //包含解码器信息
	#include "libavformat/avformat.h"	//包含封装信息
	#include "libswscale/swscale.h"     //处理每一帧图像数据
}

int _tmain(int argc, _TCHAR* argv[])
{
	AVFormatContext *pFormatCtx;
	char *filepath = "Input.mp4";
	char *file_out = "Output.yuv";
	char *file_out1 = "Output.h264";
	FILE *fp_out;
	FILE *fp_out1;
	errno_t err,err1;

	err = fopen_s(&fp_out, file_out, "wb+");
	if (err != 0)
	{
		printf("The file 'crt_fopen_s.c' was opened\n");
		return -1;
	}

	err1 = fopen_s(&fp_out1, file_out1, "wb+");
	if (err1 != 0)
	{
		printf("The file 'crt_fopen_s.c' was opened\n");
		return -1;
	}

	av_register_all();    //注册所有组件
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();   //开辟内存
	if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) < 0) //打开输入视频文件
	{
		printf("Can't open the input stream.\n");
		return -1;
	}
	if (avformat_find_stream_info(pFormatCtx,NULL)<0)     //判断文件流，视频流还是音频流
	{
		printf("Can't find the stream information!\n");
		return -1;
	}

	int index_video = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)      //如果是视频流，则记录存储。
		{
			index_video = i;
			break;
		}		
	}
	if (index_video == -1)
	{
		printf("Can't find a video stream;\n");
		return -1;
	}

	AVCodecContext *pCodecCtx = pFormatCtx->streams[index_video]->codec;

	AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);     //查找解码器
	if (pCodec == NULL)
	{
		printf("Can't find a decoder!\n");
		return -1;
	}

	//if (pCodecCtx->codec_id == AV_CODEC_ID_H264)
	//{
	//	av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);
	//	av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);
	//}


	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)   //打开编码器
	{
		printf("Can't open the decoder!\n");
		return -1;
	}


	AVFrame *pFrame = av_frame_alloc();  //this only allocates the AVFrame itself, not the data buffers
	AVFrame *pFrameYUV = av_frame_alloc();

	uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height));  //开辟缓冲区
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);//帧和申请的内存结合

	AVPacket *pkt = (AVPacket *)av_malloc(sizeof(AVPacket));;
	av_init_packet(pkt);
	
	SwsContext * img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
	
	int frame_cnt = 0;
	int get_frame;
	int y_size = pCodecCtx->width*pCodecCtx->height;

	BMP24RGB *BMPImage = InitialIMGArray(pCodecCtx->width, pCodecCtx->height);
	int Loopnum = 0;

	while (av_read_frame(pFormatCtx,pkt) >=0  )
	{
		if (pkt->stream_index == index_video)
		{
			fwrite(pkt->data,1,pkt->size,fp_out1);
			if (avcodec_decode_video2(pCodecCtx, pFrame, &get_frame, pkt) < 0)
			{
				printf("Decode Error!\n");
				return -1;
			}
			if (get_frame)
			{
				printf("Decoded frame index: %d\n", frame_cnt);
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameYUV->data, pFrameYUV->linesize);
				fwrite(pFrameYUV->data[0], 1, y_size, fp_out);
				fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_out);
				fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_out);

				for (int Loopnum1 = 0; Loopnum1 < pCodecCtx->height; Loopnum1++)
				{
					for (int Loopnum2 = 0; Loopnum2 < pCodecCtx->width; Loopnum2++)
					{
						BMPImage[(pCodecCtx->height - Loopnum1) * pCodecCtx->width + Loopnum2].B =
							(unsigned int)ConvertYCrCbToRGB((int)pFrameYUV->data[0][Loopnum1 * pCodecCtx->width + Loopnum2], (int)pFrameYUV->data[1][(Loopnum1 * pCodecCtx->width + Loopnum2) / 4], (int)pFrameYUV->data[2][(Loopnum1 * pCodecCtx->width + Loopnum2) / 4]).rgbBlue;
						BMPImage[(pCodecCtx->height - Loopnum1) * pCodecCtx->width + Loopnum2].G =
							(unsigned int)ConvertYCrCbToRGB((int)pFrameYUV->data[0][Loopnum1 * pCodecCtx->width + Loopnum2], (int)pFrameYUV->data[1][(Loopnum1 * pCodecCtx->width + Loopnum2) / 4], (int)pFrameYUV->data[2][(Loopnum1 * pCodecCtx->width + Loopnum2) / 4]).rgbGreen;
						BMPImage[(pCodecCtx->height - Loopnum1) * pCodecCtx->width + Loopnum2].R =
							(unsigned int)ConvertYCrCbToRGB((int)pFrameYUV->data[0][Loopnum1 * pCodecCtx->width + Loopnum2], (int)pFrameYUV->data[1][(Loopnum1 * pCodecCtx->width + Loopnum2) / 4], (int)pFrameYUV->data[2][(Loopnum1 * pCodecCtx->width + Loopnum2) / 4]).rgbRed;
					}
				}

				BMP24RGBIMAGE RGBImage1;											//	RGBImage1ノBMP24RGB篈瓜钩戈矪瞶
				RGBImage1.XSIZE = pCodecCtx->width;										//	肚患XSIZE戈癟
				RGBImage1.YSIZE = pCodecCtx->height;										//	肚患YSIZE戈癟
				RGBImage1.IMAGE_DATA = BMPImage;
				//	肚患紇钩戈

				char FileNameBMP[100000];
				sprintf(FileNameBMP, "%d", frame_cnt);

				BmpWriteV1(ArrayToRAWImage(RGBImage1.IMAGE_DATA, RGBImage1.XSIZE, RGBImage1.YSIZE), RGBImage1.XSIZE, RGBImage1.YSIZE, FileNameBMP);
				//	㊣BmpWrite捌祘Α糶BMP郎
				
				//system("pause");

				frame_cnt++;
			}

		}
		av_free_packet(pkt);
	}

	//close
	fclose(fp_out);
	fclose(fp_out1);

	//free
	sws_freeContext(img_convert_ctx);
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	avformat_free_context(pFormatCtx);

	return 0;
}

