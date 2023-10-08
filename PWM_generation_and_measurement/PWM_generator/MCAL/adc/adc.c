/*
 * adc.c
 *
 *  Created on: Oct 6, 2023
 *      Author: mecam
 */

#include "adc.h"
#include "avr/interrupt.h"

#if ADC_READ_MODE == ADC_READ_MODE_INTERRUPT
void(* volatile adc_callback_local)(void);
#endif

void adc_init(const adc_config_t * adc_config){
	/*set the selected ADC reference*/
	ADMUX = (ADMUX & ~(0xC0)) | ((adc_config -> adc_reference) << 6);

	/*select right adjust of ADC result*/
	ADMUX &= ~(1 << ADLAR);

	/*set the initial channel to the selected one*/
	ADMUX = (ADMUX & ~(ADC_CHANNELS_MASK)) | (adc_config -> adc_default_channel & 0X1F);

	/*clear the interrupt flag before enabling interrupt*/
	ADCSRA |= (1 << ADIF);

	/*enable interrupt or disable it based on the configuration*/
	if(adc_config -> adc_interrupt_state == adc_interrupt_enable){
		ADCSRA |= (1 << ADIE);
	}else{
		ADCSRA &= ~(1 << ADIE);
	}

	/*select the ADC auto trigger mode*/
	SFIOR = (SFIOR & ~(0xE0)) | (((adc_config->adc_auto_trigger_select) << 5) & 0xE0);

	/*if an auto trigger mode is selected set this bit to enable auto trigger*/
	if(((adc_config->adc_auto_trigger_select) >= 1)&&((adc_config->adc_auto_trigger_select) <= 7)){
		ADCSRA |= (1 << ADATE);
	}

	/*set the selected prescaler*/
	ADCSRA = (ADCSRA & ~(0X07)) | (adc_config -> adc_prescaler & 0X07);
	ADCSRA |= (1 << ADEN);
}

#if ADC_READ_MODE == ADC_READ_MODE_INTERRUPT
void adc_readChannel(adc_channel_t channel){
	ADMUX = (ADMUX & ~(ADC_CHANNELS_MASK)) | (channel & 0X1F);
	ADCSRA |= (1<<ADSC);
}
#elif ADC_READ_MODE == ADC_READ_MODE_POLLING
uint16 adc_readChannel(adc_channel_t channel){
	ADMUX = (ADMUX & ~(ADC_CHANNELS_MASK)) | (channel & 0X1F);
	ADCSRA |= (1<<ADSC);
	while(!((1<<ADIF)&ADCSRA));
	ADCSRA &= ~(1<<ADIF);
	return ADC;
}
#endif

#if ADC_READ_MODE == ADC_READ_MODE_INTERRUPT
/*This function is used to initiate the callback function for the ADC interrupt*/
void adc_callbackInit(void(* const callback_ptr)(void)){
	adc_callback_local = callback_ptr;
}
ISR(ADC_vect){
	if(adc_callback_local != NULL_PTR){
		adc_callback_local();
	}
}
#endif
