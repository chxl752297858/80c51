#include <reg52.h>
#include "seri12864.h"	

	
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
void     wri_12864(uchar value,uchar  dat)   
    {
              send_byte(value);//串口控制格式 1111 1000     RW=0,RS=0  RW=0表示数据从MCU写到LCD RS=0表示数据是控制指令 
              send_byte(dat&0xF0);//并行8位数据高4位格式 1111 0000  低四位置0  RW=1表示数据从LCD写到MCD RS=1表示显示数据 
              send_byte((dat&0x0F)<<4);//并行8位数据低4位格式 高四位置0 0000 1111  左移4位后位1111 0000 
			  
    }

	 void dis_line(uchar n,uchar m,uchar *s) //n表示行m表示列 s 表示字符串 写字符在哪行那列
{
  
    switch(n)
 {
     case 1:wri_12864(comd, 0x80+m-1);break;
     case 2:wri_12864(comd,0x90+m-1);break;
     case 3:wri_12864(comd,0x88+m-1);break;
     case 4:wri_12864(comd,0x98+m-1);break;
  default:;
 }

  while(*s>0)
 {
    wri_12864(date,*s);
    s++;
    delay_50us(1000);
 }
}

void dis_page(uchar *s)  //写完第一页
{uchar datecnt;

 for(datecnt=0;datecnt<64;datecnt++)
 {  
   
  if(datecnt==0) wri_12864(comd,0x80);
  if(datecnt==16)wri_12864(comd,0x90);
  if(datecnt==32)wri_12864(comd,0x88);
  if(datecnt==48)wri_12864(comd,0x98);
  
  
  
  wri_12864(date,*s);
  s++;
  delay_50us(2000);	
   while(*s=='\0')
   continue;

   }
 
   
 
}
 

 

 


   //初始化     LCM   
 void     init_12864(void)   
    {   
             delay_50us(100); //初始化等待时间40ms  
             wri_12864(comd,0x30);//功能设置，一次送8位数据，基本指令集   
             delay_50us(3);	//等待是间要大于100个us
			 wri_12864(comd,0x30);//功能设置，一次送8位数据，基本指令集 
			 delay_50us(2);	//等待是间要大于37个us 
			 wri_12864(comd,0x0c);//0000,1100       整体显示，游标off，游标位置off 
			 delay_50us(3);	//等待是间要大于37个us  
             wri_12864(comd,0x01);//0000,0001     清屏   
             delay_50us(200);	//等待时间大于10ms
			 wri_12864(comd,0x06);//0000,0010     DDRAM地址归位   
             delay_50us(10);
    }
