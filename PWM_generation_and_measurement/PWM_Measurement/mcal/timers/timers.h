 /******************************************************************************
 *
 * Module: Timers
 *
 * File Name: timers.h
 *
 * Description: Header file for the timers drivers
 *
 * Author: Amr Gaber
 *
 *******************************************************************************/

#ifndef MCAL_TIMERS_TIMERS_H_
#define MCAL_TIMERS_TIMERS_H_
 /*******************************************************************************
 *                                Includes
 *******************************************************************************/
#include "../common_macros.h"
#include "../std_types.h"
#include "avr/io.h"
/*******************************************************************************
 *                       User-defined type difinitions
 *******************************************************************************/
typedef enum{
	timer0_no_clock_source = 0,
	timer0_no_prescaler,
	timer0_prescaler_8,
	timer0_prescaler_64,
	timer0_prescaler_256,
	timer0_prescaler_1024,
}timer0_clock_select_t;

typedef enum{
	timer0_interrupt_disable,
	timer0_interrupt_enable,
}timer0_interrupt_state_t;

typedef enum{
	timer1_no_clock_source = 0,
	timer1_no_prescaler,
	timer1_prescaler_8,
	timer1_prescaler_64,
	timer1_prescaler_256,
	timer1_prescaler_1024,
}timer1_clock_select_t;

typedef enum{
	timer1_PWM_state_disable,
	timer1_PWM_state_enable
}timer1_PWM_state_t;

typedef enum{
	timer1_PWM_non_inverting = 2,
	timer1_PWM_inverting = 3
}timer1_PWM_inversion_t;

typedef enum{
	timer1_ICU_falling_edge,
	timer1_ICU_rising_edge
}timer1_ICU_edge_select_t;

typedef struct{
	timer0_clock_select_t timer0_clock_select;
	uint8 timer0_compare_value;
	timer0_interrupt_state_t timer0_interrupt_state;
}timer0_CTC_config_t;

typedef struct{
	timer1_PWM_state_t timer1_PWM_state;
	timer1_PWM_inversion_t timer1_PWM_inversion_select;
	timer1_clock_select_t timer1_clock_select;
	uint16 timer1_PWM_compare_value;
	uint16 timer1_PWM_TOP;
}timer1_PWM_config_t;

typedef struct{
	timer1_clock_select_t timer1_clock_select;
	timer1_ICU_edge_select_t timer1_ICU_edge_select;
}timer1_ICU_config_t;
/*******************************************************************************
 *                                 Macros
 *******************************************************************************/

 /*******************************************************************************
 *                           Functions Prototypes
 *******************************************************************************/
/*
 * This function initiates timer0 to run in compare mode
 * */
void timer0_compare_mode_init(const timer0_CTC_config_t * timer0_CTC_config);

/*This function is used to initialize timer0 callback function*/
void timer0_callbackInit(void(* timer0_callbackFunction)(void));
/*
 * This function initializes the timer1 to run in PWM mode
 * the value of the TOP(OCR1B) and the compare value(OCR1A) controls the period
 * and the duty cycle
 * the frequency is calculated based on the following equation
 * (fPWM = fCPU/(Prescaler*(1+TOP))
 * with the compare value, the TOP value, the prescaler and the inversion selection as
 * input to the function
 * */
void timer1_PWM_init(const timer1_PWM_config_t * timer1_PWM_config);

/*
 * This function is used to change compare value which changes the duty cycle
 * */
void timer1_PWMSetCompareValue(const uint16 timer1_compare_value);

/*
 * This function deinitializes the timer1 to the reset state to make timer1
 * available for other uses
 **/
void timer1_PWM_deinit(void);

/*
 * disable timer1 clock which stops timer1 and PWM consecutively
 * */
void timer1_PWM_disable(void);

/*
 * re-enable timer1 clock which enables timer1 and PWM consecutively
 * */
void timer1_PWM_enable(void);

/*
 * This function initialized timer1 to be used as an input capture unit
 * the trigger edge (falling or rising) and the prescaler are selected based on
 * the based configurations
 * */
void timer1_ICU_init(const timer1_ICU_config_t * timer1_ICU_config);

/*
 * This function deinitializes the timer1 to the reset state to make timer1
 * available for other uses
 **/
void timer1_ICU_deinit(void);

/*
 * This function stops The ICU clock
 * */
void timer1_ICU_disable(void);

/*
 * This function starts The ICU clock based on the previously set value
 * */
void timer1_ICU_enable(void);


/*
 * This is function is used to give a value to the call back function to be called
 * by the ISR of the ICU
 * */
void timer1_ICU_callbackInit(void(* ICU_callbackFunction)(void));

/*
 * This function returns the value stored in the 16-bit input capture unit register
 * */
uint16 timer1_ICU_getCapturedValue(void);

/*
 * This function is used to change the trigger edge for the ICU in runtime
 * */
void timer1_ICU_edgeSelect(const timer1_ICU_edge_select_t timer1_ICU_edge_select);
#endif /* MCAL_TIMERS_TIMERS_H_ */
