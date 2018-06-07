#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<ilcd.h>


#define s1_clr bit_is_clear(PIND,0)
#define s2_clr bit_is_clear(PIND,1)
#define s3_clr bit_is_clear(PIND,2)
#define s4_clr bit_is_clear(PIND,3)
#define s1_set bit_is_set(PIND,0)
#define s2_set bit_is_set(PIND,1)
#define s3_set bit_is_set(PIND,2)
#define s4_set bit_is_set(PIND,3)
#define fwd PORTC=5
#define right PORTC=6
#define left PORTC=9
#define STOP PORTC=0
#define XL 4
#define YL 4

void stgt(int);
int main()
{
	DDRD=0b00000000;
	DDRB=0b00000010;
	DDRC=0x0F;
	while(1)
	{
	stgt(3);
	}	
}
void stgt(int a)
{
int i=0;
for(i=0;i<a;)
{
	if(s1_clr && s2_clr && s3_clr && s4_clr)
	{
	PORTB=++i;
	fwd;
	while(PIND==0);
	}
	if(s2_clr && s3_clr && s1_set && s4_set) 
	{
		fwd;
	}
	if(s1_set && s2_set && s3_clr && s4_set)
	{
		right;
	}
	if(s1_set && s2_clr && s3_set && s4_set)
	{
		left;
	}
	if(bit_is_clear(PIND,7))
	PORTC=0;
		
	}
PORTC=0;
}