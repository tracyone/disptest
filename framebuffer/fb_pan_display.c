#include "fbcommon.h"

int main(int argc, char *argv[])
{
	int ret = 1;
	struct fb_object *drawfb = NULL;
	int x = 0;

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

	/*define area that interest
	 * only work in the case of fb rotate
	 * function was enabled*/
	drawfb->vinfo.reserved[0] = 0;
	drawfb->vinfo.reserved[1] = 0;
	drawfb->vinfo.reserved[2] = drawfb->vinfo.xres;
	drawfb->vinfo.reserved[3] = drawfb->vinfo.yres;

	while(x < drawfb->vinfo.yres) {
		drawfb->vinfo.xoffset = 0;
		drawfb->vinfo.yoffset = x;
		x+=5;
		drawfb->fb_device_pan_dispaly(drawfb);
	}

	while(x >= 0) {
		drawfb->vinfo.xoffset = 0;
		drawfb->vinfo.yoffset = x;
		x-=5;
		drawfb->fb_device_pan_dispaly(drawfb);
	}

	ret = 0;
FREE_FB:
	drawfb->fb_object_free(drawfb);
OUT:
	return ret;
}
