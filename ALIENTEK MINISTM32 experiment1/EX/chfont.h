#ifndef __CHFONT_H
#define __CHFONT_H	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板——扩展实验
//自定义汉字显示  代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/7/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
 
//PCtolcd2002 设置->点阵：16 ，其他设置同OLED实验。
const u8 tfont16[][16]=
{
{0x02,0x00,0x42,0x02,0x42,0x04,0x42,0x08,0x42,0x30,0x7F,0xC0,0x42,0x00,0x42,0x00},
{0x42,0x00,0x42,0x00,0x7F,0xFE,0x42,0x00,0x42,0x00,0x42,0x00,0x42,0x00,0x00,0x00},/*"开",0*/
{0x00,0x02,0x08,0x04,0x7C,0x08,0x08,0x11,0x08,0xE1,0x0F,0x82,0xF9,0x62,0x09,0x14},
{0x09,0x08,0x49,0x14,0x29,0xE4,0x38,0x02,0x08,0x03,0x08,0x02,0x08,0x00,0x00,0x00},/*"发",1*/
{0x08,0x20,0x08,0xC0,0x0B,0x00,0xFF,0xFE,0x0A,0x00,0x09,0x88,0x00,0x70,0x7F,0x82},
{0x46,0x04,0x45,0x88,0x44,0x50,0x84,0x70,0x85,0x8C,0x86,0x06,0x00,0x04,0x00,0x00},/*"板",2*/
{0x08,0x20,0x06,0x20,0x80,0x7E,0x61,0x80,0x06,0x02,0x20,0x04,0x38,0x04,0x27,0x08},
{0x20,0xD0,0x20,0x20,0x20,0xD0,0x27,0x08,0x38,0x0C,0x20,0x06,0x00,0x04,0x00,0x00},/*"汉",3*/
{0x00,0x00,0x08,0x40,0x30,0x40,0x24,0x40,0x24,0x40,0x24,0x42,0xA4,0x41,0x64,0xFE},
{0x25,0x40,0x26,0x40,0x24,0x40,0x20,0x40,0x28,0x40,0x30,0x40,0x00,0x40,0x00,0x00},/*"字",4*/
{0x10,0x20,0x8C,0x3F,0x61,0xC0,0x06,0x00,0x00,0x01,0x7F,0xE2,0x40,0x0C,0x4F,0xF0},
{0x40,0x08,0x7F,0xE6,0x00,0x00,0x1F,0xE0,0x00,0x02,0x00,0x01,0xFF,0xFE,0x00,0x00},/*"测",5*/
{0x02,0x00,0x42,0x00,0x3B,0xFE,0x10,0x04,0x00,0x08,0x09,0x04,0x09,0x04,0x09,0xF8},						   
{0x09,0x08,0x09,0x08,0xFF,0x80,0x08,0x60,0x48,0x18,0x38,0x04,0x08,0x1E,0x00,0x00},/*"试",6*/ 
};
//PCtolcd2002 设置->点阵：24 ，其他设置同OLED实验。
const u8 tfont24[][24]=
{   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x08,0x08,0x02,0x08,0x08,0x04,0x08,0x08,0x08,0x08,0x08,0x10,0x08,0x08,0xE0},
{0x0F,0xFF,0x80,0x08,0x08,0x00,0x08,0x08,0x00,0x08,0x08,0x00,0x08,0x08,0x00,0x08,0x08,0x00,0x08,0x08,0x00,0x0F,0xFF,0xFE},
{0x0F,0xFF,0xFE,0x08,0x08,0x00,0x08,0x08,0x00,0x10,0x08,0x00,0x10,0x10,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"开",0*/
{0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x04,0x01,0x80,0x08,0x0F,0x00,0x10,0x1C,0x80,0x60,0x00,0x81,0xC2,0x00,0x87,0x02},
{0x00,0xFC,0x02,0x1F,0xF0,0x04,0x3C,0x9C,0x08,0x20,0x92,0x08,0x00,0x91,0x90,0x00,0x90,0x70,0x00,0x90,0x60,0x20,0x91,0x90},
{0x10,0x93,0x18,0x1C,0x9C,0x08,0x0C,0x98,0x0C,0x00,0x80,0x04,0x01,0x00,0x06,0x01,0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x00},/*"发",1*/
{0x00,0x00,0x00,0x00,0x00,0xC0,0x01,0x01,0x00,0x01,0x0E,0x00,0x01,0xF8,0x00,0x3F,0xFF,0xFE,0x21,0x20,0x00,0x01,0x10,0x02},
{0x02,0x0C,0x0E,0x00,0x00,0x12,0x00,0x07,0xE4,0x0F,0xFE,0x04,0x08,0x40,0x08,0x08,0x78,0x10,0x08,0x47,0x30,0x08,0x41,0xE0},
{0x10,0x41,0xE0,0x10,0x47,0x30,0x30,0x7C,0x18,0x30,0xE0,0x0C,0x10,0x00,0x0C,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00},/*"板",2*/
{0x00,0x00,0x00,0x00,0x80,0x40,0x00,0xC0,0x40,0x00,0x70,0xFC,0x10,0x01,0xFE,0x0C,0x07,0x02,0x04,0x38,0x02,0x01,0xC0,0x02},
{0x02,0x00,0x04,0x08,0x00,0x04,0x0E,0x00,0x08,0x09,0xE0,0x10,0x08,0x1C,0x20,0x08,0x03,0x40,0x08,0x01,0x80,0x08,0x07,0x40},
{0x08,0x1C,0x30,0x08,0xF0,0x10,0x0F,0x80,0x18,0x08,0x00,0x0C,0x00,0x00,0x0E,0x00,0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x00},/*"汉",3*/
{0x00,0x00,0x00,0x00,0x02,0x00,0x01,0x02,0x00,0x03,0x02,0x00,0x0E,0x02,0x00,0x04,0x42,0x00,0x04,0x42,0x00,0x04,0x42,0x00},
{0x04,0x42,0x04,0x04,0x42,0x04,0x04,0x42,0x06,0x24,0x4F,0xFE,0x38,0x4F,0xFC,0x04,0x52,0x00,0x04,0x62,0x00,0x04,0x62,0x00},
{0x04,0xC2,0x00,0x04,0xC2,0x00,0x05,0x42,0x00,0x0E,0x02,0x00,0x0C,0x06,0x00,0x00,0x06,0x00,0x00,0x02,0x00,0x00,0x00,0x00},/*"字",4*/
{0x00,0x00,0x00,0x01,0x00,0x80,0x00,0x80,0x80,0x20,0xE0,0xFC,0x18,0x03,0xFC,0x0C,0x3C,0x00,0x00,0xC0,0x02,0x3F,0xFF,0x84},
{0x1F,0xFF,0x88,0x10,0x00,0x30,0x13,0xFF,0xC0,0x11,0xFC,0x20,0x10,0x00,0x20,0x10,0x00,0x18,0x1F,0xFF,0x0C,0x00,0x00,0x00},
{0x00,0x00,0x00,0x07,0xFF,0xC4,0x00,0x00,0x04,0x00,0x00,0x06,0x3F,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"测",5*/
{0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x10,0x10,0x00,0x0C,0x10,0x00,0x06,0x3F,0xFC,0x00,0x00,0x08,0x00,0x00,0x10},
{0x01,0x00,0x68,0x01,0x08,0x48,0x01,0x08,0x18,0x01,0x08,0x10,0x01,0x0F,0xF0,0x01,0x08,0x10,0x01,0x10,0x20,0x01,0x10,0x20},
{0x3F,0xFC,0x00,0x11,0x07,0x80,0x11,0x00,0xE0,0x19,0x00,0x38,0x0D,0x00,0x0C,0x03,0x00,0x3E,0x01,0x00,0x46,0x00,0x00,0x00},/*"试",6*/
};
#endif
