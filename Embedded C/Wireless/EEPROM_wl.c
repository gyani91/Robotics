#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<multiutil.h>
#include<avr/interrupt.h>


#define SID '3'
#define CID '+'
#define RID '2'


int main()
{
	init();
	input();
	while(1);
	transmit();
}
void transmit(void)
{
	while(1)
	{
	if(bit_is_clear(PINB,4))
	{
		transmit_char(RID);
		transmit_char('a');
		break;
	}
	}
}	

void init()
{
	 DDRD=0x00;
     DDRC=0xFF;
     DDRB=0x0F;
	 PORTB=0xF0;
     lcd_init();
	 init_usart();
	 sei();
	 config_wl();
	 lcd_goto(1,1);
	 write_data(UDR);
	 _delay_ms(1000);
	 
	
}

config_wl()
{
	transmit_char('<');
	transmit_char(SID);
	transmit_char(CID);
	transmit_char('>');
}

void dynamic(char a)
{
	loop_until_bit_is_clear(EECR,EEWE);
	EEAR=10;
	EEDR=a;
	EECR=4;
	EECR=6;
	//loop_until_bit_is_clear(EECR,EEWE);
}
SIGNAL(SIG_USART_RECV)
{
	char data;
	data=UDR;
	lcd_goto(2,1);
	write_data(data);
	_delay_ms(1000);
	dynamic(data);
	input();
	_delay_ms(3000);
}
void input()
{
	char data;
	loop_until_bit_is_clear(EECR,EEWE);
	EEAR=10;
	EECR=1;
	data=EEDR;
	lcd_goto(2,2);
	write_data(data);
}	