/*****12M晶振stc89c52单片机*********************/


#include<stc12.h>
#define uint unsigned int
#define uchar unsigned char
uchar table[]={"i love you my love"};
uchar i;

sbit e_clk=P3^7;  //串行时钟信号
sbit rw_sid=P3^6; //串行数据线
         
void  delay_50us(uint j)   //延时50uS  
    {   
        uint  i; 
        for(; j>0; j--); 
        for(i=19; i>0; i--);  
    }

//串行发送一字节数据   
void send_byte(uchar  dat)   
    {   
                 uchar i;   
                 for(i=0;i<8;i++)   
                {   
                  e_clk=0;   
                  if(dat&0x80)//读取最高位的值如果是1则rw_sid为1，否则rw_sid为0；
                  rw_sid=1;
                  else     
                  rw_sid=0; 
				  e_clk=1;   
                  dat=dat<<1; //右移一位  
                 }   
    } 

//写控制命令分三个字节写  
void     write_com(uchar  dat)   
    {
              send_byte(0xF8);//串口控制格式 1111 1000     RW=0,RS=0  RW=0表示数据从MCU写到LCD RS=0表示数据是控制指令 
              send_byte(dat&0xF0);//并行8位数据高4位格式 1111 0000  低四位置0  RW=1表示数据从LCD写到MCD RS=1表示显示数据 
              send_byte((dat&0x0F)<<4);//并行8位数据低4位格式 高四位置0 0000 1111  左移4位后位1111 0000 
			  
    }


//写显示数据或单字节字符   
void     write_data(uchar dat)   
    
    {         send_byte(0xFA);//1111 1010     RW=0,RS=1   
              send_byte(dat&0xF0);//高四位   
              send_byte((dat&0x0F)<<4);//低四位  
    }

 


 //初始化     LCM   
void     init_12864(void)   
    {   
             delay_50us(100); //初始化等待时间40ms  
             write_com(0x30);//功能设置，一次送8位数据，基本指令集   
             delay_50us(3);	//等待是间要大于100个us
			 write_com(0x30);//功能设置，一次送8位数据，基本指令集 
			 delay_50us(2);	//等待是间要大于37个us 
			 write_com(0x0C);//0000,1100       整体显示，游标off，游标位置off 
			 delay_50us(3);	//等待是间要大于37个us  
             write_com(0x01);//0000,0001     清屏   
             delay_50us(200);	//等待时间大于10ms
			 write_com(0x06);//0000,0010     DDRAM地址归位   
             delay_50us(10);
    }

 

 main(void)   
    { 
	  init_12864();
	while(1)
	{ 
	write_com(0x80);
	delay_50us(2);
	for(i=0;i<16;i++)
	{
	write_data(table[i]);
	}
	//write_data(0x31);
	 }
	
	}