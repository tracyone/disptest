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

void fb_cap_usage()
{
	loge("fb_cap -path <bmp file path> -fb_id <id>\n");
}

int parse_cmdline(int argc, char **argv, struct bmp_t *p, int *fb_id)
{
	int err = 0;
	int i = 0;

	if (!p || !fb_id)
		return -1;

	while (i < argc) {
		if ( ! strcmp(argv[i], "-path")) {
			if (argc > i+1) {
				i+=1;
				p->pic_path = (char*)malloc(strlen(argv[i]) + 1);
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

		if ( ! strcmp(argv[i], "-fb_id")) {
			if (argc > i+1) {
				i+=1;
				*fb_id = atoi(argv[i]);
			}
			else{
				printf("-fb_id para error!\n");
				err++;
			}
		}


		if (!strcmp(argv[i], "-h")) {
			return -1;
		}

		i++;
	}

	if (err > 0) {
		fb_cap_usage();
		return -1;
	} else
		return 0;
}

int main(int argc, char *argv[])
{
	int ret = 1;
	struct fb_object *drawfb = NULL;
	struct bmp_t *pbmp = NULL;
	int fb_id = 0;

	ret = bmp_init(&pbmp);
	if (ret || !pbmp)
		goto OUT;

	ret = parse_cmdline(argc, argv, pbmp, &fb_id);
	if (ret)
		goto FREE_BMP;

	ret = fb_object_init(&drawfb, fb_id); /*/dev/fb0*/
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


	pbmp->data_buf = (unsigned char*)malloc(drawfb->finfo.smem_len);
	memcpy(pbmp->data_buf, drawfb->framebuffer, drawfb->finfo.smem_len);

	ret = pbmp->bmp_gen_bmp_file(pbmp, drawfb->vinfo.xres, drawfb->vinfo.yres,
			       drawfb->vinfo.bits_per_pixel);
	if (ret)
		goto FREE_FB;

	pbmp->bmp_print_info(pbmp);

FREE_FB:
	drawfb->fb_object_free(drawfb);
FREE_BMP:
	pbmp->bmp_free(pbmp);
OUT:
	return ret;
}
