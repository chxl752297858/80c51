/*-----������λ�Ĵ���74ls164ʵ�ִ����ӿ�ת������------*/
/*-----2011.4.7   ��������д---------*/

#include <reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int
uchar  table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};   //0��9����
uchar  bufer[]={0x7f,0xbf,0xdf,0xef}; //�����λѡ����
uint i,aa,miao,fen;
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


 
void chushihua()           //��ʼ����ʱ��
{ 
 
    TMOD=0x01;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    EA=1;
    ET0=1;
    TR0=1; 
}


 /*-----------������------------------*/
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

void timer0() interrupt 1 //�ж��ۼ�
{
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    aa++; 
    if(aa==20)    //��aa==20ʱ��Ϊһ��
       {
         aa=0;       //aa����
         miao++;
         if(miao==60)//����֮��miao��һ
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



