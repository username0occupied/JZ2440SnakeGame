

extern const unsigned char fontdata_8x16[];
extern const unsigned char fontdata_16x16[];
/* ���LCD���� */
static unsigned int fb_base;
static int xres, yres, bpp;

void font_init(void)
{
	get_lcd_params(&fb_base, &xres, &yres, &bpp);
}

/* ������ĸ�ĵ�����LCD���軭���� */

void fb_print_char(int x, int y, char c, unsigned int color)
{
	int i, j;
	
	/* ����c��ascii����fontdata_8x16�еõ��������� */
	unsigned char *dots = &fontdata_8x16[c * 16];

	unsigned char data;
	int bit;

	/* ���ݵ��������ö�Ӧ���ص���ɫ */
	for (j = y; j < y+16; j++)
	{
		data = *dots++;
		bit = 7;
		for (i = x; i < x+8; i++)
		{
			/* ���ݵ����ĳλ�����Ƿ�����ɫ */
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
			if (x >= xres) /* ���� */
			{
				x = 0;
				y = y+16;
			}
		}
		i++;
	}
}
//revise ��ʾ���֣���ʼλ�ã����ֱ�ţ���ɫ16bpp
void fb_print_cnchar(int x, int y, char c, unsigned int color)
{
	int i, j;

	unsigned char *dots = &fontdata_16x16[c * 32];

	unsigned short data;
	int bit;

	/* ���ݵ��������ö�Ӧ���ص���ɫ */
	for (j = y; j < y+16; j++)
	{
		data = ((*dots)<<8)|(*(dots+1));
		dots =2+dots;
		bit = 15;
		for (i = x; i < x+16; i++)
		{
			/* ���ݵ����ĳλ�����Ƿ�����ɫ */
			if (data & (1<<bit))
				fb_put_pixel16(i, j, color);
			bit--;
		}
	}
}


//rivise�����ַ�����ӡ���� λ�� �ַ�����Ҫchar�������� ��ɫ16bpp
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
			if (x >= xres) /* ���� */
			{
				x = 0;
				y = y+16;
			}
		}
		i++;
	}
}
