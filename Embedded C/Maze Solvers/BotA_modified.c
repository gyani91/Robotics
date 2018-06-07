                                  //Grid Solver
#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<multiutil.h>
#include<avr/interrupt.h>


#define S1 0
#define S2 1
#define S3 2
#define S4 3
#define SEN_PIN PINB

#define s1_clr bit_is_clear(SEN_PIN,S1)
#define s2_clr bit_is_clear(SEN_PIN,S2)
#define s3_clr bit_is_clear(SEN_PIN,S3)
#define s4_clr bit_is_clear(SEN_PIN,S4)

#define s1_set bit_is_set(SEN_PIN,S1)
#define s2_set bit_is_set(SEN_PIN,S2)
#define s3_set bit_is_set(SEN_PIN,S3)
#define s4_set bit_is_set(SEN_PIN,S4)

#define FWD PORTC=9
#define REV PORTC=6
#define RIGHT PORTC=10
#define LEFT PORTC=5
#define STOP PORTC=0

#define XL 8
#define YL 8

#define delay_fwd 1000
#define delay_trn 1500

#define trn_pwm 500
#define spd_rgt OCR1A=850
#define spd_lft OCR1B=850

#define SID '2'
#define CID '8'

void init(void);
void err(void);

void stgt(int);
void rgt_trn(void);
void lft_trn(void);
void abt_trn(void);

void dir(int);
void c_dir(int);
void xy(void);

void zigzag(void);
void fallback(void);

void retrieve(int,int);
void revive(int,int);
void interupt(int,int);

void nml(void);
 
void lcd(int,int,int,int);


uint8_t X=1,Y=1,direct=1,O=0;

int main()
{
	init();
	gripout();
	//zigzag();
}
//INITIALIZATION
void init(void)
{
     DDRD=0x00;
     DDRC=0xFF;
     DDRB=0x00;
	 GICR=64;
	 MCUCR=0;
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

void right(void)
{	
	RIGHT;
	OCR1A=trn_pwm;
	OCR1B=trn_pwm;
}

void left(void)
{	
	LEFT;
	OCR1A=trn_pwm;
	OCR1B=trn_pwm;
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
                _delay_ms(550);    
			}
			else if(s2_clr && s3_clr) 
			{
				fwd();
			}
			else if(s2_set)
			{
				RIGHT;
				OCR1A=0;
				OCR1B=512;
				
			}
			else if(s3_set)
			{
				LEFT;
				OCR1A=512;
				OCR1B=0;
            }
	}
}

//TURNS

void rgt_trn()
{
	
	fwd();
	_delay_ms(delay_fwd);
	right();
	_delay_ms(delay_trn);
	while(!(s2_clr && s3_clr));
	dir(1);
}
void lft_trn()
{
	fwd();
	_delay_ms(delay_fwd);
	left();
	_delay_ms(delay_trn);
	while(!(s2_clr && s3_clr));
	dir(-1);
}
void abt_trn()
{
	fwd();
	_delay_ms(delay_fwd);
	right();
	_delay_ms(delay_trn);
	while(!(s2_clr && s3_clr));
	_delay_ms(delay_trn);
	while(!(s2_clr && s3_clr));
	dir(1);
	dir(1);
}


//DIRECTION

void dir(int a)
{
	direct=direct+a;
	if(direct==5)
		direct=1;
	if(direct==0)
		direct=4;
    	lcd(X,Y,O,direct);
}

void c_dir(int b)
{
	uint8_t c;
	c=direct-b;
	
	if(c==1 || c==-3)
		lft_trn();
	if(c==2 || c==-2)
		abt_trn();
	if(c==3 || c==-1)
		rgt_trn();
 	lcd(X,Y,O,direct);
}
void xy()
{
     if(direct==1)
     Y++;
	if(direct==2)
		X++;
	if(direct==3)
		Y--;
	if(direct==4)
		X--;
	lcd(X,Y,O,direct);
}

//ZIG ZAG FOLLOWER

void zigzag()
{
	
	sei();
	lcd(X,Y,O,direct);
	while(1)
	{
	if(X==XL)
	{
		if(X%2!=0)
		{
			stgt(YL-1);
			abt_trn();
		}
		fallback();
		break;
	}
	
	if(X%2!=0)
	{
		c_dir(1);
		stgt(YL-Y);
		if(Y==YL)
		{
			rgt_trn();
			stgt(1);
			rgt_trn();

		}
	}
	else
	{
		c_dir(3);
		stgt(Y-1);
		if(Y==1)
		{
			lft_trn();
			stgt(1);
			lft_trn();
		}
	}
	}
}

void nml(void)
{	
	fwd();
	while(!(s4_clr && s2_clr && s3_clr));
	rgt_trn();
	fwd();
	while(!(s1_clr && s2_clr && s3_clr));
	lft_trn();
	fwd();
	while(!(s1_clr && s2_clr && s3_clr));
	lft_trn();
	fwd();
	while(!(s4_clr && s2_clr && s3_clr));
	rgt_trn();
	fwd();
	while(!(s4_clr && s2_clr && s3_clr));
	rgt_trn();
	fwd();
	while(!(s1_clr && s2_clr && s3_clr));
	lft_trn();
	fwd();
	
}

//DROP N COME BACK MECHANISM
SIGNAL(SIG_INTERRUPT0)
{
	STOP;
	O=1;
	lcd(X+1,Y+1,O,direct);
	uthana();
	fwd();
	_delay_ms(2500);
	c_dir(4);
	lcd(X,Y,O,direct);
	retrieve(X,Y);
}

void retrieve(int a,int b)
{
	
	stgt(a-1);
	lft_trn();
	stgt(b-2);
	O=0;
	rakhna();
	c_dir(1);
	lcd(X,Y,O,direct);
	stgt(b-2);
	rgt_trn();
	stgt(a-1);
}

//FINAL
void fallback()
{
	stgt(YL-1);
	rgt_trn();
	stgt(XL-1);
	rgt_trn();
	STOP;
}

//DISPLAY

void lcd(int a,int b,int c,int d)
{
     lcd_goto(1,1);
     lcd_prints("Dir");
     lcd_goto(2,2);
     lcd_printi(d);
     
     lcd_goto(7,1);
     lcd_prints("X");
     lcd_goto(7,2);
     lcd_printi(a);
     
     lcd_goto(11,1);
     lcd_prints("Y");
     lcd_goto(11,2);
     lcd_printi(b);
     
     lcd_goto(14,1);
     lcd_prints("O");
     lcd_goto(14,2);
     lcd_printi(c);
}
//ERROR

void err(void)
{
     sbi(PORTB,3);
     _delay_ms(1500);
     cbi(PORTB,3);
}
void uthana(void)
{
	gripdown();
	fwd();
	_delay_ms(200);
	gripin();
	gripup();
}

void rakhna(void)
{
	gripdown();
	gripout();
	gripup();
}

void gripup(void)
{
	PORTC=0x40;
	OCR1A=700;
	OCR1B=0;
	_delay_ms(3300);
	PORTC=0;
}

void gripdown(void)
{
	PORTC=0x80;
	OCR1A=700;
	OCR1B=0;
	_delay_ms(2500);
	PORTC=0;
	
}

void gripout(void)
{
	PORTC=0x20;
	OCR1A=700;
	OCR1B=0;
	_delay_ms(1500);
	PORTC=0;
}

void gripin(void)
{
	PORTC=0x10;
	OCR1A=700;
	OCR1B=0;
	_delay_ms(3000);
	PORTC=0;
}