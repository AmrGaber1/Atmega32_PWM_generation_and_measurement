/*
 * lcd.h
 *
 *  Created on: Sep 30, 2023
 *      Author: mecam
 */

#ifndef LCD_H_
#define LCD_H_

#include "../../mcal/gpio/gpio.h"
#include <util/delay.h>

#define LCD_RS_PORT PORTA_ID
#define LCD_RS_PIN PIN0_ID

#define LCD_EN_PORT PORTA_ID
#define LCD_EN_PIN PIN1_ID

#define LCD_DATA_BUS_PORT PORTA_ID
#define LCD_DATA_BUS_PORT_OFFSET 2
#define LCD_DATA_BUS_SIZE 4

#if LCD_DATA_BUS_SIZE == 4
#define LCD_DATA_BUS_MASK 0x3C

#elif LCD_DATA_BUS_SIZE == 8
#define LCD_DATA_BUS_MASK 0xFD

#else
#error "Invalid data bus size"
#endif

#define LCD_COMMAND_CLEAR_DISPLAY 0X01
#define LCD_COMMAND_RETURN_HOME 0X02
#define LCD_COMMAND_DISPLAY_OFF 0X08
#define LCD_COMMAND_DISPLAY_ON_CURSOR_OFF 0X0C
#define LCD_COMMAND_DISPLAY_ON_CURSOR_ON 0X0E
#define LCD_COMMAND_2_LINE_8_BIT 0X38
#define LCD_COMMAND_2_LINE_4_BIT 0X28
#define LCD_COMMAND_ENTRY_MODE_CURSOR_INCREAMENT_NO_SHIFT 0X06

void lcd_init(void);
void lcd_sendCommand(uint8 command);
void lcd_displayCharacter(uint8 character);
void lcd_displayString(const char * str);
void lcd_displaySubString(const char * str, uint32 length);
void lcd_setPosition(uint8 row, uint8 col);
void lcd_displayNumber(sint32 number);
#endif /* LCD_H_ */
