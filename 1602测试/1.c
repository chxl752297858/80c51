#include <AT89X52.H>
#include<INTRINS.H>
#define LCD_DATA_PORT  P1   //��Һ�����ݿ�

#define LCD_EN  P1_3   //��Һ��ʹ�ܶ�
#define LCD_RS  P1_2   //RS=1д���ݣ�RS=0д����


/**********************��������***************************/
void LCD_write_char (unsigned char Data,unsigned char command);//д�����д���ݺ���
/*******************��ʱ����*************************/
//ϵͳʱ�ӣ�12MHZ
void delay_nus(unsigned int n)       //N us��ʱ����
  {
   unsigned int i=0 ;
   for (i=0;i<n;i++)
       _nop_();
  }
  
void delay_nms(unsigned int n)       //N ms��ʱ����
  {
   unsigned char  i=0;
   while(n--)
   {
   for (i=0;i<120;i++);
   }
  }

/********************Һ����ʼ������************************/
void LCD_init(void)       
{
 delay_nms(5);
 LCD_write_char(0x28,0);  //4λ��ʾ
 LCD_write_char(0x0c,0);  //��ʾ��
 LCD_write_char(0x06,0);
 LCD_write_char(0x01,0);  //����
}
/********************Һ��ʹ�ܺ���************************/
void LCD_en_write(void)  
{ LCD_EN=0;
  delay_nus(2);
  LCD_EN=1;
  delay_nus(2);
  LCD_EN=0;
}

/*********************д�����д���ݺ���*******************/
void LCD_write_data(unsigned char Data_command)
{ unsigned char temp;
  temp=Data_command;
  LCD_DATA_PORT&=0X0f; //LCD_DATA_PORT=LCD_DATA_PORT&0X0f+
  LCD_DATA_PORT|=temp&0xf0; //д����λ
  LCD_en_write();
  temp=temp<<4;
  LCD_DATA_PORT&=0x0f;
  LCD_DATA_PORT|=temp&0xf0;  //д����λ
  LCD_en_write();
}
/*********************д�����д���ݺ���*******************/
//������command=1,д���ݣ�command=0д����
void LCD_write_char(unsigned char Data_or_command,unsigned char command) 
{
 if(command==0)
 {
 LCD_RS=0;   //RS=0��ѡ��ָ��Ĵ���
  LCD_write_data(Data_or_command);  
 }
 else
 {
  LCD_RS=1;  //RS=1��ѡ�����ݼĴ���
  LCD_write_data(Data_or_command);
  }
}
/**********************������ʾ��ַ����********************/
  void LCD_set_xy( unsigned char x, unsigned char y )
  {
    unsigned char address;
    if (y == 0) 
  address = 0x80 + x;
    else 
         address = 0xc0 + x;
    LCD_write_char( address, 0 );
  }
/*********************д�ַ�������************************/
//������XΪ��ʾ�ڵڼ���,YΪ��ʾ�ڵڼ���,sΪ����ʾ�ַ���
void LCD_write_string(unsigned char X,unsigned char Y,unsigned char *s)
  { 
    LCD_set_xy( X, Y ); //д��ַ


    while (*s)  // д��ʾ�ַ�
      {
        LCD_write_char(*s,1);
  s++;
      }
  }
/*******************������*************************/
void main(void)
{
     LCD_RS=1;
     LCD_EN=1;
     LCD_init();
     while(1)    //ѭ��
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

 

