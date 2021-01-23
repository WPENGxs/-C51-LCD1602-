#include "reg51.h" 
#include "stdio.h"
#define uint unsigned int
#define uchar unsigned char
#define KEY P1
sbit RS=P2^6;
sbit RW=P2^5;
sbit E=P2^7;
void delay(uint n)//延时函数
{
  uint x,y;
	for(x=n;x>0;x--)
	   for(y=110;y>0;y--);
}
void wcomlcd(uchar com)//1602显示屏写命令
{
  RS=0;
	RW=0;
	P0=com;//命令送入P0
	delay(5);//延时用于接受数据
	E=1;
	E=0;
}
void wdatlcd(uchar dat)//1602显示屏写数据
{
  RS=1;
	RW=0;
	P0=dat;
	delay(5);//延时用于接受数据
	E=1;
	E=0;
}
void initlcd()//1602显示屏初始化
{
  wcomlcd(0x38);
	wcomlcd(0x0c);//开启显示屏，关光标，光标不闪烁
	wcomlcd(0x06);//显示地址递增，写第一个数据后显示位置会右移一位
	wcomlcd(0x01);//清屏
}
int keyscan()
{
  uchar i=0;
	uint keyvalue;
	KEY=0x0f;//00001111
	if(KEY!=0x0f)
	{
	  delay(100);//软件消抖
		if(KEY!=0x0f)
		{
		  KEY=0x0f;
			switch(KEY)//列查找
			{
			  case(0x07):keyvalue=1;break;
				case(0x0b):keyvalue=2;break;
				case(0x0d):keyvalue=3;break;
				case(0x0e):keyvalue=4;break;
			}
			KEY=0xf0;
			switch(KEY)//行查找
			{
			  case(0x70):keyvalue=keyvalue;break;
				case(0xb0):keyvalue=keyvalue+4;break;
				case(0xd0):keyvalue=keyvalue+8;break;
				case(0xe0):keyvalue=keyvalue+12;break;
			}
		}
	}
	while((i<50)&&(KEY!=0xf0))//检测按键松手
	{
	  delay(5);
		i++;
	}
	return keyvalue;
}
void main()
{
  uint k,ii,f,y1,y2,y3,ch,ad[6]={0};
	uint d=0,g=0,i=1,z=0,x2=0,r[4]={0},w[4]={0};
	uchar x1,adc[8];
	initlcd();//显示屏初始化
	wcomlcd(0x80);//显示地址设为80H(即00H) 上排第一位
	delay(5);
	while(1)
	{
		KEY=0x0f;
		if(KEY!=0x0f)
	  {
	    k=keyscan();
		  switch(k)
		  {
		  	case 1:x1='1';x2=1;break;
		  	case 2:x1='2';x2=2;break;
		  	case 3:x1='3';x2=3;break;
		  	case 4:x1='+';d=1;break;
		  	case 5:x1='4';x2=4;break;
		  	case 6:x1='5';x2=5;break;
		  	case 7:x1='6';x2=6;break;
		  	case 8:x1='-';d=2;break;
		  	case 9:x1='7';x2=7;break;
		  	case 10:x1='8';x2=8;break;
		  	case 11:x1='9';x2=9;break;
		  	case 12:x1='*';d=3;break;
		  	case 13:x1='C';z=1;break;
	  		case 14:x1='0';x2=0;break;
	  		case 15:x1='=';z=2;break;
	  		case 16:x1='/';d=4;break;
	  	}
	  	wdatlcd(x1);
			if(z==2)
			{
				if(i==3) ch=w[3],w[3]=w[2],w[2]=ch;
				if(i==4) ch=w[3],w[3]=w[1],w[1]=ch;
				if(i==5) ch=w[3],w[3]=w[0],w[0]=ch,ch=w[2],w[2]=w[1],w[1]=ch;
			y1=r[3]+r[2]*10+r[1]*100+r[0]*1000;
		  y2=w[3]+w[2]*10+w[1]*100+w[0]*1000;
			   switch(g)
				 {
				   case 1:y3=y1+y2;break;
					 case 2:y3=y1-y2;break;
					 case 3:y3=y1*y2;break;
					 case 4:y3=y1/y2;break;
				 }
				 ad[0]=y3/100000%10;
				 ad[1]=y3/10000%10;
				 ad[2]=y3/1000%10;
				 ad[3]=y3/100%10;
				 ad[4]=y3/10%10;
				 ad[5]=y3%10;
				 for(f=0;f<8;f++)
				 {
				    switch(ad[f])
						{
						  case 1:adc[f]='1';break;
							case 2:adc[f]='2';break;
							case 3:adc[f]='3';break;
							case 4:adc[f]='4';break;
							case 5:adc[f]='5';break;
							case 6:adc[f]='6';break;
							case 7:adc[f]='7';break;
							case 8:adc[f]='8';break;
							case 9:adc[f]='9';break;
							case 0:adc[f]='0';break;
							default:adc[f]='0';break;
						}
				 }
				wcomlcd(0x80+0x40);
				for(ii=0;ii<6;ii++)
				  wdatlcd(adc[ii]);
				 z=0;
			}
			if(d==0)
			{
			  r[4-i]=x2;
				i++;
			}
			if(d==5)
			{
			  w[4-i]=x2;
		  	i++;
		  }
			
			if(d==1||d==2||d==3||d==4) 
			{
				if(i==3) ch=r[3],r[3]=r[2],r[2]=ch;
				if(i==4) ch=r[3],r[3]=r[1],r[1]=ch;
				if(i==5) ch=r[3],r[3]=r[0],r[0]=ch,ch=r[2],r[2]=r[1],r[1]=ch;
				g=d,d=5,i=1;
			}
			if(z==1) 
      {
        initlcd();wcomlcd(0x80);z=0;
				for(ii=0;ii<8;ii++)
				  adc[ii]='0';
				for(ii=0;ii<7;ii++)
				  {r[ii]=0;w[ii]=0;}
					d=0,g=0,i=1,z=0,x2=0;
			}
	  }
	}
}
