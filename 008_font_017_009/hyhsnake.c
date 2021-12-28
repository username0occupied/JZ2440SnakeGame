#include "stdlib.h"
#include "s3c2440_soc.h"
#include "lcd/font.h"
#include "my_printf.h"
#include "hyhsnake.h"
#define Width 30
#define High 17
#define Area 510
extern volatile unsigned char Dir;

int Fresh(int start)
{
    static short int Length=3;
    static short int Head[2]={0},Map[Area] = {0},Size[2]={Width,High};
    // static char z = '0';
    char score[4];
    short int i;
    short int *p;
    if(start)
    {   
        // Map = calloc(Area, 1);
        //不检查是否分到地址
        Map[1] = -1;
        TCON |= (1<<0) | (1<<3);
		TCNTB0 = (int)7812;
        Map[0] = Length;

    }
    
    fb_print_cnchar(Head[0] * 16,Head[1] * 16, 12, 0x320);
    // p = Head + (Dir & 0x1);
    if(Dir&(1<<1)){
        Head[(Dir & 0x1)]++;}
    else{
        Head[(Dir & 0x1)]--;}
        //??
    Head[(Dir & 0x1)] = (Head[(Dir & 0x1)] + Size[(Dir & 0x1)]) % Size[(Dir & 0x1)];
    // *p = (*p + Width) % Width;
    p = Map + Head[0] + Head[1] * Width;
    // printf("Dir%d",Dir) ;
    if((*p)>1)
    {
        TCON &=	~((1<<0) | (1<<3));
        fb_print_string(180, 120, "Game over", 0x0000);
        fb_print_string(200, 140, "Score", 0x0000);
        fb_print_string(210, 160,itoa(Length,score,10) , 0x0000);

        // return 0;
    }else if(*p==-1)
    {
        ++Length;
        *p = Length;
        // srand(Dir+Length);
        while(Map[i = rand() % Area])
            srand(i+Length);
            // while (!(Map[i = rand() % Area]))
            // {
        --Map[i];

                //            Lengther = 1;
                //打印蛇头
        fb_print_cnchar(Head[0] * 16, Head[1] * 16, 13+Dir, 0x320);
        //打印食物
        fb_print_cnchar((i % Width) * 16, (i / Width) * 16, 11, 0x320);
            // }
        fb_print_cnchar(0, 256, 12, 0xfe0);
        fb_print_cnchar(16, 256, 12, 0xfe0);
        fb_print_string(0, 256,itoa(Length,score,10) , 0x0000);
    }
    else if(*p==0)
    {
        for (i = 0; i < Area;i++)
        {
            // if(Map[i]==Length-1)
            // {
            //     Map[i]--;
            //     fb_print_cnchar((i % Width) * 16, (i / Width) * 16, 12, 0x320);
            // }

            if(Map[i]==1)
            {
                //蛇尾消失
                Map[i]--;
                // printf("s") ;

                fb_print_cnchar((i % Width) * 16, (i / Width) * 16, 12, 0x7fe0);
            }else if(Map[i]==2)
            {
                //打印蛇尾
                Map[i]--;
                fb_print_cnchar((i % Width) * 16, (i / Width) * 16, 9, 0x7fe0);
                // printf("%d,%dw",i,Map[i]) ;

            }else if(Map[i]>2)
            {
                Map[i]--;
            }
            //逻辑摆放位置

        }
        *p = Length;
        fb_print_cnchar(Head[0] * 16, Head[1] * 16, 13+Dir, 0x320);
    }
    // fb_print_cnchar(180, 50, 1, 0x320);
    // fb_print_cnchar(200, 50, 2, 0x7fe0);



    //换行
//    if(i%)
    // for (i = 0; i < Area;)
    // {
    //     if(Lengther = 1)
    //     {

    //     }
    //     if(Lengther=0)
    // }
    return 0;
}
