/* File			: nuv_m2354_hal_it.c
* Author		:
* Company		: Eoxys Systems India Private Limited
* Description	: Interrupt driver function implementation
* Created on	:
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_can.h>
#include <nuv_m2354_hal_eint.h>
#include <nuv_m2354_hal_uart.h>
#include "NuMicro.h"
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

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
	UART_RXTX_INT_HANDLER(_UART0);
}
void UART1_IRQHandler(void)
{
	UART_RXTX_INT_HANDLER(_UART1);
}
void UART2_IRQHandler(void)
{
	UART_RXTX_INT_HANDLER(_UART2);
}
void UART3_IRQHandler(void)
{
	UART_RXTX_INT_HANDLER(_UART3);
}
void CAN0_IRQHandler(void)
{
	CAN_RXTX_INT_HANDLER(_CAN0);
}
void EINT0_IRQHandler(void)
{
	EINT_HANDLER(_EINT0);
}
void EINT1_IRQHandler(void)
{
	EINT_HANDLER(_EINT1);
}
void EINT2_IRQHandler(void)
{
	EINT_HANDLER(_EINT2);
}
void EINT3_IRQHandler(void)
{
	EINT_HANDLER(_EINT3);
}
void EADC0_IRQHandler(void)
{
	EADC_HANDLER(_EADC0,0);
}
void EADC1_IRQHandler(void)
{
	EADC_HANDLER(_EADC0,1);
}
void EADC2_IRQHandler(void)
{
	EADC_HANDLER(_EADC0,2);
}
void EADC3_IRQHandler(void)
{
	EADC_HANDLER(_EADC0,3);
}
//void I2C0_IRQHandler(void)
//{
//    I2C_RXTX_INT_HANDLE(_I2C0);
//}
	
