
#ifndef _LCD_H
#define _LCD_H


enum {
	NORMAL = 0,
	INVERT = 1,
};

/* NORMAL : ��������
 * INVERT : ��ת����
 */
typedef struct pins_polarity {
	int de;    /* normal: �ߵ�ƽʱ���Դ������� */
	int pwren; /* normal: �ߵ�ƽ��Ч */
	int vclk;  /* normal: ���½��ػ�ȡ���� */
	int rgb;   /* normal: �ߵ�ƽ��ʾ1 */
	int hsync; /* normal: ������ */
	int vsync; /* normal: ������ */
}pins_polarity, *p_pins_polarity;

typedef struct time_sequence {
	/* ��ֱ���� */
	int tvp; /* vysnc������ */
	int tvb; /* �ϱߺڿ�, Vertical Back porch */
	int tvf; /* �±ߺڿ�, Vertical Front porch */

	/* ˮƽ���� */
	int thp; /* hsync������ */
	int thb; /* ��ߺڿ�, Horizontal Back porch */
	int thf; /* �ұߺڿ�, Horizontal Front porch */

	int vclk;
}time_sequence, *p_time_sequence;


typedef struct lcd_params {
	char *name;
	
	/* ���ż��� */
	pins_polarity pins_pol;
	
	/* ʱ�� */
	time_sequence time_seq;
	
	/* �ֱ���, bpp */
	int xres;
	int yres;
	int bpp;
	
	/* framebuffer�ĵ�ַ */
	unsigned int fb_base;
}lcd_params, *p_lcd_params;

void get_lcd_params(unsigned int *fb_base, int *xres, int *yres, int *bpp);

#endif /* _LCD_H */


