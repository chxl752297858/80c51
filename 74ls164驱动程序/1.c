/*-----������λ�Ĵ���74ls164ʵ�ִ����ӿ�ת������------*/
/*-----2011.4.7   ��������д---------*/

#include <reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int

sbit dat_164=P1^0;//164������
sbit clk_164=P1^1;//164ʱ����


void delay(uint z)    //��ʱ
{
    uint x,y;
    for(x=0;x<z;x++)
        for(y=0;y<110;y++);
}


void sendchar(uchar ch)//���������һ�ֽ���ʾ����
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


 


 /*-----------������------------------*/
void main()
{ 
 
 while(1)
 {
 
 ;  
}
}

