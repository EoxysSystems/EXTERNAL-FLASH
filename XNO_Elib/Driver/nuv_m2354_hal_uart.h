/* File			: nuv_m2354_hal_uart.h
* Author		:
* Company		: Eoxys Systems India Private Limited
* Description	: UART driver implementation
* Created on	:
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_public.h>
#include "NuMicro.h"
#include "cmsis_os.h"
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Local definitions
*----------------------------------------------------------------------------*/
#define XNO_UART_BAUDRATE_9600					(9600UL)
#define XNO_UART_BAUDRATE_19200					(19200UL)
#define XNO_UART_BAUDRATE_38400					(38400UL)
#define XNO_UART_BAUDRATE_57600					(57600UL)
#define XNO_UART_BAUDRATE_115200				(115200UL)
#define XNO_UART_BAUDRATE_230400				(230400UL)
#define XNO_UART_BAUDRATE_460800				(460800UL)
#define XNO_UART_BAUDRATE_921600				(921600UL)

#define UART_DEFAULT_BAUDRATE					XNO_UART_BAUDRATE_115200
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
#ifndef __NUV_M2354_HAL_HANDLE_UART_H_
#define __NUV_M2354_HAL_HANDLE_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

struct XNO_UART_VAR
{
	UART_T* 			interface;
	XNO_UART_NUM_EM		num;
	uint32_t 			baudRate;
	uint32_t 			timeOutCnt;
	uint32_t            dataWidth;
	uint32_t            dataParity;
	uint32_t            stopBits;

	uint8_t*			txBuff;
	uint32_t			txLen;
	uint32_t			txCounter;
	osSemaphoreId		txLock;
	osSemaphoreDef_t	txLockDef;

	uint8_t*			rxBuff;
	uint32_t			rxLen;
	uint32_t			rxCounter;
	osSemaphoreId		rxLock;
	osSemaphoreDef_t	rxLockDef;
};
typedef struct XNO_UART_VAR XNO_UART_VAR_ST;

/**
  * @brief  UART complete call back function
  * @param  uart_var  - Pointer to specified UART module
  * @retval None
  */
typedef void (*uart_cplt_cb)(XNO_UART_VAR_ST *uart_var);

/**
  * @brief  Uart Transmit function
  * @param  uart_var  - Pointer to specified UART module
  * @param  txbuf - Pointer to the transmit buffer
  * @param 	txlen - Transmit length
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*uart_transmit_func)(XNO_UART_VAR_ST *uart_var, uint8_t *txbuf, uint32_t txlen);

/**
  * @brief  Uart Receive function
  * @param  uart_var  - Pointer to specified UART module
  * @param  rxbuf - Pointer to the Receive buffer
  * @param 	rxlen - Receive length
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*uart_receive_func)(XNO_UART_VAR_ST *uart_var, uint8_t *rxbuf, uint32_t rxlen);

/**
  * @brief  Uart transmit interupt function
  * @param  uart_var  - Pointer to specified UART module
  * @param  txbuf - Pointer to the transmit buffer
  * @param 	txlen - transmit length
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*uart_transmit_it_func)(XNO_UART_VAR_ST *uart_var, uint8_t *txbuf, uint32_t txlen);

//*Uart Receive Interupt function*/
/**
  * @brief  Uart Receive interupt function
  * @param  uart_var  - Pointer to specified UART module
  * @param  rxbuf - Pointer to the Receive buffer
  * @param 	rxlen - Receive length
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*uart_receive_it_func)(XNO_UART_VAR_ST *uart_var, uint8_t *rxbuf, uint32_t rxlen);

struct XNO_UART_FUNC
{
	uart_cplt_cb		            txCpltCb;
	uart_cplt_cb		            rxCpltCb;
	uart_cplt_cb		            errCb;
	uart_transmit_func		        uartTransmit;
	uart_transmit_it_func	        uartTransmitInt;
	uart_receive_func		        uartReceive;
	uart_receive_it_func	        uartReceiveInt;
};
typedef struct XNO_UART_FUNC XNO_UART_FUNC_ST;

struct XNO_UART
{
	XNO_UART_VAR_ST var;
	XNO_UART_FUNC_ST func;
};
typedef struct XNO_UART XNO_UART_ST;

XNO_ERR_STS XNO_UART_Init(UART_T *interface,
							XNO_UART_ST *uart,
							XNO_UART_NUM_EM uart_num,
							uint32_t baudRate,
							uint32_t timeOutInSecs,
							uart_cplt_cb txCpltCb,
							uart_cplt_cb rxCpltCb,
							uart_cplt_cb errCb);


//void uart_defaultRxCb (XNO_uart_t * uart);
//
//void uart_defaultTxCb (XNO_uart_t * uart);
//
//void uart_defaultErrCb (XNO_uart_t * uart);
extern void UART_RXTX_INT_HANDLER(XNO_UART_NUM_EM uart_num);

#ifdef __cplusplus
}
#endif
#endif /* __NUV_M2354_HAL_HANDLE_UART_H */


