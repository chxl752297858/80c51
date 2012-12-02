/********12M����stc89c52��Ƭ��DS18B20����****************/
  #include<reg52.h>
  #include<intrins.h>
  #define uchar unsigned char
  #define uint  unsigned char
 
  sbit e_clk=P0^1;  //����ʱ���ź�
  sbit rw_sid=P0^0; //����������
         

  sbit dq=P0^7;//DS18B20��������
  uint temp,flag,ch;
  
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


  //******************************************
  //��ʱ����
  //*****************************************
 
  void delay(unsigned int us)
   {
   while(us--);
   }
   //******************************************
   //ds18b20��λ����
   //******************************************
   void reset(void)
   {
    uchar x;//
    dq=1;//
    delay(8);//��ʱ
	dq=0;
	delay(80); //���ͳ���480~960us
	dq=1; //
	delay(20);//���߳���60~120us
	x=dq;
	delay(20);
    }
 //******************************************
 //��ds18b20�ж�һ���ֽ�
 //******************************************
 uchar readbyte(void)
 {uchar i,value;
  
  for(i=0;i<8;i++)
  {dq=0;
   value>>=1;
   dq=1;
   if(dq)
   value|=0x80;
   delay(4);//��ʱ�õ�Ƭ������
   }
  return(value);
  }
    //****************************************
 // ��ds18b20��дһ���ֽ�
 //***************************************
 void writebyte(uchar dat)
 {
  uchar i=0;
  for(i=0;i<8;i++)
  {dq=0;
   dq=dat&0x01;	//�ӵ�λ��ʼд
   delay(5); //��ʱ60~120US
   dq=1;   //	����
   dat>>=1; //����1λ
   }
   delay(4);
  }
 
   
   //********************************************
  //  ��DS18B20�ж�ȡʵʱ�¶�ֵ
     //********************************************
  uchar readtemp(void)
  {
  
  uchar a,b;
 { 
  reset(); //��ʼ��18b20
  writebyte(0xcc);//����ROM
  writebyte(0x44);//�����¶Ȳ���
  delay(50);

  reset(); //��ʼ��18b20
  writebyte(0xcc);//����ROM
  writebyte(0xbe);//��9���Ĵ�����ǰ�����¶�
  
  a=readbyte(); //��λ
  b=readbyte(); //��λ
  if(b>0x0f) //��ʾ���¶�
  {
  a=~a+1;
  if(a==0)
  b=~b+1;
  else 
  b=~b;
  flag=0xff; //��ʾ����λ-
  }
  else 	//��ʾ���¶�
  flag=0x00; //��ʾ����λ+
  ch=a&0x0f; //С����λ
  a>>=4;
  b<<=4;
  b=b|a;   // ����λ
  
  }
  
   return(b);
   }
 //*******************************
 //���붯̬ɨ�躯��
 //******************************
 
 
//**************************************
//������
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
 

