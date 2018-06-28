/*
 * framebuffer/fbbmp/fbbmp.c
 *
 * Copyright (c) 2007-2018 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
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
#include "fbcommon.h"
#include "bmp.h"

void fbmp_usage()
{
	loge("fbbmp -path <bmp file path> [-x xoffset] [-y offset] [-h]\n");
}

int parse_cmdline(int argc, char **argv, struct bmp_t *p)
{
	int err = 0;
	int i = 0;

	if (!p)
		return -1;

	while (i < argc) {
		if ( ! strcmp(argv[i], "-path")) {
			if (argc > i+1) {
				i+=1;
				p->pic_path = (char*)malloc(strlen(argv[i] + 1));
				if (!p->pic_path) {
					++err;
					loge("-path malloc error!\n");
				} else {
					sscanf(argv[i], "%s", p->pic_path);
				}
			}
			else{
				loge("-path para error!\n");
				err++;
			}
		}


		if (!strcmp(argv[i], "-h")) {
			return -1;
		}

		i++;
	}

	if (err > 0) {
		fbmp_usage();
		return -1;
	} else
		return 0;
}

int main(int argc, char *argv[])
{
	int ret = 1;
	struct fb_object *drawfb = NULL;
	struct bmp_t *pbmp = NULL;
	struct fb_rect rect;
	memset(&rect, 0, sizeof(struct fb_rect));

	ret = bmp_init(&pbmp);
	if (ret || !pbmp)
		goto OUT;

	ret = parse_cmdline(argc, argv, pbmp);
	if (ret)
		goto FREE_BMP;

	ret = fb_object_init(&drawfb, 0); /*/dev/fb0*/
	if (ret || !drawfb)
		goto FREE_BMP;

	ret = drawfb->fb_device_open(drawfb);
	if (ret < 0)
		goto FREE_FB;

	ret = drawfb->fb_device_get_finfo(drawfb);

	drawfb->print_fixed_info(drawfb);

	ret = drawfb->fb_device_get_vinfo(drawfb);

	drawfb->print_var_info(drawfb);

	ret = drawfb->fb_device_mmap(drawfb);
	if (ret || !drawfb->framebuffer)
		goto FREE_FB;

	drawfb->fb_clear_screen(drawfb);

	ret = pbmp->bmp_open_file(pbmp);
	if (ret)
		goto  FREE_FB;
	ret = pbmp->bmp_get_bmp_header(pbmp);
	if (ret)
		goto  FREE_FB;

	rect.x = drawfb->vinfo.xres / 2 - pbmp->bmp_info_head.biWidth / 2;
	rect.y = drawfb->vinfo.yres / 2 - pbmp->real_height / 2;
	rect.width = pbmp->bmp_info_head.biWidth;
	rect.height = pbmp->real_height;

	ret = drawfb->fb_device_draw_pic(drawfb,pbmp,&rect);

FREE_FB:
	drawfb->fb_object_free(drawfb);
FREE_BMP:
	pbmp->bmp_free(pbmp);
OUT:
	return ret;
}
