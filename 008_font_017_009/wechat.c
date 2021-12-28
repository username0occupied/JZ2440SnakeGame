#include "lcd/font.h"
#include "lcd/geometry.h"
#include "wechat.h"
#include "s3c2440_soc.h"
#define buffback 0xffffff
#define allback 0xededed
extern const unsigned char wechat_1[] ;
extern const unsigned char wechat_0[] ;
extern volatile unsigned char icharset;
extern volatile unsigned char charset;
extern volatile unsigned char charsetr;
unsigned char stringsend[51];
unsigned char stringrice[51];




short int wechat_init(void)
{
    ClearScr(allback);
    ClearScrBlock(16, 256, 480, 272, buffback);
    fb_print_string(150, 0, "HuangYaohui", 0x0);
	TCNTB0 = (int)15625;

    return 0;
}
short int wechat(short int Length,char who)
{
    static short int Line=0;
    //mine
    if(who==1)
    {
        ClearScrBlock(432-(Length+1)*8,43 + Line*34,438,68 + Line*34, 0x95ea68);
        fb_print_string(432-Length*8,48 + Line*34,stringsend,0x0);
        draw_picture(443, 43 + Line*34, 443 + 32, 43 + Line*34 + 32, &wechat_1);
        charbuff(0);
    }else if(who==0)
    {
        ClearScrBlock(45,43 + Line*34,50+Length*8,68 + Line*34, 0xffffff);
        fb_print_string(48,48 + Line*34,stringrice,0x0);
        draw_picture(5, 43 + Line*34, 5 + 32, 43 + Line*34 + 32, &wechat_0);
    }
    Line++;
    if(Line==6)
        Line = 0;
    return 0;
}
short int charbuff(short int fresh)
{
    static short int Length=0;
    static short int Lengthr=0;
    if(fresh==0)
    {
        Length=0;
        ClearScrBlock(16, 256, 480, 272, buffback);

    }else if(fresh==2)
    {
        if((charset==13)||(Length==49))
        {
            stringsend[Length] = 0;
            wechat(Length,1);
            printf("%c",13);
        }else
        {
            fb_print_char(16+Length*8, 256, 219, buffback);
            fb_print_char(16+Length*8, 256, charset, 0x0000);
            stringsend[Length] = charset;
            Length++;
            icharset = 0;
            printf("%c",charset);
        }
        icharset = 0;
    }else if(fresh==1)
    {
        if(charset=='[')
            charset=32;
        fb_print_char(16+Length*8, 256, 219, buffback);
        fb_print_char(16+Length*8, 256, charset, 0x0000);
    }else if(fresh==3)
    {
        if(charsetr==13)
        {
            stringrice[Lengthr] = 0;
            wechat(Lengthr,0);
            Lengthr = 0;
            // printf("%c",charsetr);

        }else
        {
            stringrice[Lengthr] = charsetr;
            Lengthr++;
            // printf("%c",charsetr);

        }
    }
    return Length;
}