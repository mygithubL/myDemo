#ifndef _LCD_ILI9325_H
#define _LCD_ILI9325_H
#include "pbdata.h"


//6000 0000
//A16  16为通讯

#define Bank1_LCD_D (u32)0x60020000
#define Bank1_LCD_C (u32)0x60000000

#define RGB565(r, g, b) ((r >> 3) << 11 | (g >> 2) << 5 | (b >> 3))


// SRT 是string的缩写
#define STR_WIDTH_s		6		      /* 字符宽度 */
#define STR_WIDTH_m		10		    /* 字符宽度 */
#define STR_WIDTH_b		14		    /* 字符宽度 */
#define STR_HEIGHT_s	14		    /* 字符高度 */
#define STR_HEIGHT_m	23		    /* 字符高度 */
#define STR_HEIGHT_b	30		    /* 字符高度 */
#define STR_dzs_s	    16		    /* 点阵数目 */
#define STR_dzs_m	    36		    /* 点阵数目 */
#define STR_dzs_b	    64		    /* 点阵数目 */
#define far           15        /* 两数距离 */

// CH 是chinese的缩写
#define CH_WIDTH		16		  /* 汉字宽度 */
#define CH_HEIGHT		16		  /* 汉字高度 */

#define RED          0xF800	/* 红色 */
#define WHITE		 		 0xFFFF	/* 白色 */
#define BLACK        0x0000	/* 黑色 */
#define BLUE         0x001F	/* 蓝色 */
#define GREEN        0x07E0	/* 绿色 */
#define YELLOW       0xFFE0	/* 黄色 */
#define GREY         0xF7DE	/* 灰色 */
#define BLUEQ        0x051F	/* 浅蓝色*/
#define MAGENTA      0xF81F	/* 红紫色，洋红色 */
#define CYAN         0x7FFF	/* 蓝绿色，青色 */

void LCD_ILI9325_Init(void);                                         //LCD初始化函数
void LCD_WR_REG(u16 index);                                          //写命令函数
void LCD_WR_Data(u16 val);                                           //写数据
void LCD_WR_CMD(u16 index,u16 val);                                  //写命令数据
void LCD_ILI9325_Draw_Point(u8 x,u16 y,u16 color);                   //描点函数
void LCD_ILI9325_CLEAR(u16 color);                                   //清屏函数
void LCD_DispChar(u16 x, u16 y, u8 ascii, u16 qj, u16 bj);           //显示一个字符
void LCD_DispStr_X(u16 x,u16 y,u8 *pstr,u16 qj,u16 bj);              //显示字符串（横屏）
void LCD_DispStr_Y(u16 x,u16 y,u8 *pstr,u16 qj,u16 bj);              //显示字符串（竖屏）
void LCD_DispNum_X(u16 x,u16 y,u32 num, u16 qj,u16 bj);              //显示数字（横屏）
void LCD_DispNum_Y(u16 x,u16 y,u32 num, u16 qj,u16 bj);							 //显示数字（竖屏）
void LCD_Draw_Circle(u16 x,u16 y,u8 r,u16 color);                    //画一个圆(空心)
void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);        //画一条线
void LCD_Draw_Rectangle_k(u16 x1, u16 y1, u16 x2, u16 y2,u16 color); //画一个矩形(空心)
void LCD_Draw_Rectangle_s(u16 x1, u16 y1, u16 x2, u16 y2,u16 color); //画一个矩形(实心)
void show_Font(u16 x,u16 y,u8 id,u16 qj,u16 bj);
void show_Font_Mini(u16 x,u16 y,u8 id,u16 qj,u16 bj);

#endif

