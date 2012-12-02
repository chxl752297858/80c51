/*-----利用移位寄存器74ls164实现串并接口转换程序------*/
/*-----2011.4.7   陈贤龙编写---------*/

#include <reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int
uchar  table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};   //0—9段码
uchar  bufer[]={0x7f,0xbf,0xdf,0xef}; //数码管位选数组
uint i,aa,miao,fen;
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


 
void chushihua()           //初始化定时器
{ 
 
    TMOD=0x01;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    EA=1;
    ET0=1;
    TR0=1; 
}


 /*-----------主函数------------------*/
void main()
{ 
 chushihua(); 
 
 while(1)
 {
 
    P2=bufer[0];
    sendchar(table[miao%10]);
    delay(5);

    P2=bufer[1];
    sendchar(table[miao/10]);
    delay(5);

    P2=bufer[2];
    sendchar(table[fen%10]);
    delay(5);   

   	P2=bufer[3];
    sendchar(table[fen/10]);
    delay(5);   


     
 
 
 
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



