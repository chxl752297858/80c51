//****************************************************************//
//AMϵ�ж�������ʹ�÷��� 
//��Ƭ�� ��AT89S52 �� STC89C52RC 
// ����  �����ڷ�����ʪ������  ������ 9600 
// ����  ��12M (�û�ϵͳʱ���粻��12M �������غ궨�弰ע�͵���ʱʱ��)
// ���뻷��:  Keil3
// ��˾  �����ɵ���    
//****************************************************************//

#include "reg52.h"
#include <intrins.h>

#define FOSC  12000000
#define BAUD  9600

sbit  P2_0  = P1^0 ;




 
sfr T2MOD  = 0xC9;  //ADD

typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) */




U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;


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
	   	//��ʱ������forѭ��		  
	   	 if(U8FLAG==1)break;
	   	//�ж�����λ��0����1	 
	   	   
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	   	 
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   
	}

	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

	void RH(void)
	{
	  //��������18ms 
       P2_0=0;
	   Delay(180);
	   P2_0=1;
	 //������������������ ������ʱ20us
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	   P2_0=1;
	 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	   if(!P2_0)		 //T !	  
	   {
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	   while((!P2_0)&&U8FLAG++);
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while((P2_0)&&U8FLAG++);
	 //���ݽ���״̬		 
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
	 //����У�� 
	 
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

  InitUART();          //��ʼ���ڷ��ͺ���
  while(1)
  {	 
     RH();

    Delay_N1ms(1000);	  // ��ʱ 2S(���ζ�ȡ�������2S)
	UART_PutStringAndNum("current humidity  is    : ",U8RH_data_H);	
    Delay_N1ms(1000);	  // ��ʱ 2S(���ζ�ȡ�������2S)
//	UART_PutStringAndNum("current humidity  is    : ",U8RH_data_L);		
//	Delay_N1ms(1000);	  // ��ʱ 2S(���ζ�ȡ�������2S)
	UART_PutStringAndNum("current temperature is  : ",U8T_data_H);	
//	Delay_N1ms(1000);	  // ��ʱ 2S(���ζ�ȡ�������2S)
//	UART_PutStringAndNum("current temperature is  :",U8T_data_L);	 
  }
}