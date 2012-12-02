/****12M晶振下89c52程序*******************/

#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar *s;
sbit rs=P3^5;    
sbit rw=P3^6;
sbit e=P3^7;

uchar second,minute,hour,count;

void delay_50us(uint t)  //延时50US
{uchar j;
for(;t>0;t--)
for(j=19;j>0;j--);
}

void write_com(uchar com)  //写数据  
{

rs=0;
rw=0;
delay_50us(1);
P1=com;
e=1;
delay_50us(20);
e=0;
}

void write_data(uchar dat) //写指令
{
rs=1;
rw=0;
P1=dat;
e=1;
delay_50us(10);
e=0;
delay_50us(2);
}

void init_12864(void)  //初始化
{   delay_50us(3);//初始化等待时间大于40ms 
    write_com(0x30);//功能设置，一次送8位数据，基本指令集  
    delay_50us(4); //等待是间要大于100个us
    write_com(0x30);//功能设置，一次送8位数据，基本指令集 
    delay_50us(1); //等待是间要大于37个us 
    write_com(0x0f);//0000,1100       整体显示，游标off，游标位置off 
    delay_50us(1); //等待是间要大于37个us 
     write_com(0x01);//0000,0001     清屏   
     delay_50us(240);//等待时间大于10ms
    write_com(0x06); //0000,0010     DDRAM地址归位 
    delay_50us(10);
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
    delay_50us(1000);
 }
}

write_separate_constant(uchar x,uchar add,uchar date) //分离常数分别显示程序
{
uchar decade,entry;
decade=date/10;
entry=date%10;
write_com(x+add);
write_data(0x30+decade);
write_data(0x30+entry);
return 0;
}

void init_time0(void)
{TMOD=0X01;
 TH0=(65536-50000)/256;
 TL0=(65536-50000)%256;
 EA=1;
 ET0=1;
 TR0=1;
}




void  main()  //主函数
{

 init_time0();
 init_12864();
 write_com(0x0c);
 
 while(1)
  {

  write_com(0x80+2);
  write_data(0x10);
  delay_50us(10);
  s="I love you";
  display(1,2,s); //写字符在哪行那列

  delay_50us(10);
  s="00时00分00秒";
  display(2,2,s); //写字符在哪行那列

  delay_50us(10);
  s="MY darling！";
  display(3,2,s); //写字符在哪行那列

  
  

 while(1)
 {
   if(count==20)    //当aa==20时，为一秒
       {
         count=0;       //aa清零
         second++;
         if(second==60)//清零之后，miao加一
         {   
          second=0;
            minute++;
            if(minute==60)
            {
             minute=0;
     hour++;
     if(hour==24)
     {hour=0;
   
    
    
        }

     write_separate_constant(0x90,1,hour)	;
             }
     write_separate_constant(0x90,3,minute)	;
         }
    write_separate_constant(0x90,5,second)	;
        
  }                    
     


  }


// while(1);


}


 
 
 
 
 
 
 
 }






  void timer0() interrupt 1	//延时1秒
{

TH0=(65536-50000)/256;
TL0=(65536-50000)%256;
count++;
}



