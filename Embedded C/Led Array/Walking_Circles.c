#include<avr/io.h>
#include<util/delay.h>

int main(void)
{
	/*DDRC=0b11111111;
	int i;
	
	while(1)
	{
		int i=3;
		while(i<=200)
		{
			PORTC=i;
			i=i*2;
			_delay_ms(1000);
		}
	} walking */
	
	/*DDRC=0b11111111;
	int i;
	
	while(1)
	{   
		i=1;
		while(i<=70)
		{
			PORTC=i;
			i=i*4;
			_delay_ms(1000);
		}
		PORTC=128;
		_delay_ms(1000);
		
		int i=32;
		while(i>=1)
		{
			PORTC=i;
			i=i/4;
			_delay_ms(1000);
		}
		
		
	} CCW */
	
	DDRC=0b11111111;
	int i,j=0;
	
	while(1)
	{   
		i=1+j;
		while(i<=128+j)
		{
			PORTC=i;
			i=i*4+j;
			_delay_ms(1000);
		}
		i=64+j;
		while(i>=4+j)
		{
			PORTC=i;
			i=i/4+j;
			_delay_ms(1000);
		}
		i=i*4;
		j=j+i;
		
		
	}

}
	
