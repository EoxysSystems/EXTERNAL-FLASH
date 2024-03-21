/* File: XNO_IPC_MESG.h
* Author: Prabhakar, Rajesh
* Company: Eoxys
* Description: IPC Message APIs
* Created on:  9-DEC-2023
* Version History
* v1.0 Initial release
*/
#ifndef INC_XNO_IPC_MESG_H_
#define INC_XNO_IPC_MESG_H_
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
typedef enum{
	IPC_REQ_NONE,
	IPC_REQ_FW_ERASE,
	IPC_REQ_FW_DNLD,
	IPC_REQ_FW_HDRUPD,
	IPC_REQ_BLE_ADV,
	IPC_REQ_HTTP_DATA_SEND,
	IPC_REQ_TCP_DATA_SEND,
	IPC_REQ_MQTT_DATA_SEND,
	IPC_REQ_UART_DATA_SEND,
	IPC_REQ_CAN_TX
}IPC_CMD_REQ_EM;

typedef enum{
	IPC_RESP_NOT_AVAIL,
	IPC_RESP_DONE,
	IPC_RESP_ERROR,
	IPC_RESP_TIMEOUT
}IPC_CMD_RESP_EM;
typedef enum{
	SRC_NONE,
	SRC_DBG,
	SRC_FOTA,
	SRC_MODEM,
	SRC_INTERFACE,
	SRC_NDP,
	SRC_CAN,
	SRC_FWFLASH,
	SRC_WIFI_TASK,
	SRC_ANY,
	SRC_PBLE_TASK
}IPC_SOURCES_ST;
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/

typedef struct{
	uint32_t source;
	uint32_t dest;
	IPC_CMD_REQ_EM cmd_req;
	IPC_CMD_RESP_EM cmd_resp;
	FlagStatus mesg_inprogress;
	void	*req_data_obj;
	void	*resp_data_obj;
}XNO_IPCMESG_VAR_ST;

/*----------------------------------------------------------------------------
* 							Exported functions
*----------------------------------------------------------------------------*/
//User functions
typedef XNO_ERR_STS (*reset_func)(XNO_IPCMESG_VAR_ST *var);
typedef XNO_ERR_STS (*sendReq_func)(XNO_IPCMESG_VAR_ST *var, IPC_CMD_REQ_EM cmd);
typedef XNO_ERR_STS (*sendResp_func)(XNO_IPCMESG_VAR_ST *var, IPC_CMD_RESP_EM resp);
typedef XNO_ERR_STS (*sendReqWithDataObj_func)(XNO_IPCMESG_VAR_ST *var, IPC_CMD_REQ_EM cmd,void *req_data_obj);
typedef XNO_ERR_STS (*sendRespWithDataObj_func)(XNO_IPCMESG_VAR_ST *var, IPC_CMD_RESP_EM resp,void *resp_data_obj);
typedef FlagStatus (*isReqAvailable_func)(XNO_IPCMESG_VAR_ST *var);
typedef FlagStatus (*isRespAvailable_func)(XNO_IPCMESG_VAR_ST *var);
typedef FlagStatus (*isReqInProgress_func)(XNO_IPCMESG_VAR_ST *var);
typedef struct{
	reset_func reset;
	sendReq_func sendReq;
	sendResp_func sendResp;
	sendReqWithDataObj_func sendReqWithDataObj;
	sendRespWithDataObj_func sendRespWithDataObj;
	isReqAvailable_func isReqAvailable;
	isRespAvailable_func isRespAvailable;
	isReqInProgress_func isReqInProgress;
}XNO_IPCMESG_FUNC_ST;
typedef struct{
	XNO_IPCMESG_VAR_ST var;
	XNO_IPCMESG_FUNC_ST func;
}XNO_IPCMESG_ST;

extern XNO_ERR_STS XNO_IPCMESG_Init(XNO_IPCMESG_ST *ipcmsg,
				uint32_t source,
				uint32_t dest,
				void *req_data_obj,
				void *resp_data_obj);

#endif /* INC_XNO_IPC_MESG_H_ */
