/****12M������89c52��Ƭ��lcd1602��������*************/


#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar table[]={"i love you dear!"};
uint i;

sbit rs=P3^5;    
sbit rw=P3^6;
sbit e=P3^7;


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
P1=com;
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
P1=dat;
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


void  main()  //������
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
 




