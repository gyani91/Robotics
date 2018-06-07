#include<avr/io.h>
#include<util/delay.h>

int main()
{
	int a[8]= {1,4,16,64,128,32,8,2},n,i,j;
	DDRC=255;
	
	while(1)
	{
		int b[8]= {1,4,16,64,128,32,8,2};
		j=0;
		
		for(n=7;n>=0;n--)
		{
			for(i=0;i<=n;i++)
			{
				b[i]=b[i]+j;
				PORTC=b[i];
				_delay_ms(1000);
			}
			j=a[n];
		}
	}
}
		
	
	