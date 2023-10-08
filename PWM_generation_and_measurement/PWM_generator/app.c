/*
 * app.c
 *
 *  Created on: Oct 7, 2023
 *      Author: mecam
 */

#include "MCAL/timers/timers.h"
#include "MCAL/adc/adc.h"

void adc_callback(void);
void timer0_callback(void);

volatile uint16 adc_result;
volatile uint8 adc_first_reading_flag = 0;

int main(){

	/*Enable interrupts*/
	SREG |= 1 << 7;


	timer1_PWM_config_t timer1_PWM_config;
	/*for a CPU clock source of 1MHz and a PWM frequency of 100Hz
	 * following the equation from the data sheet (fPWM = fCPU/(Prescaler*(1+TOP)))
	 * the TOP must be 999 */
	timer1_PWM_config.timer1_PWM_TOP = 9999;

	/*for a duty cycle of any value with precision of 0.1% set the compare value
	 * to the duty cycle in percentage multiplied by 10
	 * for example: for a duty cycle of 25% the compare value must be 2500
	 * this value is just an initial value which will be overwritten later when an ADC
	 * value is read
	 * */
	timer1_PWM_config.timer1_PWM_compare_value = 2500;

	/*for non-invertning mode (normal mode)*/
	timer1_PWM_config.timer1_PWM_inversion_select = timer1_PWM_non_inverting;

	/*selecting a prescaler of 1*/
	timer1_PWM_config.timer1_clock_select = timer1_no_prescaler;

	/*disable PWM until an ADC reading is done*/
	timer1_PWM_config.timer1_PWM_state = timer1_PWM_state_disable;

	/*initiate timer1 PWM based on configurations*/
	timer1_PWM_init(&timer1_PWM_config);

	adc_config_t adc_config;

	/*using the VCC as the reference voltage*/
	adc_config.adc_reference = adc_reference_AVCC;

	/*the frequency of the ADC circuit must not exceed 200kHz*/
	adc_config.adc_prescaler = adc_prescaler_16;

	/*Set a default channel -> will be the used channel
	 * if ADC is running in a non-free-running mode*/
	adc_config.adc_default_channel = adc_channel_0;

	/*ADC is used in interrupt mode*/
	adc_config.adc_interrupt_state = adc_interrupt_enable;

	/*initiate ADC to run in free mode. conversion will start on every timer0 ISR call*/
	adc_config.adc_auto_trigger_select = adc_auto_trigger_free_running;

	/*Initiate ADC on interrupt callback*/
	adc_callbackInit(adc_callback);

	/*initiate ADC based on configurations*/
	adc_init(&adc_config);

	/*timer0 configuration for ADC periodic process*/
	timer0_CTC_config_t timer0_config;

	/*for a cpu frequency of 1MHz setting timer0 prescaler to 64 will make it's period 0.064 ms
	 * then for a periodic ADC start every 5ms the timer0 compare value has to be
	 * 5/0.064 ~ 78;*/
	timer0_config.timer0_clock_select = timer0_prescaler_256;
	timer0_config.timer0_compare_value = 200;

	/*timer0 interrupt is enabled to start an ADC conversion on every timer0 ISR*/
	timer0_config.timer0_interrupt_state = timer0_interrupt_enable;

	/*set the call back function for timer0*/
	timer0_callbackInit(timer0_callback);

	timer0_compare_mode_init(&timer0_config);

	while(1){

	}
}

void adc_callback(void){
	adc_result = ADC;
	timer1_PWMSetCompareValue(adc_result*10);
	if(adc_first_reading_flag == 0){
		adc_first_reading_flag = 1;
		timer1_PWM_enable();
	}
}

void timer0_callback(void){
	adc_readChannel(adc_channel_0);
}
