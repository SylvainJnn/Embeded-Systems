/*
 * lab3__JANNIN_Sylvain.c
 *
 * Created: 03/11/2021 10:23:52
 * Author : sylva
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)



void USART_init(void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); //set baud rate
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); //enable transmit
	UCSR0C = (3<<UCSZ00); //set 8-bit (default)
}

unsigned char USART_receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); // Wait to receive data
	return(UDR0); // Read data from UDR
}

int main()
{
	USART_init();		//init UART
	
	DDRB |= 1 << PINB0;	//set PB0 as output 
	
	unsigned char data;	//init the type of data UART will received
	
	while(1)
	{
		data = USART_receive();	//wait for the User to send a data
		switch(data)
		{
			case('Y'):	//if it is 'Y'
				PORTB |= (1 << PINB0);	//Turn on led PB0
				break;
			case('N'):	//if it is 'N'
				PORTB &= ~(1 << PINB0); //Turn off led PB0
				break;
				
			default:	//Otherwise, do nothing
				break;			
		}		
	}
}
