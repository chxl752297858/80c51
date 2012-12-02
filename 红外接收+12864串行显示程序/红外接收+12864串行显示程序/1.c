#include<regx51.h>   
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar irtime;//hong wai shi jian
uchar startflag;//qi dong jie shou
uchar irdata[33];//chun shu ju
uchar bitnum;//�ڼ�λ
uchar irreceok;//hong wai jie shou wan l
uchar ircode[4];// ma
uchar irprosok;
uchar disp[8];




uchar *s;

uchar irtime;//����ʱ��

sbit E_CLK=P3^7;  //����ʱ���ź�
sbit RW_SID=P3^6; //
         
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
                  E_CLK=0;   
                  if(dat&0x80)
      RW_SID=1;
                  else     
               RW_SID=0;   
                  E_CLK=1;   
                  dat=dat<<1;   
                 }   
    } 

//д��������   
void     write_com(uchar  dat)   
    {
              send_byte(0xF8);//1111 1000     RW=0,RS=0  RW=0��ʾ���ݴ�MCUд��LCD RS=0��ʾ�����ǿ���ָ�� 
              send_byte(dat&0xF0);//����λ       RW=1��ʾ���ݴ�LCDд��MCD RS=1��ʾ��ʾ���� 
              send_byte((dat&0x0F)<<4);//����λ  
    }
//д��ʾ���ݻ��ֽ��ַ�   
void     write_data(uchar dat)   
    
 {
              send_byte(0xFA);//1111 1010     RW=0,RS=1   
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
   // delay_50us(10);
 }
}



 void init_time(void)
{
 TMOD=0X02;
 TH0=0x00;
 TL0=0x00;
 ET0=1;	//���ж�
 EA=1;	//�������ж�
 TR0=1;	//�����ж�

 }

 
void     init_lcm(void)   
    {   
             delay_50us(10);  
             write_com(0x30);//�������ã�һ����8λ���ݣ�����ָ�   
             write_com(0x0C);//0000,1100       ������ʾ���α�off���α�λ��off   
             write_com(0x01);//0000,0001     ��DDRAM   
             write_com(0x02);//0000,0010     DDRAM��ַ��λ   
             write_com(0x08);//1000,0000     �趨DDRAM     7λ��ַ000��0000����ַ������AC   
    }


void irpros(void)//�������
{
  uchar k,i,j;
  uchar value;
  k=1;
  for(j=0;j<4;j++)//��ȡ4���� ���������û��� һ�������� һ�����ݷ���
  {
	   for(i=0;i<8;i++)	//ÿѭ���˴���ȡһ������ֵ
	   {
	          value=value>>1;//�����ߴ�
	    if(irdata[k]>6)//�ж�8��
	    {
	      value=value|0x80;	//��λ��һ
	    }
	    k++;
  }
  ircode[j]=value;
 }
 irprosok=1;  // ������ɱ�־
}


 
void init_int0(void)	//�ⲿ�ж�
{
   IT0=1;
   EX0=1;
   EA=1; 
}

separate_constant(uchar x,uchar count) //���볣��
{
//uchar count;
if(count<=9)
{
write_com(x);
write_data(0x30+count);
}
if(count>9)
{
 write_com(x);
write_data(0x40+count-9);

}
return 0;
}

void irwork(void)  //����ת������
{
 disp[0]=ircode[0]/16;// ����û���
 disp[1]=ircode[0]%16;// ����û���

 disp[2]=ircode[1]/16;// ����û���
 disp[3]=ircode[1]%16;// ����û���

 disp[4]=ircode[2]/16;// �������ԭ��
 disp[5]=ircode[2]%16;// �������ԭ��

 disp[6]=ircode[3]/16;// ������ݷ���
 disp[7]=ircode[3]%16;//������ݷ���

}
  



void display1(void)
{  separate_constant(0x86,disp[0]);
   separate_constant(0x87,disp[1]);
   separate_constant(0x96,disp[2]);
   separate_constant(0x97,disp[3]);
   separate_constant(0x8e,disp[4]);
   separate_constant(0x8f,disp[5]);
   separate_constant(0x9e,disp[6]);
   separate_constant(0x9f,disp[7]);
}








void  main(void)   
    {   
	      init_time();
		  init_int0();
          init_lcm();     //12864��ʼ������
		  delay_50us(2);
s="�ͻ���8 λ";
display(1,1,s);  ///����ڵ�һ��
s="�ͻ���8 λ";
display(2,1,s);  ///����ڵ�һ��
s="ԭ��  8 λ";
display(3,1,s);  ///����ڵ�һ��
s="����  8 λ";
display(4,1,s);  ///����ڵ�һ��
          while(1) 

       {  
	   
	  if(irreceok==1)
  {
   irpros();
  irreceok=0;
  }
  if(irprosok)
  {
   irwork();
  irprosok=0;
  }
   display1();
   



}

   }

  
  void timer0() interrupt 1
{
   irtime++;//255 
}

 
void int0() interrupt 0
{
     if(startflag)
  {
     if(irtime>32)//���������9����ʱ��
  {
    bitnum=0; 
  }
  irdata[bitnum]=irtime;
  irtime=0;
  bitnum++;
  if(bitnum==33)//�����������
  {
   bitnum=0;	//������һ��ʹ��
   irreceok=1; //���������ϱ�־λ 

  } 
  }
  else
  {
   startflag=1;
  irtime=0;
  } 
} 
 




