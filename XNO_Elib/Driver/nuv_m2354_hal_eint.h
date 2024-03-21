/* File			: nuv_m2354_hal_eint.h
* Author		:
* Company		: Eoxys Systems India Private Limited
* Description	: EINT driver implementation
* Created on	:
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_public.h>
#include "NuMicro.h"
#include "cmsis_os.h"
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Local definitions
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							static global variables
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 							Global variables
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Local functions
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Exported functions
*----------------------------------------------------------------------------*/
#ifndef DRIVERS_INC_NUV_M2354_HAL_EXTERNAL_IT_H_
#define DRIVERS_INC_NUV_M2354_HAL_EXTERNAL_IT_H_
#ifdef __cplusplus
extern "C" {
#endif


typedef struct XNO_EINT_VAR
{
	XNO_EINT_NUM_EM  eint_num;
	GPIO_T			 *gpioPort;
	uint32_t		 gpioPin;
}XNO_EINT_VAR_ST;

//typedef void (*eint_isr_handler)(XNO_EINT_VAR_ST *eint_var);
/**
  * @brief  EINT interrupt service routine handler
  * @param  eint_var - Pointer to the EINT module
  * @retval None
  *
*/
typedef void (*eint_isr_handler)(void);

typedef struct XNO_EINT_FUNC
{
	eint_isr_handler	isrHandler;
}XNO_EINT_FUNC_ST;

typedef struct XNO_EINT
{
	XNO_EINT_VAR_ST var;
	XNO_EINT_FUNC_ST func;
}XNO_EINT_ST;

extern void EINT_HANDLER(XNO_EINT_NUM_EM eint_num);

extern XNO_ERR_STS XNO_EINT_Init( XNO_EINT_NUM_EM  eint_num,
								XNO_EINT_ST	*eint,
								XNO_EINT_TYPE_EM interruptType,
								GPIO_T *gpioPort,
								uint32_t  gpioPin,
								eint_isr_handler isrHandler);



#ifdef __cplusplus
}
#endif
#endif /* DRIVERS_INC_NUV_M2354_HAL_SPI_H_ */
