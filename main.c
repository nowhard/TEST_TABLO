
#include<avr/interrupt.h>
#include "timer1.h"
#include "timer0.h"
#include "uart.h"

#include <ctype.h>
#include <stdint.h>
//#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include <util/delay.h>
#include "led_process.h"
//#include <string.h>
#include "adc.h"
 #include <avr/wdt.h> 
#include "pt/pt.h"

#include "protocol.h"

volatile struct pt pt1, pt2,pt3;
extern volatile unsigned int adc_result_1,adc_result_2;
extern unsigned char  brightness;//€ркость табло

float led_out_fl[10]={0.0000,11.111,222.22,3333.3,44444,5.5555,66.666,777.77,8888.8,99999};
//-----------------------------------------------
void Port_Init(void);


//FILE uart_str = FDEV_SETUP_STREAM(&uart_putchar, &uart_getchar, _FDEV_SETUP_RW);
PT_THREAD(Display_Out_Process(struct pt *pt));
PT_THREAD(Proto_Process(struct pt *pt));
//-----------------------------------------------
int main(void)
{

//stdout = stdin = &uart_str;
UART_Init();
Timer0_Init();
Port_Init();
INIT_ADC();
Timer1_Init();
LED_SetPoint(INDICATOR_1,2);
wdt_enable(WDTO_1S);
	PT_INIT(&pt1);
    PT_INIT(&pt2);
	PT_INIT(&pt3);
sei();
	while(1)
	{
		Display_Out_Process(&pt1);
		Proto_Process(&pt2);
		LED_Process(&pt3);
		wdt_reset();
	}
}
//-----------------------------------------------
void Port_Init(void)
{
	DDRB=0xFF;
	DDRD=0x2;//RX-in TX-out
	DDRD|=0x10;
	PORTD|=0x14;

	DDRC=0x7;
	PORTC=0x0;
}
//-----------------------------------------------
ISR(TIMER0_OVF_vect) //обработчик прерывани€ таймера0 
{
	pt1.pt_time++;
	pt2.pt_time++;
	pt3.pt_time++;

	PORTB^=0x40;

	TCNT0=130;
	TIFR0&=!(1<<TOV0);
	
	return;
}
//-----------------------------------------------------------------------------
PT_THREAD(Display_Out_Process(struct pt *pt))
 {
   static signed char bar=0, direct=0, color=0, dec_counter=0;
   PT_BEGIN(pt);

  while(1) {
	PT_DELAY(pt,100+((adc_result_2+2)>>6)*100);
	LED_Set_Brightness(INDICATOR_1,(adc_result_1)>>6);
	LED_Set_Brightness(INDICATOR_2,(adc_result_1)>>6);
   	
	
	if(dec_counter<5)
	{
		LED_SetPoint(INDICATOR_1,5-dec_counter);	
	}
	else
	{
		LED_SetPoint(INDICATOR_1,10-dec_counter);
	}
	LED_Out_Float(INDICATOR_1,led_out_fl[dec_counter]);
	dec_counter++;
	if(dec_counter>=10)
	{
		dec_counter=0;
	}

	PT_DELAY(pt,50);
//----------------bar-----------------
	if(direct)
	{
		bar++;
		if(bar>32)
		{
			bar=32;
			direct=0;
		}
	}
	else
	{
		bar--;
		if(bar<0)
		{	
			bar=0;
			color^=1;
			direct=1;
		}
	}
//------------------------------------	
	if(color)
	{
		SetBarGraph(bar,33,33,0);
	}
	else
	{
		SetBarGraph(bar,0,0,0);
	}
   }
   PT_END(pt);
 }
//-----------------------------------------------------------------------------
PT_THREAD(Proto_Process(struct pt *pt))
 {
   PT_BEGIN(pt);

  while(1) {
	 PT_DELAY(pt,100+((adc_result_2+2)>>6)*100);
	 brightness=(adc_result_1)>>6;
	Protocol_Handler();	
   }
   PT_END(pt);
 }
