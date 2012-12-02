/********12M����stc89c52��Ƭ��DS18B20����****************/
  #include<reg52.h>
  #include<intrins.h>
  #define uchar unsigned char
  #define uint  unsigned char
  uchar code table[]={0Xc0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90}; //0��9�������ʾ  
  uchar code table1[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10}; //0��9��С�����������ʾ  
  uchar code table2[]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x06,0x07,0x08,0x08,0x09,0x09};//С�����
 
  sbit dq=P1^1;//DS18B20��������
  uint temp,flag,ch;
  
  
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
 void scandisp()
                   
{	P2=0x7f; //��ʾ�¶Ȱ�λ
    P0=table[temp/100];
    delay(200);
   
    P2=0xbf;
    P0=table[temp%100/10];  //��ʾ�¶�ʮλ
    delay(200);

    P2=0xdf;    //��ʾ�¶ȸ�λ
    P0=table1[temp%100%10];
    delay(200);

    

    P2=0xef;
    P0=table[table2[ch]]; //��ʾС��λ
    delay(200);


     
}
//**************************************
//������
//***************************************

void main()
{
 
 

temp=readtemp();
  scandisp();
 

 

}
 

