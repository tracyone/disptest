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
	     "File size:%d\n"
	     "Bit per pixel:%d\n"
	     "Height:%d\n"
	     "Width:%d\n"
	     "biSize:%d\n"
	     "biPlanes:%d\n"
	     "biCompression:%d\n"
	     "\n",
	     pbmp->bmp_file_head.bfSize,
	     pbmp->bmp_info_head.biBitCount,
	     pbmp->bmp_info_head.biHeight,
	     pbmp->bmp_info_head.biWidth,
	     pbmp->bmp_info_head.biSize,
	     pbmp->bmp_info_head.biPlanes,
	     pbmp->bmp_info_head.biCompression);

	ret = 0;
OUT:
	return ret;
}

/**
 * @name       :bmp_get_bmp_header
 * @brief      :get bmp header from file
 * @param[IN]  :
 * @param[OUT] :
 * @return     :
 */
static int bmp_get_bmp_header(struct bmp_t *pbmp)
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

	pbmp->real_height = (pbmp->bmp_info_head.biHeight < 0)
				? (-pbmp->bmp_info_head.biHeight)
				: pbmp->bmp_info_head.biHeight;

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
		free(pbmp);
		pbmp = NULL;
		ret = 0;
	}

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

	p_obj->bmp_get_bmp_header = bmp_get_bmp_header;
	p_obj->bmp_open_file = bmp_open_file;
	p_obj->bmp_init = bmp_init;
	p_obj->bmp_free = bmp_free;
	p_obj->bmp_print_info = bmp_print_info;
	ret = 0;
OUT:
	return ret;
}
