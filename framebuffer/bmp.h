/*
 * framebuffer/fbbmp/fbbmp.h
 *
 * Copyright (c) 2007-2018 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
 *
 * bmp header
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

#ifndef __BMP_H__
#define __BMP_H__

#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <debug.h>
#include <stdint.h>

//file header
struct BitMapFileHeader {
	uint8_t bfType[2];
	//file type
	uint32_t bfSize;
	//bmp file size
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} __attribute__((packed)) ;

//infomation head
struct BitMapInfoHeader {
	uint32_t biSize;
	// file header size
	int32_t biWidth;
	//bmp width
	int32_t biHeight;
	//bmp height
	uint16_t biPlanes;
	//always 1
	uint16_t biBitCount;
	//bit per pixel
	uint32_t biCompression;
	//always BI_RGB
	uint32_t biSizeImage;
	//bmp size, unit:byte
	int32_t biXPelsPerMeter;
	//x resolution
	int32_t biYPelsPerMeter;
	//y resolution
	uint32_t biClrUsed;
	//color index
	uint32_t biClrImportant;
	//...
} __attribute__((packed)) ;


/**
 * bmp object
 */
struct bmp_t {
	struct BitMapFileHeader bmp_file_head;
	struct BitMapInfoHeader bmp_info_head;
	char *pic_path;
	FILE *pic_fd;
	int real_height;
	int (*bmp_init)(struct bmp_t **pbmp);
	int (*bmp_free)(struct bmp_t *pbmp);
	int (*bmp_open_file)(struct bmp_t *pbmp);
	int (*bmp_print_info)(struct bmp_t *pbmp);
	int (*bmp_get_bmp_header)(struct bmp_t *pbmp);
	int (*bmp_rgb24_to_rgb32)(struct bmp_t *pbmp);
};

int bmp_init(struct bmp_t **pbmp);

#endif //__BMP_H__
