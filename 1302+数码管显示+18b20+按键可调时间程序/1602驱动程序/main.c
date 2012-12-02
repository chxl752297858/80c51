/****12M晶振下89c52单片机lcd1602驱动程序*************/


#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar table[]={"i love you dear!"};
uint i;

sbit rs=P3^5;    
sbit rw=P3^6;
sbit e=P3^7;


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
P1=com;
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
P1=dat;
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


void  main()  //主函数
{

 
 init_1602();

 
 while(1)
    {
	 write_com(0x80);
	 for(i=0;i<16;i++)
	 {

	 write_data(table[i]);
	 delay_50us(4);
  	 }
	 }

}
 




