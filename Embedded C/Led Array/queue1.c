#include<avr/io.h>
#include<util/delay.h>

int main()
{
	int i,n,j;
	
	
	DDRB=255;
	
	while(1)
	{
		int a[8] = {1,4,16,64,128,32,8,2};
		n=7;
		PORTB=0;
		_delay_ms(1000);
		for(i=0;i<8;i++)
		{
			PORTB=a[i];
			_delay_ms(1000);
		}
		for(i=2;i<=130;)
		{
			for(j=0;j<n;j++)
			{
				a[j]=a[j]+i;
				PORTB=a[j];
				_delay_ms(1000);
			}
			n--;
			i*=4;
		}
		for(i=64;i>2;)
		{
			for(j=0;j<n;j++)
			{
				a[j]=a[j]+i;
				PORTB=a[j];
				_delay_ms(1000);
			}
			n--;
			i/=4;
		}
	}
	return 1;
}