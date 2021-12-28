
#include "s3c2440_soc.h"
#include "lcd/font.h"
#include "hyhsnake.h"
#include "wechat.h"
extern volatile unsigned long sign;
volatile unsigned char icharset;
volatile unsigned char charset;
volatile unsigned char Dir=(1<<1)|(0<<0);
void timer_irq(void)
{
	/* ��Ƽ��� */
	
	static int i = 0;

	// GPFDAT|=GPFDAT46S;
    // GPFDAT &= (~i)<<4;
	// i=i<<1;       
	// if(i == 8)
	// 	i = 1;

	// int tmp;

	// cnt++;

	// tmp = ~cnt;
	// tmp &= 7;
	// GPFDAT &= ~(7<<4);
	// GPFDAT |= (tmp<<4);
	switch (sign)
	{
	case 1:
		BulbLight(i++,0);
		if(i==3)
			i=0;
		BulbLight(i,1);
		break;
	case 2:
		Fresh(0);
		break;
	case 3:
		charset = icharset * 9 + i+'A';
		charbuff(1);
		i++;
		if(i==9)
			i = 0;
		break;
	default:
		break;
	}
}

void timer_init(void)
{
	/* ����TIMER0��ʱ�� */
	/* Timer clk = PCLK / {prescaler value+1} / {divider value} 
	             = 50000000/(99+1)/16
	             = 31250hz
	 */
	TCFG0 = 99;  /* Prescaler 0 = 99, ����timer0,1 */
	TCFG1 &= ~0xf;
	TCFG1 |= 3;  /* MUX0 : 1/16 */

	/* ����TIMER0�ĳ�ֵ */
	TCNTB0 = (int)15625;  /* 0.5s�ж�һ�� */

	/* ���س�ֵ, ����timer0 */
	TCON |= (1<<1);   /* Update from TCNTB0 & TCMPB0 */

	/* ����Ϊ�Զ����ز����� */
	TCON &= ~(1<<1);
	TCON |= (1<<0) | (1<<3);  /* bit0: start, bit3: auto reload */
	//revise ���μ���
//	TCON &=	~(1<<3);
	/* �����ж� */
	register_irq(10, timer_irq);
}


