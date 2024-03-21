/* File: NBIOT_APIs.c
* Author:unni0
* Company: Eoxys
* Description: API implementation NBIOT module.
* Created on:  9-MARCH-2023
* Version History
* v1.0 Initial release
*/

#ifndef XNO_NBIOT_TASK_H_
#define XNO_NBIOT_TASK_H_
/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
#include <XNO_COMMON.h>
#include <XNO_MODEM.h>

#include "nuv_m2354_hal_uart.h"

/*----------------------------------------------------------------------------
* 							Global definitions
*----------------------------------------------------------------------------*/
//#define NBIOT_UART                	 hlpuart1
#define NBIOT_UART_INST				 UART1
#define NBIOT_PARSE_BUFFER_SIZE        20
#define NBIOT_RING_BUFFER_SIZE         1500
#define NBIOT_IOBUFRX_SIZE 			 1500
#define NBIOT_IOBUFTX_SIZE 			 2052
/*----------------------------------------------------------------------------
* 								Defines
*----------------------------------------------------------------------------*/
#define PARAM_STR_SIZE 			1500
#define NBIOT_SENSOR_BUF_LEN   	100
#define NBIOT_SOC_RESP_SIZE	  	20
#define NBIOT_RESP_PARAM_SIZE	    1400
#define NBIOT_PAYLOAD_SIZE	    1024


#define NBIOT_RESP_FIELD_SIZE      	10

/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Macros
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Static global variables
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Global Variables
*----------------------------------------------------------------------------*/
extern osThreadId NBIOTTaskHandle;
extern NBIOT_CONTEXT_ST NBIOT_ctx;
extern XNO_UART_ST NBIOT_MOD_UART;
extern NBIOT_RESP_PARSED_ST NBIOT_resp_parsed;
extern FlagStatus sim_network_registration_ready;
/*----------------------------------------------------------------------------
* 								Static Functions
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Exported Functions
*----------------------------------------------------------------------------*/
extern uint16_t Process_NBIOT_Resp(NBIOT_RESP_PARSED_ST *resp_parsed,uint8_t *resp);
/*
 * @brief NBIOT UART Initialization
 * @Param none
 * @retval NONE
 */
extern void XNO_NBIOT_UART_Init();

/*
 * @brief NBIOT Power Initialization
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Power_Init(void);

/*
 * @brief NBIOT Device Turn on
 * @Param none
 * @retval none
 */
extern void XNO_NBIOT_Power_ON(void);

/*
 * @brief NBIOT Device Turn off
 * @Param none
 * @retval none
 */
extern void XNO_NBIOT_Power_OFF(void);

/*
 * @brief NBIOT Enable char echo
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Enable_Echo(void);

/*
 * @brief NBIOT Device Reset
 * @Param none
 * @retval none
 */
extern void XNO_NBIOT_Reset(void);

/*
 * @brief NBIOT Uart Initialization
 * @Param none
 * @retval none
 */
extern void XNO_NBIOT_UART_Init();

/*
 * @brief NBIOT Uart Enter Manufacturing mode
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Enter_Manufacturing_Mode(void);

/*
 * @brief NBIOT Uart Configuration
 * @Param  (1)baud_rate - Standard baudrate in (1200, 2400, 4800, 9600, 19200,38400, 57600, 115200). Default value is 115200.
		   (2)format - Word length in (5, 6, 7, 8). Default value is 8.
		   (3)parity - Parity possible values in (none, even, odd, mark,space). Default is “none”
		   (4)stopBits - Number of stop bits in (1, 1.5, 2). Default value is 2.
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Set_UART_Configuration(uint32_t baud_rate, uint32_t format, uint32_t parity, uint32_t stopbit);

/*
 * @brief NBIOT Get Uart Configuration
 * @Param (1)*baud_rate - Pointer to UART baudrate
 * @retval XNO_ERR_STS - Error Status
 * */
extern XNO_ERR_STS XNO_NBIOT_Get_Baudrate(uint32_t *baud_rate);

/*
 * @brief NBIOT Restart
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Restart();

/*
 * @brief NBIOT Shutdown
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Shutdown();

/*
 * @brief NBIOT Uart Enter Airplane mode
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Enter_Airplane_Mode(void);

/*
 * @brief NBIOT Uart Enter Full Functionality  mode
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Enter_Full_Functionality_Mode(void);

/*
 * @brief NBIOT Uart Enter Full Manufacturing  mode
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Enter_Manufacturing_Mode(void);

/*
 * @brief NBIOT Check hardware functionality
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Check_Sim_Polling_Sts(uint8_t *polling_sts);

extern XNO_ERR_STS XNO_NBIOT_Get_Sim_Card_Status(uint8_t *sim_rdy_sts);

/*
 * @brief NBIOT Check Simcard state
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Check_Simcard_State(void);

/*
 * @brief NBIOT Enable Simcard polling detection
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Enable_Sim_Polling_Detection(void);

/*
 * @brief NBIOT Enable Check SIM behavior
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Check_SIM_behavior(void);

/*
 * @brief NBIOT Uart Set 4G Standard  mode
 * @Param none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Set_4G_Standard_Mode(void);

/*
 * @brief NBIOT Uart baud rate Reschedule
 * @Param  (1)baud_rate - Standard baudrate in (1200, 2400, 4800, 9600, 19200,38400, 57600, 115200). Default value is 115200.
 * @retval XNO_ERR_STS - Error Status
 */
extern void XNO_NBIOT_UART_Reschedule(uint32_t baudrate);

/*
 * @brief NBIOT Get Network Quality
 * @Param  (1)rssi - Received signal strength indication
           (2)ber - Channel bit error rate (in percent)
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Get_Signal_Quality(uint8_t *rssi, uint8_t *bit_err_rate);

/*
 * @brief NBIOT Get Network Registration status
 * @Param  (1)*nw_reg_sts - Network registration status
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Get_Network_Registration_status(XNO_NBIOT_NW_REG_STS *nw_reg_sts);

/*
 * @brief NBIOT Get PDP context
 * @param  (1)pdp_ctx_st - PDP context structure
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Get_PDP_Context(PDP_CONTEXT_ST *pdp_ctx_st);

/*
 * @brief NBIOT Set PDP Context State
 * @Param  (1)state - 1 for activation, 0 for deactivation
		   (2)context_ID - Cell IDentifier.(cid)
		   (3)parity - Parity possible values in (none, even, odd, mark,space). Default is “none”
		   (4)stopBits - Number of stop bits in (1, 1.5, 2). Default value is 2.
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Set_PDP_Context_State( uint8_t state, uint8_t context_ID);

/*
 * @brief NBIOT Check IP Address
 * @Param  (1)*IPv4_adr - Pointer to IPV4 address
		   (2)*IPv6_adr - Pointer to IPV6 address
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Check_IP_Address(uint8_t *IPv4_adr, uint8_t *IPv6_adr, uint8_t *cid);

/*
 * @brief NBIOT Read Current Date and Time
 * @Param  (1)time - Time Date structure
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Read_Current_Date_And_Time();

/*
 * @brief NBIOT Automatic Time Zone Update With NITZ Protocol
 * @param  none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Automatic_Time_Zone_Update_With_NITZ_Protocol();

/*
 * @brief NBIOT Automatic Time Zone Update With NTP Protocol
 * @param  none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Automatic_Time_Zone_Update_With_NTP_Protocol();

/*
 * @brief NBIOT Factory reset
 * @param  none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Factory_Reset();


/*
 * @brief NBIOT Get firmware version
 * @param  none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Get_Firmware_Version();

/*
 * @brief NBIOT Get imei number
 * @param  none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Get_IMEI(char* imei);

/*
 * @brief NBIOT Enable URC Reception
 * @param  none
 * @retval XNO_ERR_STS - Error Status
 */
extern XNO_ERR_STS XNO_NBIOT_Enable_URC_Reception();

#endif //XNO_NBIOT_TASK_H
