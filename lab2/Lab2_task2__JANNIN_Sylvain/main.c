/*
 * Lab2_task2__JANNIN_Sylvain.c
 *
 * Created: 27/10/2021 10:50:14
 * Author : Sykvain Jannin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL




void init_PWM()	
{
	DDRD |= 1 << 6 ;	//set PD6 as enable output,
	DDRB |= 1 << PINB0 ;	//set PB0 as output,
	DDRB |= 1 << PINB1 ;	//set PB1 as output,
	PORTB |= (1 << PINB0);	// init the direction
	TCCR0A = 0x83;	
	TCCR0B = 0b0000101;	//prescaler of 1024
}


void initADC(void)		
{
	ADMUX |= (1 << REFS0) | (1 << MUX2) | (1 << MUX0); //reference voltage on AVCC, MUX = 0101 -> ADC5
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //ADC clock prescaler / 8
	ADCSRA |= (1 << ADEN); //enables the ADC
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
		
		default:
			OCR0A = 0;	// stop the PWM
			break;		//in case the number is wrong
	}
	
}

void Change_Direction()
{
	if( PINB & (1<< 0))		  // if PB0 is is on     
	{
		PORTB &= ~(1<<0);	  //turn off PB0
		PORTB |= (1<<1);	  //turn on PB1
	}
		
	else					  //Otherwise
	{
		PORTB &= ~(1<<1);     //turn off PB1
		PORTB |= (1<<0);      //turn on PB0
	}
	
}

uint16_t get_PotentiometerValue()	
{
	uint16_t potentiometerValue;
	initADC();
	ADCSRA |= (1 << ADSC); //start ADC conversion
	while((ADCSRA & (1 << ADSC))) //wait until ADSC bit is clear, i.e., ADC conversion is done
	{}
	//read ADC value
	uint8_t theLowADC = ADCL;
	potentiometerValue = ADCH << 8 | theLowADC;		//shift ADCH of 8 bits 
	return potentiometerValue;
}



int main(void)
{
	int counter = 0;
	int Analog_control = 0;
	uint16_t potentiometerValue;
    
	//init in and output 
	DDRC = 0;	//Declare all as input
	init_PWM();
	

    while (1) 
    {
		if(Analog_control)	//Used the potentiometer to control the motor
		{
			potentiometerValue = get_PotentiometerValue();
			unsigned char dutycycle = potentiometerValue >> 2;		// Arduino is coded on 10 bits, the ADC on 8bits --> We change the value, we shift it of two bits inorder for the value to be fit on 8 bits and have something more accruate 
			OCR0A = dutycycle;
		}
		
		else             	//Used the switch to control the motor	
		{
			if(PINC & (1 << 3))	// if PC3 button is pressed, change the duty cycle
			{
				while(PINC & (1 << 3));		//wait for the user to unleash the switch
				switch_dutycycle(counter);
				counter = counter +1;
							
				if(counter >= 4)	// if counter is equal to 3 to higher, we reset its value
				{
					counter = 0;
				}
			}
		}
		
		if(PINC & (1 << 4))	// if PC4 is pressed, change the direction
		{
			while(PINC & (1 << 4));	//wait for the user to unleash the switch
			Change_Direction();
			
		}		
				
		
    }
	return(0);
}
	
