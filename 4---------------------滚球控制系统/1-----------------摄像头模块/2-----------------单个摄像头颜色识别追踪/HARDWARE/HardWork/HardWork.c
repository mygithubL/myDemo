#include "pbdata.h"
#include "HardWork.h"
/************************************************/
	/* 0 __________106__________213__________ 321*/
	/*  |           |            |            |   */
	/*  |     A     |     B      |      C     |   */
	/*  |           |            |            |   */
	/*80|___________|____________|____________|*/
	/*  |           |            |            |   */
	/*  |           |            |            |   */
	/*  |     D     |      E     |      F     |   */
 /*160|___________|____________|____________|*/
	/*  |           |            |            |   */
	/*  |           |            |            |   */
	/*  |     G     |      H     |      I     |   */
  /*  |___________|____________|____________|*/
	/*241                                         */
/************************************************/

extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义	

u32 a=0,b=0;     //行、列下标
u32 color_flag=0;//识别颜色像素点个数
u8 Goal_flag=0; //识别目标物标识
u16 r,y;

void camera_refresh(void)
{
	
		RESULT Resu;
	/***********Condition(最小色调，最大色调，最小饱和度，最大饱和度，最小亮度，最大亮度，最小宽度，最大宽度，最小高度，最大高度)**************/
	TARGET_CONDI Condition={0,160,0,160,230,255,20,20,320,240};	//API参数  hls的阈值，识别时用的
//	TARGET_CONDI Condition={215,240,20,240,30,160,30,30,320,240};	//API参数  hls的阈值，识别时用的
	u32 j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新？
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//将显示区域设置到屏幕中央
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
		for(j=0;j<76800;j++)
		{
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			LCD->LCD_RAM=color;    
		}  
 
		if(Trace(&Condition,&Resu) )                      //API
			{				
				
				LCD_Fill(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.x+Resu.w/2,Resu.y-Resu.h/2+1,0xf800);//u16 x,u16 y,u16 width,u16 hight,u16 Color
				LCD_Fill(Resu.x-Resu.w/2,Resu.y-Resu.h/2,Resu.x-Resu.w/2+1,Resu.y+Resu.h/2,0xf800);
				LCD_Fill(Resu.x-Resu.w/2,Resu.y+Resu.h/2,Resu.x+Resu.w/2,Resu.y+Resu.h/2+1,0xf800);
				LCD_Fill(Resu.x+Resu.w/2,Resu.y-Resu.h/2,Resu.x+Resu.w/2+1,Resu.y+Resu.h/2,0xf800);
				LCD_Fill(Resu.x-2,Resu.y-2,Resu.x+2,Resu.y+2,0xf800);
		
				LED1=!LED1;
				r=Resu.x;
				y=Resu.y;
			}
					
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}	
