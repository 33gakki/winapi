/*
#include <stdio.h>
#include <iostream>
extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include "libswscale\swscale.h"
#include "libswresample\swresample.h"
#include "libavutil\imgutils.h"
#include "libavutil\timestamp.h"


}
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"avfilter.lib")
static int get_format_from_sample_fmt(const char** fmt, enum AVSampleFormat sample_fmt)
{
	int i;
	struct sample_fmt_entry {
		enum AVSampleFormat sample_fmt; const char* fmt_be, * fmt_le;
	} sample_fmt_entries[] = {
		{ AV_SAMPLE_FMT_U8,  "u8",    "u8"    },
		{ AV_SAMPLE_FMT_S16, "s16be", "s16le" },
		{ AV_SAMPLE_FMT_S32, "s32be", "s32le" },
		{ AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
		{ AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
	};
	*fmt = NULL;

	for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
		struct sample_fmt_entry* entry = &sample_fmt_entries[i];
		if (sample_fmt == entry->sample_fmt) {
			*fmt = AV_NE(entry->fmt_be, entry->fmt_le);
			return 0;
		}
	}

	fprintf(stderr,
		"sample format %s is not supported as output format\n",
		av_get_sample_fmt_name(sample_fmt));
	return -1;
}

int main(void)
{

	printf("%s\n", avcodec_configuration());
	//av_register_all();
	AVFormatContext* pFormat = NULL;
	const char* path = "resource/yuanshen.mp4";
	const char* video_dst_filename = "Video_Test_out.h264";
	const char* audio_dst_filename = "Video_Test_out.aac";
	//����Ƶ��
	int ret;
	ret = avformat_open_input(&pFormat, path, NULL, NULL);
	if (ret)
	{
		printf("open movie file failed!!!");
		return -1;
	}
	printf("#######open movie file success#######");
	//�鿴��Ӧ������Ϣ
	ret = avformat_find_stream_info(pFormat, NULL);
	if (ret)
	{
		printf("avformat_find_stream_info failed!!!");
		return -1;
	}
	printf("#######avformat_find_stream_info success#######");
	int time = pFormat->duration;
	int mmintime = (time / 1000000) / 60;
	int mbittime = (time / 1000000) % 60;
	printf("%d��:%d��\n", mmintime, mbittime);
	av_dump_format(pFormat, NULL, path, 0);

	//��Ƶ��������ʼ��
	//1.��ȡ��Ƶ��Ӧ��streamindex
	int VideoStreamindex = -1;
	VideoStreamindex = av_find_best_stream(pFormat, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, NULL);

	//2.��ȡstream����
	AVStream* video_st = pFormat->streams[VideoStreamindex];

	//3.������Ϣ���ҽ�����
	const AVCodec* videostreamcode;
	videostreamcode = avcodec_find_decoder(pFormat->streams[VideoStreamindex]->codecpar->codec_id);
	if (!videostreamcode)
	{
		printf("avcodec_find_decoder failed!!!\n");
		return -1;
	}
	printf("avcodec_find_decoder!!!\n");
	//4.��ʼ������Ƶ������
	AVCodecContext* video_decode_context = avcodec_alloc_context3(videostreamcode);
	avcodec_parameters_to_context(video_decode_context, video_st->codecpar);

	ret = avcodec_open2(video_decode_context, videostreamcode, NULL);
	if (ret)
	{
		printf("avcodec_open2 failed!!!\n");
		return -1;
	}
	printf("avcodec_open2 success!!!\n");

	//5.���� codec ��ز�������������������
	avcodec_parameters_to_context(video_decode_context, video_st->codecpar);

	//6.��ʼ�����򿪽�����
	AVDictionary* video_opts = NULL;
	avcodec_open2(video_decode_context, videostreamcode, &video_opts);


	//��Ƶ���봦��
	// 1.��ȡ��Ƶ��Ӧ��stream_index
	int audio_stream_idx = av_find_best_stream(pFormat, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	printf("\n#===> �ҵ���Ƶ�� = %d\n", audio_stream_idx);

	// 2.��ȡ��stream ����
	AVStream* audio_st = pFormat->streams[audio_stream_idx];

	// 3.���� codec_id ���ҽ�����
	const AVCodec* audio_dec = avcodec_find_decoder(audio_st->codecpar->codec_id);
	printf("\n#===>�ҵ�������: %s,  ������id:%d  ����: %s\n", audio_dec->name, audio_dec->id, audio_dec->long_name);

	// 4.��ʼ����������������Ϣ
	AVCodecContext* audio_dec_ctx = avcodec_alloc_context3(audio_dec);

	// 5.���� codec ��ز�������������������
	avcodec_parameters_to_context(audio_dec_ctx, audio_st->codecpar);

	// 6.����������Ƶ������
	AVDictionary* audio_opts = NULL;
	avcodec_open2(audio_dec_ctx, audio_dec, &audio_opts);

	// 7.������Ƶ�⸴�ú�Ҫ�����λ��
	//sprintf_s(video_dst_filename, "%s.%s", input_filename, video_dec->name);
	//sprintf_s(audio_dst_filename, "%s.%s", input_filename, audio_dec->name);

	FILE* video_dst_file = NULL, * audio_dst_file = NULL;
	ret = fopen_s(&video_dst_file, video_dst_filename, "wb");
	printf("open file:%s   ret:%d\n", video_dst_filename, ret);
	ret = fopen_s(&audio_dst_file, audio_dst_filename, "wb");
	printf("open file:%s   ret:%d\n", audio_dst_filename, ret);


	uint8_t* video_dst_data[4] = { NULL };
	int video_dst_linesize[4] = { 0 };

	//��ʼ������Ƶ
	printf("###########��ʼ������Ƶ#########\n");
	size_t video_dst_bufsize = av_image_alloc(video_dst_data, video_dst_linesize, video_decode_context->width,
		video_decode_context->height, video_decode_context->pix_fmt, 1);

	//���䲢��ʼ��AVFrame.AVPacket
	AVFrame* frame = av_frame_alloc();

	AVPacket* packet;

	packet = av_packet_alloc();
	//av_init_packet(&packet);
	packet->data = NULL;
	packet->size = 0;

	int video_frame_count = 0, audio_frame_count = 0;
	printf("��ʼ�� frame!!!!!\n");
	ret = av_read_frame(pFormat, packet);
	printf("av_read_frame:%d\n", ret);
	while (av_read_frame(pFormat, packet) >= 0)
	{
		if (packet->stream_index == VideoStreamindex)
		{
			ret = avcodec_send_packet(video_decode_context, packet);
			while (ret >= 0)
			{
				ret = avcodec_receive_frame(video_decode_context, frame);
				if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
				{
					ret = 0;
					break;
				}
				av_image_copy(video_dst_data, video_dst_linesize,
					(const uint8_t**)(frame->data), frame->linesize, video_decode_context->pix_fmt, video_decode_context->width, video_decode_context->height);

				ret = (int)fwrite(video_dst_data[0], 1, video_dst_bufsize, video_dst_file);
			}



		}

		else if (packet->stream_index == audio_stream_idx)
		{
			// 11.1 �� packet ���� ���͸�������
			ret = avcodec_send_packet(audio_dec_ctx, packet);

			// 11.2 ��ȡ������֡����
			int m = frame->format;
			while (ret >= 0) {
				ret = avcodec_receive_frame(audio_dec_ctx, frame);

				if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
				{
					ret = 0;
					break;
				}
				// 11.3 д���ļ�
				//printf("#===> audio_frame n:%d  nb_samples:%d  pts:%s ",
				//		audio_frame_count++, frame->nb_samples, av_ts2timestr(frame->pts, &audio_dec_ctx->time_base));
				ret = (int)fwrite(frame->extended_data[0], 1, frame->nb_samples * av_get_bytes_per_sample(AVSampleFormat(frame->format)), audio_dst_file);
				//printf("Write Size:%d\n", ret);
			}
		}

		av_frame_unref(frame);
		// ���AVPacket�ṹ������
		av_packet_unref(packet);

		if (ret < 0)
			break;
	}

	// 12.����һ���հ�,ˢ�½�����
	ret = avcodec_send_packet(video_decode_context, NULL);
	// 12.1 ��ȡ������֡����
	while (ret >= 0) {
		ret = avcodec_receive_frame(video_decode_context, frame);

		if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
		{
			ret = 0;
			break;
		}
		// 10.3 ����֡���ݵ���Ƶ�ļ���
		printf("#===> video_frame n:%d  coded_n:%d  ", video_frame_count++, frame->coded_picture_number);
		av_image_copy(video_dst_data, video_dst_linesize,
			(const uint8_t**)(frame->data), frame->linesize, video_decode_context->pix_fmt, video_decode_context->width, video_decode_context->height);

		// 10.4 д���ļ�
		ret = (int)fwrite(video_dst_data[0], 1, video_dst_bufsize, video_dst_file);
		printf("Write Size:%d\n", ret);
	}

	ret = avcodec_send_packet(audio_dec_ctx, NULL);
	// 12.2 ��ȡ������֡����
	while (ret >= 0) {
		ret = avcodec_receive_frame(audio_dec_ctx, frame);

		if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
		{
			ret = 0;
			break;
		}
		// 11.3 д���ļ�

		printf("#===> audio_frame n:%d  nb_samples:%d  ",
			audio_frame_count++, frame->nb_samples);

		ret = (int)fwrite(frame->extended_data[0], 1, frame->nb_samples * av_get_bytes_per_sample(AVSampleFormat(frame->format)), audio_dst_file);
		printf("Write Size:%d\n", ret);
	}


	// 13. �⸴�����
	printf("�⸴�����Demuxing succeeded.\n");
	printf("Play the output video file with the command:\n"
		"ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
		av_get_pix_fmt_name(video_decode_context->pix_fmt), video_decode_context->width, video_decode_context->height, video_dst_filename);

	enum AVSampleFormat sfmt = audio_dec_ctx->sample_fmt;
	int n_channels = audio_dec_ctx->channels;
	const char* fmt;

	if (av_sample_fmt_is_planar(sfmt)) {
		const char* packed = av_get_sample_fmt_name(sfmt);
		printf("Warning: the sample format the decoder produced is planar "
			"(%s). This example will output the first channel only.\n",
			packed ? packed : "?");
		sfmt = av_get_packed_sample_fmt(sfmt);
		n_channels = 1;
	}
	if ((ret = get_format_from_sample_fmt(&fmt, sfmt)) < 0)
		goto end;
	printf("Play the output audio file with the command:\n"
		"ffplay -f %s -ac %d -ar %d %s\n",
		fmt, n_channels, audio_dec_ctx->sample_rate,
		audio_dst_filename);

end:
	avcodec_free_context(&video_decode_context);
	avcodec_free_context(&audio_dec_ctx);
	avformat_close_input(&pFormat);
	fclose(video_dst_file);
	fclose(audio_dst_file);
	av_frame_free(&frame);
	av_free(video_dst_data[0]);


	//����ռ�

	return 0;
}
*/