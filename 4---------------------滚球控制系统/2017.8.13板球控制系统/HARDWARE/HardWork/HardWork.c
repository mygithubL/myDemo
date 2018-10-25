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

extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��	

#define xishu 25*25/(320*320+240*240)
#define juli (goal_x-zone_1_x)*(goal_x-zone_1_x)+(goal_y-zone_1_y)*(goal_y-zone_1_y)
#define tiaojie  xishu*juli/10
float A_jiaodu_new = 0;
float B_jiaodu_new = 0;
float A_jiaodu_old = 0;
float B_jiaodu_old = 0;

u32 a=0,b=0;     //�С����±�
u32 color_flag=0;//ʶ����ɫ���ص����
u8 Goal_flag=0; //ʶ��Ŀ�����ʶ
u16 r,y;



u16 zone_1_x,zone_1_y,zone_1_w,zone_1_h;//��ɫ��������
u16 zone_2_x,zone_2_y,zone_2_w,zone_2_h;//��ɫ��������
u16 zone_3_x,zone_3_y;//��ɫ��������
u16 zone_4_x,zone_4_y;//��ɫ��������
u16 zone_5_x,zone_5_y;//��ɫ��������
u16 zone_6_x,zone_6_y;//��ɫ��������
u16 zone_7_x,zone_7_y;//��ɫ��������
u16 zone_7_x,zone_8_y;//��ɫ��������
u16 zone_9_x,zone_9_y;//��ɫ��������
u16 goal_x,goal_y,goal_w,goal_h;//Ŀ��������

  u8 angle;

	RESULT Resu_goal;
	RESULT Resu_zone1;
	RESULT Resu_zone2;
	TARGET_CONDI Condition_goal={215,240,20,240,30,160,30,30,320,240};	//  ��ɫ hls����ֵ��ʶ��ʱ�õ�	
	TARGET_CONDI Condition_zone1={0,160,0,160,0,100,20,20,320,240};	  //  1�����ɫ hls����ֵ��ʶ��ʱ�õ�
	
	
	
	TARGET_CONDI Condition_zone2={0,160,0,160,0,100,20,20,320,240};	  //  2����׹� hls����ֵ��ʶ��ʱ�õ�
//  TARGET_CONDI Condition_zone2={0,160,0,160,0,100,20,20,320,240};
void camera_refresh(void)
{ 
	u32 j;
 	u16 color;	 
	if(ov_sta)//��֡�жϸ��£�
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//����ʾ�������õ���Ļ����
		else if(lcddev.id==0X9||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
		for(j=0;j<76800;j++)
		{
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//������
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//������
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
				printf("goal_x=%d      goal_y=%d \r\n",Resu_goal.x,Resu_goal.y);

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
//				printf("Resu_zone1.w=%d  Resu_zone1.h=%d\r\n",Resu_zone1.w,Resu_zone1.h);
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
					
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}

u8 PTC(void)
{
	u16 x_cha=0;//Ŀ�����
	u16 y_cha=0;//��Ȧ���
	if((zone_1_x-goal_x < 2)&&(zone_1_y-goal_y<2))//����ͺ�Ȧ���غ����
	{
//		printf("�غ�\n");
		return 1;
	}
	else
	{
//		printf("���غ�\n");
		return 0;
	}
}
int arm_Init(void)
{
	/**************��ʼ����������*****************/
	if(Trace_zone1(&Condition_zone1,&Resu_zone1)) 
	{
		       zone_1_x=Resu_zone2.x;
			   zone_1_y=Resu_zone2.y;
		
//		    printf("zone_1_x=%d       zone_1_y=%d\r\n",zone_1_x,zone_1_y);
	}
	if(Trace_zone2(&Condition_zone2,&Resu_zone2)) 
	{
		        zone_2_x=Resu_zone2.x;
				zone_2_y=Resu_zone2.y;
//				printf("zone_1_x=%d      zone_1_y=%d\r\n",zone_1_x,zone_1_y);
 
	}
	/*************��ʼ��Ŀ��������******************/
	if(Trace_goal(&Condition_goal,&Resu_goal)) 
	{
		goal_x=Resu_goal.x;
		goal_y=Resu_goal.y;
//				printf("goal_x=%d      goal_y=%d\r\n",goal_x,goal_y);

	}
	/*************���Ŀ����������н�**************/
	 angle=180-atan((goal_y-zone_1_y)/(goal_x-zone_1_x));	
	 return angle;	
}
u8 RestGoal(void)
{	
		do{
			arm_Init();	
			if(angle>135&&angle<180)
			{
				 Rotate(16700*5,0,1,16700*5,0,1);
			//		Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
			/*******A�����3��B�����-1��********/
			}

			if(angle>90&&angle<135)
			{
				Rotate(16700*5,0,1,16700*5,0,1);
			//		Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
			/*******A�����3��B�����0��********/
			}
		}while(PTC());
return 1;	
}

u8 NO_1_1(void)
{
	
	/***************�������ʹ��ƽ����������*******************/
     RestGoal();		
				/*******A�����3��B�����-1��********/	
	return 1;	
}

u8 NO_1_2(void)
{
	float ratio=0;
	int tem = 0;
	int flag1 = 0 ,flag2 = 0;
	float cha1 = 0;
	float cha2 = 0;
	A_jiaodu_old = A_jiaodu_new;
	B_jiaodu_old = B_jiaodu_new;
	

	/****��ʮ�����ڴ�����1������������******/
//      Rotate(1,1,1,16700*10,1,1);
	/***************1.���Ե������A��B, 5������1����*45�ȽǷ���***************/	
	/***************2.PTC()�õ�����ȣ��ж��Ƿ񵽴�ָ������***************/
	A_jiaodu_new = xishu*juli;
	if(A_jiaodu_new>10) A_jiaodu_new = 10;
	else if(A_jiaodu_new < 0) A_jiaodu_new = 0;
	B_jiaodu_new = xishu*juli;
	if(B_jiaodu_new>10) B_jiaodu_new = 10;
	else if(B_jiaodu_new < 0) B_jiaodu_new = 0;
	cha1 = (A_jiaodu_new-A_jiaodu_old);
	cha2 = (B_jiaodu_new-B_jiaodu_old);	
	if(A_jiaodu_new >= A_jiaodu_old)
		flag1 = 1;
	else {flag1 = 0;cha1=-1*cha1;}
	if(B_jiaodu_new >= B_jiaodu_old)
		flag2 = 1;
	else {flag2 = 0;cha2=-1*cha2;}
	if((A_jiaodu_new>90)||(A_jiaodu_new <-90)||(B_jiaodu_new>90)||(B_jiaodu_new <-90))
	{
		A_jiaodu_new = A_jiaodu_old;
		B_jiaodu_new = B_jiaodu_old;
		return 1;
	}

	//printf("cha2 = %f\n",cha2);
	printf("goal_x = %d\n",goal_x-zone_1_x);
	printf("goal_y = %d\n",goal_y-zone_1_y);		
	//printf("zone_1_x = %f\n",zone_1_x);
	
	//printf("zone_1_y = %f\n",zone_1_y);

	 ratio=0.5;
	if((goal_x-zone_1_x < 2)&&(goal_y-zone_1_y> 2))
		Rotate(1,1,0,16700*ratio,1,1);//B̧10��
	/*��һ����*/
	else if((goal_x-zone_1_x > 2)&&(goal_y-zone_1_y > 2))
		Rotate(16700*ratio,1,1,1,1,0);//A̧10��
	/*��������*/
	else if((goal_x-zone_1_x < 2)&&(goal_y-zone_1_y< 2))
		Rotate(16700*ratio,0,1,1,1,0);//A��10��
	/*��������*/
	else if((goal_x-zone_1_x > 2)&&(goal_y-zone_1_y< 2))
		Rotate(1,1,0,16700*ratio,0,1);//B��10��
	/*�ڶ�����*/
	
//	if((goal_x-zone_1_x < 0)&&(goal_y-zone_1_y> 0))
//		Rotate(1,1,0,16700*cha2,flag2,1);//B̧10��
//	/*��һ����*/
//	else if((goal_x-zone_1_x > 0)&&(goal_y-zone_1_y > 0))
//		Rotate(16700*cha1,1,flag1,1,1,0);//A̧10��
//	/*��������*/
//	else if((goal_x-zone_1_x < 0)&&(goal_y-zone_1_y< 0))
//		Rotate(16700*cha1,flag1,1,1,1,0);//A��10��
//	/*��������*/
//	else if((goal_x-zone_1_x > 0)&&(goal_y-zone_1_y< 0))
//		Rotate(1,1,0,16700*cha2,flag2,1);//B��10��
			/***************1.ϸ΢�������A��B��ʹ֮����ָ������ ***************/		

	return 1;
}
u8 NO_1_3(void)
{
		/****1.����С�������1���뵽����4��ͣ��ʱ�䲻����2�룩******/
	  /****2.����С�������4���뵽����5******/
	  /****3.ʱ�������20��֮��******/


    NO_1_2();	
	/*******************�ȴ�2sִ����һ��*************************/
	delay_ms(1000);
	delay_ms(1000);
	
	/***************1.���Ե������A��B, 9������5����*0�ȽǷ���***************/
//	Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
	
	/***************2.PTC()�õ�����ȣ��ж��Ƿ񵽴�ָ������******************/
  while(PTC()<0.8)
	{
//		Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
			/***************1.ϸ΢�������A��B��ʹ֮����ָ������ ***************/		
	}
   return 1;
	
}
u8 NO_1_4(void)
{
	  /****1.����С�������1���뵽����9******/
	  /****2.������9��ͣ��������2s******/
	  /****3.ʱ�������30��֮��******/
	RestGoal();
	/***************1.���Ե������A��B, 4������1����*90�ȽǷ���***************/
	
	/***************2.PTC()�õ�����ȣ��ж��Ƿ񵽴�ָ������******************/
  while(PTC()<0.8)
	{
//		Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
			/***************1.ϸ΢�������A��B��ʹ֮����ָ������ ***************/		
	}
      /**********ע�⣺5����ͣ��***************************/	
	
	/***************1.���Ե������A��B, 5������4����*0�ȽǷ���***************/
	
	
	/***************2.PTC()�õ�����ȣ��ж��Ƿ񵽴�ָ������******************/
  while(PTC()<0.8)
	{
//		Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
			/***************1.ϸ΢�������A��B��ʹ֮����ָ������ ***************/		
	}
	
		return 1;
	
}
u8 NO_2_1(void)
{
		/****1.����С�������1���뵽����2.6.9******/
	  /****2.������9��ͣ��������2s******/
	  /****3.ʱ�������40��֮��******/
	 RestGoal();	
	/***************1.���Ե������A��B, 2������1����*0�ȽǷ���***************/	
	/***************2.PTC()�õ�����ȣ��ж��Ƿ񵽴�ָ������******************/
  while(PTC()<0.8)
	{
//		Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
			/***************1.ϸ΢�������A��B��ʹ֮����ָ������ ***************/		
	}
	
	
	RestGoal();	
	
	/***************1.���Ե������A��B, 6������2����*45�ȽǷ���***************/	
	
	
	/***************2.PTC()�õ�����ȣ��ж��Ƿ񵽴�ָ������******************/
  while(PTC()<0.8)
	{
//		Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
			/***************1.ϸ΢�������A��B��ʹ֮����ָ������ ***************/		
	}	
		return 1;
	

}	
u8 NO_2_2(void)
{
//	Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
		return 1;

}	
u8 NO_2_3(void)
{
//	Rotate(int ANum,int ADir,int ENA,int BNum, int BDir,int ENB);
		return 1;

}	
