#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<ilcd.h>
#include<avr/interrupt.h>
#include<usart.h>

#define SID '2'
#define CID '8'
int main()
{

	
}
config_wl()
{
	transmit_char('<');
	transmit_char(SID);
	transmit_char(CID);
	transmit_char('>');
}

SIGNAL(SIG_USART_RECV)
{
	char data;
	data=UDR;
}
	