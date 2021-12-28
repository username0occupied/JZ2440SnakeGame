#include "s3c2440_soc.h"
#include "lcd/font.h"
#include "lcd/geometry.h"
#include "init.h"
#include "hyhsnake.h"
#include "wechat.h"
#include "my_printf.h"
#define INT_UART0     (1<<28)
#define  INT_RXD0_msk   (1<<0)
typedef void(*irq_func)(int);
irq_func irq_array[32];

volatile unsigned long sign=0;
extern volatile unsigned char Dir;
extern volatile unsigned char icharset;
extern volatile unsigned char charset;
volatile unsigned char charsetr;
void KeySign(int key,int state);
void KeyLight(int light,int state);
void BulbState(int state);
void BulbLight(char light,int state);
void StateSwitch(void);
/* SRCPND 用来显示哪个中断产生了, 需要清除对应位
 * bit0-eint0
 * bit2-eint2
 * bit5-eint8_23
 */

/* INTMSK 用来屏蔽中断, 1-masked
 * bit0-eint0
 * bit2-eint2
 * bit5-eint8_23
 */

/* INTPND 用来显示当前优先级最高的、正在发生的中断, 需要清除对应位
 * bit0-eint0
 * bit2-eint2
 * bit5-eint8_23
 */

/* INTOFFSET : 用来显示INTPND中哪一位被设置为1
 */

/* 初始化中断控制器 */
void interrupt_init(void)
{
	INTMSK &= ~((1<<0) | (1<<2) | (1<<5));
	INTMSK &= ~(1<<10);  /* enable timer0 int */
	INTMSK &= (~INT_UART0);
	INTSUBMSK &= ~(INT_RXD0_msk);
}

/* 读EINTPEND分辨率哪个EINT产生(eint4~23)
 * 清除中断时, 写EINTPEND的相应位
 */


void key_eint_irq(int irq)
{
	unsigned int val = EINTPEND;
	unsigned int val1 = GPFDAT;
	unsigned int val2 = GPGDAT;

	if (irq == 0) /* eint0 : s2 控制 D12 */
	{
		if (val1 & (1<<0)) /* s2 --> gpf6 */
		{
			/* 松开 */
//			GPFDAT |= (1<<6);
			switch (sign)
			{
			case 1:
				KeySign(0,1);
				break;
			case 2:
				if((((~val1)>>2)&1)^(Dir&1))
				{
					Dir = (((~val1)>>2)&1);
				}
				break;
			case 3:
				TCON &=	~((1<<0) | (1<<3));
				charbuff(2);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (sign)
			{
			case 1:
				KeySign(0,0);
				break;
			case 2:
				break;
			case 3:
				icharset = 0;
				TCON |= (1<<0) | (1<<3);
				break;
			default:
				break;
			}
			/* 按下 */
//			GPFDAT &= ~(1<<6);
			
		}
		
	}
	else if (irq == 2) /* eint2 : s3 控制 D11 */
	{
		if (val1 & (1<<2)) /* s3 --> gpf5 */
		{
			/* 松开 */
			// GPFDAT |= (1<<5);
			switch (sign)
			{
			case 1:
				KeySign(2,1);
				break;
			case 2:
				break;
			case 3:
				TCON &=	~((1<<0) | (1<<3));
				charbuff(2);
				break;
			default:
				break;
			}
		}
		else
		{
			/* 按下 */
			// GPFDAT &= ~(1<<5);
			switch (sign)
			{
			case 1:
				KeySign(2,0);
				break;
			case 2:
				break;
			case 3:
				icharset = 1;
				TCON |= (1<<0) | (1<<3);
				break;
			default:
				break;
			}
		}
		
	}
	else if (irq == 5) /* eint8_23, eint11--s4 控制 D10, eint19---s5 控制所有LED */
	{
		if (val & (1<<11)) /* eint11 */
		{
			if (val2 & (1<<3)) /* s4 --> gpf4 */
			{
				/* 松开 */
				// GPFDAT |= (1<<4);
				switch (sign)
				{
				case 1:
					KeySign(11,1);
					break;
				case 2:
				if((((~val1)>>2)&1)^(Dir&1))
				{
					Dir = (((~val1)>>2)&1)|(1<<1);
				}
				break;
				case 3:
					TCON &=	~((1<<0) | (1<<3));
					charbuff(2);
					break;
				default:
					break;
				}				
			}
			else
			{
				/* 按下 */
				// GPFDAT &= ~(1<<4);
				switch (sign)
				{
				case 1:
					KeySign(11,0);
					break;
				case 2:
					break;
				case 3:
					icharset = 2;
					TCON |= (1<<0) | (1<<3);
					break;
				default:
					break;
				}
				
			}
		}
		else if (val & (1<<19)) /* eint19 */
		{
			if (val2 & (1<<11))
			{
				/* 松开 */
				/* 熄灭所有LED */
				// GPFDAT |= ((1<<4) | (1<<5) | (1<<6));
				sign++;
				StateSwitch();
			}
			else
			{
				/* 按下: 点亮所有LED */
				// GPFDAT &= ~((1<<4) | (1<<5) | (1<<6));

			}
		}
	}
	else if(irq==28)
	{

		unsigned long val3 = SUBSRCPND;
		// printf("%d", 1);
		if(val3 & (1<<0)){

			switch (sign)
			{
			case 3:
				charsetr=URXH0;
				charbuff(3);
				break;
			
			default:
				break;
			}


			SUBSRCPND = (1<<0);
		}
	}
	EINTPEND = val;
}


void handle_irq_c(void)
{
	/* 分辨中断源 */
	int bit = INTOFFSET;

	/* 调用对应的处理函数 */
	irq_array[bit](bit);
	
	/* 清中断 : 从源头开始清 */
	SRCPND = (1<<bit);
	INTPND = (1<<bit);	
}

void register_irq(int irq, irq_func fp)
{
	irq_array[irq] = fp;

	INTMSK &= ~(1<<irq);
}


/* 初始化按键, 设为中断源 */
void key_eint_init(void)
{
	/* 配置GPIO为中断引脚 */
	GPFCON &= ~((3<<0) | (3<<4));
	GPFCON |= ((2<<0) | (2<<4));   /* S2,S3被配置为中断引脚 */

	GPGCON &= ~((3<<6) | (3<<22));
	GPGCON |= ((2<<6) | (2<<22));   /* S4,S5被配置为中断引脚 */
	

	/* 设置中断触发方式: 双边沿触发 */
	EXTINT0 |= (7<<0) | (7<<8);     /* S2,S3 */
	EXTINT1 |= (7<<12);             /* S4 */
	EXTINT2 |= (7<<12);             /* S5 */

	/* 设置EINTMASK使能eint11,19 */
	EINTMASK &= ~((1<<11) | (1<<19));

	register_irq(0, key_eint_irq);
	register_irq(2, key_eint_irq);
	register_irq(5, key_eint_irq);
	register_irq(28, key_eint_irq);

}

//revise按键状态判断函数
void KeySign(int key,int state)
{
//	static int sign = 0x02;
//	static int sign ;
	char temp;
	switch (key)
	{
	case 11:
		temp=2;
		break;
	case 2:
		temp=1;
		break;
	case 0:
		temp=0;
	default:
		break;
	}
	

	if(state==0){
		BulbState(temp);
		KeyLight(temp,1);
	}else
	{
		KeyLight(temp,0);
	}

		
}
//revise 按键亮函数
void KeyLight(int light,int state)
{
	fb_print_cnchar(0+light*16, 255,12, 0xffff);
	if(state)
	{
		fb_print_cnchar(0+light*16, 255,light*2+6, 0x0000);
	}else
	{
		fb_print_cnchar(0+light*16, 255,light*2+7, 0x0000);
	}
}
//revise 灯状态函数
void BulbState(int state)
{
	//关timer
	TCON &=	~((1<<0) | (1<<3));
	char i;
	switch (state)
	{
	case 2:
		for(i=0;i<3;i++)
		{
			BulbLight(i,1);
		}
		break;
	
	case 1:
		for(i=0;i<3;i++)
		{
			BulbLight(i,0);
		}
		break;
		
	case 0:
		TCON |= (1<<0) | (1<<3);
		break;
	default:
		break;
	}
}
//revise 灯亮函数
void BulbLight(char light,int state)
{
	fb_print_cnchar(0+light*16, 200,12, 0xffff);
	if(state==1)
	{
		fb_print_cnchar(0+light*16, 200,4, 0xffe0);
	}
	if(state==0)
	{
		fb_print_cnchar(0+light*16, 200,5, 0x7453);
	}
}
void StateSwitch(void)
{
	char i;
	switch (sign)
	{
	case 0:
		/* code */
		break;
	case 1:
		ClearScr(0xffffff);
		for(i=0;i<3;i++)
		{
			BulbLight(i,1);
			KeyLight(i,0);
		}
			timer_init();
			break;
	case 2:
		TCON &=	~((1<<0) | (1<<3));
		ClearScr(0x7CFC00);
		INTMSK |=  (1<<2);
		// 关eint2中断
		GPFCON &= ~(3<<4);
  /* S3 gpf5被配置为输入引脚 */

		Fresh(1);
		break;
	case 3:
		TCON &=	~((1<<0) | (1<<3));
		wechat_init();
		INTMSK &= ~((1<<0) | (1<<2) | (1<<5));
		GPFCON &= ~((3<<0) | (3<<4));
		GPFCON |= ((2<<0) | (2<<4));
		break;
	case 4:
		charset = 13;
		charbuff(2);
		sign = 3;
		break;
	default:
		break;
	}
}
