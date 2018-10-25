#include "pbdata.h"
#include "HardWork.h"
double atan(double x);
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



u16 zone_1_x,zone_1_y,zone_1_w,zone_1_h;//黑色区域坐标
u16 zone_2_x,zone_2_y,zone_2_w,zone_2_h;//黑色区域坐标
u16 zone_3_x,zone_3_y;//黑色区域坐标
u16 zone_4_x,zone_4_y;//黑色区域坐标
u16 zone_5_x,zone_5_y;//黑色区域坐标
u16 zone_6_x,zone_6_y;//黑色区域坐标
u16 zone_7_x,zone_7_y;//黑色区域坐标
u16 zone_7_x,zone_8_y;//黑色区域坐标
u16 zone_9_x,zone_9_y;//黑色区域坐标
u16 goal_x,goal_y,goal_w,goal_h;//目标物坐标

  u8 angle;

	RESULT Resu_goal;
	RESULT Resu_zone1;
	RESULT Resu_zone2;
	TARGET_CONDI Condition_goal={215,240,20,240,30,160,30,30,320,240};	//  红色 hls的阈值，识别时用的	
	TARGET_CONDI Condition_zone1={0,160,0,160,0,100,20,20,320,240};	  //  1区域黑色 hls的阈值，识别时用的
	
	
	
	TARGET_CONDI Condition_zone2={0,160,0,160,0,100,20,20,320,240};	  //  2区域白光 hls的阈值，识别时用的
//  TARGET_CONDI Condition_zone2={0,160,0,160,0,100,20,20,320,240};
void camera_refresh(void)
{ 

	u32 j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新？
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//将显示区域设置到屏幕中央
		else if(lcddev.id==0X9||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//将显示区域设置到屏幕中央
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
 
     if(Trace_goal(&Condition_goal,&Resu_goal) )                      //API
			{		
				LCD_Fill(Resu_goal.x-Resu_goal.w/2,Resu_goal.y-Resu_goal.h/2,Resu_goal.x+Resu_goal.w/2,Resu_goal.y-Resu_goal.h/2+1,RED);//u16 x,u16 y,u16 width,u16 hight,u16 Color
				LCD_Fill(Resu_goal.x-Resu_goal.w/2,Resu_goal.y-Resu_goal.h/2,Resu_goal.x-Resu_goal.w/2+1,Resu_goal.y+Resu_goal.h/2,RED);
				LCD_Fill(Resu_goal.x-Resu_goal.w/2,Resu_goal.y+Resu_goal.h/2,Resu_goal.x+Resu_goal.w/2,Resu_goal.y+Resu_goal.h/2+1,RED);
				LCD_Fill(Resu_goal.x+Resu_goal.w/2,Resu_goal.y-Resu_goal.h/2,Resu_goal.x+Resu_goal.w/2+1,Resu_goal.y+Resu_goal.h/2,RED);
				LCD_Fill(Resu_goal.x-4,Resu_goal.y-4,Resu_goal.x+4,Resu_goal.y+4,0xf800);
				LED1=!LED1;
				goal_x=Resu_goal.x;
				goal_y=Resu_goal.y;
				
				goal_w=Resu_goal.w;
				goal_h=Resu_goal.h;
//				printf("goal_x=%d      goal_y=%d\r\n",Resu_goal.x,Resu_goal.y);

			}

			if(Trace_zone1(&Condition_zone1,&Resu_zone1) )                      //API
			{		
				LCD_Fill(Resu_zone1.x-Resu_zone1.w/2,Resu_zone1.y-Resu_zone1.h/2,Resu_zone1.x+Resu_zone1.w/2,Resu_zone1.y-Resu_zone1.h/2+1,BLACK);//u16 x,u16 y,u16 width,u16 hight,u16 Color
				LCD_Fill(Resu_zone1.x-Resu_zone1.w/2,Resu_zone1.y-Resu_zone1.h/2,Resu_zone1.x-Resu_zone1.w/2+1,Resu_zone1.y+Resu_zone1.h/2,BLACK);
				LCD_Fill(Resu_zone1.x-Resu_zone1.w/2,Resu_zone1.y+Resu_zone1.h/2,Resu_zone1.x+Resu_zone1.w/2,Resu_zone1.y+Resu_zone1.h/2+1,BLACK);
				LCD_Fill(Resu_zone1.x+Resu_zone1.w/2,Resu_zone1.y-Resu_zone1.h/2,Resu_zone1.x+Resu_zone1.w/2+1,Resu_zone1.y+Resu_zone1.h/2,BLACK);
				LCD_Fill(Resu_zone1.x-4,Resu_zone1.y-4,Resu_zone1.x+4,Resu_zone1.y+4,0);
				LED1=!LED1;
				zone_1_x=Resu_zone1.x;
				zone_1_y=Resu_zone1.y;
				zone_1_w=Resu_zone1.w;
				zone_1_h=Resu_zone1.h;
//				printf("zone_1_x=%d      zone_1_y=%d\r\n",zone_1_x,zone_1_y);
			}
//			if(Trace_zone2(&Condition_zone2,&Resu_zone2) )                      //API
//			{		
//				LCD_Fill(Resu_zone2.x-Resu_zone2.w/2,Resu_zone2.y-Resu_zone2.h/2,Resu_zone2.x+Resu_zone2.w/2,Resu_zone2.y-Resu_zone2.h/2+1,BLUE);//u16 x,u16 y,u16 width,u16 hight,u16 Color
//				LCD_Fill(Resu_zone2.x-Resu_zone2.w/2,Resu_zone2.y-Resu_zone2.h/2,Resu_zone2.x-Resu_zone2.w/2+1,Resu_zone2.y+Resu_zone2.h/2,BLUE);
//				LCD_Fill(Resu_zone2.x-Resu_zone2.w/2,Resu_zone2.y+Resu_zone2.h/2,Resu_zone2.x+Resu_zone2.w/2,Resu_zone2.y+Resu_zone2.h/2+1,BLUE);
//				LCD_Fill(Resu_zone2.x+Resu_zone2.w/2,Resu_zone2.y-Resu_zone2.h/2,Resu_zone2.x+Resu_zone2.w/2+1,Resu_zone2.y+Resu_zone2.h/2,BLUE);
//				LCD_Fill(Resu_zone2.x-4,Resu_zone2.y-4,Resu_zone2.x+4,Resu_zone2.y+4,0);
//				LED1=!LED1;
//				zone_2_x=Resu_zone2.x;
//				zone_2_y=Resu_zone2.y;
//				
//				zone_2_w=Resu_zone2.w;
//				zone_2_h=Resu_zone2.h;
////				printf("zone_2_x=%d      zone_2_y=%d\r\n",zone_2_x,zone_2_y);

//			}	
					
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}


int goal_Init(void)
{
	/**************初始化区域坐标*****************/
	if(Trace_zone1(&Condition_zone1,&Resu_zone1)) 
	{
		    zone_1_x=Resu_zone2.x;
				zone_1_y=Resu_zone2.y;
		
//		    printf("zone_1_x=%d      zone_1_y=%d\r\n",zone_1_x,zone_1_y);
	}
	if(Trace_zone2(&Condition_zone2,&Resu_zone2)) 
	{
		    zone_2_x=Resu_zone2.x;
				zone_2_y=Resu_zone2.y;
//				printf("zone_1_x=%d      zone_1_y=%d\r\n",zone_1_x,zone_1_y);

	}
	/*************初始化目标物坐标******************/
	if(Trace_goal(&Condition_goal,&Resu_goal)) 
	{
		    goal_x=Resu_goal.x;
				goal_y=Resu_goal.y;
//				printf("goal_x=%d      goal_y=%d\r\n",goal_x,goal_y);

	}
	/*************求出目标物与区域夹角**************/
	 angle=180-atan((goal_y-zone_1_y)/(goal_x-zone_1_x));

	
	 return angle;	
}
u8 RestGoal(void)
{
	 goal_Init();
	
	if(angle>135&&angle<180)
	{
	   Rotate(1,1,1,1,0,1);  //电机转动

		/*******A电机（3）B电机（-1）********/
	}

	if(angle>90&&angle<135)
	{
    Rotate(1,1,1,1,0,1);  //电机转动
		/*******A电机（3）B电机（0）********/
	}
  return 0;	
}
u8 PTC(void)
{
  float PTC=0;
	u16 arer_goal=0;
  u16 arer_zone=0;
	
	arer_goal=goal_x*goal_y;
	arer_zone=zone_1_x*zone_1_y;
	PTC=arer_goal/arer_zone;

  return PTC;	
}
u8 NO_1_1(void)
{
	goal_Init();
	/***************调整电机使板平衡至少五秒*******************/
	while(PTC()<0.8)
	{
		
				/*******A电机（3）B电机（-1）********/

		
	}
	
	return 1;	
}
u8 NO_1_2(void)
{
	/****在十五秒内从区域1出发到区域五******/

	RestGoal();
	/***************1.粗略调整电机A，B, 5区域在1区域*45度角方向***************/
	
	/***************2.PTC()得到面积比，判断是否到达指定区域***************/
  while(PTC()<0.8)
	{
			/***************1.细微调整电机A，B，使之到达指定区域 ***************/		
	}	
	return 1;
}
u8 NO_1_3(void)
{
		/****1.控制小球从区域1进入到区域4（停留时间不少于2秒）******/
	  /****2.控制小球从区域4进入到区域5******/
	  /****3.时间控制在20秒之内******/


  NO_1_2();	
	/*******************等待2s执行下一步*************************/
	
	
	/***************1.粗略调整电机A，B, 9区域在5区域*0度角方向***************/
	
	
	/***************2.PTC()得到面积比，判断是否到达指定区域******************/
  while(PTC()<0.8)
	{
			/***************1.细微调整电机A，B，使之到达指定区域 ***************/		
	}
   return 1;
	
}
u8 NO_1_4(void)
{
		/****1.控制小球从区域1进入到区域9******/
	  /****2.在区域9，停留不少于2s******/
	  /****3.时间控制在30秒之内******/
	RestGoal();
	/***************1.粗略调整电机A，B, 4区域在1区域*90度角方向***************/
	
	/***************2.PTC()得到面积比，判断是否到达指定区域******************/
  while(PTC()<0.8)
	{
			/***************1.细微调整电机A，B，使之到达指定区域 ***************/		
	}
      /**********注意：5区域不停留***************************/	
	
	/***************1.粗略调整电机A，B, 5区域在4区域*0度角方向***************/
	
	
	/***************2.PTC()得到面积比，判断是否到达指定区域******************/
  while(PTC()<0.8)
	{
			/***************1.细微调整电机A，B，使之到达指定区域 ***************/		
	}
	
		return 1;
	
}
u8 NO_2_1(void)
{
		/****1.控制小球从区域1进入到区域2.6.9******/
	  /****2.在区域9，停留不少于2s******/
	  /****3.时间控制在40秒之内******/
	 RestGoal();	
	/***************1.粗略调整电机A，B, 2区域在1区域*0度角方向***************/	
	/***************2.PTC()得到面积比，判断是否到达指定区域******************/
  while(PTC()<0.8)
	{
			/***************1.细微调整电机A，B，使之到达指定区域 ***************/		
	}
	
	
	RestGoal();	
	
	/***************1.粗略调整电机A，B, 6区域在2区域*45度角方向***************/	
	
	
	/***************2.PTC()得到面积比，判断是否到达指定区域******************/
  while(PTC()<0.8)
	{
			/***************1.细微调整电机A，B，使之到达指定区域 ***************/		
	}	
		return 1;
	

}	
u8 NO_2_2(void)
{
		return 1;

}	
u8 NO_2_3(void)
{
		return 1;

}	
