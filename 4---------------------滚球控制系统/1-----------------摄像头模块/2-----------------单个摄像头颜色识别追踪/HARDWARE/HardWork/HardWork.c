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

extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��	

u32 a=0,b=0;     //�С����±�
u32 color_flag=0;//ʶ����ɫ���ص����
u8 Goal_flag=0; //ʶ��Ŀ�����ʶ
u16 r,y;

void camera_refresh(void)
{
	
		RESULT Resu;
	/***********Condition(��Сɫ�������ɫ������С���Ͷȣ���󱥺Ͷȣ���С���ȣ�������ȣ���С��ȣ�����ȣ���С�߶ȣ����߶�)**************/
	TARGET_CONDI Condition={0,160,0,160,230,255,20,20,320,240};	//API����  hls����ֵ��ʶ��ʱ�õ�
//	TARGET_CONDI Condition={215,240,20,240,30,160,30,30,320,240};	//API����  hls����ֵ��ʶ��ʱ�õ�
	u32 j;
 	u16 color;	 
	if(ov_sta)//��֡�жϸ��£�
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//����ʾ�������õ���Ļ����
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//����ʾ�������õ���Ļ����
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
					
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}	
