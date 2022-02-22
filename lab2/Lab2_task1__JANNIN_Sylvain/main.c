/*
 * Lab2_task1__JANNIN_Sylvain.c
 *
 * Created: 27/10/2021 10:50:14
 * Author : Sykvain Jannin
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL


void init_PWM()
{
	DDRD |= 1 << 6 ;	//set PD6 as enable output, 
	PORTB |= (1 << PINB1);	
	TCCR0A = 0x83;			//set fast PWM, non inverted
	TCCR0B = 0b0000101;	//prescaler of 1024
}


void switch_dutycycle(int counter)	
{
	switch(counter)
	{
		case(0):	
			OCR0A = 0;	// stop the PWM
			break;
		
		case(1):
			OCR0A = 63;	//duty cycle of 25%
			break;
		
		case(2):
			OCR0A = 159;	//duty cycle of 62.5%
			break;
		
		case(3):
			OCR0A = 223;	//duty cycle of 87.5%
			break;
		
		default:		//in case the number is wrong
			OCR0A = 0;	// stop the PWM
			break;		
	}
	
}


int main(void)
{
	int counter = 0;
    
	//init in and output 
	DDRC = 0;	//Declare all as input
	init_PWM();
    while (1) 
    {
		if(PINC & (1 << 0))	// if the button is pressed oh PC0
		{		
			while(PINC & (1 << 0));
			switch_dutycycle(counter);
			counter = counter +1;
			
			if(counter >= 4)	// if counter is equal to 3 to higher, we reset its value
			{
				counter = 0;	
			}
			
		}	
    }
	return(0);
}
