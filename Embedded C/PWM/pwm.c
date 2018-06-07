#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<multiutil.h>


#define S1 4
#define S2 5
#define S3 6
#define S4 7
#define SO 2
#define SEN_PIN PINB
#define s1_clr bit_is_clear(SEN_PIN,S1)
#define s2_clr bit_is_clear(SEN_PIN,S2)
#define s3_clr bit_is_clear(SEN_PIN,S3)
#define s4_clr bit_is_clear(SEN_PIN,S4)
#define s1_set bit_is_set(SEN_PIN,S1)
#define s2_set bit_is_set(SEN_PIN,S2)
#define s3_set bit_is_set(SEN_PIN,S3)
#define s4_set bit_is_set(SEN_PIN,S4)
#define FWD PORTC=5
#define REV PORTC=10
#define RIGHT PORTC=6
#define LEFT PORTC=9
#define STOP PORTC=0
#define XL 4
#define YL 4
#define delay_fwd 300
#define spd_rgt OCR1A=512
#define spd_lft OCR1B=512

#define spd_rtrn_rgt OCR1A=484 
#define spd_rtrn_lft OCR1B=512
#define spd_ltrn_rgt OCR1A=512
#define spd_ltrn_lft OCR1B=484

#define SID '2'
#define CID '8'
#define RID '3'

void main()
{
	init();
	stgt(3);
	rgt_trn();
	stgt(1);
	rgt_trn();
	stgt(3);
	lft_trn();
	stgt(1);
}

void init()
{
	DDRD=0;
	DDRC=0xFF;
	DDRB=0x0F;
	init_pwm(10);
	/*init_usart();
	sei();
	config_l();
	lcd_init();*/
}

void config_l()
{
	transmit_char('<');
	transmit_char(SID);
	transmit_char(CID);
	transmit_char('>');
}

void stgt(int a)
{
	int i;
	for(i=0;i<a;)
	{	
            if(s1_clr && s2_clr && s3_clr && s4_clr)
			{
				fwd();
				PORTB=i;
				while(s1_clr && s2_clr && s3_clr && s4_clr);    
				i++;
				//xy();
			}
			else if(s2_clr && s3_clr) 
			{
				fwd();
			}
			else if(s2_set && s3_clr)
			{
				right();
			}
			else if(s2_clr && s3_set)
			{
				left();
            }
			else if(s1_set && s2_set && s3_set && s4_set)
			{
				STOP;
			}
	}
}

void rgt_trn()
{
	fwd();
	_delay_ms(delay_fwd);
	right();
	while(!(s2_clr && s3_set));
	right();
	while(!(s2_clr && s3_clr));
	//dir(1);
}

void lft_trn()
{
	fwd();
	_delay_ms(delay_fwd);
	left();
	while(!(s2_set && s3_clr));
	left();
	while(!(s2_clr && s3_clr));
	//dir(-1);
}

void abt_trn()
{
	fwd();
	_delay_ms(delay_fwd);
	right();
	while(!(s2_clr && s3_set));
	right();
	while(!(s2_clr && s3_clr));
	right();
	while(!(s2_clr && s3_set));
	right();
	while(!(s2_clr && s3_clr));
	//dir(1);
	//dir(1);
}


void fwd(void)
{
	FWD;
	spd_rgt;
	spd_lft;
}


void right(void)
{
	RIGHT;
	//spd_rtrn_rgt;
	//spd_rtrn_lft;
}

void left(void)
{
	LEFT;
	//spd_ltrn_rgt;
	//spd_ltrn_lft;
}










