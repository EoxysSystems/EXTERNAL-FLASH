/* File			: nuv_m2354_hal_it.h
* Author		:
* Company		: Eoxys Systems India Private Limited
* Description	:
* Created on	: Interrupt driver implementation
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/

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

#ifndef __NUV_M2354_HAL_IT_H_
#define __NUV_M2354_HAL_IT_H_
#ifdef __cplusplus
extern "C" {
#endif

extern void UART0_IRQHandler(void);

extern void UART1_IRQHandler(void);

extern void UART2_IRQHandler(void);

extern void UART3_IRQHandler(void);

extern void CAN0_IRQHandler(void);

//extern void I2C0_IRQHandler(void);

extern void EINT0_IRQHandler(void);
//
extern void EINT1_IRQHandler(void);
//
//extern void EINT2_IRQHandler(void);

//extern void EINT3_IRQHandler(void);

//extern void EADC0_IRQHandler(void);
//
//extern void EADC1_IRQHandler(void);
//
//extern void EADC2_IRQHandler(void);
//
//extern void EADC3_IRQHandler(void);

#ifdef __cplusplus
}
#endif
#endif /* __NUV_M2354_HAL_IT_H_ */
