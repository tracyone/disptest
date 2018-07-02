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

void fbraw_usage()
{
	loge("fbbmp -path <rgb32 raw file path> [-x xoffset] [-y offset] [-h]\n");
}



int parse_cmdline(int argc, char **argv,struct fbraw_t *p)
{
	int err = 0;
	int i = 0;

	if (!p)
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
					p->pic_fd = fopen(p->pic_path, "r+");
					if (!p->pic_fd) {
						perror("\n");
						++err;
					}
				}
			}
			else{
				loge("-path para error!\n");
				err++;
			}
		}

		if ( ! strcmp(argv[i], "-w")) {
			if (argc > i+1) {
				i+=1;
				p->rect.width = atoi(argv[i]);
			}
			else{
				printf("-w para error!\n");
				err++;
			}
		}
		if ( ! strcmp(argv[i], "-h")) {
			if (argc > i+1) {
				i+=1;
				p->rect.height = atoi(argv[i]);
			}
			else{
				printf("-h para error!\n");
				err++;
			}
		}

		if ( ! strcmp(argv[i], "-x")) {
			if (argc > i+1) {
				i+=1;
				p->rect.x = atoi(argv[i]);
			}
			else{
				printf("-x para error!\n");
				err++;
			}
		}

		if ( ! strcmp(argv[i], "-y")) {
			if (argc > i+1) {
				i+=1;
				p->rect.y = atoi(argv[i]);
			}
			else{
				printf("-y para error!\n");
				err++;
			}
		}



		if (!strcmp(argv[i], "-h")) {
			return -1;
		}

		i++;
	}


	if (err > 0) {
		fbraw_usage();
		return -1;
	} else
		return 0;
}


int main(int argc, char *argv[])
{
	int ret = 1;
	struct fb_object *drawfb = NULL;
	struct fbraw_t rawfile;
	memset(&rawfile, 0, sizeof(struct fbraw_t));

	ret = parse_cmdline(argc, argv, &rawfile);
	if (ret || !rawfile.pic_fd)
		goto OUT;

	ret = fb_object_init(&drawfb, 0); /*/dev/fb0*/
	if (ret || !drawfb)
		goto FREE_RAW;

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

	ret = drawfb->fb_device_draw_raw_pic(drawfb,&rawfile);

FREE_FB:
	drawfb->fb_object_free(drawfb);
FREE_RAW:
	if (rawfile.pic_path)
		free(rawfile.pic_path);
	if (rawfile.pic_fd)
		fclose(rawfile.pic_fd);
OUT:
	return ret;
}
