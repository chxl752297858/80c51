/****12M晶振下89c52单片机lcd1602+164驱动程序*************/


#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar table[]={"love you my love"};
uint i,shi,fen,miao,aa;
sbit rs=P3^5;  //  
sbit rw=P3^6;  //写数据线
sbit e=P3^7;  //使能线

sbit dat_164=P3^3;//164数据线
sbit clk_164=P3^4;//164时钟线


 void sendchar(uchar ch)//向1602送一字节显示数据
{

uchar i;
for(i=0;i<8;i++)
{
clk_164=0;
dat_164=ch&0x01; //从地位开始读
clk_164=1;
ch>>=1;
}
}


void delay_50us(uint t)  //延时50US
{uchar j;
for(;t>0;t--)
for(j=19;j>0;j--);
}

void write_com(uchar com)  //写数据  
{

e=0;
rs=0;
rw=0;
sendchar(com); //
delay_50us(10);
e=1;
delay_50us(20);
e=0;
}

void write_data(uchar dat) //写指令
{

e=0;
rs=1;
rw=0;
sendchar(dat);
e=1;
delay_50us(10);
e=0;
delay_50us(20);
}


void init_1602(void)  //初始化
{   delay_50us(300); //延时15毫秒
    write_com(0x38);//不检测忙信号
    delay_50us(100);//延时5毫秒
    write_com(0x38);//不检测忙信号
    delay_50us(100);//延时5毫秒
	write_com(0x38);//不检测忙信号
	write_com(0x38);//显示模式设置
    write_com(0x08);//显示关闭
    write_com(0x01);//显示清屏
    write_com(0x06);//显示光标移动设置  
    write_com(0x0c);//显示开及光标移动设置
}

void init_timer0()           //初始化
{ 
 
    TMOD=0x01;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    EA=1;
    ET0=1;
    TR0=1; 
}

void  main()  //主函数
{

 
 init_1602();
 init_timer0();
 
 while(1)
    {
	 write_com(0x80);
	 write_data(0x30+(fen/10));
	 
	 write_com(0x81);
	 write_data(0x30+(fen%10));
	 write_com(0x82);
	 write_data(':');
	 write_com(0x83);
	 write_data(0x30+(miao/10));
	 
	 write_com(0x84);
	 write_data(0x30+(miao%10));


  	 }

}
 


   void timer0() interrupt 1 //中断累加
{
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    aa++; 
    if(aa==20)    //当aa==20时，为一秒
       {
         aa=0;       //aa清零
         miao++;
         if(miao==60)//清零之后，miao加一
         {   
            miao=0;
            fen++;
            if(fen==60)
            {
             fen=0;
            }
         }
       }                    



}



