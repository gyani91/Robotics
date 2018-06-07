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


void lcd(void);
void wall(void);
void obs(void);
void range(void);

int x,a;
int err;
int idl;

int main(void)
{
    init();
    while(1)
	wall();
}

void wall(void)
{
        OCR1A=spd_normal;
		OCR1B=spd_normal;
		FWD;
		x=read_adc(2);
		idl=520;
		err=idl-x;
		lcd();
		if(err>0)
		{
		
		OCR1B=OCR1B-(err*4);
	
		}
		
		else
		{
		
		OCR1A=OCR1A-(err*4);

		}
		obs();
		
}
void init(void)
{
     DDRC=0XFF;
	DDRA=0b11110011; 
	DDRD=15;
	init_adc();
	lcd_init();
	init_pwm(10);
}
void obs(void)
{
     if((bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)))
		{
			OCR1A=spd_obs;
		    OCR1B=spd_obs;
            FWD;
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
			else
			{
                lcd_clrscr();
                lcd_goto(2,1);
                lcd_prints("Error!");
             }
        }
}

void range(void)
{
	while(1)
	{
            if(x>470 && x<570)
	        {
                     a=1;
	        }
	        else
	        {
		             a=0;
		             break;
	        }
	        if(bit_is_clear(PINA,3)
            break;
      }
	  lcd_goto(6,2);
	  lcd_printi(a);
}
void lcd(void)
{             
lcd_clrscr();
lcd_goto(1,1);
lcd_prints("Challenge");
lcd_goto(2,2);
lcd_prints("Accept-Ted");
lcd_goto(11,1);
lcd_printi(x);
lcd_goto(12,2);
lcd_printi(1);
_delay_ms(100);
}
