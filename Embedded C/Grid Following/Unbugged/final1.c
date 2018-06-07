#include<avr/io.h>
#include<util/delay.h>
#include<compat/deprecated.h>
#include<ilcd.h>
#define s1_clr bit_is_clear(PIND,0)
#define s2_clr bit_is_clear(PIND,1)
#define s3_clr bit_is_clear(PIND,2)
#define s4_clr bit_is_clear(PIND,3)
#define s1_set bit_is_set(PIND,0)
#define s2_set bit_is_set(PIND,1)
#define s3_set bit_is_set(PIND,2)
#define s4_set bit_is_set(PIND,3)
#define fwd PORTC=5
#define right PORTC=6
#define left PORTC=9
#define  A (1,2)  
#define  B (3,2)
#define  C (2,3)
#define  D (2,1)

void stgt(int);
void rgtturn();
void leftturn();
void zigzag(int,int);
void lead(int,int);
void lcd(int,int,int);
void xycount();
void changepath();

int x=2,y=2,dir=1,x1=0,y1=0,col,row;

int main()
{
DDRD=0xF0;
DDRC=0b11111111;
DDRB=0x0F;

lead A;
lead B;
lead C;
lead D;
PORTC=0;
}



void stgt(int a)
{
int i;
for(i=1;i<=a;)
{
	xycount();
	if(s1_clr && s2_clr && s3_clr && s4_clr)
	{
	i++;
	fwd;
	while(PIND==0);
	}
	if(s2_clr && s3_clr && s1_set && s4_set) 
	{
		fwd;
	}
	if(s1_set && s2_set && s3_clr && s4_set)
	{
		right;
	}
	if(s1_set && s2_clr && s3_set && s4_set)
	{
		left;
	}
	--a;
	lcd(x1,y1,dir);
}
PORTC=0;
}

void rgtturn()
{
	if(s2_clr && s3_clr && s1_set && s4_set)
	{
		right;
		while(PIND!=7);
		right;
		while(PIND!=9);
	}
	else
	{
		fwd;
		_delay_ms(1500);
		right;
		while(PIND!=7);
		right;
		while(PIND!=9);
	}
}
void leftturn()
{
	if(s2_clr && s3_clr && s1_set && s4_set)
	{
		left;
		while(PIND!=14);
		left;
		while(PIND!=9);
	}
	else
	{
		fwd;
		_delay_ms(1500);
		left;
		while(PIND!=14);
		left;
		while(PIND!=9);
	}
}
void abtturn()
{
	if(s2_clr && s3_clr && s1_set && s4_set)
	{
		right;
		while(PIND!=7);
		right;
		while(PIND!=9);
		right;
		while(PIND!=7);
		right;
		while(PIND!=9);
	}
	else
	{
		fwd;
		_delay_ms(1500);
		right;
		while(PIND!=7);
		right;
		while(PIND!=9);
		right;
		while(PIND!=7);
		right;
		while(PIND!=9);
	}
}
	
void xycount()
{
	if(dir==1)
	{
		++y1;
	}
	if(dir==2)
	{
		++x1;
	}
	if(dir==3)
	{
		--y1;
	}
	if(dir==4)
	{
		--x1;
	}
	if(x1==x && y1==y)
	{
		if(dir==1)
		{
			--y1;
		}
		if(dir==2)
		{
			--x1;
		}
		if(dir==3)
		{	
			++y1;
		}
		if(dir==4)
		{
			++x1;
		}	
		changepath();
	}
}

void changepath()
{
	if((x1==0 && row>y1 && col>=x1)||(x1==1 && row>y1 && col>=x1)||(x1==2 && row>y1 && col>=x1) && dir==1)
	{
		rgtturn();
		dir=2;
		stgt(1);
		leftturn();
		dir=1;
		lcd(x1,y1,dir);
		lead (col,row);
	}
	else if(dir==1 && (x1==1 && row>y1 && col<=x1)||(x1==2 && row>y1 && col<=x1)||(x1==3 && row>y1 && col<=x1))
	{
		leftturn();
		dir=4;
		stgt(1);
		rgtturn();
		dir=1;
	}
	else if(dir==2 && row>y1)
	{
			leftturn();
			dir=1;
			lead(col,row);
	}
	else if(dir==2 && row<y1)
	{
		rgtturn();
		dir=3;
		stgt(1);
		abtturn();
		dir=1;
		lead (col,row);
	}
	else if(dir==3 && (x1==3 && row<y1 && col<=x1)||(x1==1 && row<y1 && col<=x1)||(x1==2 && row<y1 && col<=x1))
	{
		rgtturn();
		dir=4;
		stgt(1);
		rgtturn();
		dir=1;
		lead(col,row);
	}
	else if(dir==3 && (x1==0 && row<y1 && col>=x1)||(x1==1 && row<y1 && col>=x1)||(x1==2 && row<y1 && col>=x1))
	{
		leftturn();
		dir=2;
		stgt(1);
		leftturn();
		dir=1;
		lead(col,row);
	}
	else if(dir==4 && (x1==3 && row>y1 && col<=x1)||(x1==1 && row>y1 && col<=x1)||(x1==2 && row>y1 && col<=x1))
	{
		rgtturn();
		dir=1;
		lead(col,row);
	}
	else if(dir==4 && (x1==3 && row<y1 && col<=x1)||(x1==1 && row<y1 && col<=x1)||(x1==2 && row<y1 && col<=x1))
	{
		leftturn();
		dir=3;
		stgt(1);
		abtturn();
		dir=1;
		lead(col,row);
	}
	lcd(x1,y1,dir);
}
void lcd(int a,int b,int c)
{
	lcd_init();
	lcd_goto(1,1);
	lcd_prints("X");
	lcd_goto(9,1);
	lcd_prints("Y");
	lcd_goto(13,1);
	lcd_prints("DIR");
	lcd_goto(1,2);
	lcd_printi(a);
	lcd_goto(9,2);
	lcd_printi(b);
	lcd_goto(14,2);
	lcd_printi(c);
}
void lead(int c,int r)
{
	lcd(x1,y1,dir);
	int a,b;
	a=c-x1;
	b=r-y1;
	row=r;
	col=c;
	if(a>0 && b>0)
	{
		
		stgt(b);
		rgtturn();
		dir=2;
		stgt(a);
		leftturn();
		dir=1;
		lcd(x1,y1,dir);
	}

	else if(a<0 && b<0)
	{
		a=a*(-1);
		b=b*(-1);
		leftturn();
		dir=4;
		stgt(a);
		leftturn();
		dir=3;
		stgt(b);
		abtturn();
		dir=1;
		lcd(x1,y1,dir);
	}
	else if(a<0 && b==0)
	{
		a=a*(-1);
		leftturn();
		dir=4;
		stgt(a);
		rgtturn();
		dir=1;
		lcd(x1,y1,dir);
	}
	else if(a<0 && b>0)
	{
		a=a*(-1);
		stgt(b);
		leftturn();
		dir=4;
		stgt(a);
		rgtturn();
		dir=1;
		lcd(x1,y1,dir);
	}
	else if(a==0 && b<0)
	{
		b=b*(-1);
		abtturn();
		dir=3;
		stgt(b);
		abtturn();
		dir=1;
		lcd(x1,y1,dir);
	}
	else if(a==0 && b>0)
	{
		stgt(b);
	}
	else if(a>0 && b<0)
	{
		b=b*(-1);
		rgtturn();
		dir=2;
		stgt(a);
		leftturn();
		dir=3;
		stgt(b);
		abtturn();
		dir=1;
		lcd(x1,y1,dir);
	}
	else if(a>0 && b==0)
	{
		rgtturn();
		dir=2;
		stgt(a);
		leftturn();
		dir=1;
		lcd(x1,y1,dir);
	}
	
	x1=c;
	y1=r;
}	

/*void zigzag(int r,int c)
{
	
	int i;
	stgt(r);
	for(i=1;i<=c;i++)
	{
		if(i%2!=0)
		{
			rgtturn();
			stgt(1);
			rgtturn();
			stgt(r);
		}
		if(i%2==0)
		{
			leftturn();
			stgt(1);
			leftturn();
			stgt(r);
		}
	}
	if(c%2==0)
	{
		abtturn();
		stgt(r);
		rgtturn();
		stgt(c);
		rgtturn();
	}
	if(c%2!=0)
	{
		rgtturn();
		stgt(c);
		rgtturn();
	}
}*/
