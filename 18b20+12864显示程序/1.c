/********12M晶振stc89c52单片机DS18B20程序****************/
  #include<reg52.h>
  #include<intrins.h>
  #define uchar unsigned char
  #define uint  unsigned char
 
  sbit e_clk=P0^1;  //串行时钟信号
  sbit rw_sid=P0^0; //串行数据线
         

  sbit dq=P0^7;//DS18B20数据引脚
  uint temp,flag,ch;
  
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
 
 
//**************************************
//主函数
//***************************************

void main()
{
 init_12864(); 
while(1)
{ 

temp=readtemp();
write_com(0x98);
write_data(temp/100+0x30);
write_data(temp%100/10+0x30); 
write_data(temp%100%10+0x30);
write_data('.');		  
write_data((ch*0.6)+0x30); 
//write_data(ch%10+0x30); 
 }

}
 

