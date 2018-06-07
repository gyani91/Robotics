
#include <avr/io.h>    							// The necessary header file required for avr atmega 16/32
#include <util/delay.h>							// Header file required for Delay
#include<compat/deprecated.h>					// Used for the commands like sbi,cbi,bit_is_clear etc..
#include<stdlib.h>

#ifndef ilcd_H
#define ilcd_H


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
#endif


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
}
