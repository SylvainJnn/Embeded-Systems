/*
 * Mini_Project__JANNIN_Sylvain.c
 *
 * Created: 17/11/2021 14:41:22
 * Author : Sylvain Jannin - H00387879
 */ 

#define D0 eS_PORTD0	//For LCD library
#define D1 eS_PORTD1
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTB0
#define EN eS_PORTB2
#define F_CPU 16000000UL
#include <avr/io.h>
#include "lcd.h"
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

//init global variables
int counter_overflow;	//count the number of time the timer has overflowed
int limit_overflow;		//number of overflowed. These 2 variables have a timer used
int detect_flag;		//indicate if it has detected someone for the last "x" milliseconds (the x values depend on the values of limit_overflow: if limit_overflow = 1000 -> x = 1000 ms)
int LED_flag;			//Indicates if the LED is on or not. 
int door_flag;			//Indicates is the door is open or not. 



// ---------------- init register ----------------
void timer_1ms()
{
	int N = 250 ;
	TCNT0 = 256 -1 - N;
	TCCR0A = 0x00;
	TCCR0B = 0b00000011;	//prescaler of 64
}

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

void initADC(void)
{
	ADMUX |= (1 << REFS0) | (1 << MUX2) | (1 << MUX0); //reference voltage on AVCC, MUX = 0101 -> ADC5
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);		       //ADC clock prescaler / 8
	ADCSRA |= (1 << ADEN);							   //enables the ADC
}


void init_registers()
{
	
	DDRC = 0;	//set PORTC as Input
	DDRD = 0xFF;//set PORTD as output
	DDRB = 0xFF;//set PORTB as output

	DDRC |= 1 << PINC1;	//set PC1 as output	//for the LED
	
	init_PWM();		
	initADC();
	Lcd8_Init();	//init LCD
	
	TIMSK0 = (1 <<TOIE0); //enable Timer0 overflow interrupt
	sei();			      //enable interrupt

}


// ---------------- function ----------------
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



void Open_door()
{
	if(PINC & (1 << PINC4))	//if PINC4 is high = if it detects something
	{
		OCR1A = 5000;		//Open the door
		timer_1ms();		//start the timer
		detect_flag = 1;	//raise detect_flag
		door_flag = 1;		//raise door_flag
	}
	else
	{
		OCR1A = 3000;		//close the door
		door_flag = 0;		//Low door_flag
	}
	
}

void Light_sensor()//need to check a timer                                                                                                                                                                                 n,
{	
	uint16_t potentiometerValue;
	potentiometerValue = get_PotentiometerValue();		//Read the light sensor
	if(potentiometerValue < 510 && detect_flag == 1)	//if it is too dark + it detects something 
	{
		PORTC |= (1 << PINC1);	//Turn on led PC1
		LED_flag = 1;			//Raise flag 
	}
	else
	{
		PORTC &= ~(1 << PINC1); //Turn off led PC1
		LED_flag = 0;			//Low flag 
	}
}

void update_LCD()
{
	Lcd8_Clear();			//clear the LCD	
	
	//init variables we send to the LCD
	char* data_door;	
	char* data_light;
	
	//Is door open or close
	if(door_flag == 1)
	{
		data_door = "Door : OPEN";
	}
	else
	{
		data_door = "Door : CLOSE";
	}
	
	//Light on or off
	if(LED_flag == 1)
	{
		data_light = "Light: ON";
	}
	
	else
	{	
		data_light = "Light: OFF";
	}
	
	Lcd8_Set_Cursor(0,0);	//set the position of the cursor on the first line
	Lcd8_Write_String(data_door);//write on the LCD data_door
	Lcd8_Set_Cursor(2,0);	//set the position of the cursor on the second line
	Lcd8_Write_String(data_light);//write on the LCD data_light
}


// ---------------- interrupt ----------------
ISR(TIMER0_OVF_vect)	//TimerOverflow interrupt
{
	counter_overflow ++;	//each time the interrupt occurred 
	detect_flag = 1;		//raise the detect_flag
	if(counter_overflow >= limit_overflow)	//If the number of overflowed is reached
	{
		//Stop timer
		TCCR0B = 0; 
		TIFR0 = (1<<TOV0);
		counter_overflow = 0;	//reset the counter
		detect_flag = 0;		//Low the flag
	}
}

// ---------------- Main ----------------
int main(void)
{	
	limit_overflow = 5000;	//The timer count for 1ms, the value of limit overflow makes the timer having a period of : T = 1ms * limit_overflow. In our case it is five seconds
	counter_overflow = 0;	//count the number of overflow before the limit
	double counter_LCD = 0;

	init_registers();		//Initialiase registers, Input and output 
	
	while(1)
	{
		Open_door();		//Manage the door: actuator and servo
		Light_sensor();		//Manage the light
		
		counter_LCD ++;
		if(counter_LCD >= 10000)//We cannot rewrite on the LCD for every iteration and adding a delay would the 2d timer working properly, so to have a quick implementation we update it for every x iterations, the values has been choosed if the result was satifaying enough since we do not have constraint enough
		{
			update_LCD();	//update the LCD
			counter_LCD=0;	//reset counter
		}
	}
	return(1);
}

