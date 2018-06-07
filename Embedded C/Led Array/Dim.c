#include<avr/io.h>
#include<util/delay.h>

int main()
{	
	int i;
	DDRC=255;
	
	while(1)
	{
		for(i=10000;i>1;i--)
		{
			PORTC=1;
			_delay_ms(100);
			PORTC= 0;
			_delay_us(i);
		}
	}
}	
	