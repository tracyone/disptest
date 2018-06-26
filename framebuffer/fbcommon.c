/*
 * fbcommon.c
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
 *
 */

#include "fbcommon.h"

/**
 * @name       fb_device_open
 * @brief      open fb device
 * @param[IN]  pfd:pointer of fb object
 * @return     fd
 */
static int fb_device_open(struct fb_object *pfb)
{
	int ret = -1;
	if (!pfb)
		goto OUT;
	char fbname[40];
	snprintf(fbname, 40, "/dev/fb%d", pfb->fb_id);
	pfb->fd = open(fbname, O_RDWR);
	ret = pfb->fd;
	if (ret < 0)
		perror("\n");
OUT:
	return ret;
}

/**
 * @name       fb_device_close
 * @brief      close fb device
 * @param[IN]  pfd:pointer of fb object
 * @return     0 if success
 */
static int fb_device_close(struct fb_object *pfb)
{
	int ret = -1;
	if (!pfb)
		goto OUT;
	char fbname[40];
	snprintf(fbname, 40, "/dev/fb%d", pfb->fb_id);
	ret = close(pfb->fd);
	if (ret < 0)
		perror("\n");
OUT:
	return ret;
}

/**
 * @name       print_fixed_info
 * @brief      print fixed info of fb device
 * @param[IN]  pfb:pointer of fb object
 * @return     none
 */
static void print_fixed_info(struct fb_object *pfb)
{
	if (pfb) {
		printf("Fixed screen info:\n"
		       "\tid: %s\n"
		       "\tsmem_start: 0x%lx\n"
		       "\tsmem_len: %d\n"
		       "\ttype: %d\n"
		       "\ttype_aux: %d\n"
		       "\tvisual: %d\n"
		       "\txpanstep: %d\n"
		       "\typanstep: %d\n"
		       "\tywrapstep: %d\n"
		       "\tline_length: %d\n"
		       "\tmmio_start: 0x%lx\n"
		       "\tmmio_len: %d\n"
		       "\taccel: %d\n"
		       "\n",
		       pfb->finfo.id, pfb->finfo.smem_start,
		       pfb->finfo.smem_len, pfb->finfo.type,
		       pfb->finfo.type_aux, pfb->finfo.visual,
		       pfb->finfo.xpanstep, pfb->finfo.ypanstep,
		       pfb->finfo.ywrapstep, pfb->finfo.line_length,
		       pfb->finfo.mmio_start, pfb->finfo.mmio_len,
		       pfb->finfo.accel);
	} else
		printf("Fixed screen info:NULL pointer\n");
}

/**
 * @name       print_var_info_
 * @brief      print var info of fb device
 * @param[IN]  pfb:pointer of fb object
 * @return     none
 */
static void print_var_info(struct fb_object *pfb)
{
	if (pfb) {
		printf("Var screen info:\n"
		       "\txres: %d\n"
		       "\tyres: %d\n"
		       "\txres_virtual: %d\n"
		       "\tyres_virtual: %d\n"
		       "\tyoffset: %d\n"
		       "\txoffset: %d\n"
		       "\tbits_per_pixel: %d\n"
		       "\tygrayscale: %d\n"
		       "\tred offset: %d length: %2d, msb_right: %2d\n"
		       "\tgreen offset: %d length: %2d, msb_right: %2d\n"
		       "\tblue offset: %d length: %2d, msb_right: %2d\n"
		       "\ttransp offset: %2d, length: %2d, msb_right: %2d\n"
		       "\tnonstd: %d\n"
		       "\tactivate: %d\n"
		       "\twidth: %d\n"
		       "\theight: %d\n"
		       "\taccel_flag: %d\n"
		       "\tpixclock: %d\n"
		       "\tleft_margin: %d\n"
		       "\tright_margin: %d\n"
		       "\tupper_margin: %d\n"
		       "\tlower_margin: %d\n"
		       "\thsync_len: %d\n"
		       "\tvsync_len: %d\n"
		       "\tsync: %d\n"
		       "\tmode: %d\n"
		       "\treserved: %d %d %d %d\n"
		       "\n",
		       pfb->vinfo.xres, pfb->vinfo.yres,
		       pfb->vinfo.xres_virtual, pfb->vinfo.yres_virtual,
		       pfb->vinfo.xoffset, pfb->vinfo.yoffset,
		       pfb->vinfo.bits_per_pixel, pfb->vinfo.grayscale,
		       pfb->vinfo.red.offset, pfb->vinfo.red.length,
		       pfb->vinfo.red.msb_right, pfb->vinfo.green.offset,
		       pfb->vinfo.green.length, pfb->vinfo.green.msb_right,
		       pfb->vinfo.blue.offset, pfb->vinfo.blue.length,
		       pfb->vinfo.blue.msb_right, pfb->vinfo.transp.offset,
		       pfb->vinfo.transp.length, pfb->vinfo.transp.msb_right,
		       pfb->vinfo.nonstd, pfb->vinfo.activate,
		       pfb->vinfo.height, pfb->vinfo.width,
		       pfb->vinfo.accel_flags, pfb->vinfo.pixclock,
		       pfb->vinfo.left_margin, pfb->vinfo.right_margin,
		       pfb->vinfo.upper_margin, pfb->vinfo.lower_margin,
		       pfb->vinfo.hsync_len, pfb->vinfo.vsync_len,
		       pfb->vinfo.sync, pfb->vinfo.vmode,
		       pfb->vinfo.reserved[0],pfb->vinfo.reserved[1],pfb->vinfo.reserved[2],pfb->vinfo.reserved[3]
		       );
	} else
		printf("Var screen info:NULL pointer\n");
}

static int fb_object_free(struct fb_object *pfb)
{
	int ret = -1;
	if (pfb) {
		if (pfb->fb_id >= 0)
			pfb->fb_device_close(pfb);
		if (pfb->framebuffer)
			pfb->fb_device_unmap(pfb);
		free(pfb);
		pfb = NULL;
	}

	ret = 0;
	return ret;
}

static int fb_device_get_finfo(const struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || pfb->fd < 0)
		goto OUT;
	ret = ioctl(pfb->fd, FBIOGET_FSCREENINFO, &pfb->finfo);
	if (ret == -1) {
		perror("Error reading fixed infomation\n");
		goto OUT;
	}
OUT:
	return ret;
}

static int fb_device_get_vinfo(const struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || pfb->fd < 0)
		goto OUT;
	ret = ioctl(pfb->fd, FBIOGET_VSCREENINFO, &pfb->vinfo);
	if (ret == -1) {
		perror("Error reading var infomation\n");
		goto OUT;
	}
OUT:
	return ret;
}

static int fb_device_set_vinfo(struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || pfb->fd < 0)
		goto OUT;
	ret = ioctl(pfb->fd, FBIOPUT_VSCREENINFO, &pfb->vinfo);
	if (ret == -1) {
		perror("Error reading var infomation\n");
		goto OUT;
	}
OUT:
	return ret;
}

static int fb_device_pan_dispaly(struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || pfb->fd < 0)
		goto OUT;
	ret = ioctl(pfb->fd, FBIOPAN_DISPLAY, &pfb->vinfo);
	if (ret == -1) {
		perror("pan dispaly\n");
		goto OUT;
	}
OUT:
	return ret;
}

static int fb_device_unmap(struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || pfb->fd < 0 || !pfb->finfo.smem_len)
		goto OUT;

	munmap(pfb->framebuffer, pfb->finfo.smem_len);

	ret = 0;
OUT:
	return ret;
}

static int fb_device_mmap(struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || pfb->fd < 0 || !pfb->finfo.smem_len)
		goto OUT;

	pfb->framebuffer = (char *)mmap(0, pfb->finfo.smem_len, PROT_READ | PROT_WRITE,
					MAP_SHARED, pfb->fd, 0);
	if (pfb->framebuffer == MAP_FAILED) {
		perror("Failed to map framebuffer device to memory\n");
		goto OUT;
	}
	ret = 0;
OUT:
	return ret;
}
static void draw_rect_rgb32(struct fb_object *pfb, int x0, int y0, int width,
			    int height, int color)
{
	const int Bpp = pfb->vinfo.bits_per_pixel / 8;

	char *dest = pfb->framebuffer +
		     (y0 + pfb->vinfo.yoffset) * pfb->finfo.line_length +
		     (x0 + pfb->vinfo.xoffset) * Bpp;
	int x, y, j;
	/*move alpha to low byte*/
	if (Bpp == 4)
		color =
		    ((color >> 24) & 0x000000ff) | ((color << 8) & 0xffffff00);

	for (y = 0; y < height; ++y) {
		for (x = 0; x < width * Bpp; x += Bpp) {
			for (j = 0; j < Bpp; ++j) {
				dest[x + j] = (color >> 8 * (Bpp - 1 - j));
			}
		}
		dest += pfb->finfo.line_length;
	}
}

static int fb_clear_screen(struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || !pfb->framebuffer) 
		goto OUT;

	memset(pfb->framebuffer,0,pfb->finfo.smem_len);
#ifdef SUNXI_DISP2_FB_ROTATE
	/*define area that interest
	 * only work in the case of fb rotate
	 * function was enabled*/
	pfb->vinfo.reserved[0] = 0;
	pfb->vinfo.reserved[1] = 0;
	pfb->vinfo.reserved[2] = pfb->vinfo.xres;
	pfb->vinfo.reserved[3] = pfb->vinfo.yres;
	/*for compatiable purpose in the case fb rotate*/
	ret = pfb->fb_device_pan_dispaly(pfb);
#endif
OUT:
	return ret;
}

static int fb_draw_rect(struct fb_object *pfb, int x0, int y0, int width,
			int height, int color)
{
	int ret = -1;
	if (!pfb || !pfb->framebuffer)
		goto OUT;

	switch (pfb->vinfo.bits_per_pixel) {
	case 32:
	case 24:
		draw_rect_rgb32(pfb, x0, y0, width, height, color);
		ret = 0;
		break;
	default:
		printf("Warning: not impletement for color depth:%d\n",
		       pfb->vinfo.bits_per_pixel);
		break;
	}
#ifdef SUNXI_DISP2_FB_ROTATE
	/*define area that interest
	 * only work in the case of fb rotate
	 * function was enabled*/
	pfb->vinfo.reserved[0] = 0;
	pfb->vinfo.reserved[1] = 0;
	pfb->vinfo.reserved[2] = pfb->vinfo.xres;
	pfb->vinfo.reserved[3] = pfb->vinfo.yres;
	/*for compatiable purpose in the case fb rotate*/
	ret = pfb->fb_device_pan_dispaly(pfb);
#endif

OUT:
	return ret;
}

/**
 * @name       fb_object_init
 * @brief      init fb object
 * @param[IN]  fb_id:device num
 * @param[OUT] pfb:pointer of fb_object to be malloced
 * @return     0 if init successfully
 */
int fb_object_init(struct fb_object **pfb, int fb_id)
{
	int ret = -1;
	*pfb = (struct fb_object *)malloc(sizeof(struct fb_object));
	if (!(*pfb)) {
		perror("\n");
		goto OUT;
	}
	struct fb_object *p_obj = *pfb;
	p_obj->fb_id = fb_id;
	p_obj->fd = -1;
	p_obj->fb_object_init = fb_object_init;
	p_obj->fb_object_free = fb_object_free;
	p_obj->print_var_info = print_var_info;
	p_obj->print_fixed_info = print_fixed_info;
	p_obj->fb_device_open = fb_device_open;
	p_obj->fb_device_close = fb_device_close;
	p_obj->fb_device_get_finfo = fb_device_get_finfo;
	p_obj->fb_device_get_vinfo = fb_device_get_vinfo;
	p_obj->fb_device_set_vinfo = fb_device_set_vinfo;
	p_obj->fb_device_pan_dispaly = fb_device_pan_dispaly;
	p_obj->fb_device_mmap = fb_device_mmap;
	p_obj->fb_device_unmap = fb_device_unmap;
	p_obj->fb_draw_rect = fb_draw_rect;
	p_obj->fb_clear_screen = fb_clear_screen;
	ret = 0;
OUT:
	return ret;
}


/*End of File*/
