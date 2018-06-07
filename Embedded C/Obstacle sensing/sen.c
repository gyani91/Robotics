#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<compat/deprecated.h>

#define s1_clr bit_is_clear(PIND,0)
#define s2_clr bit_is_clear(PIND,1)
#define s3_clr bit_is_clear(PIND,2)
#define s4_clr bit_is_clear(PIND,3)
#define s1_set bit_is_set(PIND,0)
#define s2_set bit_is_set(PIND,1)
#define s3_set bit_is_set(PIND,2)
#define s4_set bit_is_set(PIND,3)
#define FWD PORTC=5
#define RIGHT PORTC=6
#define LEFT PORTC=9
#define STOP PORTC=0

void stgt(void);

int main()
{
	DDRD=0xF0;
	DDRC=0xFF;
	DDRB=0b00011000;
	PORTB=0b00001000;
	GICR=32;
	sei();
	while(1)
	{
		stgt();
		
	}
}
void stgt(void)
{
	if(s1_clr && s2_clr && s3_clr && s4_clr)
			{
				//if(X==1 && Y==2)
					//interupt(1,2);
				while(PIND==0)    
                    FWD;
			}
			else if(s2_clr && s3_clr) 
			{
				FWD;
			}
			else if(s2_set && s3_clr)
			{
				RIGHT;
			}
			else if(s2_clr && s3_set)
			{
				LEFT;
            }
	
}

SIGNAL(SIG_INTERRUPT2)
{
	STOP;
}