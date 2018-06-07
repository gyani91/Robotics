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

#define delay_fwd 1
#define spd_lft OCR1A=512
#define spd_rgt OCR1B=512
#define spd_right_lft OCR1A=612
#define spd_right_rgt OCR1B=0
#define spd_left_lft OCR1A=0
#define spd_left_rgt OCR1B=612


void fwd(void);
void stgt(int);
void rgt_trn(void);
void lft_trn(void);
void abt_trn(void);



int main()                                                                                                   
{
	init();
	stgt(2);
	rgt_trn();
	stgt(1);
	lft_trn();
	stgt(1);
	STOP;
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

void fwd(void)
{
	FWD;
	spd_rgt;                                                           
	spd_lft;
}


/*void fwd1(void)
{
	FWD;
	OCR1A=427;
    OCR1B=427;

}*/



void stgt(int a)
{
	int i;
	for(i=0;i<a;)
	{	
            if(s1_clr && s2_clr && s3_clr && s4_clr)
			{
				fwd();
				i++;
				lcd_goto(1,3);
				lcd_printi(i);
				while(s1_clr && s2_clr && s3_clr && s4_clr);    
				
			}
			else if(s2_clr && s3_clr) 
			{
				fwd();
			}
			else if(s2_set && s3_clr)
			{      
				FWD;
				spd_right_rgt;
				spd_right_lft;
			}
			else if(s2_clr && s3_set)
			{
				FWD;
				spd_left_rgt;
				spd_left_lft;
            }
	}
}

//TURNS

void rgt_trn()
{
	//fwd();
	//_delay_ms(delay_fwd);
	RIGHT;
	OCR1A=250;
	OCR1B=250;
	while(!(s2_clr && s3_set));
	RIGHT;
	OCR1A=250;
	OCR1B=250;
	while(!(s2_clr && s3_clr));
	
}
void lft_trn()
{
	//fwd();
	//_delay_ms(delay_fwd);
	LEFT;
	OCR1A=250;
	OCR1B=250;
	while(!(s2_set && s3_clr));
	LEFT;
	OCR1A=250;
	OCR1B=250;
	while(!(s2_clr && s3_clr));
	
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
	 
}
