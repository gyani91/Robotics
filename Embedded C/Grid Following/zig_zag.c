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
#define fwd PORTC=5
#define right PORTC=6
#define left PORTC=9  



void stgt(int);
void rgtturn();
void leftturn();
void zigzag(int,int);

int main()
{
DDRD=0xF0;
DDRC=0b11111111;
DDRB=0x0F;
int b,l;
b=2;
l=3;
while(1)
{
zigzag(b-1,l-1);
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
}
PORTC=0;
}

void rgtturn()
{
	fwd;
	_delay_ms(2000);
	right;
	while(PIND!=7);
	right;
	while(PIND!=9);	
}
void leftturn()
{
	fwd;
	_delay_ms(2000);
	left;
	while(PIND!=14);
	left;
	while(PIND!=9);
}
void abtturn()
{
	fwd;
	_delay_ms(2000);
	right;
	while(PIND!=7);
	right;
	while(PIND!=9);
	right;
	while(PIND!=7);
	right;
	while(PIND!=9);
}
void zigzag(int r,int c)
{
	
	int i;
	stgt(r);
	for(i=1;i<=c;i++)
	{
		if(i%2!=0)
		{
			rgtturn();
			stgt(1);
			rgtturn();
			stgt(r);
		}
		if(i%2==0)
		{
			leftturn();
			stgt(1);
			leftturn();
			stgt(r);
		}
	}
	if(c%2==0)
	{
		abtturn();
		stgt(r);
		rgtturn();
		stgt(c);
		rgtturn();
	}
	if(c%2!=0)
	{
		rgtturn();
		stgt(c);
		rgtturn();
	}
}
