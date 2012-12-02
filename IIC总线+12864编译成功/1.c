#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit sda=P2^0;
sbit scl=P2^1;

sbit e_clk=P3^7;  //串行时钟信号
sbit rw_sid=P3^6; //串行数据线


uchar a,k;
void delay()
{_nop_(); 
 _nop_(); 
 _nop_(); 
}
void start()  //开始信号
{	
	sda=1;
	delay();
	scl=1;
	delay();
	sda=0;
	delay();
}

void stop()   //停止
{
	sda=0;
	delay();
	scl=1;
	delay();
	sda=1;
	delay();
}

void respons()  //应答
{
	uchar i;
	scl=1;
	delay();
	while((sda==1)&&(i<25))i++;
	scl=0;
	delay();
}

void init()
{
	sda=1;
	delay();
	scl=1;
	delay();
}

void write_byte(uchar date)
{
	uchar i,temp;
	temp=date;


	for(i=0;i<8;i++)
	{
		temp<<=1;
		scl=0;
	    delay();
		sda=CY;
		delay();
		scl=1;
		delay();
	//	scl=0;
     //   delay();
	}
	scl=0;
	delay();
	sda=1;
	delay();
}

uchar read_byte()
{
	uchar i,k;
	scl=0;
	delay();
	sda=1;
	delay();
	for(i=0;i<8;i++)
	{
		scl=1;
		delay();	
		k=(k<<1)|sda;
		scl=0;
		delay();	
	}
	return k;
}

void delay1(uint i)
{
	uchar j;
	for(;i>0;i--)
	 for(j=6245;j>0;j--);
}

void write_add(uchar address,uchar date)//写数据
{
	start();  //起始信号
	write_byte(0xa0);
	respons();//应答信号
	write_byte(address); //写地址
	respons(); //应答信号
	write_byte(date); //写数据
	respons();//应答信号
	stop();	//停信号
}


uchar read_add(uchar address) //读地址
{
	uchar date;
	start();
	write_byte(0xa0);
	respons();
	write_byte(address);
	respons();
	start();
	write_byte(0xa1);
	respons();
	date=read_byte();
	stop();
	return date;
}



/********串行12864显示驱动程序***********/
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

   /*
void display(uchar n,uchar m,uchar *s) //写字符在哪行那列
{
  
    switch(n)
 {
     case 1:write_com(0x80+m-1);break;
     case 2:write_com(0x90+m-1);break;
     case 3:write_com(0x88+m-1);break;
     case 4:write_com(0x98+m-1);break;
  default:;
 }

  while(*s>0)
 {
    write_data(*s);
    s++;
    delay_50us(1);
 }
} 
	*/

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



void main()
{	init_12864();
    init();
   
   for(k=0;k<=64;k++)
   {
	write_add(k,k);
	delay1(10);
   
	}
   
   	while(1)
	{

	write_com(0x80);
	for(k=0;k<=64;k++)
	{
	write_data(read_add(k)+0x30);
    }

	}
}


