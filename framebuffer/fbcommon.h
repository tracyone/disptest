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

#define SUNXI_DISP2_FB_ROTATE

/**
 * fb ojecjt
 */
struct fb_object {
	int fd; /*fb fd*/
	int fb_id;
	char *framebuffer;
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
	int (*fb_draw_rect)(struct fb_object *pfb, int x0, int y0, int width,
			 int height, int color);
	int (*fb_clear_screen)(struct fb_object *pfb);
	int (*fb_device_blank)(struct fb_object *pfb, int blank_mode);
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
