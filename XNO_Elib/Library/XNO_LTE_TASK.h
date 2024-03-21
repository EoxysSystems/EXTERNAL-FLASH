/* File: LTE_task.h
* Author: Devika
* Company: Eoxys
* Description: This file provides the code for communicating with LTE
* Created on: Feb 11, 2021
* Version History
* v1.0 Initial release
*/

#ifndef XNO_LTE_TASK_H_
#define XNO_LTE_TASK_H_
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include <XNO_COMMON.h>
#include <XNO_OS_TIMERS.h>
/*----------------------------------------------------------------------------
* 							Global definitions
*----------------------------------------------------------------------------*/
//#define LTE_UARTTx_IRQn 			 LPUART1_IRQn
#define LTE_PARSE_BUFFER_SIZE        20
#define LTE_RING_BUFFER_SIZE         2000//1000
#define LTE_IOBUFRX_SIZE 			 1500
#define LTE_IOBUFTX_SIZE 			 2052
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Defines
*----------------------------------------------------------------------------*/
#define RETRY_MAX                   2
#define LTE_RESP_PAYLOAD_SIZE    	1500
#define LTE_RESP_HEADER_SIZE    	50
#define LTE_RESP_NUM_CSV_PARAMS     18
#define LTE_RESP_CSV_PARAM_SIZE     100

typedef struct{
	XNO_LTE_ATCMD_RESP_EM resp_rcvd;
	uint8_t payload[LTE_RESP_PAYLOAD_SIZE];
}LTE_RESP_ST;
typedef struct{
	uint16_t resp_start_idx;
	uint16_t num_resp;
}LTE_RESP_LINES_ST;
typedef struct{
	uint16_t num_args;
	uint8_t resp_header_str[LTE_RESP_HEADER_SIZE];
	uint8_t resp_csv_param_str[LTE_RESP_NUM_CSV_PARAMS][LTE_RESP_CSV_PARAM_SIZE];
}LTE_RESP_PARSED_ST;

extern LTE_RESP_PARSED_ST LTE_resp_parsed;
extern XNO_UART_TASK_PARAM_ST *lte_task_param;
/*----------------------------------------------------------------------------
* 							Exported variables and functions
*----------------------------------------------------------------------------*/
extern FlagStatus LTE_UartReadyRx;
extern uint8_t uartBufRx[LTE_IOBUFRX_SIZE];

extern void LTE_Timeout_Timer_Init();
extern XNO_LTE_ATCMD_RESP_EM send_LTE_cmd_and_wait_resp(uint8_t *cbuf,uint32_t timeout,LTE_RESP_LINES_ST *resp_lines);
extern XNO_LTE_ATCMD_RESP_EM send_LTE_cmd_and_wait_resp_event(uint8_t *cbuf,uint32_t timeout,LTE_RESP_LINES_ST *resp_lines);
extern XNO_LTE_ATCMD_RESP_EM send_LTE_cmd_with_data_and_wait_resp_event(uint8_t *cbuf,uint8_t *data, uint32_t timeout,LTE_RESP_LINES_ST *resp_lines);
extern XNO_LTE_ATCMD_RESP_EM send_LTE_cmd_with_data_and_wait_resp(uint8_t *cbuf,uint8_t *data, uint32_t timeout,LTE_RESP_LINES_ST *resp_lines);
extern XNO_LTE_ATCMD_RESP_EM send_LTE_HTTP_cmd_and_wait_resp(uint8_t *cbuf,uint32_t timeout,LTE_RESP_LINES_ST *resp_lines);
//extern XNO_LTE_ATCMD_RESP_EM send_LTE_HTTP_cmd_and_wait_resp(uint8_t *cbuf,uint32_t timeout,LTE_RESP_LINES_ST *resp_lines);
//extern void LTE_HAL_UART_TxCpltCallback(UART_HandleTypeDef *uart);
//extern void LTE_HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart);
extern void LTE_UART_Task(void const* argument);
extern void LTE_UART_PrintBuf(uint8_t *sbuf,uint16_t buflen);
extern void LTE_UART_Send(uint8_t *sbuf);
extern uint8_t is_LTE_http_action_TimeOut_Occured();

//extern LTE_CONTEXT_ST LTE_ctx;
//extern RINGBUFF_T LTE_rxring_buf;
//extern XNO_LTE_ATCMD_RESP_EM cur_resp_rcvd;
extern LTE_RESP_ST resp_st;
extern LTE_CONTEXT_ST LTE_ctx;
//extern LTE_RESP_ST LTE_resp_st[LTE_RESP_FIELD_SIZE];
//extern LTE_RESP_ST resp_st;
//
//extern uint8_t Get_LTE_resp(LTE_RESP_ST* LTE_resp, LTE_RESP_LINES_ST *resp_lines,uint16_t idx);
//extern uint16_t Process_LTE_Resp(LTE_RESP_PARSED_ST *resp_parsed,uint8_t *resp);
//extern uint8_t is_LTE_resp_line_available();
//extern uint8_t LTE_Dequeue_current_resp_lines(LTE_RESP_LINES_ST *resp_lines);
//extern void LTE_Reset_current_resp_rcvd();
extern XNO_LTE_ATCMD_RESP_EM LTE_Get_current_resp_rcvd();
extern XNO_LTE_ATCMD_RESP_EM data_send_rcvd;
//extern void LTE_Parser_Init();
//extern XNO_LTE_ATCMD_RESP_EM Parse_LTE_resp_line(uint8_t *resp_line_str,uint16_t resp_line_str_len);
//extern uint8_t Read_LTE_resp_line(uint8_t *resp_line_str,uint16_t *resp_line_str_len);
//extern void XNO_LTE_event_cb(XNO_LTE_ATCMD_RESP_EM event_id, uint8_t* data_ptr);
#endif /* SRC_LTE_TASK_H_ */
