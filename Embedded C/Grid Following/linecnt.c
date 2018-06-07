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
DDRD=0xF0;
DDRC=0b11111111;
DDRB=0x0F;
int i=0;
for(i=0;i<3;)
{
	if(s1_clr && s2_clr && s3_clr && s4_clr)
	{
	PORTB=++i;
	PORTC=9;
	while(PIND==0);
	}
	if(s2_clr && s3_clr && s1_set && s4_set) 
	{
		PORTC=9;
	}
	if(s1_set && s2_set && s3_clr && s4_set)
	{
		PORTC=10;//right
	}
	if(s1_set && s2_clr && s3_set && s4_set)
	{
		PORTC=5;//left
	}
}
PORTC=0;
}