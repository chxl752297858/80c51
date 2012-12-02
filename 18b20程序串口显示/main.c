			  //****************************************************************//
//18b20读单总线使用范例 
//单片机 ：AT89S52 或 STC89C52RC 
// 功能  ：串口发送温湿度数据  波特率 9600 
// 晶振  ：12M (用户系统时钟如不是12M 请更改相关宏定义及注释的延时时间)
// 编译环境:  Keil4
// 编译者  ：陈贤龙  
// 编译日期 ：2012年10月9日   
//****************************************************************//

#include "reg52.h"
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned char

//用户根据自己的晶振修改相应值
#define FOSC  12000000
#define BAUD  9600 



uchar code table2[]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x06,0x07,0x08,0x08,0x09,0x09};//小数查表

sbit dq=P0^7;  //DS18B20数据引脚
 
  
sfr T2MOD  = 0xC9;  //ADD

uint temp,flag,ch;


/********************************************\
|* 功能： 延时	晶振为12M时					*|
|* 延时大约 1ms			    				*|
\********************************************/ 
void Delay_N1ms(unsigned int t)
{
  unsigned int i;
  unsigned int j;
  for(j=t;j>0;j--)
     for(i=124;i>0;i--);  //延时大约 1ms
}
/********************************************\
|* 功能： 初始化串口					    *|
\********************************************/
void InitUART(void)
  {
    unsigned int iTmpBaud;
    unsigned long lTmpBaud;
    iTmpBaud = 0; //首先选定定时器2作为波特率发生器,16位定时器,自动装载
    SCON = 0x50;	//SM0 SM1 SM2 REN TB8 RB8 TI RI		//0   1   0   1   0   0   0  0	
  	PCON = 0x00;	//PCON的地址是87H,这里SMOD =0
    T2CON = 0x30;	//TF2 EXF2 RCLK TCLK EXEN2 TR2 C(/T2) CP(/RL2) //0 0 1 1 0 0 0 0 
  	T2MOD = 0x00;	// /	/	/	/		/	/	T2OE	DCEN   //0 0 0 0 0 0 0 0
    lTmpBaud = FOSC/BAUD;
  	lTmpBaud /= 32;						//12T-mode
  	iTmpBaud = lTmpBaud & 0xFFFF;		
  	iTmpBaud = 65536 - iTmpBaud;
  	RCAP2H = (iTmpBaud>>8) & 0x0FF;
  	RCAP2L = iTmpBaud & 0x0FF;
	RI = 0;			//清除接收中断标志
  	REN = 1;		//允许串行接收
  	ES = 1;			//允许串行中断
  	TR2 = 1;		//启动定时器1
    EA=1;           //开总中断
  }  

/********************************************\
|* 功能： 串口发送函数 			            *|
\********************************************/
void UARTSend(char UCHAR)
  {
    SBUF=UCHAR;
    while(TI==0);
    TI=0;
  }
/********************************************\
|* 功能： 串口中断函数 			            *|
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
|* 功能： 串口发送子函数 			        *|
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
   UART_PutString("°C"); 
   UART_PutString("\r\n");
  }







//******************************************
  //延时函数
  //*****************************************
 
  void delay(unsigned int us)
   {
   while(us--);
   }
   //******************************************
   //ds18b20复位函数
   //******************************************
   void reset(void)
   {
    uchar x;//
    dq=1;//
    delay(8);//延时
	dq=0;
	delay(80); //拉低持续480~960us
	dq=1; //
	delay(20);//拉高持续60~120us
	x=dq;
	delay(20);
    }
 //******************************************
 //从ds18b20中读一个字节
 //******************************************
 uchar readbyte(void)
 {uchar i,value;
  
  for(i=0;i<8;i++)
  {dq=0;
   value>>=1;
   dq=1;
   if(dq)
   value|=0x80;
   delay(4);//延时让单片机采样
   }
  return(value);
  }
 //****************************************
 // 从ds18b20中写一个字节
 //***************************************
 void writebyte(uchar dat)
 {
  uchar i=0;
  for(i=0;i<8;i++)
  {dq=0;
   dq=dat&0x01;	//从地位开始写
   delay(5); //延时60~120US
   dq=1;   //	拉高
   dat>>=1; //右移1位
   }
   delay(4);
  }
 
   
   //********************************************
  //  从DS18B20中读取实时温度值
     //********************************************
  uchar readtemp(void)
  {
  
  uchar a,b;
 { 
  reset(); //初始化18b20
  writebyte(0xcc);//跳过ROM
  writebyte(0x44);//启动温度测量
  delay(50);

  reset(); //初始化18b20
  writebyte(0xcc);//跳过ROM
  writebyte(0xbe);//读9个寄存器，前两个温度
  
  a=readbyte(); //低位
  b=readbyte(); //高位
  if(b>0x0f) //显示负温度
  {
  a=~a+1;
  if(a==0)
  b=~b+1;
  else 
  b=~b;
  flag=0xff; //显示符号位-
  }
  else 	//显示正温度
  flag=0x00; //显示符号位+
  ch=a&0x0f; //小数点位
  a>>=4;
  b<<=4;
  b=b|a;   // 整数位
  
  }
  
   return(b);
   }



void main(void)
{

  InitUART();          //初始串口发送函数
  while(1)
  {
	temp=readtemp();
    Delay_N1ms(1000);	  // 延时 2S(两次读取间隔至少2S)
	UART_PutStringAndNum("current temperature is :  ",temp);	
	
	}
}
