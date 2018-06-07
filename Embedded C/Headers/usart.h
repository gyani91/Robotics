//USART header

//Header files
#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>

void init_usart(void);
void transmit_char(char);
void transmit_string(char *);
 
void init_usart()
{
	//Communication protocol set
	//with 1 stop bit & 8 data bits
	UCSRC=(1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
	
	//Reception & transmission is enabled
	//Reception of data will occur on interrupt
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	
	//Baud rate = 9600
	UBRRH=0x00;
	UBRRL=103;
}

void transmit_char(char data)
{
	loop_until_bit_is_set(UCSRA,UDRE);
	UDR=data;
}

void transmit_string(char *data)
{
	while(*data)
	{
		transmit_char(*data);
		data++;
	}
		
}