#include "fbcommon.h"

#define RED 0xffff0000
#define GREEN 0xff00ff00
#define BLUE 0xff0000ff


int main(int argc, char *argv[])
{
	int ret = 1;
	struct fb_object *drawfb = NULL;

	ret = fb_object_init(&drawfb, 0); /*/dev/fb0*/
	if (ret || !drawfb)
		goto OUT;

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

	drawfb->fb_draw_rect(drawfb,drawfb->vinfo.xres / 8,drawfb->vinfo.yres/8,drawfb->vinfo.xres/4,drawfb->vinfo.yres/4, RED);

	drawfb->fb_draw_rect(drawfb,drawfb->vinfo.xres*3 / 8,drawfb->vinfo.yres*3/8,drawfb->vinfo.xres/4,drawfb->vinfo.yres/4,GREEN);

	drawfb->fb_draw_rect(drawfb,drawfb->vinfo.xres*5 / 8,drawfb->vinfo.yres*5/8,drawfb->vinfo.xres/4,drawfb->vinfo.yres/4,BLUE);


	ret = 0;
FREE_FB:
	drawfb->fb_object_free(drawfb);
OUT:
	return ret;
}
