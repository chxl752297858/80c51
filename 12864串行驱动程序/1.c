/*****12M����stc89c52��Ƭ��*********************/


#include<stc12.h>
#define uint unsigned int
#define uchar unsigned char
uchar table[]={"i love you my love"};
uchar i;

sbit e_clk=P3^7;  //����ʱ���ź�
sbit rw_sid=P3^6; //����������
         
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

 

 main(void)   
    { 
	  init_12864();
	while(1)
	{ 
	write_com(0x80);
	delay_50us(2);
	for(i=0;i<16;i++)
	{
	write_data(table[i]);
	}
	//write_data(0x31);
	 }
	
	}