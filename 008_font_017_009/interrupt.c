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
/* SRCPND ������ʾ�ĸ��жϲ�����, ��Ҫ�����Ӧλ
 * bit0-eint0
 * bit2-eint2
 * bit5-eint8_23
 */

/* INTMSK ���������ж�, 1-masked
 * bit0-eint0
 * bit2-eint2
 * bit5-eint8_23
 */

/* INTPND ������ʾ��ǰ���ȼ���ߵġ����ڷ������ж�, ��Ҫ�����Ӧλ
 * bit0-eint0
 * bit2-eint2
 * bit5-eint8_23
 */

/* INTOFFSET : ������ʾINTPND����һλ������Ϊ1
 */

/* ��ʼ���жϿ����� */
void interrupt_init(void)
{
	INTMSK &= ~((1<<0) | (1<<2) | (1<<5));
	INTMSK &= ~(1<<10);  /* enable timer0 int */
	INTMSK &= (~INT_UART0);
	INTSUBMSK &= ~(INT_RXD0_msk);
}

/* ��EINTPEND�ֱ����ĸ�EINT����(eint4~23)
 * ����ж�ʱ, дEINTPEND����Ӧλ
 */


void key_eint_irq(int irq)
{
	unsigned int val = EINTPEND;
	unsigned int val1 = GPFDAT;
	unsigned int val2 = GPGDAT;

	if (irq == 0) /* eint0 : s2 ���� D12 */
	{
		if (val1 & (1<<0)) /* s2 --> gpf6 */
		{
			/* �ɿ� */
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
			/* ���� */
//			GPFDAT &= ~(1<<6);
			
		}
		
	}
	else if (irq == 2) /* eint2 : s3 ���� D11 */
	{
		if (val1 & (1<<2)) /* s3 --> gpf5 */
		{
			/* �ɿ� */
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
			/* ���� */
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
	else if (irq == 5) /* eint8_23, eint11--s4 ���� D10, eint19---s5 ��������LED */
	{
		if (val & (1<<11)) /* eint11 */
		{
			if (val2 & (1<<3)) /* s4 --> gpf4 */
			{
				/* �ɿ� */
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
				/* ���� */
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
				/* �ɿ� */
				/* Ϩ������LED */
				// GPFDAT |= ((1<<4) | (1<<5) | (1<<6));
				sign++;
				StateSwitch();
			}
			else
			{
				/* ����: ��������LED */
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
	/* �ֱ��ж�Դ */
	int bit = INTOFFSET;

	/* ���ö�Ӧ�Ĵ����� */
	irq_array[bit](bit);
	
	/* ���ж� : ��Դͷ��ʼ�� */
	SRCPND = (1<<bit);
	INTPND = (1<<bit);	
}

void register_irq(int irq, irq_func fp)
{
	irq_array[irq] = fp;

	INTMSK &= ~(1<<irq);
}


/* ��ʼ������, ��Ϊ�ж�Դ */
void key_eint_init(void)
{
	/* ����GPIOΪ�ж����� */
	GPFCON &= ~((3<<0) | (3<<4));
	GPFCON |= ((2<<0) | (2<<4));   /* S2,S3������Ϊ�ж����� */

	GPGCON &= ~((3<<6) | (3<<22));
	GPGCON |= ((2<<6) | (2<<22));   /* S4,S5������Ϊ�ж����� */
	

	/* �����жϴ�����ʽ: ˫���ش��� */
	EXTINT0 |= (7<<0) | (7<<8);     /* S2,S3 */
	EXTINT1 |= (7<<12);             /* S4 */
	EXTINT2 |= (7<<12);             /* S5 */

	/* ����EINTMASKʹ��eint11,19 */
	EINTMASK &= ~((1<<11) | (1<<19));

	register_irq(0, key_eint_irq);
	register_irq(2, key_eint_irq);
	register_irq(5, key_eint_irq);
	register_irq(28, key_eint_irq);

}

//revise����״̬�жϺ���
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
//revise ����������
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
//revise ��״̬����
void BulbState(int state)
{
	//��timer
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
//revise ��������
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
		// ��eint2�ж�
		GPFCON &= ~(3<<4);
  /* S3 gpf5������Ϊ�������� */

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
