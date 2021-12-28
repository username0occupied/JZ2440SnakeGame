
#include "geometry.h"
#include "font.h"
const unsigned char string[3]={01,02,03,'/0'};
extern const unsigned char myprofile[] ;
void lcd_test(void)
{
	unsigned int fb_base;
	int xres, yres, bpp;
	int x, y;
	unsigned short *p;
	// unsigned int *p2;
		
	/* 初始化LCD */
	lcd_init();

	/* 使能LCD */
	lcd_enable();

	/* 获得LCD的参数: fb_base, xres, yres, bpp */
	get_lcd_params(&fb_base, &xres, &yres, &bpp);
	fb_get_lcd_params();
	font_init();
	
	/* black */
	p = (unsigned short *)fb_base;
	for (x = 0; x < xres; x++)
		for (y = 0; y < yres; y++)
			*p++ = 0x0000;

//revise 大头照
	draw_picture(0, 0, 270,270 ,&myprofile);
	/* 输出文字 */
	// fb_print_string(10, 10, "www.100ask.net\n\r100ask.taobao.com", 0xff00);
	fb_print_cnstring(280, 10,string, 0xffe0);
	fb_print_string(280, 26, "2019112037", 0xff00);
	fb_print_string(280, 42, "2552866132@qq.com", 0xff00);
	fb_print_string(280, 58, "Small & Beautiful", 0xff00);



}


