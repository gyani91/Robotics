#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<multiutil.h>
#include<avr/interrupt.h>


#define S1 0
#define S2 1
#define S3 2
#define S4 3
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

#define XL 3
#define YL 3

#define delay_fwd 2200

#define spd_rgt OCR1A=683
#define spd_lft OCR1B=683

void init(void);

void stgt(int);
void rgt_trn(void);
void lft_trn(void);
void abt_trn(void);

int main()
{
	init();
	sei();
	while(1)
	{
		stgt(1);
	}
}

void init(void)
{
     DDRD=0x00;
     DDRC=0xFF;
     DDRB=0x0F;
	 PORTB=0xF0;
	 init_pwm(10);
     lcd_init();
}

//GRID FOLLOWER

void fwd(void)
{
	FWD;
	spd_rgt;
	spd_lft;
}
void stgt(int a)
{
	uint8_t i;
	for(i=0;i<a;)
	{	
            if(s1_clr && s2_clr && s3_clr && s4_clr)
			{
				fwd();
				i++;
				xy();
                while(s1_clr && s2_clr && s3_clr && s4_clr);    
			}
			else if(s2_clr && s3_clr) 
			{
				fwd();
			}
			else if(s2_set && s3_clr)
			{
				RIGHT;
			}
			else if(s2_clr && s3_set)
			{
				LEFT;
            }
	}
}

//TURNS

void rgt_trn()
{
	
	fwd();
	_delay_ms(delay_fwd);
	RIGHT;
	while(!(s2_clr && s3_set));
	RIGHT;
	while(!(s2_clr && s3_clr));
	dir(1);
}
void lft_trn()
{
	fwd();
	_delay_ms(delay_fwd);
	LEFT;
	while(!(s2_set && s3_clr));
	LEFT;
	while(!(s2_clr && s3_clr));
	dir(-1);
}
void abt_trn()
{
	fwd();
	_delay_ms(delay_fwd);
	RIGHT;
	while(!(s2_clr && s3_set));
	RIGHT;
	while(!(s2_clr && s3_clr));
	RIGHT;
	while(!(s2_clr && s3_set));
	RIGHT;
	while(!(s2_clr && s3_clr));
	dir(1);
	dir(1);
}
