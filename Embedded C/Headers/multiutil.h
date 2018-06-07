
#include <avr/io.h>    							// The necessary header file required for avr atmega 16/32
#include <util/delay.h>							// Header file required for Delay
#include<compat/deprecated.h>					// Used for the commands like sbi,cbi,bit_is_clear etc..
#include<stdlib.h>

#ifndef multiutil_H
#define multiutil_H


#define LCD_PORT PORTA							// PortA as LCD command port	
#define LCD_DDR  DDRA



#define rs 0									// connect RS pin to LCD_C_PORT ie PA0								// connect RW pin to LCD_C_PORT	ie PA6			
#define en 1									// connect EN pin to LCD_C_PORT ie PA1




void lcd_init(void);							// Function to init lcd ie to start display ,4 bit mode, cursor ON, etc

void write_command(unsigned char comm);			// Function to write command in lcd 
	
void write_data(unsigned char lcd_data);		// Fuction to write data in lcd

void ilabs_lcd_prints(char *print_str);			// Function to print String On LCD

void lcd_printi(int num);

void lcd_clrscr(void);

void lcd_goto(uint8_t,uint8_t);

void lcd_printc(char);

void init_usart(void);
void transmit_char(char);
void transmit_string(char *);

void init_pwm(int mode);

void init_adc(void);
int read_adc(char adc);

void start_i2c(void);
void stop_i2c(void);
void init_i2c(void);
void write_i2c(char);
char read_i2c(char);
 
#endif

//LCD
void lcd_init(void)
{
	_delay_ms(300);
	
	LCD_DDR=0b11110011;					//Initialize the LCD port first
	
	write_command(0x80);					//initial address of DDRAM for first line
	write_command(0x28);					//To select 4 bit data
		
	write_command(0x0e);					//Command to on the display & cursor blink/blink off
	write_command(0x01);					//Command to clear LCD display
	
	write_command(0x06);					//Command for character entry mode
}


void write_command(unsigned char comm)
{
	
	_delay_ms(2);
	LCD_PORT=((comm & 0xF0)|(1<<en)); 	//Sending 4 MSB bits of command, & Enable=1,RW=0,RS=0;
	cbi(LCD_PORT,en);
	
	LCD_PORT=((comm<<4)|(1<<en)); 		//Sending 4 LSB bits of command
	cbi(LCD_PORT,en);
}


 void write_data(unsigned char lcd_data)
{
	_delay_ms(2);
	
	LCD_PORT=((lcd_data & 0xF0)|(1<<en)|(1<<rs)); 	//Sending 4 MSB bits of command, & Enable=1,RW=0,RS=1;
	cbi(LCD_PORT,en);
	
	LCD_PORT=((lcd_data<<4)|(1<<en)|(1<<rs)); 		//Sending 4 LSB bits of command
	cbi(LCD_PORT,en);
}	

void lcd_prints(char *print_str)
{
	while(*print_str)
	{
		if(*print_str=='\0')
		break;
		write_data(*print_str);
		print_str++;
	}
}

void lcd_printi(int num)
{
	char buff[]={'0','0','0','0','0'};
	itoa(num,buff,10);
	lcd_prints(buff);
}

void lcd_printc(char data)
{
		if(data=='\n')
			lcd_goto(0,2);
		write_data(data);
}
void lcd_goto(uint8_t x,uint8_t y)
{
	if(y==1)
		write_command(0x80|x);
	else if(y==2)
		write_command(0xC0|x);
}

void lcd_clrscr(void)
{
	write_command(0x01);
	_delay_ms(3);
	write_command(0x80);
}

//USART
void init_usart()
{
	//Communication protocol set
	//with 1 stop bit & 8 data bits
	UCSRC=(1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
	
	//Reception & transmission is enabled
	//Reception of data will occur on interrupt
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	
	//Baud rate = 9600
	UBRRH=0x00;
	UBRRL=103;
}

void transmit_char(char data)
{
	loop_until_bit_is_set(UCSRA,UDRE);
	UDR=data;
}

void transmit_string(char *data)
{
	while(*data)
	{
		transmit_char(*data);
		data++;
	}
		
}


//PWM
void init_pwm(int mode)
{
	DDRD|=(1<<PD4)|(1<<PD5);
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1);
	TCCR1B|=(1<<CS10)|(1<<CS11);
	switch(mode)
	{
		case 8:TCCR1A|=(1<<WGM10); TCCR1B|=(1<<WGM12);break;
		case 9:TCCR1A|=(1<<WGM11); TCCR1B|=(1<<WGM12);break;
		case 10:TCCR1A|=(1<<WGM10)|(1<<WGM11); TCCR1B|=(1<<WGM12);break;
		default:TCCR1A|=(1<<WGM10); TCCR1B|=(1<<WGM12);break;
	} 
}

//ADC
void init_adc(void)
{
	ADMUX=(1<<REFS0);
	ADCSRA=(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)|(1<<ADEN);
}

int read_adc(char adc)
{
	ADMUX&=(0b11100000);
	ADMUX|=(adc);
	//`ADMUX=0b01
	
	ADCSRA|=(1<<ADEN)|(1<<ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	return ADC;
}

//I2C
void init_i2c(void)
{
	PORTC |= _BV(0)|_BV(1);					
	TWBR=0x47;
}

void start_i2c(void)
{
	TWCR|=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
}

void stop_i2c(void)
{
	TWCR|=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void write_i2c(char data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
}
char read_i2c(char last)
{
	if(last==0)
		TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	else
	{
		TWCR=(1<<TWINT)|(1<<TWEN);
	}
	while((TWCR&(1<<TWINT))==0);
	return TWDR;
}

