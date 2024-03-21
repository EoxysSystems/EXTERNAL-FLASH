/* File: TIMEOUT_TIMERS.h
* Author: Devika
* Company: Eoxys
* Description: Timer-related APIs
* Created on:  10-Jul-2022
* Version History
* v1.0 Initial release
*/

/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "nuv_m2354_hal_public.h"
/*----------------------------------------------------------------------------
* 							Defines
*----------------------------------------------------------------------------*/

#define TIMER_ONE_SHOT	 0
#define TIMER_PERIODIC	 1

#define TIMEOUT_1_MIN		60000UL
#define TIMEOUT_3_MIN		180000UL
#define TIMEOUT_5_MIN		300000UL
#define TIMEOUT_60_SECS 	60000UL
#define TIMEOUT_50_SECS		50000UL
#define TIMEOUT_35_SECS 	35000UL
#define TIMEOUT_30_SECS		30000UL
#define TIMEOUT_25_SECS 	25000UL
#define TIMEOUT_20_SECS		20000UL
#define TIMEOUT_15_SECS		15000UL
#define TIMEOUT_10_SECS		10000UL
#define TIMEOUT_5_SECS		5000UL
#define TIMEOUT_3_SECS  	3000UL
#define TIMEOUT_2_SECS		2000UL
#define TIMEOUT_1_SECS		1000UL
#define TIMEOUT_500_MSECS	500UL
#define INDEFINITE_TIMEOUT   0xFFFFFFFF

#ifndef INC_XNO_OS_TIMERS_H_
#define INC_XNO_OS_TIMERS_H_
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
typedef struct{
	osTimerId TimeoutHandle;
	uint8_t timer_type;
	uint32_t timeout;
	FlagStatus timeout_flag;
}XNO_OSTIMER_VAR_ST;
/*----------------------------------------------------------------------------
* 							Exported functions
*----------------------------------------------------------------------------*/
//User functions
typedef void (*startTimer_func)(XNO_OSTIMER_VAR_ST *var,uint32_t timeout);
typedef void (*stopTimer_func)(XNO_OSTIMER_VAR_ST *var);
typedef FlagStatus (*isTimeoutOccurred_func)(XNO_OSTIMER_VAR_ST *var);
//Callback  function
typedef void (*TIMER_CB_FUNC)( void);
typedef struct{
	startTimer_func startTimer;
	stopTimer_func stopTimer;
	isTimeoutOccurred_func isTimeoutOccurred;
	TIMER_CB_FUNC timer_cb_func;
}XNO_OSTIMER_FUNC_ST;
typedef struct{
	XNO_OSTIMER_VAR_ST var;
	XNO_OSTIMER_FUNC_ST func;
}XNO_OSTIMER_ST;

extern XNO_ERR_STS XNO_OSTIMER_Init(XNO_OSTIMER_ST *ostmr,uint8_t timer_type,TIMER_CB_FUNC timer_cb_func);
#endif /* INC_XNO_OS_TIMERS_H_ */
