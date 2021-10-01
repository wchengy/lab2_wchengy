/*
 * GccApplication4.c
 *
 * Created: 9/27/2021 22:13:00
 * Author : wcy_1969
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart.h"
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)


volatile int edge1=0;
volatile int edge2=0;
volatile int period=0;
volatile int spacing=0;
volatile int overflow=0;

char String[80];


void Initialize()
{
	cli();
	DDRB &= ~(1<<DDB0);// PB0 as input
	
	//set timer1 to be internal div by 1024
	//15625Hz timer clock
	TCCR1B |= (1<<CS10); 
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	//set timer one to normal
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	TCCR1B &= ~(1<<ICES1); //find falling edge
	
	TIFR1 |=(1<<ICF1);//clear interrupt flag
	TIMSK1 |=(1<<ICIE1);//enable input capture interrupts
	TIMSK1 |=(1<<TOIE1);
	sei();
}

ISR(TIMER1_OVF_vect)
{
	overflow++;
	TIMSK1 |=(1<<TOIE1);
}

ISR(TIMER1_CAPT_vect)
{
	TIFR1 |=(1<<ICF1);
	if (PINB &(1<<PINB0))
	{
		edge2=ICR1+overflow*15625;
	}
	else
	{
		edge1=ICR1+overflow*15625;
	}
	if(edge1>edge2)
	{
		spacing=(edge1-edge2)/(F_CPU/1024/1000);
		if ((spacing>400))
		{
			sprintf(String,"space\n");
			UART_putstring(String);
		}
	}
	else
	{
		period=(edge2-edge1)/(F_CPU/1024/1000);
		if ((period>30)&(period<200))
		{
			sprintf(String,"Dot\n");
			UART_putstring(String);
		
		}
		else if ((period>200)&(period<400))
		{
			sprintf(String,"Dash\n");
			UART_putstring(String);
		}
	}
	TCCR1B ^= (1<<ICES1);
}
int main(void)
{
	UART_init(BAUD_PRESCALER);
	Initialize();
	while (1); 
}

