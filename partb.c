/*
 * GccApplication7.c
 *
 * Created: 9/29/2021 09:11:06
 * Author : wcy_1969
 */ 

#include <avr/io.h>


int main(void)
{
    DDRB &= ~(1<<DDB0);
	DDRB |= (1<<DDB5);
    while (1) 
    {
		while(PINB & (1<<PINB0));
		PORTB |=(1<<PORTB5);
		while(!(PINB &(1<<PINB0)));
		PORTB &=~(1<<PORTB5);
    }
}

