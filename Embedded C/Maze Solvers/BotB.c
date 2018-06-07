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
#define XL 1
#define YL 1
#define delay_fwd 2200
#define spd_rgt OCR1A=683
#define spd_lft OCR1B=683
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

void safeline(void);
void zigzag(void);
void fallback(void);

void retrieve(int,int);
void revive(int,int);
void interupt(int,int);

void lcd(int,int,int,int);
uint8_t X=4,Y=4,direct=3,O=0;

int main()
{
init();
while(1)
zigzag();
}
//INITIALIZATION
void init(void)
{
     GICR |= (1 << 6)
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

//safeline scanning

void safeline(void)
{
     lft_trn();
     stgt(7);
     abt_trn();
     stgt(7);
     lft_trn();
     stgt(1);
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
			lft_trn();
			stgt(1);
			lft_trn();

		}
	}
	else
	{
		c_dir(3);
		stgt(Y-1);
		if(Y==1)
		{
			rgt_trn();
			stgt(1);
			rgt_trn();
		}
	}
	}
}

//DROP N COME BACK MECHANISM
SIGNAL(SIG_INTERRUPT0)
{
	STOP;
	O=1;
	lcd(X+1,Y+1,O,direct);
	//pick
	fwd();
	_delay_ms(delay_obs);
	c_dir(2);
	stgt(1);
	rgt_trn();
	stgt(Y-1);
	rgt_trn();
	stgt(X-1);
	rgt_trn();
	stgt(Y-2);
	O=0;
	//drop
	abt_trn();
	stgt(XL-X);
	//retrive
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

void err()
{
     sbi(PORTB,3);
     _delay_ms(1500);
     cbi(PORTB,3);
}
