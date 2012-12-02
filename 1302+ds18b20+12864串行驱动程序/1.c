/*****12M����stc89c52��Ƭ��*********************/


#include<reg51.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar second1,second2,minute1,minute2,hour1,hour2,day1,day2,month1,month2,week1,week2,year1,year2;
uchar time_data[7];//��ʾ������
uchar i;
uint temp,flag,ch;

sbit e_clk=P3^7;  //����ʱ���ź�
sbit rw_sid=P3^6; //����������

sbit dq=P1^1;//DS18B20�������� 
        
sbit sck=P0^0;
sbit io=P0^1;
sbit rst=P0^2;

sbit KEY1=P1^7;  
sbit KEY2=P1^6;  
/*ds1302��������*/
 
  void write_ds1302_byte(uchar dat)	//д���ֽ�
  {uchar i;
   for(i=0;i<8;i++)
   {sck=0;
    io=dat&0x01;
	dat>>=1;
	sck=1;
   }
  }


  void write_ds1302(uchar add,uchar dat)
  {rst=0;
   _nop_();
   sck=0;
   _nop_();
   rst=1;
   _nop_();
   write_ds1302_byte(add);
   write_ds1302_byte(dat);
   rst=0;
   _nop_();
   io=1;
   sck=1;
   }
  
  uchar read_ds1302(uchar add)
  
  {uchar i,value;
    rst=0;
    _nop_();
	sck=0;
	_nop_();
	rst=1;
	_nop_();
	write_ds1302_byte(add);
	for(i=0;i<8;i++)
	{
	value=value>>1;
	sck=0;
	if(io)
	value=value|0x80;
	sck=1;
	}
	rst=0;
	_nop_();
	sck=0;
	_nop_();	   
	sck=1;
	io=1;
	return value;
 }
 	

 void set_rtc(uchar add,value)//�趨ʱ�Ӻ���ע�� BCD���ת��
{ 
  uchar temp,temp1,temp2;
  write_ds1302(0x8e,0x00);//ȥ��д����
  temp1=value/10; //����ʮλ
  temp2=value%10;//�����λ
  temp=temp2+temp1*16; //ת�����BCD��
  write_ds1302(add,temp);
  write_ds1302(0x8e,0x80); //����д����
}	

void read_rtc(void)//��ȡʱ�Ӻ���
{
time_data[0]=read_ds1302(0x8d);	//�����
time_data[1]=read_ds1302(0x8b);	//������
time_data[2]=read_ds1302(0x89);	//����
time_data[3]=read_ds1302(0x87);	//����
time_data[4]=read_ds1302(0x85);	//��ʱ
time_data[5]=read_ds1302(0x83); //����
time_data[6]=read_ds1302(0x81);	//����
}



void time_proce() //���ݴ�����������ת��Ϊ��Ӧ��10������
{   second1=time_data[6]/16; //ת��Ϊ10��������λ
    second2=time_data[6]%16	;//ת��Ϊ10��������λ
    minute1=time_data[5]/16;
    minute2=time_data[5]%16	;
    hour1=time_data[4]/16 ;
    hour2=time_data[4]%16 ;
    day1=time_data[3]/16;
    day2=time_data[3]%16;
	month1=time_data[2]/16;
    month2=time_data[2]%16;
	week1=time_data[1]/16;
    week2=time_data[1]%16;
	year1=time_data[0]/16;
    year2=time_data[0]%16;
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

 /*****ds18b20����*******/

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
 
 void disp_time() //��ʾ
   {
    
    write_com(0x95);
    write_data(second1+0x30);
	write_data(second2+0x30);
    write_com(0x93);
    write_data(minute1+0x30);
	write_data(minute2+0x30);
	write_com(0x91);
    write_data(hour1+0x30);
	write_data(hour2+0x30);

    write_com(0x86);
    write_data(day1+0x30);
	write_data(day2+0x30);
	write_com(0x84);
    write_data(month1+0x30);
	write_data(month2+0x30);
	write_com(0x82);
    write_data(year1+0x30);
	write_data(year2+0x30);
	
	write_com(0x8d);
    write_data(week2+0x30);
	}

 void disp_temp()//��ʾ�¶Ⱥ���
 {

write_com(0x9b);
write_data(temp/100+0x30);
write_data(temp%100/10+0x30); 
write_data(temp%100%10+0x30);
write_data('.');		  
write_data((ch*0.6)+0x30); 
}

void set_time()//�趨ʱ�����
{

  set_rtc(0X80,30);	//�趨��
  set_rtc(0X82,16);	//�趨��
  set_rtc(0X84,22);	//�趨Сʱ
  set_rtc(0X86,31);	//�趨����
  set_rtc(0X88,11);	//�趨�·�
  set_rtc(0X8A,1);	//�趨����
  set_rtc(0X8C,11);	 //�趨���
  	
}



void init_int0()  // �ж�0��ʼ��
{
 IT0=1;//����INT0Ϊ�½��ش���
 EX0=1;	 //�ֲ����жϣ�������INT0�ж�
 IT1=1;//����INT0Ϊ�½��ش���
 EX1=1;	 //�ֲ����жϣ�������INT0�ж�
 EA=1;	   // ȫ���ж�
}

	
 main(void)   
    { 
	
	init_12864();
	init_int0();  // �ж�0��ʼ��
  	set_time();
	display(1,2,"20  ��  ��  ��");
    display(2,3,"ʱ  ��  ��");
    display(3,3,"����");
    display(4,1,"�¶�        ��");
  
  	write_data(0X86);
	delay_50us(10);
	write_com(0X0F);  
	
	while(1)
	{
	
	temp=readtemp();
    read_rtc();
    time_proce();
  	disp_time();
 	disp_temp();
	 
	 }
	
	}


	void serv_int0() interrupt  0  //�ⲿ�ж�0 ʵ�ֵ���PWM�Ĵ�С
{
 if(INT0==0)
  {
 delay_50us(100);
   
KEY1=~KEY1;	

  }

}
void serv_int1() interrupt  2  //�ⲿ�ж�2 ʵ�ֵ���PWM�Ĵ�С
{
 if(INT1==0)
  {
  delay_50us(100);  
   KEY2=~KEY2;
}

}



