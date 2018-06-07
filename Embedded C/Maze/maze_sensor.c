//Program to Test the bridges in the Maze
//Programmed on AVR Mini (chhotu SHREE)
//Date: 12/12/11 Time: 11:00 PM

//Sensor Connections
//Leftmost S1 --> PD0
// S2 --> PD1
// S3 --> PD2
// S4 --> PD3
// S5 --> PD4
// S6 --> PD5

 
//Header Files
#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<multiutil.h>
#include<avr/interrupt.h>

//Macros 
#define FWD PORTC=0b00000110
#define REV PORTC=0b00001001
#define LEFT PORTC=0b00001010
#define RIGHT PORTC=0b00000101
#define STOP PORTC=0

#define s1_clear bit_is_clear(PIND,0)
#define s2_clear bit_is_clear(PIND,1)
#define s3_clear bit_is_clear(PIND,2)
#define s4_clear bit_is_clear(PIND,3)
#define s5_clear bit_is_clear(PIND,4)
#define s6_clear bit_is_clear(PIND,5)


#define s1_set bit_is_set(PIND,0)
#define s2_set bit_is_set(PIND,1)
#define s3_set bit_is_set(PIND,2)
#define s4_set bit_is_set(PIND,3)
#define s5_set bit_is_set(PIND,4)
#define s6_set bit_is_set(PIND,5)

#define node 1
#define next_bridge 2
#define r_up 3
#define r_down 1
#define l_up 12
#define l_down 8
#define detected 1

//Function Prototypes 
void line_follow(void);
void init_port(void);
void check_node(void);
void find_greatest(void);
void init_timer(void);
 
//Global Variables 
 uint8_t flag=node,bridge=0,r_up_count,r_dn_count,l_up_count,l_dn_count,bridge_flag=0;
 volatile int samples[50],count=0;
 
//Main Function 
int main(void)
{
	//Initialisation
	
	init_port();
	lcd_init();
	init_timer();
	
	//Putting initially 0 to sample array
	//to avoid any kind of garbage values
	
	for(uint8_t i=0;i<50;i++)
	samples[i]=0;
	
	/*lcd_prints("test");
	_delay_ms(1000);*/
	
	while(1)
	{
		//Check the state of the Bot, at Node or finding Next_bridge
		check_node();
		
	switch(flag)
	{
		//At Node, process the samples
				
		case node:
				STOP;
			for(uint8_t i=0;i<50;i++)
			{
				bridge=samples[i];
				//count bridge conditions in the samples
				switch(bridge)
				{
					case r_up:   r_up_count++;   break;
					case r_down: r_dn_count++;   break;
					case l_up:   l_up_count++;   break;
					case l_down: l_dn_count++;  
					
				}
			
			}//End Of For
			
			//Find the most appearing bridge amongst samples
			//& display it
			find_greatest();
			//Cross the Node 
			FWD;
			while(s2_clear && s3_clear&&s4_clear && s5_clear);
			FWD;
			_delay_ms(100);
			count=0; //Reset Counter
			
			//Flush the samples array
			for(uint8_t i=0;i<50;i++)
			{
				samples[i]=0;
			}
			
			break;
			
	case next_bridge:
					//Follow line 
					
						line_follow();
					
					//Bridge Detected for the first time
					if((s1_clear&&s6_set&&count==0)||(s6_clear&&s1_set&&count==0))
					{
							init_timer();
							sei();
							//bridge_flag=detected;
					}
					//Take 100 samples & stop timer
					break;	
					
				}	//End of Switch
			}	//End of while(1)
}	//End of main()		


void line_follow()
{
	
	if(s3_clear && s4_clear&&s2_set&&s5_set)
	FWD;
	else if(s3_set && s4_clear)
	LEFT;
	else if(s4_set && s3_clear)
	RIGHT;
	
}

void init_timer()
{
	TCCR0=(1<<CS02)|(1<<CS00);
	TCNT0=0;
	TIMSK=(1<<TOIE0);
}

void check_node()
{
	if(s5_clear && s2_clear&&s3_clear && s4_clear) //&& s5_clear)
	{
		flag=node;
		bridge_flag=0;
	}
	else
	flag=next_bridge;
	
}

void init_port()
{
	DDRD=0x00;
	DDRB=0x0f;
	DDRC=0xff;
	
}

void find_greatest()
{
	lcd_clrscr();
	if(r_up_count>r_dn_count&&r_up_count>l_dn_count&&r_up_count>l_up_count)
		lcd_prints("Right UP");
	else if(r_dn_count>r_up_count&&r_dn_count>l_dn_count&&r_dn_count>l_up_count)
		lcd_prints("Right Down");
	else if(l_dn_count>r_dn_count&&l_dn_count>r_up_count&&l_dn_count>l_up_count)
		lcd_prints("Left Down");
	else if(l_up_count>r_up_count&&l_up_count>l_dn_count&&l_up_count>r_dn_count)
		lcd_prints("Left Up");	
}

//Timer Overflow routine
//Occurs after every 16 ms.
ISR(TIMER0_OVF_vect)
{
	
	if(count>=50)
	{
		TCCR0=0; TIMSK=0; //Disable Timer after 50 samples
		lcd_clrscr();
		lcd_prints("End of Sampling");	
		
	}
	else
	{
	//lcd_clrscr();
	lcd_goto(0,1);
	lcd_prints("sampling ");
	lcd_printi(count);
	
	if(s1_clear)
	samples[count]|=(0<<0);//store sensor1 reading at LSB
	else
	samples[count]|=(1<<0);
	
	if(s2_clear)
	samples[count]|=(0<<1);
	else
	samples[count]|=(1<<1);
	
	if(s5_clear)
	samples[count]|=(0<<2);
	else
	samples[count]|=(1<<2);
	
	if(s6_clear)
	samples[count]|=(0<<3);
	else
	samples[count]|=(1<<3);//Store Sensor6 reading at 3rd location	
	
	count++;
	}
	
}