#include "fbcommon.h"

#define RED 0xffff0000
#define GREEN 0xff00ff00
#define BLUE 0xff0000ff

void fb_set_format_usage()
{
	loge("fb_cap -path <bmp file path> -fb_id <id>\n");
}

int parse_cmdline(int argc, char **argv, int *fmt)
{
	int err = 0;
	int i = 0;

	if (!fmt)
		return -1;

	while (i < argc) {
		if ( ! strcmp(argv[i], "-fmt")) {
			if (argc > i+1) {
				i+=1;
				*fmt = atoi(argv[i]);
			}
			else{
				printf("-fb_id para error!\n");
				err++;
			}
		}


		if (!strcmp(argv[i], "-h")) {
			fb_set_format_usage();
			return -1;
		}

		i++;
	}

	if (err > 0) {
		fb_set_format_usage();
		return -1;
	} else
		return 0;
}

int main(int argc, char *argv[])
{
	int ret = 1;
	struct fb_object *drawfb = NULL;
	struct fb_rect fill_rect;
	int fmt = 0;

	ret = parse_cmdline(argc, argv, &fmt);

	ret = fb_object_init(&drawfb, 0); /*/dev/fb0*/
	if (ret || !drawfb)
		goto OUT;

	ret = drawfb->fb_device_open(drawfb);
	if (ret < 0)
		goto FREE_FB;


	ret = drawfb->fb_device_get_vinfo(drawfb);

	if (fmt == 0)
		drawfb->fb_set_pixformat(drawfb, FB_FORMAT_ARGB8888);
	else
		drawfb->fb_set_pixformat(drawfb, FB_FORMAT_RGB888);

	ret = drawfb->fb_device_get_vinfo(drawfb);
	drawfb->print_var_info(drawfb);

	ret = drawfb->fb_device_get_finfo(drawfb);
	drawfb->print_fixed_info(drawfb);


	ret = drawfb->fb_device_mmap(drawfb);
	if (ret || !drawfb->framebuffer)
		goto FREE_FB;

	drawfb->fb_clear_screen(drawfb);

	memset(&fill_rect, 0, sizeof(struct fb_rect));

	fill_rect.x = drawfb->vinfo.xres / 8;
	fill_rect.y = drawfb->vinfo.yres / 8;
	fill_rect.width = drawfb->vinfo.xres / 4;
	fill_rect.height = drawfb->vinfo.yres / 4;
	drawfb->fb_fill_rect(drawfb, &fill_rect, RED);

	fill_rect.x = drawfb->vinfo.xres * 3 / 8;
	fill_rect.y = drawfb->vinfo.yres * 3 / 8;
	drawfb->fb_fill_rect(drawfb, &fill_rect, GREEN);

	fill_rect.x = drawfb->vinfo.xres * 5 / 8;
	fill_rect.y = drawfb->vinfo.yres * 5 / 8;
	drawfb->fb_fill_rect(drawfb, &fill_rect, BLUE);

	ret = 0;
FREE_FB:
	drawfb->fb_object_free(drawfb);
OUT:
	return ret;
}
