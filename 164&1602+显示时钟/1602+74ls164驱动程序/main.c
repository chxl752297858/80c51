/****12M������89c52��Ƭ��lcd1602+164��������*************/


#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar table[]={"love you my love"};
uint i,shi,fen,miao,aa;
sbit rs=P3^5;  //  
sbit rw=P3^6;  //д������
sbit e=P3^7;  //ʹ����

sbit dat_164=P3^3;//164������
sbit clk_164=P3^4;//164ʱ����


 void sendchar(uchar ch)//��1602��һ�ֽ���ʾ����
{

uchar i;
for(i=0;i<8;i++)
{
clk_164=0;
dat_164=ch&0x01; //�ӵ�λ��ʼ��
clk_164=1;
ch>>=1;
}
}


void delay_50us(uint t)  //��ʱ50US
{uchar j;
for(;t>0;t--)
for(j=19;j>0;j--);
}

void write_com(uchar com)  //д����  
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

void write_data(uchar dat) //дָ��
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


void init_1602(void)  //��ʼ��
{   delay_50us(300); //��ʱ15����
    write_com(0x38);//�����æ�ź�
    delay_50us(100);//��ʱ5����
    write_com(0x38);//�����æ�ź�
    delay_50us(100);//��ʱ5����
	write_com(0x38);//�����æ�ź�
	write_com(0x38);//��ʾģʽ����
    write_com(0x08);//��ʾ�ر�
    write_com(0x01);//��ʾ����
    write_com(0x06);//��ʾ����ƶ�����  
    write_com(0x0c);//��ʾ��������ƶ�����
}

void init_timer0()           //��ʼ��
{ 
 
    TMOD=0x01;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    EA=1;
    ET0=1;
    TR0=1; 
}

void  main()  //������
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



