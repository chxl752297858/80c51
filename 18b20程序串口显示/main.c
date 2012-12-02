			  //****************************************************************//
//18b20��������ʹ�÷��� 
//��Ƭ�� ��AT89S52 �� STC89C52RC 
// ����  �����ڷ�����ʪ������  ������ 9600 
// ����  ��12M (�û�ϵͳʱ���粻��12M �������غ궨�弰ע�͵���ʱʱ��)
// ���뻷��:  Keil4
// ������  ��������  
// �������� ��2012��10��9��   
//****************************************************************//

#include "reg52.h"
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned char

//�û������Լ��ľ����޸���Ӧֵ
#define FOSC  12000000
#define BAUD  9600 



uchar code table2[]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x06,0x07,0x08,0x08,0x09,0x09};//С�����

sbit dq=P0^7;  //DS18B20��������
 
  
sfr T2MOD  = 0xC9;  //ADD

uint temp,flag,ch;


/********************************************\
|* ���ܣ� ��ʱ	����Ϊ12Mʱ					*|
|* ��ʱ��Լ 1ms			    				*|
\********************************************/ 
void Delay_N1ms(unsigned int t)
{
  unsigned int i;
  unsigned int j;
  for(j=t;j>0;j--)
     for(i=124;i>0;i--);  //��ʱ��Լ 1ms
}
/********************************************\
|* ���ܣ� ��ʼ������					    *|
\********************************************/
void InitUART(void)
  {
    unsigned int iTmpBaud;
    unsigned long lTmpBaud;
    iTmpBaud = 0; //����ѡ����ʱ��2��Ϊ�����ʷ�����,16λ��ʱ��,�Զ�װ��
    SCON = 0x50;	//SM0 SM1 SM2 REN TB8 RB8 TI RI		//0   1   0   1   0   0   0  0	
  	PCON = 0x00;	//PCON�ĵ�ַ��87H,����SMOD =0
    T2CON = 0x30;	//TF2 EXF2 RCLK TCLK EXEN2 TR2 C(/T2) CP(/RL2) //0 0 1 1 0 0 0 0 
  	T2MOD = 0x00;	// /	/	/	/		/	/	T2OE	DCEN   //0 0 0 0 0 0 0 0
    lTmpBaud = FOSC/BAUD;
  	lTmpBaud /= 32;						//12T-mode
  	iTmpBaud = lTmpBaud & 0xFFFF;		
  	iTmpBaud = 65536 - iTmpBaud;
  	RCAP2H = (iTmpBaud>>8) & 0x0FF;
  	RCAP2L = iTmpBaud & 0x0FF;
	RI = 0;			//��������жϱ�־
  	REN = 1;		//�����н���
  	ES = 1;			//�������ж�
  	TR2 = 1;		//������ʱ��1
    EA=1;           //�����ж�
  }  

/********************************************\
|* ���ܣ� ���ڷ��ͺ��� 			            *|
\********************************************/
void UARTSend(char UCHAR)
  {
    SBUF=UCHAR;
    while(TI==0);
    TI=0;
  }
/********************************************\
|* ���ܣ� �����жϺ��� 			            *|
\********************************************/
void UARTRead(void) interrupt 4
  {
    char temp; 
    if(RI)
    {
      RI=0;
      temp = SBUF;
    }
  }
 /********************************************\
|* ���ܣ� ���ڷ����Ӻ��� 			        *|
\********************************************/

void UART_PutString(unsigned char *buf)
  {
	while(*buf)
      UARTSend(*buf++);
  }




void UART_PutStringAndNum(unsigned char *buf ,unsigned int num)
  {
	unsigned char a[4],i;
	a[4] = '0'+table2[ch];
	a[3] = '.';
	a[2] = '0'+num%10;
	a[1] = '0'+num/10%10;
	a[0] = '0'+num/100%10;
	while(*buf)
    UARTSend(*buf++);
	UARTSend(' ');
	 for(i=0;i<5;i++)
	{
		UARTSend(a[i]);
	}
   UART_PutString("��C"); 
   UART_PutString("\r\n");
  }







//******************************************
  //��ʱ����
  //*****************************************
 
  void delay(unsigned int us)
   {
   while(us--);
   }
   //******************************************
   //ds18b20��λ����
   //******************************************
   void reset(void)
   {
    uchar x;//
    dq=1;//
    delay(8);//��ʱ
	dq=0;
	delay(80); //���ͳ���480~960us
	dq=1; //
	delay(20);//���߳���60~120us
	x=dq;
	delay(20);
    }
 //******************************************
 //��ds18b20�ж�һ���ֽ�
 //******************************************
 uchar readbyte(void)
 {uchar i,value;
  
  for(i=0;i<8;i++)
  {dq=0;
   value>>=1;
   dq=1;
   if(dq)
   value|=0x80;
   delay(4);//��ʱ�õ�Ƭ������
   }
  return(value);
  }
 //****************************************
 // ��ds18b20��дһ���ֽ�
 //***************************************
 void writebyte(uchar dat)
 {
  uchar i=0;
  for(i=0;i<8;i++)
  {dq=0;
   dq=dat&0x01;	//�ӵ�λ��ʼд
   delay(5); //��ʱ60~120US
   dq=1;   //	����
   dat>>=1; //����1λ
   }
   delay(4);
  }
 
   
   //********************************************
  //  ��DS18B20�ж�ȡʵʱ�¶�ֵ
     //********************************************
  uchar readtemp(void)
  {
  
  uchar a,b;
 { 
  reset(); //��ʼ��18b20
  writebyte(0xcc);//����ROM
  writebyte(0x44);//�����¶Ȳ���
  delay(50);

  reset(); //��ʼ��18b20
  writebyte(0xcc);//����ROM
  writebyte(0xbe);//��9���Ĵ�����ǰ�����¶�
  
  a=readbyte(); //��λ
  b=readbyte(); //��λ
  if(b>0x0f) //��ʾ���¶�
  {
  a=~a+1;
  if(a==0)
  b=~b+1;
  else 
  b=~b;
  flag=0xff; //��ʾ����λ-
  }
  else 	//��ʾ���¶�
  flag=0x00; //��ʾ����λ+
  ch=a&0x0f; //С����λ
  a>>=4;
  b<<=4;
  b=b|a;   // ����λ
  
  }
  
   return(b);
   }



void main(void)
{

  InitUART();          //��ʼ���ڷ��ͺ���
  while(1)
  {
	temp=readtemp();
    Delay_N1ms(1000);	  // ��ʱ 2S(���ζ�ȡ�������2S)
	UART_PutStringAndNum("current temperature is :  ",temp);	
	
	}
}
