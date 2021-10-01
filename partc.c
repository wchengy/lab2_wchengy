/*
 * TEST.c
 *
 * Created: 9/26/2021 23:42:41
 * Author : wcy_1969
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000UL

void Initialize()
{
	cli();
	DDRB &= ~(1<<DDB0);
	
	DDRB |= (1<<DDB1);
	
	TCCR1B &=~(1<<CS10); //Timer1 scale 1
	TCCR1B |=(1<<CS11);
	TCCR1B &=~(1<<CS12);
	
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	TCCR1B &= ~(1<<ICES1); //find falling edge
	
	TIFR1 |=(1<<ICF1);
	
	TIMSK1 |=(1<<ICIE1);
	sei();
	
}

ISR(TIMER1_CAPT_vect)
{
	TIFR1 |=(1<<ICF1);
	if (PINB &(1<<PINB0))
	{
		PORTB &= ~(1<<PORTB1);
	}
	else
	{
		PORTB |= (1<<PORTB1);
	}	
	TCCR1B ^= (1<<ICES1);
}

int main(void)
{
	
	Initialize();
    /* Replace with your application code */
    while (1); 
	
}

