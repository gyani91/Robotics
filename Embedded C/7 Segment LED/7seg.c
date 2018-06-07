#include<avr/io.h>
#include<util/delay.h>

	int main()
	{
	DDRD=255;
	int i,v,rev=4,a[4];
	for(i=2396;i>0;i=i/10)
	{
		v=i%10;
		a[rev]=v;
		rev--;
	}
	rev=0;
	for(i=10000;i>0;i=i/10)
		{
		switch(a[rev])
		{
		case 0:
			PORTD=0x40;
			_delay_ms(1000);
		break;
		case 1:
			PORTD=0x79;
			_delay_ms(1000);
		break;
		case 2:
			PORTD=0x24;
			_delay_ms(1000);
		break;
		case 3:
			PORTD=0x30;
			_delay_ms(1000);
		break;
		case 4:
			PORTD=0x19;
			_delay_ms(1000);
		break;
		case 5:
			PORTD=0x12;
			_delay_ms(1000);
		break;
		case 6:
			PORTD=0x02;
			_delay_ms(1000);
		break;
		case 7:
			PORTD=0x78;
			_delay_ms(1000);
		break;
		case 8:
			PORTD=0x00;
			_delay_ms(1000);
		break;
		case 9:
			PORTD=0x10;
			_delay_ms(1000);
		break;
		
		}
		rev++;
		}
	
	}
	