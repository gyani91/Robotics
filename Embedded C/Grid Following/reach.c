                //October 19 Project
#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<multiutil.h>

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
#define XL 4
#define YL 4
#define delay_fwd 1800
#define spd_rgt OCR1A=683
#define spd_lft OCR1B=683
#define SID '2'
#define CID '8'


#define PA 1,2
#define PB 0,2
#define PC 3,3
#define PD 1,0
#define EX 0
#define EY 3

void init(void);
void err(void);

void fwd(void);
void stgt(int);
void rgt_trn(void);
void lft_trn(void);
void abt_trn(void);

void sw(void);
void order(void);
void lcd_order(char);

void dir(int);
void c_dir(int);
void xy(void);

void reach(int,int);
void x_travel(int);
void y_travel(int);
void lcd(int,int,int);

int X=0,Y=0,direct=1,a[4];

int main()
{
    init();
	sw();
	lcd_clrscr();
	lcd_init();
	order();
    _delay_ms(1200);
	STOP;
}
//INITIALIZATION
void init(void)
{
     DDRD=0x00;
     DDRC=0xFF;
     DDRB=0x0F;
	 PORTB=0xF0;
	 init_pwm(10);
     lcd_init();
}
//SWITCH
void sw()
{
	int i=0;
	while(1)
	{
		if(bit_is_clear(PINB,4))
		{
			a[i]=1;
			lcd_order(5*i);
			lcd_prints("A");
			_delay_ms(1000);
			i++;
		}
		else if(bit_is_clear(PINB,5))
		{
			a[i]=2;
			lcd_order(5*i);
			lcd_prints("B");
			_delay_ms(1000);
			i++;
		}
		else if(bit_is_clear(PINB,6))
		{	
			a[i]=3;
			lcd_order(5*i);
			lcd_prints("C");
			_delay_ms(1000);
			i++;
		}
		else if(bit_is_clear(PINB,7))
		{	
			a[i]=4;
			lcd_order(5*i);
			lcd_prints("D");
			_delay_ms(1000);
			i++;
		}
		if(i==4)
			break;
	}
	_delay_ms(1000);
	lcd_clrscr();
	lcd_init();
	lcd_goto(5,1);
	lcd_prints("Ready!");
	while(!(s2_clr && s3_clr));
}

void order()
{
	int i;
	for(i=0;i<4;i++)
	{	
		switch(a[i])
		{	
			case 1:
				lcd_goto(6,1);
                lcd_prints("A");
                reach(PA);
				break;
			case 2:
				lcd_goto(6,1);
                lcd_prints("B");
                reach(PB);
				break;
			case 3:
				lcd_goto(6,1);
                lcd_prints("C");
                reach(PC);
				break;
			case 4:
				lcd_goto(6,1);
                lcd_prints("D");
                reach(PD);
				break;
			default:
				STOP;
				err();
		}
	}
}
config_wl()
{
	transmit_char('<');
	transmit_char(SID);
	transmit_char(CID);
	transmit_char('>');
}

SIGNAL(SIG_USART_RECV)
{
	char data;
	data=UDR;
}

void lcd_order(char x)
{
           lcd_goto(2,1);
           lcd_prints("Enter Order");
           write_command(0xC0+x);
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
	int i;
	for(i=0;i<a;)
	{	
            if(s1_clr && s2_clr && s3_clr && s4_clr)
			{
				fwd();
				
				while(s1_clr && s2_clr && s3_clr && s4_clr);    
				i++;
				xy();
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
    lcd(X,Y,direct);
}

void c_dir(int b)
{
	int c;
	c=direct-b;
	
	if(c==0);
	if(c==1 || c==-3)
		lft_trn();
	if(c==2 || c==-2)
		abt_trn();
	if(c==3 || c==-1)
		rgt_trn();
    lcd(X,Y,direct);
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
	lcd(X,Y,direct);
}

//Traversing on X-axis

void x_travel(int a)
{
     if(a>0)
             {
                    c_dir(2);
                    stgt(a);
             }
             else if(a<0)
             {
        			a=-a;
                    c_dir(4);
                    stgt(a);
             }
}

//Traversing on Y-axis
void y_travel(int b)
{
     if(b>0)
             {
                    c_dir(1);
                    stgt(b);
             }
        	 else if(b<0)
             {
                    b=-b;
        			c_dir(3);
                    stgt(b);
             }
}

//REACH

void reach(int p,int q)
{
     int a,b;
     
     lcd_goto(4,2);
     lcd_prints("(");
     lcd_goto(5,2);
     lcd_printi(p);
     lcd_goto(6,2);
     lcd_prints(",");
     lcd_goto(7,2);
     lcd_printi(q);
     lcd_goto(8,2);
     lcd_prints(")");
     
     a=p-X;
     b=q-Y;
     
     if((q==EY && ((X<=EX && EX<p) || (p<EX && EX<=X))) || (X==EX && ((Y<=EY && EY<q) || (q<EY && EY<=Y))))
     {
              lcd_goto(15,1);
              lcd_prints("E");
              
             if(a!=0 && b!=0)
             {
                     x_travel(a);
                     y_travel(b);
             }
             else if(a==0)
             {
                  if(X<=2)
                  {
                          c_dir(2);
                          stgt(1);
                          y_travel(b);
                          c_dir(4);
                          stgt(1);
                  }
                  else
                  {
                          c_dir(4);
                          stgt(1);
                          y_travel(b);
                          c_dir(2);
                          stgt(1);
                  }
             }
             else
             {
                  if(Y<=2)
                  {
                          c_dir(1);
                          stgt(1);
                          x_travel(a);
                          c_dir(3);
                          stgt(1);
                  }
                  else
                  {
                          c_dir(3);
                          stgt(1);
                          x_travel(a);
                          c_dir(1);
                          stgt(1);
                  }
             }
     }
     else
     {
      
             lcd_goto(15,1);
             lcd_prints("_");
             y_travel(b);
             x_travel(a);
     }
}

//DISPLAY

void lcd(int a,int b,int c)
{
     lcd_goto(1,1);
     lcd_prints("X");
     lcd_goto(1,2);
     lcd_printi(a);
     
     lcd_goto(11,1);
     lcd_prints("Y");
     lcd_goto(11,2);
     lcd_printi(b);
     
     lcd_goto(14,1);
     lcd_prints("D");
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
