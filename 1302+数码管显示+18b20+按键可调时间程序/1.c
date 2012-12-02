/*****12M����stc89c52��Ƭ��*********************/


#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define LED_CON P2	//λѡ�˿�

uchar second1,second2,minute1,minute2,hour1,hour2,day1,day2,month1,month2,week1,week2,year1,year2;
uchar time_data[7];//��ʾ������
uchar  table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90};   //0��9����
uchar table_dp[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10}; //������0��9��������
//uchar  bufer[]={0x7f,0xbf,0xdf,0xef};	//�����λѡ
 uchar code table2[]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x06,0x07,0x08,0x08,0x09,0x09};//С�����
uchar second,minute,hour,month=1,week=1,day=1,year;  //�������ڲ���
uint temp,flag,ch; //��ʾ�¶Ȳ���
uchar time_flag; //����ʱ���־λ
bit bell_flag;//���㱨ʱ��־λ


     
sbit sck=P1^0;//1302ʱ����
sbit io=P1^1; //1302�ź���
sbit rst=P1^2;//1302��λ��

sbit dq=P1^3;//DS18B20��������  
  
sbit bell1 = P1^4; //���忪��ָʾ�� 
 

sbit bell = P2^0;  //���㱨ʱ



sbit KEY20 = P3^2;
sbit KEY21 = P3^3;
sbit KEY22 = P3^4;
sbit KEY23 = P3^5;

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


void disp_time()//������ʾʱ�亯��
{
    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���,��4λ����
	LED_CON|=(0xef&0xf0);//

    P0=table[hour1];
    delay(2);

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
	LED_CON|=(0xdf&0xf0);//
	if(second2%2==0)
    P0=table[hour2]; 
    else
	P0=table_dp[hour2];
	delay(2);

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
	LED_CON|=(0xbf&0xf0);//
    P0=table[minute1];
    delay(4);   

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
   	LED_CON|=(0x7f&0xf0);//
    P0=table[minute2];
    delay(4);

 }

 //���㱨ʱ����
 void bell_clock()
 {
 
if((second1==0)&&((second2<6)&&(second2>=0))&&(minute1==0)&&(minute2==0)&&(bell_flag==0)) //���㱨ʱ
bell=0;
else
bell=1;

 
 }

void disp_set_time()//������ʾ���庯��
{
    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���,��4λ����
	LED_CON|=(0xef&0xf0);//

    P0=table[hour/10];
    delay(2);

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
	LED_CON|=(0xdf&0xf0);//
//	if(second2%2==0)
   P0=table[hour%10]; 
//   else
//	P0=table_dp[hour%10];
	delay(2);

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
	LED_CON|=(0xbf&0xf0);//
    P0=table[minute/10];
    delay(4);   

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
   	LED_CON|=(0x7f&0xf0);//
    P0=table_dp[minute%10];
    delay(4);

 }

/*************18B20��������************************/
 //******************************************
  //��ʱ����
  //*****************************************
 
  void delay1(unsigned int us)
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
    delay1(8);//��ʱ
	dq=0;
	delay1(80); //���ͳ���480~960us
	dq=1; //
	delay1(20);//���߳���60~120us
	x=dq;
	delay1(20);
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
   delay1(4);//��ʱ�õ�Ƭ������
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
   delay1(5); //��ʱ60~120US
   dq=1;   //	����
   dat>>=1; //����1λ
   }
   delay1(4);
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
  delay1(50);

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






void disp_temp()//������ʾ���庯��
{
    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���,��4λ����
	LED_CON|=(0xef&0xf0);//

    P0=table[temp/100];
    delay(2);

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
	LED_CON|=(0xdf&0xf0);//

    P0=table[temp%100/10];  //��ʾ�¶�ʮλ; 
	delay(2);

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
	LED_CON|=(0xbf&0xf0);//
    P0=table_dp[temp%100%10];
    delay(4);   

    LED_CON&=0X0f;	 //���ֵ�4λ���ݲ���
   	LED_CON|=(0x7f&0xf0);//
    P0=table[table2[ch]]; //��ʾС��λ
    delay(4);

 }





 //*******************************
 //���붯̬ɨ�躯��
 //******************************




void set_time()
  
{ 
 
 bit key20_flag,key21_flag,key22_flag,key23_flag; 

 if((KEY20==0)&&(key20_flag==0))
{
delay(10);
if((KEY20==0)&&(key20_flag==0))
 {
time_flag++;
if(time_flag==3)
time_flag=0;

key20_flag=1;
}
}
if(KEY20==1)
{
key20_flag=0;

}

 if((KEY21==0)&&(key21_flag==0))
{
delay(10);
if((KEY21==0)&&(key21_flag==0))
 {
 if(time_flag==2)
 {
hour++;;
if(hour==24)
hour=0;
set_rtc(0X84,hour);
key21_flag=1;
}
}
}
if(KEY21==1)
{
key21_flag=0;
}

if((KEY22==0)&&(key22_flag==0))
{
delay(10);
if((KEY22==0)&&(key22_flag==0))
 {
 if(time_flag==2)
 {
  minute++;;
 if(minute==60)
 minute=0;
set_rtc(0X82,minute);

  set_rtc(0X80,0);	//�趨��
 // set_rtc(0X82,16);	//�趨��
 // set_rtc(0X84,22);	//�趨Сʱ
  set_rtc(0X86,1);	//�趨����
  set_rtc(0X88,11);	//�趨�·�
  set_rtc(0X8A,1);	//�趨����
  set_rtc(0X8C,11);	 //�趨���

key22_flag=1;
}
}
}
if(KEY22==1)
{
key22_flag=0;
}

if((KEY23==0)&&(key23_flag==0))
{
delay(10);
if((KEY23==0)&&(key23_flag==0))
 {
 
bell_flag=~bell_flag;
bell1=bell_flag;
key23_flag=1;
}
}
if(KEY23==1)
{
key23_flag=0;
}



}

void main(void)   
    { 
	bell1=1;
	bell_flag=1;
  //  set_rtc(0X80,50);
	while(1)
	{
	 time_proce(); //1302ʱ�䴦����
	 read_rtc();	   //ʵʱʱ�Ӻ���
     temp=readtemp();   //��ȡ�¶ȳ���
	 set_time();	 //�趨ʱ�����
	 bell_clock(); //�趨���忪�س���

  
	if(time_flag==0)
	{
    disp_time();//������ʾʱ�亯��
     }
   if(time_flag==1)
	{
		disp_temp();//��ʾ�¶ȳ���
	}
    if(time_flag==2)
	{
	disp_set_time(); //�趨�趨ʱ�����
	}
    
    
	  
	}
	}

