/*
 * lab1_task2__JANNIN_Sylvain.c
 *
 * Created: 30/09/2021 09:00:37
 * Author : sylvain J
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <time.h>
#define F_CPU 16000000UL




void dice_choice(int dice)
{
	switch(dice)
	{
		case(1):
			PORTB = 0b00000001;	// do a 1 with LEDs
			break;
		
		case(2):
			PORTB = 0b00000110; // do a 2 with LEDs
			break;
			
		case(3):
			PORTB = 0b00000111; // do a 3 with LEDs
			break;
		
		case(4):
			PORTB = 0b00001110; // do a 4 with LEDs
			break;
			
		case(5):
			PORTB = 0b00001111; // do a 5 with LEDs
			break;
		
		case(6):
			PORTB = 0b00011110; // do a 6 with LEDs
			break;
		default:
			break; // in case the number is not good, do nothing
	}
	
}


void my_delay()
{
	int n = 45313; // value that depends on the "n" value // slide ??
	TCNT1  = 65536 -1 - n; // TCNT1 = TCNT1H + TCNT1L
	TCCR1A = 0x00;	
	TCCR1B |= (1<<CS10); //// no need to set CS11 to 0 because it is 0 by default 
	TCCR1B |= (1<<CS12);
	
	while((TIFR1&(1<<TOV1))==0) 
	{}
		
	TCCR1B = 0; //timer stops
	TIFR1 = (1<<TOV1);	
}

int my_random_dice()
{
	int nb;
	nb = 0;
	
	nb = rand () %6; //generate a random number from 0 to 5
	nb = nb +1;
	return (nb);
}

ISR (INT0_vect)
{
	int dice;
	dice = my_random_dice();
	dice_choice(dice);
	my_delay();
	PORTB = 0x00; // reset
}



int main(void)
{	
	//# ------------- Define in and outputs ------------- #
	DDRB = 0b00011111;
	DDRD &=~(1<<PD2);	//set PD2 as input because it has the INT0 interrupt 
	
	
	//# ------------- Define register ------------- #
	//Define External Interupt 
	EIMSK = (1 << INT0); //enable external interrupt 0 // does it also work if I write : "EIMSK = 0b01;" ? 
	EICRA = 0b010; // interrupts on falling edge  
	sei(); // Global Interrupt enable 
	
	srand (time (NULL));	//initialise time for the random number
	
    while (1) 
    {		
    }
}
