//****************************************************************//
//AM系列读单总线使用范例 
//单片机 ：AT89S52 或 STC89C52RC 
// 功能  ：串口发送温湿度数据  波特率 9600 
// 晶振  ：12M (用户系统时钟如不是12M 请更改相关宏定义及注释的延时时间)
// 编译环境:  Keil3
// 公司  ：奥松电子    
//****************************************************************//

#include "reg52.h"
#include <intrins.h>

#define FOSC  12000000
#define BAUD  9600

sbit  P2_0  = P1^0 ;




 
sfr T2MOD  = 0xC9;  //ADD

typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  有符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  有符号16位整型变量 */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  无符号32位整型变量 */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  有符号32位整型变量 */
typedef float          F32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         F64;      /* double precision floating point variable (64bits) */




U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;


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
	unsigned char a[2],i;

	a[2] = '0'+num%10;
	a[1] = '0'+num/10%10;
	a[0] = '0'+num/100%10;
	while(*buf)
    UARTSend(*buf++);
	UARTSend(' ');
	 for(i=0;i<3;i++)
	{
		UARTSend(a[i]);
	} 
   UART_PutString("\r\n");
  }


   
       void Delay(U16 j)
    {      U8 i;
	    for(;j>0;j--)
	  { 	
		for(i=0;i<27;i++);

	  }
    }
       void  Delay_10us(void)
      {
        U8 i;
        i--;
        i--;
        i--;
        i--;
        i--;
        i--;
       }
	
        void  COM(void)
      {
     
	        U8 i;
          
       for(i=0;i<8;i++)	   
	    {
		
	   	    U8FLAG=2;	
	   	while((!P2_0)&&U8FLAG++);
			Delay_10us();
		    Delay_10us();
			Delay_10us();
	  		U8temp=0;
	     if(P2_0)U8temp=1;
		    U8FLAG=2;
		 while((P2_0)&&U8FLAG++);
	   	//超时则跳出for循环		  
	   	 if(U8FLAG==1)break;
	   	//判断数据位是0还是1	 
	   	   
		// 如果高电平高过预定0高电平值则数据位为 1 
	   	 
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   
	}

	//--------------------------------
	//-----湿度读取子程序 ------------
	//--------------------------------
	//----以下变量均为全局变量--------
	//----温度高8位== U8T_data_H------
	//----温度低8位== U8T_data_L------
	//----湿度高8位== U8RH_data_H-----
	//----湿度低8位== U8RH_data_L-----
	//----校验 8位 == U8checkdata-----
	//----调用相关子程序如下----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

	void RH(void)
	{
	  //主机拉低18ms 
       P2_0=0;
	   Delay(180);
	   P2_0=1;
	 //总线由上拉电阻拉高 主机延时20us
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	 //主机设为输入 判断从机响应信号 
	   P2_0=1;
	 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	   if(!P2_0)		 //T !	  
	   {
	   U8FLAG=2;
	 //判断从机是否发出 80us 的低电平响应信号是否结束	 
	   while((!P2_0)&&U8FLAG++);
	   U8FLAG=2;
	 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while((P2_0)&&U8FLAG++);
	 //数据接收状态		 
	   COM();
	   U8RH_data_H_temp=U8comdata;
	   COM();
	   U8RH_data_L_temp=U8comdata;
	   COM();
	   U8T_data_H_temp=U8comdata;
	   COM();
	   U8T_data_L_temp=U8comdata;
	   COM();
	   U8checkdata_temp=U8comdata;
	   P2_0=1;
	 //数据校验 
	 
	   U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   if(U8temp==U8checkdata_temp)
	   {
	   	  U8RH_data_H=U8RH_data_H_temp;
	   	  U8RH_data_L=U8RH_data_L_temp;
		  U8T_data_H=U8T_data_H_temp;
	   	  U8T_data_L=U8T_data_L_temp;
	   	  U8checkdata=U8checkdata_temp;
	   }//fi
	   }//fi

	}








void main(void)
{

  InitUART();          //初始串口发送函数
  while(1)
  {	 
     RH();

    Delay_N1ms(1000);	  // 延时 2S(两次读取间隔至少2S)
	UART_PutStringAndNum("current humidity  is    : ",U8RH_data_H);	
    Delay_N1ms(1000);	  // 延时 2S(两次读取间隔至少2S)
//	UART_PutStringAndNum("current humidity  is    : ",U8RH_data_L);		
//	Delay_N1ms(1000);	  // 延时 2S(两次读取间隔至少2S)
	UART_PutStringAndNum("current temperature is  : ",U8T_data_H);	
//	Delay_N1ms(1000);	  // 延时 2S(两次读取间隔至少2S)
//	UART_PutStringAndNum("current temperature is  :",U8T_data_L);	 
  }
}