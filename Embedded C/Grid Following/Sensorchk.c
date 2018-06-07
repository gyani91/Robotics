#include<avr/io.h>
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
int main()
{
	DDRC=0x0F;
	DDRD=0;
	while(1)
	{
		if(s1_clr)
		{
			PORTC=2;
		}
		else if(s2_clr)
		{
			PORTC=1;
		}
		else if(s3_clr)
		{
			PORTC=8;
		}
		else if(s4_clr)
		{
			PORTC=4;
		}
		else
		{
			PORTC=0;
		}
	}
	