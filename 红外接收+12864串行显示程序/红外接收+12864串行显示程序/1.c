#include<regx51.h>   
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar irtime;//hong wai shi jian
uchar startflag;//qi dong jie shou
uchar irdata[33];//chun shu ju
uchar bitnum;//第几位
uchar irreceok;//hong wai jie shou wan l
uchar ircode[4];// ma
uchar irprosok;
uchar disp[8];




uchar *s;

uchar irtime;//红外时间

sbit E_CLK=P3^7;  //串行时钟信号
sbit RW_SID=P3^6; //
         
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
                  E_CLK=0;   
                  if(dat&0x80)
      RW_SID=1;
                  else     
               RW_SID=0;   
                  E_CLK=1;   
                  dat=dat<<1;   
                 }   
    } 

//写控制命令   
void     write_com(uchar  dat)   
    {
              send_byte(0xF8);//1111 1000     RW=0,RS=0  RW=0表示数据从MCU写到LCD RS=0表示数据是控制指令 
              send_byte(dat&0xF0);//高四位       RW=1表示数据从LCD写到MCD RS=1表示显示数据 
              send_byte((dat&0x0F)<<4);//低四位  
    }
//写显示数据或单字节字符   
void     write_data(uchar dat)   
    
 {
              send_byte(0xFA);//1111 1010     RW=0,RS=1   
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
   // delay_50us(10);
 }
}



 void init_time(void)
{
 TMOD=0X02;
 TH0=0x00;
 TL0=0x00;
 ET0=1;	//打开中断
 EA=1;	//启动总中断
 TR0=1;	//启动中断

 }

 
void     init_lcm(void)   
    {   
             delay_50us(10);  
             write_com(0x30);//功能设置，一次送8位数据，基本指令集   
             write_com(0x0C);//0000,1100       整体显示，游标off，游标位置off   
             write_com(0x01);//0000,0001     清DDRAM   
             write_com(0x02);//0000,0010     DDRAM地址归位   
             write_com(0x08);//1000,0000     设定DDRAM     7位地址000，0000到地址计数器AC   
    }


void irpros(void)//解码程序
{
  uchar k,i,j;
  uchar value;
  k=1;
  for(j=0;j<4;j++)//提取4个码 其中两个用户码 一个数据码 一个数据反码
  {
	   for(i=0;i<8;i++)	//每循环八次提取一个编码值
	   {
	          value=value>>1;//右移七次
	    if(irdata[k]>6)//判断8次
	    {
	      value=value|0x80;	//高位置一
	    }
	    k++;
  }
  ircode[j]=value;
 }
 irprosok=1;  // 编码完成标志
}


 
void init_int0(void)	//外部中断
{
   IT0=1;
   EX0=1;
   EA=1; 
}

separate_constant(uchar x,uchar count) //分离常数
{
//uchar count;
if(count<=9)
{
write_com(x);
write_data(0x30+count);
}
if(count>9)
{
 write_com(x);
write_data(0x40+count-9);

}
return 0;
}

void irwork(void)  //数码转换程序
{
 disp[0]=ircode[0]/16;// 存放用户码
 disp[1]=ircode[0]%16;// 存放用户码

 disp[2]=ircode[1]/16;// 存放用户码
 disp[3]=ircode[1]%16;// 存放用户码

 disp[4]=ircode[2]/16;// 存放数据原码
 disp[5]=ircode[2]%16;// 存放数据原码

 disp[6]=ircode[3]/16;// 存放数据反码
 disp[7]=ircode[3]%16;//存放数据反码

}
  



void display1(void)
{  separate_constant(0x86,disp[0]);
   separate_constant(0x87,disp[1]);
   separate_constant(0x96,disp[2]);
   separate_constant(0x97,disp[3]);
   separate_constant(0x8e,disp[4]);
   separate_constant(0x8f,disp[5]);
   separate_constant(0x9e,disp[6]);
   separate_constant(0x9f,disp[7]);
}








void  main(void)   
    {   
	      init_time();
		  init_int0();
          init_lcm();     //12864初始化程序
		  delay_50us(2);
s="客户码8 位";
display(1,1,s);  ///输出在第一行
s="客户码8 位";
display(2,1,s);  ///输出在第一行
s="原码  8 位";
display(3,1,s);  ///输出在第一行
s="反码  8 位";
display(4,1,s);  ///输出在第一行
          while(1) 

       {  
	   
	  if(irreceok==1)
  {
   irpros();
  irreceok=0;
  }
  if(irprosok)
  {
   irwork();
  irprosok=0;
  }
   display1();
   



}

   }

  
  void timer0() interrupt 1
{
   irtime++;//255 
}

 
void int0() interrupt 0
{
     if(startflag)
  {
     if(irtime>32)//检测引导码9毫秒时间
  {
    bitnum=0; 
  }
  irdata[bitnum]=irtime;
  irtime=0;
  bitnum++;
  if(bitnum==33)//接收数据完成
  {
   bitnum=0;	//清零下一次使用
   irreceok=1; //红外接收完毕标志位 

  } 
  }
  else
  {
   startflag=1;
  irtime=0;
  } 
} 
 




