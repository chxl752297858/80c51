#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
uchar  table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};   //0��9����
uchar  bufer[]={0x1f,0x2f,0x4f,0x8f};

uchar miao,fen,aa;       //����miao,fen,�Լ������õ�aa(aa==20ʱ��һ����)
void display();
void delay(uint z);
void chushihua();
void main()
{   
   chushihua();//��ʼ��
    while(1) //��ѭ��
    {
 
   display();//������ʾ����
    }
}
void chushihua()           //��ʼ��
{ 
 
    TMOD=0x01;
    TH0=(65536-50000)/256;
    TL0=(65536-50000)%256;
    EA=1;
    ET0=1;
    TR0=1; 
}
void delay(uint z)    //��ʱ
{
    uint x,y;
    for(x=0;x<z;x++)
        for(y=0;y<110;y++);
}

void display()//������ʾ����
{
    P2=bufer[0];
    P0=table[fen/10];
    delay(5);

    P2=bufer[1];
    P0=table[fen%10];
    delay(5);

    P2=bufer[2];
    P0=table[miao/10];
    delay(5);   

    P2=bufer[3];
    P0=table[miao%10];
    delay(5);

     
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

