/*
 * Lab2_task3__JANNIN_Sylvain.c
 *
 * Created: 27/10/2021 23:23:39
 * Author : Sylvain JANNIN 
 */ 

#define F_CPU 1600000UL
#include <avr/io.h>
#include <util/delay.h>



void init_PWM()
{
	DDRB |= (1 << PINB1); // Set pin 9 to output PWM
	/* 1. Set Fast PWM mode 14: WGM11, WGM12, WGM13 to 1*/
	/* 2. Set pre-scale of 8 */
	/* 3. Set Fast PWM non-inverting mode */
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) |(1 << CS11);
	/* 4. Set ICR1: ICR1 is the top defining PWM period */
	ICR1 = 40000;
}


int main(void)
{
	init_PWM();
	while(1)
	{
		
		OCR1A = 1000;		// duty cycle of 2.5%
		_delay_ms(8000);
		
		OCR1A = 2000;		// duty cycle of 5%
		_delay_ms(8000);
		
		OCR1A = 3000;		// duty cycle of 7.5%
		_delay_ms(8000);
		
		OCR1A = 4000;		// duty cycle of 10%
		_delay_ms(6000);
		
		OCR1A = 5000;		// duty cycle of 12.5%
		_delay_ms(8000);
		
		OCR1A = 4000;		// duty cycle of 10%
		_delay_ms(8000);
				
		OCR1A = 3000;		// duty cycle of 7.5%
		_delay_ms(8000);
		
		OCR1A = 2000;		// duty cycle of 5%
		_delay_ms(8000);
		
	}
	return 0;
}