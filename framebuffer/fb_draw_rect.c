#include "fbcommon.h"

#define RED 0xffff0000
#define GREEN 0xff00ff00
#define BLUE 0xff0000ff


int main(int argc, char *argv[])
{
	int ret = 1;
	struct fb_object *drawfb = NULL;
	struct fb_rect fill_rect;

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

	struct fb_dot red_dot;
	red_dot.x = 100;
	red_dot.color.red=0xff;
	red_dot.color.green=0x0;
	red_dot.color.blue=0x0;
	red_dot.color.reserved=0xff;
	int i;
	drawfb->flush_fb_rotate_buffer = false;
	
	for (i = 0; i < 200; ++i) {
		red_dot.y = i;
		drawfb->fb_draw_dot(drawfb, &red_dot);
	}
	drawfb->vinfo.reserved[0] = 0;
	drawfb->vinfo.reserved[1] = 0;
	drawfb->vinfo.reserved[2] = drawfb->vinfo.xres;
	drawfb->vinfo.reserved[3] = drawfb->vinfo.yres;
	drawfb->fb_device_pan_dispaly(drawfb);
	drawfb->flush_fb_rotate_buffer = true;


	struct fb_line blue_line;
	memset(&blue_line,0,sizeof(struct fb_line));
	blue_line.color.red=0x0;
	blue_line.color.green=0x0;
	blue_line.color.blue=0xff;
	blue_line.color.reserved=0xff;
	blue_line.start.x = drawfb->vinfo.xres*3 / 8;
	blue_line.start.y = drawfb->vinfo.yres*3 / 8;;
	blue_line.end.x = 0;
	blue_line.end.y = drawfb->vinfo.yres*3 / 8;;
	
	drawfb->fb_draw_line(drawfb, &blue_line);

	blue_line.color.red=0x0;
	blue_line.color.green=0xff;
	blue_line.color.blue=0;
	blue_line.color.reserved=0xff;
	blue_line.start.x = drawfb->vinfo.xres / 8 + drawfb->vinfo.xres / 4;
	blue_line.start.y = 0;;
	blue_line.end.x = drawfb->vinfo.xres / 8 + drawfb->vinfo.xres / 4;
	blue_line.end.y = drawfb->vinfo.yres / 8;
	
	drawfb->fb_draw_line(drawfb, &blue_line);



	ret = 0;
FREE_FB:
	drawfb->fb_object_free(drawfb);
OUT:
	return ret;
}
