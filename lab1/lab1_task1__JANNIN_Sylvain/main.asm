; lab1_task1__JANNIN_Sylvain.asm
;
; Created: 13/10/2021 14:56:39
; Author : sylva
;

//Main program

LDI R16,0b111    ; 0xFF
OUT DDRB,R16	;Set PB as an output
SBIS PIND,2		;check switch
RJMP PC-1		;Jump this line and execute following instructions
SBI PORTB,0		;Switch on LED1
CALL my_delay		
CBI PORTB,0		;Switch off LED1
SBI PORTB,1		;Switch on LED2
CALL my_delay
CBI PORTB,1		;Switch off LED2
SBI PORTB,2		;Switch on LED3
CALL my_delay	
CBI PORTB,2		;Switch off LED3


; We have to load the 20 222 value, 0d20 222 = 0b0100 1110 1111 1110 
my_delay: LDI R21, 0b11111110; 1111 1110 --> the low part 
		LDI R22, 0b01001110; 1111 1110 --> the high part 
		STS TCNT1L,R21 ;load timer1
		STS TCNT1H, R22;		
		LDI R20,0x00
		STS TCCR1A,R20 ; normal mode
		LDI R20,0b101; 
		STS TCCR1B,R20 ; prescaler of 1024

AGAIN:	SBIS TIFR1,TOV1 ;  skip next line if TOV1 is set 
		RJMP AGAIN
		LDI R20,0x00
		STS TCCR1B,R20 ;timer stop
		LDI R20,0x01 
		STS TIFR1,R20 ;clear flag TOV1 in the 
