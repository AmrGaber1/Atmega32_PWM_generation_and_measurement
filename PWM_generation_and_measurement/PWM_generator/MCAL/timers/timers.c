 /******************************************************************************
 *
 * Module: Timers
 *
 * File Name: timers.c
 *
 * Description: Source file for the timers drivers
 *
 * Author: Amr Gaber
 *
 *******************************************************************************/
 /*******************************************************************************
 *                                Includes
 *******************************************************************************/
#include "avr/io.h"
#include "avr/interrupt.h"
#include "timers.h"
 /*******************************************************************************
 *             Functions Prototypes and static variables(Private)
 *******************************************************************************/
static timer1_clock_select_t PWM_clock_select = timer1_no_clock_source;
static timer1_clock_select_t ICU_clock_select = timer1_no_clock_source;
static void (* volatile ICU_callback_local)(void) = NULL_PTR;
static void (* volatile timer0_callback_local)(void) = NULL_PTR;
 /*******************************************************************************
 *                      Functions Definitions
 *******************************************************************************/
/*
 * This function initiates timer0 to run in compare mode
 * */
void timer0_compare_mode_init(const timer0_CTC_config_t * timer0_CTC_config){
	/*This bit must be set when timer0 is used in a non-PWM mode*/
	TCCR0 |= (1<<FOC0);

	/*The 2 bits WGM00 and WGM01 must be set to 0,1 consecutively*/
	TCCR0 |= (1 << WGM01);
	TCCR0 &= ~(1 << WGM00);

	/*Adjust the compare value per the configured value*/
	OCR0 = timer0_CTC_config -> timer0_compare_value;

	/*clear the interrupt flag*/
	TIFR |= (1 << OCF0);

	/*enable or disable interrupt per the configured value*/
	if((timer0_CTC_config -> timer0_interrupt_state) == timer0_interrupt_enable){
		TIMSK |= (1 << OCIE0);
	}else{
		TIMSK &= ~(1 << OCIE0);
	}

	/*set the prescaler to the selected one*/
	TCCR0 = (TCCR0 & ~(0x07))| ((timer0_CTC_config -> timer0_clock_select)&0x07);

}

/*This function is used to initialize timer0 callback function*/
void timer0_callbackInit(void(* timer0_callbackFunction)(void)){
	timer0_callback_local = timer0_callbackFunction;
}

/*
 * This function initializes the timer1 to run in PWM mode
 * the value of the TOP(OCR1B) and the compare value(OCR1A) controls the period
 * and the duty cycle
 * the frequency is calculated based on the following equation
 * (fPWM = fCPU/(Prescaler*(1+TOP))
 * with the compare value, the TOP value, the prescaler and the inversion selection as
 * input to the function
 * */
void timer1_PWM_init(const timer1_PWM_config_t * timer1_PWM_config){
	if(timer1_PWM_config != NULL_PTR){
		/*Select either inverting or non_inverting mode*/
		if(timer1_PWM_config -> timer1_PWM_inversion_select == timer1_PWM_inverting){
			TCCR1A = (TCCR1A & ~(3 << 4)) | ((timer1_PWM_inverting & 0x03) << 4);
		}else if(timer1_PWM_config -> timer1_PWM_inversion_select == timer1_PWM_non_inverting){
			TCCR1A = (TCCR1A & ~(3 << 4)) | ((timer1_PWM_non_inverting & 0x03) << 4);
		}
		/*Clear FOC1A and FOC1B bits for PWM mode*/
		TCCR1A &= ~(3 << 2);

		/*Select Fast PWM mode with OCR1A as the TOP*/
		TCCR1A |= (1 << WGM10) | (1 << WGM11);
		TCCR1B |= (1 << WGM12) | (1 << WGM13);

		/*Set the OCR1B to the compare value*/
		OCR1B = timer1_PWM_config -> timer1_PWM_compare_value;

		/*set the OCR1A to the TOP value*/
		OCR1A = timer1_PWM_config -> timer1_PWM_TOP;

		/*set the pin OC1B as output*/
		DDRD |= 1<<PD4;

		/*Enable the clock -> set the prescaler and store it for later PWM enable*/
		PWM_clock_select = timer1_PWM_config -> timer1_clock_select;

		/*Enable or disable PWM based on the given value*/
		if((timer1_PWM_config -> timer1_PWM_state) == timer1_PWM_state_enable){
			TCCR1B = (TCCR1B & (0xF8)) | (PWM_clock_select & 0x07);
		}else if((timer1_PWM_config -> timer1_PWM_state) == timer1_PWM_state_disable){
			TCCR1B = (TCCR1B & (0xF8)) | (timer1_no_clock_source & 0x07);
		}

	}
}

/*
 * This function is used to change compare value which changes the duty cycle
 * */
void timer1_PWMSetCompareValue(const uint16 timer1_compare_value){
	OCR1B = timer1_compare_value;
}

/*
 * This function deinitializes the timer1 to the reset state to make timer1
 * available for other uses
 **/
void timer1_PWM_deinit(void){
	/*set both TCCR1A and TCCR1B to 0 which is the reset value*/
	TCCR1A = 0;
	TCCR1B = 0;

	/*clear both OCR1A and OCR1B*/
	OCR1A = 0;
	OCR1B = 0;
}

/*
 * disable timer1 clock which stops timer1 and PWM consecutively
 * */
void timer1_PWM_disable(void){
	TCCR1B = (TCCR1B & (0xF8)) | ((timer1_no_clock_source & 0x07));
}

/*
 * re-enable timer1 clock which enables timer1 and PWM consecutively
 * */
void timer1_PWM_enable(void){
	TCCR1B = (TCCR1B & (0xF8)) | ((PWM_clock_select & 0x07));
}

/*
 * This function initialized timer1 to be used as an input capture unit
 * the trigger edge (falling or rising) and the prescaler are selected based on
 * the based configurations
 * */
void timer1_ICU_init(const timer1_ICU_config_t * timer1_ICU_config){
	if(timer1_ICU_config != NULL_PTR){
		/*configure timer1 to work in overflow mode*/
		TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
		TCCR1B &= ~((1 << WGM12) | (1 << WGM13));

		/*Set the ICES1 (ICU edge select) bit to the configured bit*/
		TCCR1B = (TCCR1B & ~(1 << ICES1)) |
					((timer1_ICU_config -> timer1_ICU_edge_select & 0x01) << ICES1);

		/*Set timer1 clock*/
		ICU_clock_select = timer1_ICU_config -> timer1_clock_select;
		TCCR1B = (TCCR1B & (0xF8)) | ((ICU_clock_select & 0x07));

		/*these bits are set when operating in any non_PWM mode*/
		TCCR1A |= (1 << FOC1A) | (1 << FOC1B);

		/*Clear the interrupt flag*/
		TIFR |= (1 << ICF1);
		/*enable ICU interrupt*/
		TIMSK |= (1 << TICIE1);
	}
}

/*
 * This function deinitializes the timer1 to the reset state to make timer1
 * available for other uses
 **/
void timer1_ICU_deinit(void){
	/*set both TCCR1A and TCCR1B to 0 which is the reset value*/
	TCCR1A = 0;
	TCCR1B = 0;

	/*disable ICU interrupt*/
	TIMSK &= ~(1 << TICIE1);
}

/*
 * This function stops The ICU clock
 * */
void timer1_ICU_disable(void){
	TIFR &= ~(1 << ICF1);
}

/*
 * This function starts The ICU clock based on the previously set value
 * */
void timer1_ICU_enable(void){
	TIFR |= (1 << ICF1);
}
/*
 * This function returns the value stored in the 16-bit input capture unit register
 * */
uint16 timer1_ICU_getCapturedValue(void){
	return ICR1;
}

/*
 * This is function is used to give a value to the call back function to be called
 * by the ISR of the ICU
 * */
void timer1_ICU_callbackInit(void(* ICU_callbackFunction)(void)){
	ICU_callback_local = ICU_callbackFunction;
}

/*
 * This function is used to change the trigger edge for the ICU in runtime
 * */
void timer1_ICU_edgeSelect(const timer1_ICU_edge_select_t timer1_ICU_edge_select){
	/*Set the ICES1 (ICU edge select) bit to the selected bit*/
			TCCR1B = (TCCR1B & ~(1 << ICES1)) |
						((timer1_ICU_edge_select & 0x01) << ICES1);
}

ISR(TIMER0_COMP_vect){
	/*check if the user gave a value to the callback function*/
	if(timer0_callback_local != NULL_PTR){
		/*call the callback function on interrupt if it has a value*/
		(*timer0_callback_local)();
	}
}

ISR(TIMER1_CAPT_vect){
	/*check if the user gave a value to the callback function*/
	if(ICU_callback_local != NULL_PTR){
		/*call the callback function on interrupt if it has a value*/
		(*ICU_callback_local)();
	}
}


