
#include "lcd.h"

#define LCD_FB_BASE 0x33c00000

lcd_params lcd_4_3_params = {
	.name = "lcd_4.3",
	.pins_pol = {
		.de    = NORMAL,	/* normal: �ߵ�ƽʱ���Դ������� */
		.pwren = NORMAL,    /* normal: �ߵ�ƽ��Ч */
		.vclk  = NORMAL,	/* normal: ���½��ػ�ȡ���� */
		.rgb   = NORMAL,	/* normal: �ߵ�ƽ��ʾ1 */
		.hsync = INVERT,    /* normal: ������ */
		.vsync = INVERT, 	/* normal: ������ */
	},
	.time_seq = {
		/* ��ֱ���� */
		.tvp=	10, /* vysnc������ */
		.tvb=	2,  /* �ϱߺڿ�, Vertical Back porch */
		.tvf=	2,  /* �±ߺڿ�, Vertical Front porch */

		/* ˮƽ���� */
		.thp=	41, /* hsync������ */
		.thb=	2,  /* ��ߺڿ�, Horizontal Back porch */
		.thf=	2,  /* �ұߺڿ�, Horizontal Front porch */

		.vclk=	9,  /* MHz */
	},
	.xres = 480,
	.yres = 272,
	//revise
	.bpp  = 16,  /* 16, no 24bpp */
	.fb_base = LCD_FB_BASE,
};


void lcd_4_3_add(void)
{
	register_lcd(&lcd_4_3_params);
}

