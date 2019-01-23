/*
 * fbcommon.h
 *
 * Copyright (C) 2018 tracyone
 *
 * Contacts: tracyone <tracyone@live.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _FBCOMMON_H
#define _FBCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <stdbool.h>
#include <debug.h>
#include <error.h>

#define SUNXI_DISP2_FB_ROTATE



/**
 * rect
 */
struct fb_rect {
	int x;  /*x cor*/
	int y;
	int width;
	int height;
};

/**
 * raw rgb file
 */
struct fb_raw_rgb {
	unsigned char* buf;
	struct fb_rect rect;
	unsigned char bits_per_pixel;
};

/**
 * raw rgb file
 */
struct fbraw_t {
	FILE *pic_fd;
	struct fb_rect rect;
	char *pic_path;
};

/**
 * fb_rgb:define a color
 */
struct fb_rgb {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char reserved;
} __attribute__((packed)) ;

/**
 * fb_dot:a dot's coordinate and color
 */
struct fb_dot {
	int x;
	int y;
	struct fb_rgb color;
};

/**
 * fb_line
 */
struct fb_line {
	struct fb_dot start;
	struct fb_dot end;
	struct fb_rgb color;
};

#define FOURCC(a, b, c, d) ((__u32)(a) | ((__u32)(b) << 8) | \
				 ((__u32)(c) << 16) | ((__u32)(d) << 24))

enum fb_pixel_format{
	FB_FORMAT_ARGB8888=FOURCC('A', 'R', '2', '4'), /* [31:0] A:R:G:B 8:8:8:8 little endian */
	FB_FORMAT_ABGR8888=FOURCC('A', 'B', '2', '4'), /* [31:0] A:B:G:R 8:8:8:8 little endian */
	FB_FORMAT_RGBA8888=FOURCC('R', 'A', '2', '4'), /* [31:0] R:G:B:A 8:8:8:8 little endian */
	FB_FORMAT_BGRA8888=FOURCC('B', 'A', '2', '4'), /* [31:0] B:G:R:A 8:8:8:8 little endian */
	FB_FORMAT_RGB888=FOURCC('R', 'G', '2', '4'), /* [23:0] R:G:B little endian */
	FB_FORMAT_BGR888=FOURCC('B', 'G', '2', '4'), /* [23:0] B:G:R little endian */
	FB_FORMAT_RGB565=FOURCC('R', 'G', '1', '6'), /* [15:0] R:G:B 5:6:5 little endian */
	FB_FORMAT_BGR565=FOURCC('B', 'G', '1', '6'), /* [15:0] B:G:R 5:6:5 little endian */
};

/**
 * fb ojecjt
 */
struct fb_object {
	int fd; /*fb fd*/
	int fb_id;
	char *framebuffer;
	bool flush_fb_rotate_buffer;
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	int (*fb_object_init)(struct fb_object **pfb, int fb_id);
	void (*print_fixed_info)(struct fb_object *pfb);
	void (*print_var_info)(struct fb_object *pfb);
	int (*fb_object_free)(struct fb_object *pfb);
	int (*fb_device_open)(struct fb_object *pfb);
	int (*fb_device_close)(struct fb_object *pfb);
	int (*fb_device_mmap)(struct fb_object *pfb);
	int (*fb_device_unmap)(struct fb_object *pfb);
	int (*fb_device_get_vinfo)(const struct fb_object *pfb);
	int (*fb_device_set_vinfo)(struct fb_object *pfb);
	int (*fb_device_get_finfo)(const struct fb_object *pfb);
	int (*fb_device_pan_dispaly)(struct fb_object *pfb);
	int (*fb_fill_rect)(struct fb_object *pfb, struct fb_rect *prect,
			    int color);
	int (*fb_clear_screen)(struct fb_object *pfb);
	int (*fb_device_blank)(struct fb_object *pfb, int blank_mode);
	int (*fb_draw_dot)(struct fb_object *pfb, struct fb_dot *pdot);
	int (*fb_draw_line)(struct fb_object *pfb, struct fb_line *pline);
	int (*fb_device_draw_raw_pic)(struct fb_object *pfb, struct fbraw_t *praw);
	int (*fb_device_draw_raw_buf)(struct fb_object *pfb, struct fb_raw_rgb *prgb);
	int (*fb_set_pixformat)(struct fb_object *pfb, enum fb_pixel_format fmt);
};

/**
 * @name       fb_object_init
 * @brief      init fb object
 * @param[IN]  fb_id:device num
 * @param[OUT] pfb:pointer of fb_object to be malloced
 * @return     0 if init successfully
 */
int fb_object_init(struct fb_object **pfb, int fb_id);

#ifdef __cplusplus
}
#endif

#endif /*End of file*/
