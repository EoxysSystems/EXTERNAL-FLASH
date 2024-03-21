/* File: XNO_SOCKET.h
* Author: Prabhakar, Rajesh
* Company: Eoxys
* Description: APIs for Socket communication
* Created on:  12-Oct-2023
* Version History
* v1.0 Initial release
*/
#ifndef XNO_SOCKET_H_
#define XNO_SOCKET_H_

/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								User Includes
*----------------------------------------------------------------------------*/
#include <XNO_COMMON.h>
#include <XNO_MODEM.h>
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Defines
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Macros
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Exported Variables
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Exported Functions
*----------------------------------------------------------------------------*/
typedef struct XNO_SOCKET_VAR
{
	XNO_MODEM_TYPE_EM   modemType;

	uint8_t*			txBuff;
	uint32_t			txLen;
	osSemaphoreId		txLock;
	osSemaphoreDef_t	txLockDef;

	uint8_t*			rxBuff;
	uint32_t			rxLen;
	uint32_t			rxCbEn;
	osSemaphoreId		rxLock;
	osSemaphoreDef_t	rxLockDef;
}XNO_SOCKET_VAR_ST;


typedef void (*socketCplt_cb)(XNO_SOCKET_VAR_ST *soc_var);

/*Socket Create function*/
typedef int (*socketCreate_func)(XNO_SOCKET_VAR_ST *soc_var, XNO_SOC_PROTOCOL_EM protocol, uint8_t *server_url, uint8_t *port);

/*Socket Send function*/
typedef XNO_ERR_STS (*socketSend_func)(XNO_SOCKET_VAR_ST *soc_var, XNO_SOC_DATATYPE_EM data_type, int soc_id, uint8_t *txdata, uint32_t txlen,uint32_t timeout);

//*Socket Is Data Avail function*/
typedef int (*socketIsDataAvail_func)(XNO_SOCKET_VAR_ST *soc_var, int soc_id);

//*Socket Close function*/
typedef XNO_ERR_STS (*socketClose_func)(XNO_SOCKET_VAR_ST *soc_var, int soc_id);

/*Modem ATCMD send and wait resp function*/
//typedef XNO_ATCMD_RESP (*atcmd_send_wait_resp_func)(const uint8_t *atbuf,uint32_t timeout,WIFI_RESP_LINES_STRUCT *resp_lines);

typedef struct XNO_SOCKET_FUNC
{
	//Socket callback functions
	socketCplt_cb		            rxCpltCb;
	socketCplt_cb		            errCb;
	//Socket functions
	socketCreate_func		        socketCreate;
	socketSend_func	        	socketSend;
	socketIsDataAvail_func       socketIsDataAvail;
	socketClose_func				socketClose;
	//modem UART Interface functions
//	atcmd_send_wait_resp_func		atcmdSendWaitResp;

}XNO_SOCKET_FUNC_ST;

typedef struct XNO_SOCKET
{
	XNO_SOCKET_VAR_ST var;
	XNO_SOCKET_FUNC_ST func;
}XNO_SOCKET_ST;

extern XNO_ERR_STS XNO_Socket_Init(XNO_SOCKET_ST *soc_st,
								XNO_MODEM_TYPE_EM modem_type,
								socketCplt_cb rxCpltCb,
								socketCplt_cb errCb,
								CB_EN_DIS_EM cb_status);
#endif /* XNO_SOCKET_H_ */
