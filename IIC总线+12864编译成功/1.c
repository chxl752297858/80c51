#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit sda=P2^0;
sbit scl=P2^1;

sbit e_clk=P3^7;  //����ʱ���ź�
sbit rw_sid=P3^6; //����������


uchar a,k;
void delay()
{_nop_(); 
 _nop_(); 
 _nop_(); 
}
void start()  //��ʼ�ź�
{	
	sda=1;
	delay();
	scl=1;
	delay();
	sda=0;
	delay();
}

void stop()   //ֹͣ
{
	sda=0;
	delay();
	scl=1;
	delay();
	sda=1;
	delay();
}

void respons()  //Ӧ��
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

void write_add(uchar address,uchar date)//д����
{
	start();  //��ʼ�ź�
	write_byte(0xa0);
	respons();//Ӧ���ź�
	write_byte(address); //д��ַ
	respons(); //Ӧ���ź�
	write_byte(date); //д����
	respons();//Ӧ���ź�
	stop();	//ͣ�ź�
}


uchar read_add(uchar address) //����ַ
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



/********����12864��ʾ��������***********/
void  delay_50us(uint j)   //��ʱ50uS  
    {   
        uint  i; 
        for(; j>0; j--); 
        for(i=19; i>0; i--);  
    }

//���з���һ�ֽ�����   
void send_byte(uchar  dat)   
    {   
                 uchar i;   
                 for(i=0;i<8;i++)   
                {   
                  e_clk=0;   
                  if(dat&0x80)//��ȡ���λ��ֵ�����1��rw_sidΪ1������rw_sidΪ0��
                  rw_sid=1;
                  else     
                  rw_sid=0; 
				  e_clk=1;   
                  dat=dat<<1; //����һλ  
                 }   
    } 

//д��������������ֽ�д  
void     write_com(uchar  dat)   
    {
              send_byte(0xF8);//���ڿ��Ƹ�ʽ 1111 1000     RW=0,RS=0  RW=0��ʾ���ݴ�MCUд��LCD RS=0��ʾ�����ǿ���ָ�� 
              send_byte(dat&0xF0);//����8λ���ݸ�4λ��ʽ 1111 0000  ����λ��0  RW=1��ʾ���ݴ�LCDд��MCD RS=1��ʾ��ʾ���� 
              send_byte((dat&0x0F)<<4);//����8λ���ݵ�4λ��ʽ ����λ��0 0000 1111  ����4λ��λ1111 0000 
			  
    }


//д��ʾ���ݻ��ֽ��ַ�   
void     write_data(uchar dat)   
    
    {         send_byte(0xFA);//1111 1010     RW=0,RS=1   
              send_byte(dat&0xF0);//����λ   
              send_byte((dat&0x0F)<<4);//����λ  
    }

   /*
void display(uchar n,uchar m,uchar *s) //д�ַ�����������
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

 //��ʼ��     LCM   
void     init_12864(void)   
    {   
             delay_50us(100); //��ʼ���ȴ�ʱ��40ms  
             write_com(0x30);//�������ã�һ����8λ���ݣ�����ָ�   
             delay_50us(3);	//�ȴ��Ǽ�Ҫ����100��us
			 write_com(0x30);//�������ã�һ����8λ���ݣ�����ָ� 
			 delay_50us(2);	//�ȴ��Ǽ�Ҫ����37��us 
			 write_com(0x0C);//0000,1100       ������ʾ���α�off���α�λ��off 
			 delay_50us(3);	//�ȴ��Ǽ�Ҫ����37��us  
             write_com(0x01);//0000,0001     ����   
             delay_50us(200);	//�ȴ�ʱ�����10ms
			 write_com(0x06);//0000,0010     DDRAM��ַ��λ   
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


