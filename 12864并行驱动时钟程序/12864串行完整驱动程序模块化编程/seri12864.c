#include <reg52.h>
#include "seri12864.h"	

	
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
void     wri_12864(uchar value,uchar  dat)   
    {
              send_byte(value);//���ڿ��Ƹ�ʽ 1111 1000     RW=0,RS=0  RW=0��ʾ���ݴ�MCUд��LCD RS=0��ʾ�����ǿ���ָ�� 
              send_byte(dat&0xF0);//����8λ���ݸ�4λ��ʽ 1111 0000  ����λ��0  RW=1��ʾ���ݴ�LCDд��MCD RS=1��ʾ��ʾ���� 
              send_byte((dat&0x0F)<<4);//����8λ���ݵ�4λ��ʽ ����λ��0 0000 1111  ����4λ��λ1111 0000 
			  
    }

	 void dis_line(uchar n,uchar m,uchar *s) //n��ʾ��m��ʾ�� s ��ʾ�ַ��� д�ַ�����������
{
  
    switch(n)
 {
     case 1:wri_12864(comd, 0x80+m-1);break;
     case 2:wri_12864(comd,0x90+m-1);break;
     case 3:wri_12864(comd,0x88+m-1);break;
     case 4:wri_12864(comd,0x98+m-1);break;
  default:;
 }

  while(*s>0)
 {
    wri_12864(date,*s);
    s++;
    delay_50us(1000);
 }
}

void dis_page(uchar *s)  //д���һҳ
{uchar datecnt;

 for(datecnt=0;datecnt<64;datecnt++)
 {  
   
  if(datecnt==0) wri_12864(comd,0x80);
  if(datecnt==16)wri_12864(comd,0x90);
  if(datecnt==32)wri_12864(comd,0x88);
  if(datecnt==48)wri_12864(comd,0x98);
  
  
  
  wri_12864(date,*s);
  s++;
  delay_50us(2000);	
   while(*s=='\0')
   continue;

   }
 
   
 
}
 

 

 


   //��ʼ��     LCM   
 void     init_12864(void)   
    {   
             delay_50us(100); //��ʼ���ȴ�ʱ��40ms  
             wri_12864(comd,0x30);//�������ã�һ����8λ���ݣ�����ָ�   
             delay_50us(3);	//�ȴ��Ǽ�Ҫ����100��us
			 wri_12864(comd,0x30);//�������ã�һ����8λ���ݣ�����ָ� 
			 delay_50us(2);	//�ȴ��Ǽ�Ҫ����37��us 
			 wri_12864(comd,0x0c);//0000,1100       ������ʾ���α�off���α�λ��off 
			 delay_50us(3);	//�ȴ��Ǽ�Ҫ����37��us  
             wri_12864(comd,0x01);//0000,0001     ����   
             delay_50us(200);	//�ȴ�ʱ�����10ms
			 wri_12864(comd,0x06);//0000,0010     DDRAM��ַ��λ   
             delay_50us(10);
    }
