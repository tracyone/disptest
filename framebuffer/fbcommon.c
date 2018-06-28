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
		loge("Fixed screen info:\n"
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
		loge("Fixed screen info:NULL pointer\n");
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
		loge("Var screen info:\n"
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
		loge("Var screen info:NULL pointer\n");
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
	unsigned char color_code[4];
	/*argb to bgra*/
	for (x = 0; x < 4; ++x)
		color_code[x] = (color >> (8*x));

	for (y = 0; y < height; ++y) {
		for (x = 0; x < width * Bpp; x += Bpp) {
			for (j = 0; j < Bpp; ++j) {
				dest[x + j] = color_code[j];
			}
		}
		dest += pfb->finfo.line_length;
	}
}

static int fb_device_blank(struct fb_object *pfb, int blank_mode)
{
	int ret = -1;

	if (!pfb || pfb->fd < 0)
		goto OUT;

	ret = ioctl(pfb->fd, FBIOBLANK, blank_mode);
	if (ret == -1) {
		perror("FBIOBLANK err\n");
		goto OUT;
	}
OUT:
	return ret;
}

static int fb_clear_screen(struct fb_object *pfb)
{
	int ret = -1;

	if (!pfb || !pfb->framebuffer) 
		goto OUT;

	memset(pfb->framebuffer,0,pfb->finfo.smem_len);
#ifdef SUNXI_DISP2_FB_ROTATE
	if (pfb->flush_fb_rotate_buffer) {
		/*define area that interest
		 * only work in the case of fb rotate
		 * function was enabled*/
		pfb->vinfo.reserved[0] = 0;
		pfb->vinfo.reserved[1] = 0;
		pfb->vinfo.reserved[2] = pfb->vinfo.xres;
		pfb->vinfo.reserved[3] = pfb->vinfo.yres;
		/*for compatiable purpose in the case fb rotate*/
		ret = pfb->fb_device_pan_dispaly(pfb);
	}
#endif
OUT:
	return ret;
}

static int fb_fill_rect(struct fb_object *pfb, struct fb_rect *prect,
			    int color)
{
	int ret = -1;
	if (!pfb || !pfb->framebuffer || !prect)
		goto OUT;

	switch (pfb->vinfo.bits_per_pixel) {
	case 32:
	case 24:
		draw_rect_rgb32(pfb, prect->x, prect->y, prect->width,
				prect->height, color);
		ret = 0;
		break;
	default:
		loge("Warning: not impletement for color depth:%d\n",
		       pfb->vinfo.bits_per_pixel);
		break;
	}
#ifdef SUNXI_DISP2_FB_ROTATE
	if (pfb->flush_fb_rotate_buffer) {
		/*define area that interest
		 * only work in the case of fb rotate
		 * function was enabled*/
		pfb->vinfo.reserved[0] = 0;
		pfb->vinfo.reserved[1] = 0;
		pfb->vinfo.reserved[2] = pfb->vinfo.xres;
		pfb->vinfo.reserved[3] = pfb->vinfo.yres;
		/*for compatiable purpose in the case fb rotate*/
		ret = pfb->fb_device_pan_dispaly(pfb);
	}
#endif

OUT:
	return ret;
}

/**
 * @name       fb_device_draw_pic
 * @brief      draw bmp(rgb32) file to fb
 * @param[IN]  pic_buf:pointer that store pic file
 * @param[OUT] prect:pointer that spceifed the info of pic
 * @return     of if success
 */
static int fb_device_draw_pic(struct fb_object *pfb, struct bmp_t *pbmp ,
				  struct fb_rect *prect)
{
	int ret = -1;
	if (!pfb || !pfb->fd || !pbmp->pic_fd ||!pfb->framebuffer)
		goto OUT;
	unsigned int offset =
	    (prect->y + pfb->vinfo.yoffset) * pfb->finfo.line_length / 4 +
	    (prect->x + pfb->vinfo.xoffset);

	int *dest = (int*)pfb->framebuffer + offset;

	unsigned long pic_size = pbmp->bmp_file_head.bfSize - (sizeof(struct BitMapFileHeader) +
		  sizeof(struct BitMapInfoHeader));

	if (pic_size > pfb->finfo.smem_len - offset ) {
		loge("pic is too large\n");
		goto OUT;
	}
	if (pbmp->bmp_info_head.biBitCount != 32) {
		loge("Only support rgb32\n");
		goto OUT;
	}
	fseek(pbmp->pic_fd,54,SEEK_SET);
	while(!feof(pbmp->pic_fd)) {
		ret = fread((void *)dest,prect->width*4,1, pbmp->pic_fd);
		dest += pfb->finfo.line_length/4;
	}

#ifdef SUNXI_DISP2_FB_ROTATE
	if (pfb->flush_fb_rotate_buffer) {
		/*define area that interest*/
		/*only work in the case of fb rotate*/
		/*function was enabled*/
		pfb->vinfo.reserved[0] = 0;
		pfb->vinfo.reserved[1] = 0;
		pfb->vinfo.reserved[2] = pfb->vinfo.xres;
		pfb->vinfo.reserved[3] = pfb->vinfo.yres;
		/*for compatiable purpose in the case fb rotate*/
		ret = pfb->fb_device_pan_dispaly(pfb);
	}
#endif

OUT:
	return ret;
}

/**
 * @name       fb_device_draw_pic
 * @brief      draw bmp(rgb32) file to fb
 * @param[IN]  pic_buf:pointer that store pic file
 * @param[OUT] prect:pointer that spceifed the info of pic
 * @return     of if success
 */
static int fb_device_draw_raw_pic(struct fb_object *pfb, struct fbraw_t *praw)
{
	int ret = -1;
	if (!pfb || !praw || !pfb->fd || !praw->pic_fd ||!pfb->framebuffer)
		goto OUT;
	unsigned int offset =
	    (praw->rect.y + pfb->vinfo.yoffset) * pfb->finfo.line_length / 4 +
	    (praw->rect.x + pfb->vinfo.xoffset);

	int *dest = (int*)pfb->framebuffer + offset;

	unsigned long pic_size = praw->rect.width * praw->rect.height * 4;

	if (pic_size > pfb->finfo.smem_len - offset ) {
		loge("pic is too large\n");
		goto OUT;
	}

	while(!feof(praw->pic_fd)) {
		ret = fread((void *)dest,praw->rect.width*4,1, praw->pic_fd);
		dest += pfb->finfo.line_length/4;
	}

#ifdef SUNXI_DISP2_FB_ROTATE
	if (pfb->flush_fb_rotate_buffer) {
		/*define area that interest*/
		/*only work in the case of fb rotate*/
		/*function was enabled*/
		pfb->vinfo.reserved[0] = 0;
		pfb->vinfo.reserved[1] = 0;
		pfb->vinfo.reserved[2] = pfb->vinfo.xres;
		pfb->vinfo.reserved[3] = pfb->vinfo.yres;
		/*for compatiable purpose in the case fb rotate*/
		ret = pfb->fb_device_pan_dispaly(pfb);
	}
#endif

OUT:
	return ret;
}

/**
 * @name       :fb_draw_line
 * @brief      :draw line not support slash
 * @param[IN]  :
 * @param[OUT] :
 * @return     :
 */
static int fb_draw_line(struct fb_object *pfb, struct fb_line *pline)
{
	int ret = -1;
	int x,y;
	struct fb_dot p;

	if (!pfb || !pline || !pfb->framebuffer) {
		loge("Null pointer!\n");
		goto OUT;
	}

	memcpy(&p.color, &pline->color, sizeof(struct fb_rgb));

	if ((pline->end.x != pline->start.x) &&
	    (pline->end.y == pline->start.y)) {
		for (x = MIN(pline->start.x, pline->end.x);
		     x <= MAX(pline->start.x, pline->end.x); ++x) {
			y = (pline->end.y - pline->start.y) *
				(x - pline->start.x) /
				(pline->end.x - pline->start.x) +
			    pline->start.y;
			p.x = x;
			p.y = y;
			pfb->fb_draw_dot(pfb, &p);
		}
	} else if ((pline->end.x == pline->start.x) &&
	    (pline->end.y != pline->start.y)) {
		for (y = MIN(pline->start.y, pline->end.y);
		     y <= MAX(pline->start.y, pline->end.y); ++y) {
			x = (pline->end.x - pline->start.x) *
				(y - pline->start.y) /
				(pline->end.y - pline->start.y) +
				pline->start.x;
			p.x = x;
			p.y = y;
			pfb->fb_draw_dot(pfb, &p);
		}
	} else {
		loge("Not support slash\n");
		goto OUT;
	}

	ret = 0;

#ifdef SUNXI_DISP2_FB_ROTATE
	if (pfb->flush_fb_rotate_buffer) {
		/*define area that interest
		 * only work in the case of fb rotate
		 * function was enabled*/
		pfb->vinfo.reserved[0] = 0;
		pfb->vinfo.reserved[1] = 0;
		pfb->vinfo.reserved[2] = pfb->vinfo.xres;
		pfb->vinfo.reserved[3] = pfb->vinfo.yres;
		/*for compatiable purpose in the case fb rotate*/
		ret = pfb->fb_device_pan_dispaly(pfb);
	}
#endif
OUT:
	return ret;

}

static int fb_draw_dot(struct fb_object *pfb, struct fb_dot *pdot)
{
	int ret = -1;
	if (!pfb || !pdot || !pfb->framebuffer) {
		loge("Null pointer!\n");
		goto OUT;
	}
	int Bpp = pfb->vinfo.bits_per_pixel / 8;

	char *dest = pfb->framebuffer +
		     (pdot->y + pfb->vinfo.yoffset) * pfb->finfo.line_length +
		     (pdot->x + pfb->vinfo.xoffset) * Bpp;

	memcpy(dest, &pdot->color, Bpp);

	ret = 0;
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
	p_obj->fb_fill_rect = fb_fill_rect;
	p_obj->fb_clear_screen = fb_clear_screen;
	p_obj->fb_device_blank = fb_device_blank;
	p_obj->fb_device_draw_pic = fb_device_draw_pic;
	p_obj->fb_device_draw_raw_pic = fb_device_draw_raw_pic;
	p_obj->fb_draw_dot = fb_draw_dot;
	p_obj->fb_draw_line = fb_draw_line;
	ret = 0;
	p_obj->flush_fb_rotate_buffer = true;
OUT:
	return ret;
}


/*End of File*/
