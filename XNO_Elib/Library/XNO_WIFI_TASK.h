/* File: WIFI.h
* Author: Devika
* Company: Eoxys
* Description:
* Created on:  12-Mar-2022
* Version History
* v1.0 Initial release
*/
#ifndef WIFI_APIS_H_
#define WIFI_APIS_H_
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include "XNO_COMMON.h"
#include "nuv_m2354_hal_uart.h"
/*----------------------------------------------------------------------------
* 							Global definitions
*----------------------------------------------------------------------------*/
#define RETRY_MAX		2

#define WIFI_ATCMD_BUF_SIZE				1500
#define WIFI_RESP_BUF_SIZE				1500
#define WIFI_MAX_PAYLOAD_SIZE	        1024
#define MODEM_ATCMD_SIZE				1500

#define RESP_HDRID_MSB					      0xAF
#define RESP_HDRID_LSB					      0x40
#define RESP_HRDID						      0xAF40
#define RESPID_ATCMD_SUCCESS			      0x0000
#define RESPID_ATCMD_ERROR				      0x0001
#define RESPID_ATCMD_INVALID			      0x0002
#define RESPID_WIFI_DISCONNECTED		      0x0003
#define RESPID_WIFI_SCAN_RESULTS		      0x0004
#define RESPID_WIFI_STS_IP				      0x0005
#define RESPID_WIFI_STS_RSSI			      0x0006
#define RESPID_WIFI_STS_PKT_CNT			      0x0007
#define RESPID_WIFI_STS_MAC_ADDR		      0x0008
#define RESPID_WIFI_CLI_CONNECTED   	      0x000C
#define RESPID_WIFI_SER_START 			      0x000D
#define RESPID_WIFI_SOC_SER_CONNECTED	      0x000E
#define RESPID_WIFI_SOC_CLOSE			      0x000F
#define RESPID_WIFI_TCP_DATA_RCVD		      0x0011
#define RESPID_WIFI_HTTP_DATA_RCVD		      0x0013
#define RESPID_WIFI_WAKEUP				      0x0016
#define RESPID_SW_VER						  0x0022
#define RESPID_WIFI_NTP_TIME			      0x0023
#define RESPID_WIFI_MQTT_CONN			      0x0024
#define RESPID_WIFI_MQTT_SUBSCRIBE		      0x0025
/*----------------------------------------------------------------------------
* 							Macro definitions
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
typedef enum{
	CMD_RESP,
	CMD_ERROR,
	CMD_TIMEOUT,
	CMD_OK
}WIFICMD_RESP_EM;
extern WIFI_CONTEXT_ST wifi_ctx;
extern WIFI_RESP_LINES_ST Wifi_resp_lines;
extern XNO_UART_ST WIFI_UART;
extern XNO_WIFI_BLE_RESP_ST *wifi_resp_st;
extern XNO_UART_TASK_PARAM_ST *wifi_task_param;
/*----------------------------------------------------------------------------
* 							Exported variables and functions
*----------------------------------------------------------------------------*/
/**
  * @brief  Modem UART initalization
  * @param  modem_uart - UART handle structure
  */
extern void Wifi_UART_Send(uint8_t *sbuf);

extern void Wifi_UART_Task(void const *argument);
extern void init_Wifi_Parser();
XNO_ERR_STS XNO_MODEM_Enter_Sleep_Mode(uint32_t sleepTime, void (*user_wakeup_cb)(void));
extern XNO_ERR_STS XNO_WIFI_Get_NTP_Server_Timestamp(S_RTC_TIME_DATA_T *time_st);
extern uint16_t check_for_spcl_chars(uint8_t* mod_data_buf, uint8_t* data);
extern XNO_WIFI_ATCMD_RESP_EM send_Wifi_cmd_and_wait_resp(uint8_t *cbuf,uint32_t timeout,WIFI_RESP_LINES_ST *resp_lines);

#endif /* WIFI_APIS_H_ */
