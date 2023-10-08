/*
 * adc.h
 *
 *  Created on: Oct 6, 2023
 *      Author: mecam
 */

#ifndef MCAL_ADC_ADC_H_
#define MCAL_ADC_ADC_H_

#include "../std_types.h"
#include "../common_macros.h"
#include <avr/io.h>

#define ADC_READ_MODE_INTERRUPT 1
#define ADC_READ_MODE_POLLING 0
#define ADC_READ_MODE ADC_READ_MODE_INTERRUPT

#define ADC_CHANNELS_MASK 0X1F

typedef enum{
	adc_reference_AREF =     0,
	adc_reference_AVCC =     1,
	adc_reference_internal = 3,
}adc_reference_t;

typedef enum{
	adc_prescaler_2 =      0,
	adc_prescaler_4 =      2,
	adc_prescaler_8 =      3,
	adc_prescaler_16 =     4,
	adc_prescaler_32 =     5,
	adc_prescaler_64 =     6,
	adc_prescaler_128 =    7,
}adc_prescaler_t;

typedef enum{
	adc_channel_0 = 0,
	adc_channel_1 = 1,
	adc_channel_2 = 2,
	adc_channel_3 = 3,
	adc_channel_4 = 4,
	adc_channel_5 = 5,
	adc_channel_6 = 6,
	adc_channel_7 = 7,
}adc_channel_t;

typedef enum{
	adc_auto_trigger_free_running            = 0,
	adc_auto_trigger_analog_comparator       = 1,
	adc_auto_trigger_external_interrupt_0    = 2,
	adc_auto_trigger_timer0_compare_match    = 3,
	adc_auto_trigger_timer0_overflow         = 4,
	adc_auto_trigger_timer1_compare_match_b  = 5,
	adc_auto_trigger_timer1_overflow         = 6,
	adc_auto_trigger_timer1_capture_event    = 7,
}adc_auto_trigger_select_t;

typedef enum{
	adc_interrupt_disable,
	adc_interrupt_enable
}adc_interrupt_state_t;

typedef struct{
	adc_reference_t adc_reference;
	adc_prescaler_t adc_prescaler;
	adc_channel_t adc_default_channel;
	adc_auto_trigger_select_t adc_auto_trigger_select;
	adc_interrupt_state_t adc_interrupt_state;
}adc_config_t;


void adc_init(const adc_config_t * adc_config);
#if ADC_READ_MODE == ADC_READ_MODE_INTERRUPT
void adc_readChannel(uint8 channel);
#elif ADC_READ_MODE == ADC_READ_MODE_POLLING
uint16 adc_ReadChannel(adc_channel_t channel);
#endif
void adc_callbackInit(void(* const callback_ptr)(void));

#endif /* MCAL_ADC_ADC_H_ */
