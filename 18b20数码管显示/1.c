/********12M晶振stc89c52单片机DS18B20程序****************/
  #include<reg52.h>
  #include<intrins.h>
  #define uchar unsigned char
  #define uint  unsigned char
  uchar code table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90}; //0—9数码管显示  
  uchar code table1[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10}; //0—9带小数点数码管显示  
  uchar code table2[]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x06,0x07,0x08,0x08,0x09,0x09};//小数查表
 
  sbit dq=P1^1;//DS18B20数据引脚
  uint temp,flag,ch;
  
  
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
 void scandisp()
                   
{	P2=0x7f; //显示温度百位
    P0=table[temp/100];
    delay(200);
   
    P2=0xbf;
    P0=table[temp%100/10];  //显示温度十位
    delay(200);

    P2=0xdf;    //显示温度各位
    P0=table1[temp%100%10];
    delay(200);

    

    P2=0xef;
    P0=table[table2[ch]]; //显示小数位
    delay(200);


     
}
//**************************************
//主函数
//***************************************

void main()
{
 
 

temp=readtemp();
  scandisp();
 

 

}
 

