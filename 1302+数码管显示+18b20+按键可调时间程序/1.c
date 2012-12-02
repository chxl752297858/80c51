/*****12M晶振stc89c52单片机*********************/


#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define LED_CON P2	//位选端口

uchar second1,second2,minute1,minute2,hour1,hour2,day1,day2,month1,month2,week1,week2,year1,year2;
uchar time_data[7];//显示缓冲区
uchar  table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};   //0—9段码
uchar table_dp[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10}; //带秒闪0—9段码数组
//uchar  bufer[]={0x7f,0xbf,0xdf,0xef};	//数码管位选
 uchar code table2[]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x06,0x07,0x08,0x08,0x09,0x09};//小数查表
uchar second,minute,hour,month=1,week=1,day=1,year;  //调整日期参数
uint temp,flag,ch; //显示温度参数
uchar time_flag; //调整时间标志位
bit bell_flag;//整点报时标志位


     
sbit sck=P1^0;//1302时钟线
sbit io=P1^1; //1302信号线
sbit rst=P1^2;//1302复位线

sbit dq=P1^3;//DS18B20数据引脚  
  
sbit bell1 = P1^4; //闹铃开关指示灯 
 

sbit bell = P2^0;  //整点报时



sbit KEY20 = P3^2;
sbit KEY21 = P3^3;
sbit KEY22 = P3^4;
sbit KEY23 = P3^5;

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

/*******数码管显示程序************/

void delay(uint z)    //延时
{
    uint x,y;
    for(x=0;x<z;x++)
        for(y=0;y<110;y++);
}


void disp_time()//数码显示时间函数
{
    LED_CON&=0X0f;	 //保持低4位数据不变,高4位清零
	LED_CON|=(0xef&0xf0);//

    P0=table[hour1];
    delay(2);

    LED_CON&=0X0f;	 //保持低4位数据不变
	LED_CON|=(0xdf&0xf0);//
	if(second2%2==0)
    P0=table[hour2]; 
    else
	P0=table_dp[hour2];
	delay(2);

    LED_CON&=0X0f;	 //保持低4位数据不变
	LED_CON|=(0xbf&0xf0);//
    P0=table[minute1];
    delay(4);   

    LED_CON&=0X0f;	 //保持低4位数据不变
   	LED_CON|=(0x7f&0xf0);//
    P0=table[minute2];
    delay(4);

 }

 //整点报时程序
 void bell_clock()
 {
 
if((second1==0)&&((second2<6)&&(second2>=0))&&(minute1==0)&&(minute2==0)&&(bell_flag==0)) //整点报时
bell=0;
else
bell=1;

 
 }

void disp_set_time()//数码显示闹铃函数
{
    LED_CON&=0X0f;	 //保持低4位数据不变,高4位清零
	LED_CON|=(0xef&0xf0);//

    P0=table[hour/10];
    delay(2);

    LED_CON&=0X0f;	 //保持低4位数据不变
	LED_CON|=(0xdf&0xf0);//
//	if(second2%2==0)
   P0=table[hour%10]; 
//   else
//	P0=table_dp[hour%10];
	delay(2);

    LED_CON&=0X0f;	 //保持低4位数据不变
	LED_CON|=(0xbf&0xf0);//
    P0=table[minute/10];
    delay(4);   

    LED_CON&=0X0f;	 //保持低4位数据不变
   	LED_CON|=(0x7f&0xf0);//
    P0=table_dp[minute%10];
    delay(4);

 }

/*************18B20驱动程序************************/
 //******************************************
  //延时函数
  //*****************************************
 
  void delay1(unsigned int us)
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
    delay1(8);//延时
	dq=0;
	delay1(80); //拉低持续480~960us
	dq=1; //
	delay1(20);//拉高持续60~120us
	x=dq;
	delay1(20);
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
   delay1(4);//延时让单片机采样
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
   delay1(5); //延时60~120US
   dq=1;   //	拉高
   dat>>=1; //右移1位
   }
   delay1(4);
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
  delay1(50);

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






void disp_temp()//数码显示闹铃函数
{
    LED_CON&=0X0f;	 //保持低4位数据不变,高4位清零
	LED_CON|=(0xef&0xf0);//

    P0=table[temp/100];
    delay(2);

    LED_CON&=0X0f;	 //保持低4位数据不变
	LED_CON|=(0xdf&0xf0);//

    P0=table[temp%100/10];  //显示温度十位; 
	delay(2);

    LED_CON&=0X0f;	 //保持低4位数据不变
	LED_CON|=(0xbf&0xf0);//
    P0=table_dp[temp%100%10];
    delay(4);   

    LED_CON&=0X0f;	 //保持低4位数据不变
   	LED_CON|=(0x7f&0xf0);//
    P0=table[table2[ch]]; //显示小数位
    delay(4);

 }





 //*******************************
 //数码动态扫描函数
 //******************************




void set_time()
  
{ 
 
 bit key20_flag,key21_flag,key22_flag,key23_flag; 

 if((KEY20==0)&&(key20_flag==0))
{
delay(10);
if((KEY20==0)&&(key20_flag==0))
 {
time_flag++;
if(time_flag==3)
time_flag=0;

key20_flag=1;
}
}
if(KEY20==1)
{
key20_flag=0;

}

 if((KEY21==0)&&(key21_flag==0))
{
delay(10);
if((KEY21==0)&&(key21_flag==0))
 {
 if(time_flag==2)
 {
hour++;;
if(hour==24)
hour=0;
set_rtc(0X84,hour);
key21_flag=1;
}
}
}
if(KEY21==1)
{
key21_flag=0;
}

if((KEY22==0)&&(key22_flag==0))
{
delay(10);
if((KEY22==0)&&(key22_flag==0))
 {
 if(time_flag==2)
 {
  minute++;;
 if(minute==60)
 minute=0;
set_rtc(0X82,minute);

  set_rtc(0X80,0);	//设定秒
 // set_rtc(0X82,16);	//设定分
 // set_rtc(0X84,22);	//设定小时
  set_rtc(0X86,1);	//设定日期
  set_rtc(0X88,11);	//设定月份
  set_rtc(0X8A,1);	//设定星期
  set_rtc(0X8C,11);	 //设定年份

key22_flag=1;
}
}
}
if(KEY22==1)
{
key22_flag=0;
}

if((KEY23==0)&&(key23_flag==0))
{
delay(10);
if((KEY23==0)&&(key23_flag==0))
 {
 
bell_flag=~bell_flag;
bell1=bell_flag;
key23_flag=1;
}
}
if(KEY23==1)
{
key23_flag=0;
}



}

void main(void)   
    { 
	bell1=1;
	bell_flag=1;
  //  set_rtc(0X80,50);
	while(1)
	{
	 time_proce(); //1302时间处理函数
	 read_rtc();	   //实时时钟函数
     temp=readtemp();   //读取温度程序
	 set_time();	 //设定时间程序
	 bell_clock(); //设定闹铃开关程序

  
	if(time_flag==0)
	{
    disp_time();//数码显示时间函数
     }
   if(time_flag==1)
	{
		disp_temp();//显示温度程序
	}
    if(time_flag==2)
	{
	disp_set_time(); //设定设定时间程序
	}
    
    
	  
	}
	}

