/* File: DEBUG.h
* Author: Devika
* Company: Eoxys
* Description: AT command implementation for boot loader to update the wifi, FOTA server configuration
* Created on:  27-Apr-2022
* Version History
* v1.0 Initial release
*/
#ifndef DEBUG_H_
#define DEBUG_H_
/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								User Includes
*----------------------------------------------------------------------------*/
#include <XNO_RING_BUFFER.h>

#include "nuv_m2354_hal_uart.h"

/*----------------------------------------------------------------------------
* 								Defines
*----------------------------------------------------------------------------*/
#define ATCMD_PARAM_STR_SIZE       1300
#define ATCMD_EACH_PARAM_STR_SIZE  1100
#define ATCMD_PARAM_SIZE           1100
#define NUM_ATCMD_PARAM_STR        6
#define ATCMD_SIZE   		       100
#define DBG_PRINTBUF_SIZE 		 2000

#ifndef __weak
#define __weak  __attribute__((weak))
#endif

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/
typedef enum {
	ATTYPE_INVALID,
	ATTYPE_TEST,
	ATTYPE_READ,
	ATTYPE_WRITE,
	ATTYPE_EXECUTE
}ATCMD_TYPE;

typedef struct{
	int16_t ATCmd_Id;
	uint8_t atcmd_str[100];
	uint8_t atcmd_testinfo_str[5][200];
}ATCMDS_LIST;

typedef struct{
	int16_t ATCmd_Id;
	ATCMD_TYPE ATCmd_Type;
	uint16_t ATCmd_list_idx;
	uint16_t Num_Args;
	uint8_t ATCmd[ATCMD_SIZE];
	uint8_t ATCmdParam[ATCMD_PARAM_SIZE];
	uint8_t ATCmdParamEachStr[NUM_ATCMD_PARAM_STR][ATCMD_EACH_PARAM_STR_SIZE];	
	uint16_t ATCmdParamLength;
}ATCMD_RCVD;

extern RINGBUFF_T Dbg_txring_buf;
extern RINGBUFF_T Dbg_rxring_buf;
extern FlagStatus UartReadyTx;
extern FlagStatus isDataAvailTx;
extern FlagStatus isDebugTaskReady;
extern uint16_t scanBufTxCount;
extern uint8_t PrintBuf[DBG_PRINTBUF_SIZE];
extern XNO_UART_ST DBG_UART;
/*---------------------------------------------------------------------------
* 								Macros
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Exported Variables
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Exported Functions
*----------------------------------------------------------------------------*/
/**
  * @brief  Initialization of debug UART
  * @param  dbg_uart - debug UART handle
  * @retval None
  */
//extern void XNO_DBG_UART_Init();
/**
  * @brief  To print the messages on the debug console
  * @param  dbg_uart - debug UART handle
  * @retval None
  */
extern void XNO_Printf(char *sbuf);
/**
  * @brief  To print the messages on the debug console
  * @param  dbg_uart - debug UART handle
  * @param  buflen - length of the data
  * @retval None
  */
extern void XNO_PrintBuf(uint8_t *sbuf,uint16_t buflen);
/**
  * @brief  User can overwrite ths function to implement external AT command in user file .
  * @param  atcmd_rcvd - Received AT command structure
  * @retval None
  */
//__weak void execute_atprop_cmd_ext(ATCMD_RCVD *atcmd_rcvd);
extern void execute_atprop_cmd_ext(ATCMD_RCVD *atcmd_rcvd);
/**
  * @brief  User can overwrite this function when user presses escape char.
  * @param  rchar - The escape character pressed
  * @retval None
  */
//__weak void execute_escchar_cmd_ext(uint8_t rchar);
extern void execute_escchar_cmd_ext(uint8_t rchar);
/**
  * @brief  To initialize external AT command list
  * @param  atcmd_list -  list of AT commands
  * @param  atcmd_list_len -  length of AT command list
  * @retval None
  */
//extern void XNO_init_external_ATCmd_List(ATCMDS_LIST *atcmd_list, uint16_t atcmd_list_len);
extern void XNO_DBG_UART_Reschedule();
extern uint8_t Parse_AT_Param_Str(uint8_t *text, uint8_t (*out_param_str)[ATCMD_EACH_PARAM_STR_SIZE]);

//extern void XNO_DBG_Init();
extern void DBG_UART_Task(void const * argument);
extern void DBG_Parser_Task (void const * argument);
//extern void DBG_RXTX_INT_HANDLE(void);
extern void DBG_UART_Rx_Reschedule();

extern void Run_ATCmdStateMachine();

#ifndef __weak
#define __weak  __attribute__((weak))
#endif
#endif /* DEBUG_H_ */
