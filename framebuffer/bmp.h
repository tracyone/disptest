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
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

//file header
struct BitMapFileHeader {
	unsigned char bfType[2];
	//file type
	unsigned long bfSize;
	//bmp file size
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
} __attribute__((packed)) ;

//infomation head
struct BitMapInfoHeader {
	unsigned long biSize;
	// file header size
	long biWidth;
	//bmp width
	long biHeight;
	//bmp height
	unsigned short biPlanes;
	//always 1
	unsigned short biBitCount;
	//bit per pixel
	unsigned long biCompression;
	//always BI_RGB
	unsigned long biSizeImage;
	//bmp size, unit:byte
	long biXPelsPerMeter;
	//x resolution
	long biYPelsPerMeter;
	//y resolution
	unsigned long biClrUsed;
	//color index
	unsigned long biClrImportant;
	//...
} __attribute__((packed)) ;

//data rgb
struct RgbQuad {
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Reserved;
} __attribute__((packed)) ;

/**
 * bmp object
 */
struct bmp_t {
	struct BitMapFileHeader bmp_file_head;
	struct BitMapInfoHeader bmp_info_head;
	struct RgbQuad rgb_data;
	char *pic_path;
	FILE *pic_fd;
	int real_height;
	int (*bmp_init)(struct bmp_t **pbmp);
	int (*bmp_free)(struct bmp_t *pbmp);
	int (*bmp_open_file)(struct bmp_t *pbmp);
	int (*bmp_get_bmp_header)(struct bmp_t *pbmp);
	int (*bmp_rgb24_to_rgb32)(struct bmp_t *pbmp);
};

int bmp_init(struct bmp_t **pbmp);

#endif //__BMP_H__
