
#include "framebuffer.h"

/*
 * http://blog.csdn.net/p1126500468/article/details/50428613
 */

 //-------------��Բ������������Բ�ģ��뾶����ɫ----------	
 // 	   ��1/8Բ Ȼ������7/8�Գƻ�  
 // 		 ---------------->X  
 // 		 |(0,0)   0  
 // 		 |	   7	 1	
 // 		 |	  6 	  2  
 // 		 |	   5	 3	
 // 	  (Y)V		  4  
 //  
 // 	 L = x^2 + y^2 - r^2  
//  void draw_circle(int x, int y, int r, int color)  
//  {	
// 	 int a, b, num;  
// 	 a = 0;  
// 	 b = r;  
// 	 while(22 * b * b >= r * r) 		 // 1/8Բ����  
// 	 {	
// 		 fb_put_pixel(x + a, y - b,color); // 0~1  
// 		 fb_put_pixel(x - a, y - b,color); // 0~7  
// 		 fb_put_pixel(x - a, y + b,color); // 4~5  
// 		 fb_put_pixel(x + a, y + b,color); // 4~3  
   
// 		 fb_put_pixel(x + b, y + a,color); // 2~3  
// 		 fb_put_pixel(x + b, y - a,color); // 2~1  
// 		 fb_put_pixel(x - b, y - a,color); // 6~7  
// 		 fb_put_pixel(x - b, y + a,color); // 6~5  
		   
// 		 a++;  
// 		 num = (a * a + b * b) - r*r;  
// 		 if(num > 0)  
// 		 {	
// 			 b--;  
// 			 a--;  
// 		 }	
// 	 }	
//  }	
   
//  //-----------���ߡ���������ʼ���꣬�յ����꣬��ɫ--------	
//  void draw_line(int x1,int y1,int x2,int y2,int color)	
//  {	
// 	 int dx,dy,e;  
// 	 dx=x2-x1;	 
// 	 dy=y2-y1;	
// 	 if(dx>=0)	
// 	 {	
// 		 if(dy >= 0) // dy>=0  
// 		 {	
// 			 if(dx>=dy) // 1/8 octant  
// 			 {	
// 				 e=dy-dx/2;  
// 				 while(x1<=x2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){y1+=1;e-=dx;}	   
// 					 x1+=1;  
// 					 e+=dy;  
// 				 }	
// 			 }	
// 			 else		 // 2/8 octant	
// 			 {	
// 				 e=dx-dy/2;  
// 				 while(y1<=y2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){x1+=1;e-=dy;}	   
// 					 y1+=1;  
// 					 e+=dx;  
// 				 }	
// 			 }	
// 		 }	
// 		 else			// dy<0  
// 		 {	
// 			 dy=-dy;   // dy=abs(dy)  
// 			 if(dx>=dy) // 8/8 octant  
// 			 {	
// 				 e=dy-dx/2;  
// 				 while(x1<=x2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){y1-=1;e-=dx;}	   
// 					 x1+=1;  
// 					 e+=dy;  
// 				 }	
// 			 }	
// 			 else		 // 7/8 octant	
// 			 {	
// 				 e=dx-dy/2;  
// 				 while(y1>=y2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){x1+=1;e-=dy;}	   
// 					 y1-=1;  
// 					 e+=dx;  
// 				 }	
// 			 }	
// 		 }	   
// 	 }	
// 	 else //dx<0  
// 	 {	
// 		 dx=-dx;	 //dx=abs(dx)  
// 		 if(dy >= 0) // dy>=0  
// 		 {	
// 			 if(dx>=dy) // 4/8 octant  
// 			 {	
// 				 e=dy-dx/2;  
// 				 while(x1>=x2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){y1+=1;e-=dx;}	   
// 					 x1-=1;  
// 					 e+=dy;  
// 				 }	
// 			 }	
// 			 else		 // 3/8 octant	
// 			 {	
// 				 e=dx-dy/2;  
// 				 while(y1<=y2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){x1-=1;e-=dy;}	   
// 					 y1+=1;  
// 					 e+=dx;  
// 				 }	
// 			 }	
// 		 }	
// 		 else			// dy<0  
// 		 {	
// 			 dy=-dy;   // dy=abs(dy)  
// 			 if(dx>=dy) // 5/8 octant  
// 			 {	
// 				 e=dy-dx/2;  
// 				 while(x1>=x2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){y1-=1;e-=dx;}	   
// 					 x1-=1;  
// 					 e+=dy;  
// 				 }	
// 			 }	
// 			 else		 // 6/8 octant	
// 			 {	
// 				 e=dx-dy/2;  
// 				 while(y1>=y2)	
// 				 {	
// 					 fb_put_pixel(x1,y1,color);  
// 					 if(e>0){x1-=1;e-=dy;}	   
// 					 y1-=1;  
// 					 e+=dx;  
// 				 }	
// 			 }	
// 		 }	   
// 	 }	
//  }	

//revise ��ͼ����������ʼ���꣬�յ����꣬ͼ������
void draw_picture(int x1,int y1,int x2,int y2,unsigned short *pic)
{
	int i,j;
	for (j = y1; j < y2; j++)
	{
		for (i = x1; i < x2; i++)
		{
			fb_put_pixel16(i, j, *pic);
			pic=pic+1;
		}
	}

}

//revise ����
void ClearScr(int color)
{
	int i,j;
	for (j = 0; j < 272; j++)
	{
		for (i = 0; i < 480; i++)
		{
			fb_put_pixel(i, j, color);
		}
	}
}
void ClearScrBlock(int x1,int y1,int x2,int y2,int color)
{
	int i,j;
	for (j = y1; j < y2; j++)
	{
		for (i = x1; i < x2; i++)
		{
			fb_put_pixel(i, j, color);
		}
	}
}