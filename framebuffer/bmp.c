/*
 * framebuffer/bmp/bmp.c
 *
 * Copyright (c) 2007-2018 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "bmp.h"

/**
 * @name       :bmp_open_file
 * @brief      :open bmp file
 * @param[IN]  :pbmp:pointer of bmp object
 * @return     :0 or -1 if error
 */
static int bmp_open_file(struct bmp_t *pbmp)
{
	int ret = -1;

	if (!pbmp || !pbmp->pic_path) {
		loge("Null pointer\n");
		goto OUT;
	}

	pbmp->pic_fd = fopen(pbmp->pic_path, "r+");
	if (!pbmp->pic_fd) {
		loge("open %s fail\n", pbmp->pic_path);
		goto OUT;
	}

	ret = 0;

OUT:
	return ret;
}
static int bmp_print_info(struct bmp_t *pbmp)
{
	int ret = -1;
	if (!pbmp || !pbmp->pic_fd) {
		loge("Null pointer");
		goto OUT;
	}

	loge("\n"
	     "\tFile size:%d\n"
	     "\tBit per pixel:%d\n"
	     "\tWidth:%d\n"
	     "\tHeight:%d\n"
	     "\tbiSize:%d\n"
	     "\tbiPlanes:%d\n"
	     "\tbiCompression:%d\n"
	     "\tzero_num:%d\n"
	     "\tline length:%d\n"
	     "\toffset:%d\n"
	     "\tfile header size:%d\n"
	     "\traw image size:%d\n"
	     "\t\n",
	     pbmp->bmp_file_head.bfSize,
	     pbmp->bmp_info_head.biBitCount,
	     pbmp->bmp_info_head.biWidth,
	     pbmp->bmp_info_head.biHeight,
	     pbmp->bmp_info_head.biSize,
	     pbmp->bmp_info_head.biPlanes,
	     pbmp->bmp_info_head.biCompression,
	     pbmp->zero_num,
	     pbmp->line_length,
	     pbmp->bmp_file_head.bfOffBits,
	     pbmp->bmp_info_head.biSize,
	     pbmp->bmp_info_head.biSizeImage
	     );

	ret = 0;
OUT:
	return ret;
}

/**
 * @name       :bmp_read_bmp
 * @brief      :read bmp from file
 * @param[IN]  :pbmp:
 * @return     :0 if success
 */
static int bmp_read_bmp(struct bmp_t *pbmp)
{
	int ret = -1;

	if (!pbmp || !pbmp->pic_fd) {
		loge("Null pointer\n");
		goto OUT;
	}

	if (1 != fread(&pbmp->bmp_file_head, sizeof(struct BitMapFileHeader), 1,
		       pbmp->pic_fd)) {
		loge("read BitMapFileHeader error!\n");
		goto OUT;
	}

	if (pbmp->bmp_file_head.bfType[0] != 'B' ||
	    pbmp->bmp_file_head.bfType[1] != 'M') {
		loge("Not a bmp file\n");
		goto OUT;
	}

	if (1 != fread((char *)&pbmp->bmp_info_head, sizeof(struct BitMapInfoHeader),
		       1, pbmp->pic_fd)) {
		loge("read BitMapInfoHeader error!\n");
		goto OUT;
	}
	if (pbmp->bmp_info_head.biBitCount == 24) {
		pbmp->zero_num = (4 - ((3 * pbmp->bmp_info_head.biWidth) % 4)) & 3;
	} else
		pbmp->zero_num = 0;

	pbmp->line_length =
	    pbmp->bmp_info_head.biWidth * (pbmp->bmp_info_head.biBitCount / 8) +
	    pbmp->zero_num;

	pbmp->real_height = (pbmp->bmp_info_head.biHeight < 0)
				? (-pbmp->bmp_info_head.biHeight)
				: pbmp->bmp_info_head.biHeight;

	pbmp->data_buf =
		(unsigned char*)malloc(pbmp->bmp_file_head.bfSize - pbmp->bmp_file_head.bfOffBits);

	ret = fread(pbmp->data_buf,
		    pbmp->bmp_file_head.bfSize - pbmp->bmp_file_head.bfOffBits, 1,
		    pbmp->pic_fd);


	ret = 0;

OUT:
	return ret;
}

int bmp_free(struct bmp_t *pbmp)
{
	int ret = -1;
	if (pbmp) {
		if (pbmp->pic_fd)
			fclose(pbmp->pic_fd);
		if (pbmp->pic_path)
			free(pbmp->pic_path);
		if (pbmp->data_buf)
			free(pbmp->data_buf);
		free(pbmp);
		pbmp = NULL;
		ret = 0;
	}

	return ret;
}

int bmp_rgb24_to_rgb32(struct bmp_t *pbmp)
{
	int ret = -1;
	int i ,j ;
	int srclinesize = 0, dstlinesize = 0;
	unsigned char *pdst = NULL;
	unsigned char *psrcline = NULL, *pdstline = NULL;
	unsigned char *psrcdot = NULL, *pdstdot = NULL;

	if (!pbmp || !pbmp->pic_fd || !pbmp->data_buf) {
		loge("Null pointer\n");
		goto OUT;
	}

	if (pbmp->bmp_info_head.biBitCount != 24 ||
	    pbmp->bmp_info_head.biBitCount == 32) {
		loge("bit per pixe is :%d No need to convert\n",
		     pbmp->bmp_info_head.biBitCount);
		goto OUT;
	}
	srclinesize = pbmp->line_length;
	dstlinesize = pbmp->bmp_info_head.biWidth * 4;
	pdst = (unsigned char *)malloc(pbmp->bmp_info_head.biWidth *
				       pbmp->real_height * 4);

	if (pbmp->bmp_info_head.biHeight & 0x80000000) {
		psrcline = (unsigned char *)pbmp->data_buf;
		pdstline = (unsigned char *)pdst;
		for (i = 0; i < pbmp->real_height; ++i) {
			psrcdot = psrcline;
			pdstdot = pdstline;
			for (j = 0; j < pbmp->bmp_info_head.biWidth; ++j) {
				*pdstdot++ = psrcdot[j*3];
				*pdstdot++ = psrcdot[j*3+1];
				*pdstdot++ = psrcdot[j*3+2];
				*pdstdot++ = 0xff;
			}
			psrcline += srclinesize;
			pdstline += dstlinesize;
		}
	} else {
		psrcline = (unsigned char *)pbmp->data_buf +(pbmp->real_height - 1)*pbmp->line_length;;
		pdstline = (unsigned char *)pdst;
		for (i = 0; i < pbmp->real_height; ++i) {
			psrcdot = psrcline;
			pdstdot = pdstline;
			for (j = 0; j < pbmp->bmp_info_head.biWidth; ++j) {
				*pdstdot++ = psrcdot[j*3];
				*pdstdot++ = psrcdot[j*3+1];
				*pdstdot++ = psrcdot[j*3+2];
				*pdstdot++ = 0xff;
			}
			psrcline -= srclinesize;
			pdstline += dstlinesize;
		}
	}

	free(pbmp->data_buf);
	pbmp->data_buf = (unsigned char *)malloc(pbmp->bmp_info_head.biWidth *
				       pbmp->real_height * 4);
	if (!pbmp->data_buf)
		loge("malloc data_buf fail\n");
	else {
		memcpy(pbmp->data_buf, pdst,
		       pbmp->bmp_info_head.biWidth * pbmp->real_height * 4);
		ret = 0;
	}
	
	free(pdst);
OUT:
	return ret;
}

int bmp_gen_bmp_file(struct bmp_t *pbmp, int width, int height, int bitcount)
{
	int ret = -1;

	if (!pbmp || !pbmp->pic_path) {
		loge("Null pointer\n");
		goto OUT;
	}
	if (!pbmp->data_buf) {
		loge("Please fill some rgb data to pbmp->data_buf\n");
		goto OUT;
	}

	if (!width || !height || (bitcount != 24 && bitcount != 32)) {
		loge("Invalid param! only rgb32 and rgb24 is support!");
		goto OUT;
	}

	pbmp->zero_num = (4 - ((3 * width) % 4)) & 3;
	pbmp->line_length = width * bitcount / 8 + pbmp->zero_num;

	pbmp->pic_fd = fopen(pbmp->pic_path, "w+");
	if (!pbmp->pic_fd) {
		loge("open %s fail\n", pbmp->pic_path);
		goto OUT;
	}

	pbmp->bmp_file_head.bfType[0] = 'B';
	pbmp->bmp_file_head.bfType[1] = 'M';
	pbmp->bmp_file_head.bfOffBits = 54;
	pbmp->bmp_file_head.bfSize = pbmp->line_length * height + 54;

	ret = fwrite(&pbmp->bmp_file_head, sizeof(struct BitMapFileHeader), 1,
	       pbmp->pic_fd);
	if (ret != 1)
		loge("fwrite:%s\n",strerror(errno));

	pbmp->bmp_info_head.biBitCount = bitcount;
	pbmp->bmp_info_head.biHeight = -height;
	pbmp->bmp_info_head.biWidth = width;
	pbmp->bmp_info_head.biSize = sizeof(struct BitMapInfoHeader);
	pbmp->bmp_info_head.biPlanes = 1;
	pbmp->bmp_info_head.biCompression = 0;
	pbmp->bmp_info_head.biSizeImage = pbmp->line_length * height;

	ret = fwrite(&pbmp->bmp_info_head, sizeof(struct BitMapInfoHeader), 1,
	       pbmp->pic_fd);
	if (ret != 1)
		loge("fwrite:%s\n",strerror(errno));

	ret = fwrite(pbmp->data_buf, pbmp->bmp_info_head.biSizeImage, 1,
	       pbmp->pic_fd);
	if (ret != 1)
		loge("fwrite:%s\n",strerror(errno));
	else
		ret = 0;

	pbmp->data_buf = NULL;

OUT:
	return ret;

}

int bmp_init(struct bmp_t **pbmp)
{
	int ret = -1;
	struct bmp_t *p_obj = NULL;

	*pbmp = malloc(sizeof(struct bmp_t));

	if (!(*pbmp)) {
		loge("Malloc bmp_t fail\n");
		goto OUT;
	}
	p_obj = *pbmp;
	memset(p_obj, 0, sizeof(struct bmp_t));

	p_obj->bmp_read_bmp = bmp_read_bmp;
	p_obj->bmp_open_file = bmp_open_file;
	p_obj->bmp_init = bmp_init;
	p_obj->bmp_free = bmp_free;
	p_obj->bmp_print_info = bmp_print_info;
	p_obj->bmp_rgb24_to_rgb32 = bmp_rgb24_to_rgb32;
	p_obj->bmp_gen_bmp_file = bmp_gen_bmp_file;
	ret = 0;
OUT:
	return ret;
}
