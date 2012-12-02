/*-----利用移位寄存器74ls164实现串并接口转换程序------*/
/*-----2011.4.7   陈贤龙编写---------*/

#include <reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int

sbit dat_164=P1^0;//164数据线
sbit clk_164=P1^1;//164时钟线


void delay(uint z)    //延时
{
    uint x,y;
    for(x=0;x<z;x++)
        for(y=0;y<110;y++);
}


void sendchar(uchar ch)//向数码管送一字节显示数据
{

uchar i;
for(i=0;i<8;i++)
{
clk_164=0;
dat_164=ch&0x80;
clk_164=1;
ch<<=1;
}
}


 


 /*-----------主函数------------------*/
void main()
{ 
 
 while(1)
 {
 
 ;  
}
}

