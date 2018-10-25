#include "pbdata.h"
#include "ascii.h"
void LCD_WR_REG(u16 index)              
{
	*(__IO u16 *)(Bank1_LCD_C)=index;
}

//写数据

void LCD_WR_Data(u16 val)          
{
   *(__IO u16 *)(Bank1_LCD_D)=val;
}

//写命令 写数据

void LCD_WR_CMD(u16 index,u16 val)      
{
   *(__IO u16 *)(Bank1_LCD_C)=index;
   *(__IO u16 *)(Bank1_LCD_D)=val;
}

//LCD初始化

void LCD_ILI9325_Init(void)
{
	FSMC_Config();
  GPIO_ResetBits(GPIOE, GPIO_Pin_1);	  //硬件复位
  delay(0xAFFf);					   
  GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 
	delay(0xAFFf);	
	
	LCD_WR_CMD(0x0001, 0x0100); // set SS and SM bit
	LCD_WR_CMD(0x0002, 0x0700); // set 1 line inversion
	LCD_WR_CMD(0x0003, 0x1030); // set GRAM write direction and BGR=1.
	LCD_WR_CMD(0x0004, 0x0000); // Resize register
	LCD_WR_CMD(0x0008, 0x0207); // set the back porch and front porch
	LCD_WR_CMD(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	LCD_WR_CMD(0x000A, 0x0000); // FMARK function
	LCD_WR_CMD(0x000C, 0x0000); // RGB interface setting
	LCD_WR_CMD(0x000D, 0x0000); // Frame marker Position
	LCD_WR_CMD(0x000F, 0x0000); // RGB interface polarity
	
	LCD_WR_CMD(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_CMD(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_WR_CMD(0x0012, 0x0000); // VREG1OUT voltage
	LCD_WR_CMD(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	LCD_WR_CMD(0x0007, 0x0001);
	delay(12000); // Dis-charge capacitor power voltage
	LCD_WR_CMD(0x0010, 0x1490); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_CMD(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
	delay(15500); // Delay 50ms
	LCD_WR_CMD(0x0012, 0x001C); // Internal reference voltage= Vci;
	delay(15000); // Delay 50ms
	LCD_WR_CMD(0x0013, 0x1A00); // Set VDV[4:0] for VCOM amplitude
	LCD_WR_CMD(0x0029, 0x0025); // Set VCM[5:0] for VCOMH
	LCD_WR_CMD(0x002B, 0x000C); // Set Frame Rate
	delay(15000); // Delay 50ms
	LCD_WR_CMD(0x0020, 0x0000); // GRAM horizontal Address
	LCD_WR_CMD(0x0021, 0x0000); // GRAM Vertical Address
	// ----------- Adjust the Gamma Curve ----------//
	LCD_WR_CMD(0x0030, 0x0000);
	LCD_WR_CMD(0x0031, 0x0506);
	LCD_WR_CMD(0x0032, 0x0104);
	LCD_WR_CMD(0x0035, 0x0207);
	LCD_WR_CMD(0x0036, 0x000F);
	LCD_WR_CMD(0x0037, 0x0306);
	LCD_WR_CMD(0x0038, 0x0102);
	LCD_WR_CMD(0x0039, 0x0707);
	LCD_WR_CMD(0x003C, 0x0702);
	LCD_WR_CMD(0x003D, 0x1604);
	//------------------ Set GRAM area ---------------//
	LCD_WR_CMD(0x0050, 0x0000); // Horizontal GRAM Start Address
	LCD_WR_CMD(0x0051, 0x00EF); // Horizontal GRAM End Address
	LCD_WR_CMD(0x0052, 0x0000); // Vertical GRAM Start Address
	LCD_WR_CMD(0x0053, 0x013F); // Vertical GRAM Start Address
	LCD_WR_CMD(0x0060, 0xA700); // Gate Scan Line
	LCD_WR_CMD(0x0061, 0x0001); // NDL,VLE, REV

	LCD_WR_CMD(0x006A, 0x0000); // set scrolling line
	//-------------- Partial Display Control ---------//
	LCD_WR_CMD(0x0080, 0x0000);
	LCD_WR_CMD(0x0081, 0x0000);
	LCD_WR_CMD(0x0082, 0x0000);
	LCD_WR_CMD(0x0083, 0x0000);
	LCD_WR_CMD(0x0084, 0x0000);			   
	LCD_WR_CMD(0x0085, 0x0000);
	//-------------- Panel Control -------------------//
	LCD_WR_CMD(0x0090, 0x0010);
	LCD_WR_CMD(0x0092, 0x0600);
	LCD_WR_CMD(0x0007, 0x0133); // 262K color and display ON	  
}

//描点函数240*320

void LCD_ILI9325_Draw_Point(u8 x,u16 y,u16 color)
{
	LCD_WR_CMD(0x50,x);	//x起始
	LCD_WR_CMD(0x51,x);	//x结束
	LCD_WR_CMD(0x52,y);	//y起始
	LCD_WR_CMD(0x53,y);	//y结束

	LCD_WR_CMD(0x20,x);
	LCD_WR_CMD(0x21,y);
	LCD_WR_REG(0x22);
	LCD_WR_Data(color);
}

//清屏函数

void LCD_ILI9325_CLEAR(u16 color)
{
	u16 i=0,j=0;

	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			LCD_ILI9325_Draw_Point(i,j,color);
		}
	}
}

//void show_Font(u16 x,u16 y,u8 id,u16 qj,u16 bj)
//{
//	u8 k=0,temp=0,t=0,y0=0;

//	y0=y;//初值

//	for(k=0;k<72;k++)
//	{
//		temp=font[id][k];
//		for(t=0;t<8;t++)
//		{
//			if(temp&0x80)
//			 	LCD_ILI9325_Draw_Point(x,y,qj);
//			else
//				LCD_ILI9325_Draw_Point(x,y,bj);

//			temp<<=1;
//			y++;
//			if((y-y0)==24)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}
//	}
//}
//void show_Font_Mini(u16 x,u16 y,u8 id,u16 qj,u16 bj)
//{
//	u8 k=0,temp=0,t=0,y0=0;

//	y0=y;//初值

//	for(k=0;k<32;k++)
//	{
//		temp=font_mini[id][k];
//		for(t=0;t<8;t++)
//		{
//			if(temp&0x80)
//			 	LCD_ILI9325_Draw_Point(x,y,qj);
//			else
//			 	LCD_ILI9325_Draw_Point(x,y,bj);

//			temp<<=1;
//			y++;
//			if((y-y0)==16)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}
//	}
//}

//显示一个字符 大小为 width（宽度）*height（高度）eg:'8'
//想要改变字符大小只需改动三处 点阵数 高 ASCII表 关系为(32*32)/8=64
//宽(8)*高(16)

void LCD_DispChar(u16 x, u16 y, u8 ascii, u16 qj, u16 bj)
{	
	u8 k=0,temp=0,t=0,id=0;
	u16 y0=y;
	id = ascii - ' ';
	for(k=0;k<STR_dzs_b;k++)
	{
		temp=asc2_3216[id][k];
		for(t=0;t<8;t++)
		{
			if(temp&0x80)
				LCD_ILI9325_Draw_Point(x,y,qj);
			else
				LCD_ILI9325_Draw_Point(x,y,bj);
			temp<<=1;
			y++;
			if((y-y0)==STR_HEIGHT_b)
			{
				y=y0;
				x++;
				break;
			}			
		}
	}
}

//显示字符串  （横屏）

void LCD_DispStr_X(u16 x,u16 y,u8 *pstr,u16 qj,u16 bj)
{
	while( *pstr != '\0' )
	{
		if( x > (240-STR_WIDTH_b))    // s m b加宽字符之间的距离
		{
			x = 0;
			y += STR_HEIGHT_b;
		}
		if( y > (320-STR_HEIGHT_b) )
		{
			x = 0;
			y = 0;
		}
		delay_us(200);
		LCD_DispChar(x, y, *pstr, qj,bj);
		
		x += STR_WIDTH_b;
		pstr++;
	}
}

//显示字符串    （竖屏）

void LCD_DispStr_Y(u16 x,u16 y,u8 *pstr,u16 qj,u16 bj)
{
	while( *pstr != '\0' )
	{
		if( y > (320-STR_WIDTH_b))
		{
			y = 0;
			x += STR_HEIGHT_b;
		}
		if( x > (240-STR_HEIGHT_b) )
		{
			x = 0;
			y = 0;
		}
		delay_us(200);
		LCD_DispChar(x, y, *pstr, qj,bj);
		
		y += STR_WIDTH_b;
		pstr++;
	}
}

//显示数字 （横屏）

void LCD_DispNum_X(u16 x,u16 y,u32 num, u16 qj,u16 bj)	  
{      
	u32 res=num;	
	u8 t=0;

	if( num == 0 )
	{			
		LCD_DispChar(x, y, '0', qj,bj);
		return;
	}	
	while( res )                       /*得到数字长度t*/
	{
		res/=10;
		t++;
        printf("%d\r\n",res);
	}
	while(num)
	{	
		LCD_DispChar(x+(far*(t--)-far), y, (num%10)+'0', qj,bj);/*先显示低位在显示高位*/
		num /= 10 ;
        printf("%d\r\n",num);
	}			     
} 

//显示数字  （竖屏）

void LCD_DispNum_Y(u16 x,u16 y,u32 num, u16 qj,u16 bj)	  
{      
	u32 res=num;	
	u8 t=0;

	if( num == 0 )
	{			
		LCD_DispChar(x, y, '0', qj,bj);
		return;
	}	
	while( res )                       /*得到数字长度t*/
	{
		res/=10;
		t++;
	}
	while(num)
	{	
		LCD_DispChar(x, y+(far*(t--)-far), (num%10)+'0', qj,bj);/*先显示低位在显示高位*/
		num /= 10 ;
	}			     
} 

//画一个圆      (空心)
//(x,y):中心点 r:半径

void LCD_Draw_Circle(u16 x,u16 y,u8 r,u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                      //判断下个点位置的标志
	while(a<=b)
	{
		LCD_ILI9325_Draw_Point(x+a,y-b,color);             //5
 		LCD_ILI9325_Draw_Point(x+b,y-a,color);             //0           
		LCD_ILI9325_Draw_Point(x+b,y+a,color);             //4               
		LCD_ILI9325_Draw_Point(x+a,y+b,color);             //6 
		LCD_ILI9325_Draw_Point(x-a,y+b,color);             //1       
 		LCD_ILI9325_Draw_Point(x-b,y+a,color);             
		LCD_ILI9325_Draw_Point(x-a,y-b,color);             //2             
  	LCD_ILI9325_Draw_Point(x-b,y-a,color);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}

//画一条线
//x1,y1:起点坐标
//x2,y2:终点坐标 

void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,m,n,distance; 
	int incx,incy,i,j; 
	m=x2-x1;                             //计算坐标增量 
	n=y2-y1; 
	i=x1; 
	j=y1; 
	if(m>0)incx=1;                       //设置单步方向 
	else if(m==0)incx=0;                 //垂直线 
	else {incx=-1;m=-m;} 
	if(n>0)incy=1; 
	else if(n==0)incy=0;                 //水平线 
	else{incy=-1;n=-n;} 
	if( m>n)distance=m;                  //选取基本增量坐标轴 
	else distance=n; 
	for(t=0;t<=distance+1;t++ )          //画线输出 
	{  
		LCD_ILI9325_Draw_Point(i,j,color); //画点 
		xerr+=m ; 
		yerr+=n ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			i+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			j+=incy; 
		} 
	}  
}    

//画一个矩形	  (空心)
//(x1,y1),(x2,y2):矩形的对角坐标

void LCD_Draw_Rectangle_k(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_Draw_Line(x1,y1,x2,y1,color);
	LCD_Draw_Line(x1,y1,x1,y2,color);
	LCD_Draw_Line(x1,y2,x2,y2,color);
	LCD_Draw_Line(x2,y1,x2,y2,color);
}

//画一个矩形    (实心)
//(x1,y1),(x2,y2):矩形的对角坐标

void LCD_Draw_Rectangle_s(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 i=0,j=0;

	for(i=x1;i<x2;i++)
	{
		for(j=y1;j<y2;j++)
		{
			LCD_ILI9325_Draw_Point(i,j,color);
		}
	}
} 
