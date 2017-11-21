/* Copyright (C) 
* 2011 - Michael.Kang blackfin.kang@gmail.com
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file skyeye_lcd_intf.c
* @brief The interface for lcd controller
* @author Michael.Kang blackfin.kang@gmail.com
* @version 0.1
* @date 2011-09-20
*/
#ifndef __SKYEYE_LCD_INTF_H__
#define __SKYEYE_LCD_INTF_H__
#include <skyeye_lcd_surface.h>

typedef struct lcd_touchscreen{
	conf_object_t* obj;
	void (*touchscreen_update_status) (conf_object_t* object, int *Pen_buffer);
}lcd_touchscreen_t;
#define LCD_TS_INTF_NAME "lcd_ts_intf"

typedef struct lcd_ctrl{
	conf_object_t* conf_obj;
	int (*lcd_open) (conf_object_t *lcd_dev, lcd_surface_t* surface);
	int (*lcd_close) (conf_object_t *lcd_dev);
	int (*lcd_update) (conf_object_t *lcd_dev, lcd_surface_t* surface);
	int (*lcd_filter_read) (conf_object_t *lcd_dev, uint32 addr, uint32 *data, size_t count);
	int (*lcd_filter_write) (conf_object_t *lcd_dev, uint32 addr, uint32 data, size_t count);
	uint32 (*lcd_lookup_color) (conf_object_t *lcd_dev, uint32 color); /* return RGB32 color. */
}lcd_control_intf;
#define LCD_CTRL_INTF_NAME "lcd_ctrl"

typedef struct goldfish_fb_ctrl{
	conf_object_t* conf_obj;
	void (* fb_ctrl)(char * obj_name);
}goldfish_fb_control_intf;
#define FB_CTRL_INTF_NAME "goldfish_fb_ctrl"
 
#endif
