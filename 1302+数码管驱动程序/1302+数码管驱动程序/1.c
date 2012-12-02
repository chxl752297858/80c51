/*****12M����stc89c52��Ƭ��*********************/


#include<stc12.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar second1,second2,minute1,minute2,hour1,hour2,day1,day2,month1,month2,week1,week2,year1,year2;
uchar time_data[7];//��ʾ������
uchar  table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};   //0��9����
uchar table_dp[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10}; //������0��9��������
uchar  bufer[]={0x7f,0xbf,0xdf,0xef};
//uchar i;

         
sbit sck=P1^0;
sbit io=P1^1;
sbit rst=P1^2;

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

/*******�������ʾ����************/

void delay(uint z)    //��ʱ
{
    uint x,y;
    for(x=0;x<z;x++)
        for(y=0;y<110;y++);
}


void disp_time()//������ʾ����
{
    P2=bufer[0];
    P0=table[hour1];
    delay(5);

    P2=bufer[1];
	if(second2%2==0)
    P0=table[hour2]; 
    else
	P0=table_dp[hour2];
	delay(5);

    P2=bufer[2];
    P0=table[minute1];
    delay(5);   

    P2=bufer[3];
    P0=table[minute2];
    delay(5);

 }



 main(void)   
    { 


  set_rtc(0X80,30);	//�趨��
  set_rtc(0X82,8);	//�趨��
  set_rtc(0X84,13);	//�趨Сʱ
  set_rtc(0X86,9);	//�趨����
  set_rtc(0X88,11);	//�趨�·�
  set_rtc(0X8A,3);	//�趨����
  set_rtc(0X8C,11);	 //�趨���
  	
	 
	
	while(1)
	{ 
    read_rtc();
    time_proce();
	disp_time();//������ʾ����
 
	 
	 }
	
	}