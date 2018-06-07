// Wall-E
#include<multiutil.h>
#include<compat/deprecated.h>
#include<avr/io.h>

#define FWD PORTC=0b00000110
#define RIGHT PORTC=0b00000001
#define LEFT PORTC=0b00001000

#define CLK PORTC=0b00001010
#define ANTI PORTC=0b00000101

#define delay_trn _delay_ms(500)
#define delay_fwd _delay_ms(500)

#define spd_normal 1023
#define spd_obs 700


void range(void);
int x,a;
int err;
int idl;

int main(void)
{
	DDRC=0XFF;
	DDRA=0b11110011; 
	DDRD = 0b00110000;
	init_adc();
	lcd_init();
	init_pwm(10);
	
	PORTC = 0b00000110;
	while(1)
	{
	
		OCR1A=spd_normal;
		OCR1B=spd_normal;
		FWD;
		x=read_adc(2);
		idl=520;
		err=idl-x;
		lcd_clrscr();
		lcd_goto(1,1);
		lcd_prints("Sumeet");
		lcd_goto(1,2);
		lcd_printi(x);
		lcd_goto(6,2);
		lcd_printi(1);
		_delay_ms(100);

		if(err>0)
		{
		
		OCR1B=OCR1B-(err*4);
	
		}
		
		else
		{
		
		OCR1A=OCR1A-(err*4);

		}
		if((bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)))
		{
			range();
			if((bit_is_clear(PINA,3)) && a==1)
			{
				ANTI;
				delay_trn;
				FWD;
				if(bit_is_set(PINA,3) && a==0)
				{
					CLK;
					delay_trn;
					FWD;
					delay_fwd;
					CLK;
					delay_trn;
				}
			}
			else if(bit_is_set(PINA,3) && a==0)
			{
				FWD;
				while(a=0)
				range();
			}
		}
		
	}
		
}

void range(void)
{
	if(x>470 && x<570)
	{
		a=1;
	}
	else
	{
		a=0;
	}
	lcd_goto(6,2);
	lcd_printi(a);
}