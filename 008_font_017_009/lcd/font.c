

extern const unsigned char fontdata_8x16[];
extern const unsigned char fontdata_16x16[];
/* 获得LCD参数 */
static unsigned int fb_base;
static int xres, yres, bpp;

void font_init(void)
{
	get_lcd_params(&fb_base, &xres, &yres, &bpp);
}

/* 根据字母的点阵在LCD上描画文字 */

void fb_print_char(int x, int y, char c, unsigned int color)
{
	int i, j;
	
	/* 根据c的ascii码在fontdata_8x16中得到点阵数据 */
	unsigned char *dots = &fontdata_8x16[c * 16];

	unsigned char data;
	int bit;

	/* 根据点阵来设置对应象素的颜色 */
	for (j = y; j < y+16; j++)
	{
		data = *dots++;
		bit = 7;
		for (i = x; i < x+8; i++)
		{
			/* 根据点阵的某位决定是否描颜色 */
			if (data & (1<<bit))
				fb_put_pixel(i, j, color);
			bit--;
		}
	}
}


/* "abc\n\r123" */
void fb_print_string(int x, int y, char* str, unsigned int color)
{
	int i = 0, j;
	
	while (str[i])
	{
		if (str[i] == '\n')
			y = y+16;
		else if (str[i] == '\r')
			x = 0;

		else
		{
			fb_print_char(x, y, str[i], color);
			x = x+8;
			if (x >= xres) /* 换行 */
			{
				x = 0;
				y = y+16;
			}
		}
		i++;
	}
}
//revise 显示汉字，初始位置，汉字编号，颜色16bpp
void fb_print_cnchar(int x, int y, char c, unsigned int color)
{
	int i, j;

	unsigned char *dots = &fontdata_16x16[c * 32];

	unsigned short data;
	int bit;

	/* 根据点阵来设置对应象素的颜色 */
	for (j = y; j < y+16; j++)
	{
		data = ((*dots)<<8)|(*(dots+1));
		dots =2+dots;
		bit = 15;
		for (i = x; i < x+16; i++)
		{
			/* 根据点阵的某位决定是否描颜色 */
			if (data & (1<<bit))
				fb_put_pixel16(i, j, color);
			bit--;
		}
	}
}


//rivise中文字符串打印函数 位置 字符串需要char类型数组 颜色16bpp
void fb_print_cnstring(int x, int y, char* str, unsigned int color)
{
	int i = 0, j;
	
	while (str[i])
	{
		if (str[i] == '\n')
			y = y+16;
		else if (str[i] == '\r')
			x = 0;

		else
		{
			fb_print_cnchar(x, y, str[i], color);
			//revise 16bit
			x = x+16;
			if (x >= xres) /* 换行 */
			{
				x = 0;
				y = y+16;
			}
		}
		i++;
	}
}
