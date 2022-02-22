/*
 * Lab3_task2__JANNIN_Sylvain.c
 *
 * Created: 03/11/2021 15:22:15
 * Author : sylva
 */ 
/*
#define D0 eS_PORTB2
#define D1 eS_PORTB3
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTB0
#define EN eS_PORTB1
#define F_CPU 16000000UL
#include <avr/io.h>
#include "lcd.h"
#include <util/delay.h>
#include <string.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)



void USART_init(void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); //set baud rate
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //enable transmit
	UCSR0C = (3<<UCSZ00); //set 8-bit (default)
}
void USART_send(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))); // check if data is sent
	UDR0 = data; // load new data to transmit
}

unsigned char USART_receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); // Wait to receive data
	return(UDR0); // Read data from UDR
}



int main(void)
{
	DDRD = 0xFF;	//set as output all PORTD
	DDRB = 0xFF;	//set as output all PORTB
	Lcd8_Init();	//init LCD
	USART_init();	//init UART
	
	unsigned char data;		//declare the variable used to get data from the PC
	int len_name = 14;		//"Sylvain Jannin" --> 14
	char name[len_name];	//create a list of char with a length of 14
	int counter = 0;		//variable uses to add information in "name"
	
	while(1)
	{
		data = USART_receive();		//do a first wait until user send a data trought the PC
		USART_send(data);			//resend the data to the PC to check if it is correct or not		
		name[counter] = data;		//add this information in "name"
		counter ++;					//increment the counter
		while(counter < len_name)	//redo these steps until  we have a
		{
			data = USART_receive();	//get the information send by user trought UART
			USART_send(data);		//resend the data to the PC to check if it is correct or not	
			
			name[counter] = data;	//add this information in "name"
			counter ++;				//increment the counter
		}
		
		name[counter] = '\0';	//add '\0' to indicates the end of the string
		counter = 0;			//reset counter		
	
		Lcd8_Set_Cursor(0,0);	//set the position of the cursor at the begining
		Lcd8_Write_String(name);//write on the LCD the name
		_delay_ms(10000);		//wait 10s
		Lcd8_Clear();			//clear the LCD	
	}
	return(0);
}*/
#define D0 eS_PORTD0
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

int main(void)
{
	DDRD = 0xFF;
	DDRB = 0xFF;
	char string[37] = "Welcome to B31DD Embedded Systems!\0";
	int i;
	Lcd8_Init();
	while(1)
	{
		Lcd8_Set_Cursor(0,0);
		Lcd8_Write_String(string);
		Lcd8_Set_Cursor(1,1);
		Lcd8_Write_String("JES TEST");
		for(i=0;i<strlen(string)-1;i++)
		{
			_delay_ms(500);
			Lcd8_Shift_Left();
		}
		for(i=0;i<strlen(string)-1;i++)
		{
			_delay_ms(500);
			Lcd8_Shift_Right();
		}
		Lcd8_Clear();
		// Lcd8_Write_Char('e');
		// Lcd8_Write_Char('S');
		_delay_ms(100);
	}
}
