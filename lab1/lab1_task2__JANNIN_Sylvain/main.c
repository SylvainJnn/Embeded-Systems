/*
 * lab1_task2__JANNIN_Sylvain.c
 *
 * Created: 30/09/2021 09:00:37
 * Author : sylva
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

void my_delay()
{
	int n = 45313 ; // value that depends on the "n" value // slide ??
	TCNT1  = 65536 -1 - n; // TCNT1 = TCNT1H + TCNT1L
	TCCR1A = 0x00;			
	TCCR1B |= (1<<CS10); // no need to set CS11 to 0 because it is 0 by default 
	TCCR1B |= (1<<CS12); // prescaler of 1024
	
	while((TIFR1&(1<<TOV1))==0) 
	{}
		
	TCCR1B = 0; //timer stops
	TIFR1 = (1<<TOV1);	
}


ISR (INT0_vect)
{
	PORTB = 0b0000001; //turn on 1 led
	my_delay();		   // wait 2.9seconds
	
	PORTB = 0b0000010;	//turn on one other LED and turn off The first one
	my_delay();
	
	PORTB = 0b0000100;
	my_delay();
	
	PORTB = 0x00; // turn off all the LED
}



int main(void)
{	
	//# ------------- Define in and outputs ------------- #
	DDRB |=1<<PB0 | 1<<PB1 | 1<<PB2; // set PB0, PB1 and PB2 as output
	DDRD &=~(1<<PD2);	//set PD2 as input because it has the INT0 interrupt 
	
	
	//# ------------- Define register ------------- #
	//Define External Interupt 
	EIMSK = (1 << INT0); //enable external interrupt 0 // does it also work if I write : "EIMSK = 0b01;" ? 
	EICRA = 0b010; // interrupts on falling edge  
	sei(); // Global Interrupt enable 
	
    while (1) 
    {
    }
}
