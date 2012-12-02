/*****12M晶振stc89c52单片机*********************/


#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar second1,second2,minute1,minute2,hour1,hour2,day1,day2,month1,month2,week1,week2,year1,year2;
uchar time_data[7];//显示缓冲区
uchar i;
uint temp,flag,ch;

sbit e_clk=P3^7;  //串行时钟信号
sbit rw_sid=P3^6; //串行数据线

sbit dq=P1^1;//DS18B20数据引脚 
        
sbit sck=P0^0;
sbit io=P0^1;
sbit rst=P0^2;

sbit KEY1=P1^7;  
sbit KEY2=P1^6;  
/*ds1302驱动程序*/
 
  void write_ds1302_byte(uchar dat)	//写单字节
  {uchar i;
   for(i=0;i<8;i++)
   {sck=0;
    io=dat&0x01;
	dat>>=1;
	sck=1;
   }
  }


  void write_ds1302(uchar add,uchar dat)
  {rst=0;
   _nop_();
   sck=0;
   _nop_();
   rst=1;
   _nop_();
   write_ds1302_byte(add);
   write_ds1302_byte(dat);
   rst=0;
   _nop_();
   io=1;
   sck=1;
   }
  
  uchar read_ds1302(uchar add)
  
  {uchar i,value;
    rst=0;
    _nop_();
	sck=0;
	_nop_();
	rst=1;
	_nop_();
	write_ds1302_byte(add);
	for(i=0;i<8;i++)
	{
	value=value>>1;
	sck=0;
	if(io)
	value=value|0x80;
	sck=1;
	}
	rst=0;
	_nop_();
	sck=0;
	_nop_();	   
	sck=1;
	io=1;
	return value;
 }
 	

 void set_rtc(uchar add,value)//设定时钟函数注意 BCD码的转换
{ 
  uchar temp,temp1,temp2;
  write_ds1302(0x8e,0x00);//去除写保护
  temp1=value/10; //分离十位
  temp2=value%10;//分离个位
  temp=temp2+temp1*16; //转换完成BCD码
  write_ds1302(add,temp);
  write_ds1302(0x8e,0x80); //加上写保护
}	

void read_rtc(void)//读取时钟函数
{
time_data[0]=read_ds1302(0x8d);	//读年份
time_data[1]=read_ds1302(0x8b);	//读星期
time_data[2]=read_ds1302(0x89);	//读月
time_data[3]=read_ds1302(0x87);	//读日
time_data[4]=read_ds1302(0x85);	//读时
time_data[5]=read_ds1302(0x83); //读分
time_data[6]=read_ds1302(0x81);	//读秒
}



void time_proce() //数据处理函数把数据转换为相应的10进制数
{   second1=time_data[6]/16; //转换为10进制数高位
    second2=time_data[6]%16	;//转换为10进制数低位
    minute1=time_data[5]/16;
    minute2=time_data[5]%16	;
    hour1=time_data[4]/16 ;
    hour2=time_data[4]%16 ;
    day1=time_data[3]/16;
    day2=time_data[3]%16;
	month1=time_data[2]/16;
    month2=time_data[2]%16;
	week1=time_data[1]/16;
    week2=time_data[1]%16;
	year1=time_data[0]/16;
    year2=time_data[0]%16;
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

 /*****ds18b20程序*******/

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
 //*******************************
 //数码动态扫描函数
 //******************************
 
 void disp_time() //显示
   {
    
    write_com(0x95);
    write_data(second1+0x30);
	write_data(second2+0x30);
    write_com(0x93);
    write_data(minute1+0x30);
	write_data(minute2+0x30);
	write_com(0x91);
    write_data(hour1+0x30);
	write_data(hour2+0x30);

    write_com(0x86);
    write_data(day1+0x30);
	write_data(day2+0x30);
	write_com(0x84);
    write_data(month1+0x30);
	write_data(month2+0x30);
	write_com(0x82);
    write_data(year1+0x30);
	write_data(year2+0x30);
	
	write_com(0x8d);
    write_data(week2+0x30);
	}

 void disp_temp()//显示温度函数
 {

write_com(0x9b);
write_data(temp/100+0x30);
write_data(temp%100/10+0x30); 
write_data(temp%100%10+0x30);
write_data('.');		  
write_data((ch*0.6)+0x30); 
}

void set_time()//设定时间程序
{

  set_rtc(0X80,30);	//设定秒
  set_rtc(0X82,16);	//设定分
  set_rtc(0X84,22);	//设定小时
  set_rtc(0X86,31);	//设定日期
  set_rtc(0X88,11);	//设定月份
  set_rtc(0X8A,1);	//设定星期
  set_rtc(0X8C,11);	 //设定年份
  	
}



void init_int0()  // 中断0初始化
{
 IT0=1;//设置INT0为下降沿触发
 EX0=1;	 //局部开中断，即允许INT0中断
 IT1=1;//设置INT0为下降沿触发
 EX1=1;	 //局部开中断，即允许INT0中断
 EA=1;	   // 全局中断
}

	
 main(void)   
    { 
	
	init_12864();
	init_int0();  // 中断0初始化
  	set_time();
	display(1,2,"20  年  月  日");
    display(2,3,"时  分  秒");
    display(3,3,"星期");
    display(4,1,"温度        ℃");
  
  	write_data(0X86);
	delay_50us(10);
	write_com(0X0F);  
	
	while(1)
	{
	
	temp=readtemp();
    read_rtc();
    time_proce();
  	disp_time();
 	disp_temp();
	 
	 }
	
	}


	void serv_int0() interrupt  0  //外部中断0 实现调节PWM的大小
{
 if(INT0==0)
  {
 delay_50us(100);
   
KEY1=~KEY1;	

  }

}
void serv_int1() interrupt  2  //外部中断2 实现调节PWM的大小
{
 if(INT1==0)
  {
  delay_50us(100);  
   KEY2=~KEY2;
}

}



