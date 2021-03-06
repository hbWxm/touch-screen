#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "touch.h"

//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,lcddev.height-18,200,16,16,"RST");//显示清屏区域
  POINT_COLOR=RED;//设置画笔蓝色 
	LCD_Fill(0,0,30,20,RED);
	LCD_Fill(32,0,62,20,CHENG);
	LCD_Fill(64,0,94,20,YELLOW);
	LCD_Fill(96,0,126,20,GREEN);
	LCD_Fill(128,0,158,20,QING);
	LCD_Fill(160,0,190,20,BLUE);
	LCD_Fill(192,0,222,20,ZI);
	LCD_Fill(224,0,254,20,BLACK);
	LCD_Fill(256,0,286,20,WHITE);
	LCD_Fill(288,0,318,20,GRAY);
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
//电阻触摸屏测试函数
void rtp_test(void)
{  
	while(1)
	{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]>lcddev.height-16)Load_Drow_Dialog();//清除
				else if(tp_dev.x[0]<30&&tp_dev.y[0]<20) {LCD_ShowxNum(30,120,1,1,24,0x00);  POINT_COLOR = RED;}
				else if(tp_dev.x[0]<62&&tp_dev.y[0]<20) {LCD_ShowxNum(30,120,2,1,24,0x00);  POINT_COLOR = CHENG;}
				else if(tp_dev.x[0]<94&&tp_dev.y[0]<20) {LCD_ShowxNum(30,120,3,1,24,0x00);  POINT_COLOR = YELLOW;}
				else if(tp_dev.x[0]<126&&tp_dev.y[0]<20){LCD_ShowxNum(30,120,4,1,24,0x00);  POINT_COLOR = GREEN;}
				else if(tp_dev.x[0]<158&&tp_dev.y[0]<20){LCD_ShowxNum(30,120,5,1,24,0x00);  POINT_COLOR = QING;}
				else if(tp_dev.x[0]<190&&tp_dev.y[0]<20){LCD_ShowxNum(30,120,6,1,24,0x00);  POINT_COLOR = BLUE;}
				else if(tp_dev.x[0]<222&&tp_dev.y[0]<20){LCD_ShowxNum(30,120,7,1,24,0x00);  POINT_COLOR = ZI;}
				else if(tp_dev.x[0]<254&&tp_dev.y[0]<20){LCD_ShowxNum(30,120,8,1,24,0x00);  POINT_COLOR = BLACK;}
				else if(tp_dev.x[0]<286&&tp_dev.y[0]<20){LCD_ShowxNum(30,120,9,1,24,0x00);  POINT_COLOR = WHITE;}
				else if(tp_dev.x[0]<318&&tp_dev.y[0]<20){LCD_ShowxNum(30,120,0,1,24,0x00);  POINT_COLOR = GRAY;}
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],1);		//画图	  
			}
		}else delay_ms(10);	//没有按键按下的时候 	    
	}
}
//电容触摸屏测试函数
const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
void ctp_test(void)
{
	u8 t=0;  	    
 	u16 lastpos[5][2];		//最后一次的数据 
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<OTT_MAX_TOUCH;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
	}	
}
int main(void)
{
	uart_init(115200);
	delay_init(168);
	LCD_Init();
	tp_dev.init();
//POINT_COLOR=GREEN;

//if(tp_dev.touchtype!=0XFF)LCD_ShowString(30,130,200,16,16,"Press KEY0 to Adjust");//电阻屏才显示
	
 	Load_Drow_Dialog();	
	delay_ms(1500);
	rtp_test(); 
	
//  while(1)
//	{
    
//	}
}



