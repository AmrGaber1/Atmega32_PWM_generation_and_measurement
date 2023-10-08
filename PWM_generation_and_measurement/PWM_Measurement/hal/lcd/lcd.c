/*
 * lcd.c
 *
 *  Created on: Sep 30, 2023
 *      Author: mecam
 */


#include "lcd.h"
#include <stdlib.h>
#if LCD_DATA_BUS_SIZE == 8
void lcd_sendCommand(uint8 command){
	GPIO_writePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_LOW);
	_delay_us(1);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(1);
	GPIO_writePort(LCD_DATA_BUS_PORT, command);
	_delay_us(1);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	if(command == LCD_COMMAND_DISPLAY_OFF){
		_delay_ms(2);
	}else{
		_delay_us(100);
	}
}
#elif LCD_DATA_BUS_SIZE == 4
void lcd_sendCommand(uint8 command){
	GPIO_writePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, ((command & 0xF0) >> 4) << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, ((command & 0x0F)) << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);

	if(command == LCD_COMMAND_DISPLAY_OFF){
		_delay_ms(3);
	}else{
		_delay_us(100);
	}
}
#endif


#if LCD_DATA_BUS_SIZE == 8

void lcd_init(void){
	GPIO_setupPinDirection(LCD_RS_PORT, LCD_RS_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_EN_PORT, LCD_EN_PIN, PIN_OUTPUT);
	GPIO_setupPortDirection(LCD_DATA_BUS_PORT, 0xFF);
	_delay_ms(20);
	lcd_sendCommand(LCD_COMMAND_2_LINE_8_BIT);
	lcd_sendCommand(LCD_COMMAND_DISPLAY_ON_CURSOR_OFF);
	lcd_sendCommand(LCD_COMMAND_CLEAR_DISPLAY);
}
#elif LCD_DATA_BUS_SIZE == 4
void lcd_init(void){
	GPIO_setupPinDirection(LCD_RS_PORT, LCD_RS_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_EN_PORT, LCD_EN_PIN, PIN_OUTPUT);
	GPIO_setupPortDirectionMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, PORT_OUTPUT);
	_delay_ms(20);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	GPIO_writePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, 3 << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_ms(5);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, 3 << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(600);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, 3 << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);

	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, 2 << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(600);
	lcd_sendCommand(LCD_COMMAND_2_LINE_4_BIT);
	lcd_sendCommand(LCD_COMMAND_DISPLAY_OFF);
	lcd_sendCommand(LCD_COMMAND_CLEAR_DISPLAY);
	lcd_sendCommand(LCD_COMMAND_ENTRY_MODE_CURSOR_INCREAMENT_NO_SHIFT);
	lcd_sendCommand(LCD_COMMAND_RETURN_HOME);
	lcd_sendCommand(LCD_COMMAND_DISPLAY_ON_CURSOR_OFF);

}
#endif

#if LCD_DATA_BUS_SIZE == 8

void lcd_displayCharacter(uint8 character){
	GPIO_writePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_HIGH);
	_delay_us(1);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(1);
	GPIO_writePort(LCD_DATA_BUS_PORT, character);
	_delay_us(1);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
}
#elif LCD_DATA_BUS_SIZE == 4
void lcd_displayCharacter(uint8 character){
	GPIO_writePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, ((character & 0xF0) >> 4) << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_HIGH);
	_delay_us(50);
	GPIO_writePortMasked(LCD_DATA_BUS_PORT, LCD_DATA_BUS_MASK, ((character & 0x0F)) << LCD_DATA_BUS_PORT_OFFSET);
	_delay_us(50);
	GPIO_writePin(LCD_EN_PORT, LCD_EN_PIN, LOGIC_LOW);
	_delay_us(50);
}
#endif

void lcd_displayString(const char * str){
	while(*str != '\0'){
		lcd_displayCharacter(*str);
		str++;
	}
}

void lcd_displaySubString(const char * str, uint32 length){
	uint32 temp_count = 0;
	while((temp_count < length) && (*str != '\0')){
		lcd_displayCharacter(*str);
		str++;
		temp_count++;
	}
}

void lcd_setPosition(uint8 row, uint8 col){
	while(col > 15){
		col -= 16;
		row++;
	}
	if((row < 4)&&(col < 16)){
		uint8 address;
		switch(row){
			case 0:
				address = (0x00 + col)|0x80;
				lcd_sendCommand(address);
				break;
			case 1:
				address = (0x40 + col)|0x80;
				lcd_sendCommand(address);
				break;
			case 2:
				address = (0x10 + col)|0x80;
				lcd_sendCommand(address);
				break;
			case 3:
				address = (0x50 + col)|0x80;
				lcd_sendCommand(address);
				break;
		}
	}
}

void lcd_displayNumber(sint32 number){
	char strBuffer[16];
	ltoa(number, strBuffer, 10);
	lcd_displayString(strBuffer);
}
