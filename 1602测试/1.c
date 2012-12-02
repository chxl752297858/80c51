#include <AT89X52.H>
#include<INTRINS.H>
#define LCD_DATA_PORT  P1   //接液晶数据口

#define LCD_EN  P1_3   //接液晶使能端
#define LCD_RS  P1_2   //RS=1写数据，RS=0写命令


/**********************函数声明***************************/
void LCD_write_char (unsigned char Data,unsigned char command);//写命令或写数据函数
/*******************延时函数*************************/
//系统时钟：12MHZ
void delay_nus(unsigned int n)       //N us延时函数
  {
   unsigned int i=0 ;
   for (i=0;i<n;i++)
       _nop_();
  }
  
void delay_nms(unsigned int n)       //N ms延时函数
  {
   unsigned char  i=0;
   while(n--)
   {
   for (i=0;i<120;i++);
   }
  }

/********************液晶初始化函数************************/
void LCD_init(void)       
{
 delay_nms(5);
 LCD_write_char(0x28,0);  //4位显示
 LCD_write_char(0x0c,0);  //显示开
 LCD_write_char(0x06,0);
 LCD_write_char(0x01,0);  //清屏
}
/********************液晶使能函数************************/
void LCD_en_write(void)  
{ LCD_EN=0;
  delay_nus(2);
  LCD_EN=1;
  delay_nus(2);
  LCD_EN=0;
}

/*********************写命令或写数据函数*******************/
void LCD_write_data(unsigned char Data_command)
{ unsigned char temp;
  temp=Data_command;
  LCD_DATA_PORT&=0X0f; //LCD_DATA_PORT=LCD_DATA_PORT&0X0f+
  LCD_DATA_PORT|=temp&0xf0; //写高四位
  LCD_en_write();
  temp=temp<<4;
  LCD_DATA_PORT&=0x0f;
  LCD_DATA_PORT|=temp&0xf0;  //写低四位
  LCD_en_write();
}
/*********************写命令和写数据函数*******************/
//参数：command=1,写数据，command=0写命令
void LCD_write_char(unsigned char Data_or_command,unsigned char command) 
{
 if(command==0)
 {
 LCD_RS=0;   //RS=0，选择指令寄存器
  LCD_write_data(Data_or_command);  
 }
 else
 {
  LCD_RS=1;  //RS=1，选择数据寄存器
  LCD_write_data(Data_or_command);
  }
}
/**********************设置显示地址函数********************/
  void LCD_set_xy( unsigned char x, unsigned char y )
  {
    unsigned char address;
    if (y == 0) 
  address = 0x80 + x;
    else 
         address = 0xc0 + x;
    LCD_write_char( address, 0 );
  }
/*********************写字符串函数************************/
//参数：X为显示在第几列,Y为显示在第几行,s为待显示字符串
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
  { 
    LCD_set_xy( X, Y ); //写地址


    while (*s)  // 写显示字符
      {
        LCD_write_char(*s,1);
  s++;
      }
  }
/*******************主函数*************************/
void main(void)
{
     LCD_RS=1;
     LCD_EN=1;
     LCD_init();
     while(1)    //循环
       { 

         LCD_write_char(0x01,0);
          delay_nms(10);
         LCD_write_string(0,0,"I love you!");
         LCD_write_string(0,1,"Do you love me!");
         delay_nms(10000);  
          LCD_write_string(0,0,"You(^_^)You!");
          LCD_write_string(0,1,"2011.3.25Text OK!");
        delay_nms(10000); 
       }
}

 

