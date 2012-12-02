#ifndef __seri12864_h__
#define __seri12864_h__
#include"defi.h"

#define comd 0xf8  //д����
#define date 0xfa  //д����
sbit e_clk=P0^1;  //����ʱ���ź�

sbit rw_sid=P0^0; //����������	

void delay_50us(uint j);   //��ʱ50uS
void send_byte(uchar  dat) ;
void  wri_12864(uchar value,uchar  dat) ;
void  init_12864(void)  ;
void dis_line(uchar n,uchar m,uchar *s) ;
void dis_page(uchar *s) ;
#endif