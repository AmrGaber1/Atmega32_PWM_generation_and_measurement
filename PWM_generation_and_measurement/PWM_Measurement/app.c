/*
 * app.c
 *
 *  Created on: Oct 7, 2023
 *      Author: mecam
 */

#include "mcal/timers/timers.h"
#include "hal/lcd/lcd.h"

void timer1_ICU_callback(void);

uint8 icu_count_edges = 0;
uint16 ICU_captures_based_time = 0;
uint16 ICU_captures_high_time_end = 0;
uint16 ICU_captures_low_time_end = 0;
uint8 ICU_capturedPWNDutyCycle;
uint16 ICU_CapturedPWMFrequency;
uint8 ICU_CapturedPWMflag = 0;

int main(){
	/*enable global interrupt*/
	SREG |= 1 << 7;

	/*initiate LCD*/
	lcd_init();
	/*display text on the LCD*/
	lcd_setPosition(0, 0);
	lcd_displayString("Freq: ");
	lcd_setPosition(1, 0);
	lcd_displayString("Duty Cycle: ");

	timer1_ICU_config_t timer1_ICU_config;
	/*initially set ICU to trigger a rising edge*/
	timer1_ICU_config.timer1_ICU_edge_select = timer1_ICU_rising_edge;
	/*timer1 runs with the same clock as CPU*/
	timer1_ICU_config.timer1_clock_select = timer1_no_prescaler;
	/*initiate call back function*/
	timer1_ICU_callbackInit(timer1_ICU_callback);
	/*initiate ICU with the configurations chosen*/
	timer1_ICU_init(&timer1_ICU_config);
	while(1){
	}
}

void timer1_ICU_callback(void){
	if(icu_count_edges == 0){
		TCNT1 = 0;
		icu_count_edges++;
	}else if(icu_count_edges == 1){
		icu_count_edges++;
	}else if(icu_count_edges == 2){
		timer1_ICU_edgeSelect(timer1_ICU_falling_edge);
		icu_count_edges++;
		ICU_captures_based_time = timer1_ICU_getCapturedValue();
	}else if(icu_count_edges == 3){
		timer1_ICU_edgeSelect(timer1_ICU_rising_edge);
		icu_count_edges++;
		ICU_captures_high_time_end = timer1_ICU_getCapturedValue();
	}else if(icu_count_edges == 4){
		TCNT1 = 0;
		timer1_ICU_disable();
		icu_count_edges = 0;
		ICU_captures_low_time_end = timer1_ICU_getCapturedValue();
		ICU_capturedPWNDutyCycle =
				(((uint32)ICU_captures_high_time_end - (uint32)ICU_captures_based_time) * 100UL) /
				(((uint32)ICU_captures_low_time_end - (uint32)ICU_captures_based_time));
		ICU_CapturedPWMFrequency = (1000000UL) /
				(((uint32)ICU_captures_low_time_end - (uint32)ICU_captures_based_time));
		lcd_setPosition(0, 6);
		lcd_displayNumber(ICU_CapturedPWMFrequency);
		lcd_displayString(" HZ  ");
		lcd_setPosition(1, 12);
		lcd_displayNumber(ICU_capturedPWNDutyCycle);
		lcd_displayString(" %  ");
	}
}

