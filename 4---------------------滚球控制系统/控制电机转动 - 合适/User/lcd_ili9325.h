#ifndef _LCD_ILI9325_H
#define _LCD_ILI9325_H
#include "pbdata.h"


//6000 0000
//A16  16ΪͨѶ

#define Bank1_LCD_D (u32)0x60020000
#define Bank1_LCD_C (u32)0x60000000

#define RGB565(r, g, b) ((r >> 3) << 11 | (g >> 2) << 5 | (b >> 3))


// SRT ��string����д
#define STR_WIDTH_s		6		      /* �ַ���� */
#define STR_WIDTH_m		10		    /* �ַ���� */
#define STR_WIDTH_b		14		    /* �ַ���� */
#define STR_HEIGHT_s	14		    /* �ַ��߶� */
#define STR_HEIGHT_m	23		    /* �ַ��߶� */
#define STR_HEIGHT_b	30		    /* �ַ��߶� */
#define STR_dzs_s	    16		    /* ������Ŀ */
#define STR_dzs_m	    36		    /* ������Ŀ */
#define STR_dzs_b	    64		    /* ������Ŀ */
#define far           15        /* �������� */

// CH ��chinese����д
#define CH_WIDTH		16		  /* ���ֿ�� */
#define CH_HEIGHT		16		  /* ���ָ߶� */

#define RED          0xF800	/* ��ɫ */
#define WHITE		 		 0xFFFF	/* ��ɫ */
#define BLACK        0x0000	/* ��ɫ */
#define BLUE         0x001F	/* ��ɫ */
#define GREEN        0x07E0	/* ��ɫ */
#define YELLOW       0xFFE0	/* ��ɫ */
#define GREY         0xF7DE	/* ��ɫ */
#define BLUEQ        0x051F	/* ǳ��ɫ*/
#define MAGENTA      0xF81F	/* ����ɫ�����ɫ */
#define CYAN         0x7FFF	/* ����ɫ����ɫ */

void LCD_ILI9325_Init(void);                                         //LCD��ʼ������
void LCD_WR_REG(u16 index);                                          //д�����
void LCD_WR_Data(u16 val);                                           //д����
void LCD_WR_CMD(u16 index,u16 val);                                  //д��������
void LCD_ILI9325_Draw_Point(u8 x,u16 y,u16 color);                   //��㺯��
void LCD_ILI9325_CLEAR(u16 color);                                   //��������
void LCD_DispChar(u16 x, u16 y, u8 ascii, u16 qj, u16 bj);           //��ʾһ���ַ�
void LCD_DispStr_X(u16 x,u16 y,u8 *pstr,u16 qj,u16 bj);              //��ʾ�ַ�����������
void LCD_DispStr_Y(u16 x,u16 y,u8 *pstr,u16 qj,u16 bj);              //��ʾ�ַ�����������
void LCD_DispNum_X(u16 x,u16 y,u32 num, u16 qj,u16 bj);              //��ʾ���֣�������
void LCD_DispNum_Y(u16 x,u16 y,u32 num, u16 qj,u16 bj);							 //��ʾ���֣�������
void LCD_Draw_Circle(u16 x,u16 y,u8 r,u16 color);                    //��һ��Բ(����)
void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);        //��һ����
void LCD_Draw_Rectangle_k(u16 x1, u16 y1, u16 x2, u16 y2,u16 color); //��һ������(����)
void LCD_Draw_Rectangle_s(u16 x1, u16 y1, u16 x2, u16 y2,u16 color); //��һ������(ʵ��)
void show_Font(u16 x,u16 y,u8 id,u16 qj,u16 bj);
void show_Font_Mini(u16 x,u16 y,u8 id,u16 qj,u16 bj);

#endif

