#include<LPC17xx.h>
#include <stdio.h>
unsigned int i,j, temp;
float final;
char temper[20];
unsigned long LED =0x00000010;
#define rsctrl 0x00000100 //po.8
#define enctrl 0x00000200 //po.9 enable lcd
#define dtctrl 0x000000f0 //p0.4-7 4 bit mode
#define VREF       3.3
void lcd_init();
void wr_cn();
void clr_disp();
void delay();
void lcd_com();
void wr_dn();
void lcd_data();
void clear_ports();
void lcd_puts(unsigned char *);
unsigned int i,temp1=0,temp2=0;
int main(void)
{
SystemInit() ;
SystemCoreClockUpdate();
LPC_PINCON->PINSEL1|=(1<<14);
while(1)
{
LPC_ADC->ADCR=(1<<0) | (1<<21) | (1<<24);
while(((temp=LPC_ADC->ADDR0) & (1<<31))==0);
temp=LPC_ADC->ADDR0;
temp&=0xFFF0;
temp>>=4;
final=((float)temp * VREF * 100)/4096;
sprintf(temper,"%3.2fC",final);
lcd_init();
temp1=0x80;
lcd_com();
delay(10000);
lcd_puts(&temper[0]);
}
}
void lcd_init(){
LPC_PINCON->PINSEL0=0;
LPC_GPIO0->FIODIR|=dtctrl;
LPC_GPIO0->FIODIR|=rsctrl;
LPC_GPIO0->FIODIR|=enctrl;
clear_ports();
delay(3200);
for(i=0;i<3;i++)
{
temp2=(0x30);
wr_cn();
delay(30000);
}
temp2=(0x20);
wr_cn(); delay(30000);
temp1=0x28; lcd_com();
delay(30000); temp1=0x0c;
lcd_com(); delay(800);
temp1=0x06; lcd_com();
delay(800); temp1=0x01;
lcd_com(); delay(10000);
temp1=0x80; lcd_com();
delay(800);
}
void lcd_com()
{ temp2=temp1&0xf0;
temp2=temp2;
wr_cn();
temp2=temp1&0x0f;
temp2=temp2<<4;
wr_cn(); delay(1000);
}
void wr_cn()
{
clear_ports();
LPC_GPIO0->FIOPIN=temp2;
LPC_GPIO0->FIOCLR=rsctrl;
LPC_GPIO0->FIOSET=enctrl;
delay(25);
LPC_GPIO0->FIOCLR=enctrl;
}
void lcd_data()
{
temp2=temp1&0xf0;
temp2=temp2;
wr_dn();
temp2=temp1&0x0f;
temp2=temp2<<4;
wr_dn();
delay(1000);
}
void wr_dn()
{
clear_ports();
LPC_GPIO0->FIOPIN=temp2;
LPC_GPIO0->FIOSET=rsctrl;
LPC_GPIO0->FIOSET=enctrl;
delay(25);
LPC_GPIO0->FIOCLR=enctrl;
}
void delay(unsigned int r1)
{
unsigned int r;
for(r=0;r<r1;r++);
}
void clr_disp()
{
temp1=0x01;
lcd_com();
delay(10000);
}
void clear_ports()
{
LPC_GPIO0->FIOCLR|=rsctrl;
LPC_GPIO0->FIOCLR|=enctrl;
LPC_GPIO0->FIOCLR|=dtctrl;
}
void lcd_puts(unsigned char *buff)
{
unsigned int i=0;
while(buff[i]!='\0')
{
temp1=buff[i]; i++; lcd_data();
}
}