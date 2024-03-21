/* File: XNO_HTTP.h
* Author:Rajesh
* Company: Eoxys
* Description: APIs for HTTP communication
* Created on:  24-Oct-2023
* Version History
* v1.0 Initial release
*/
#ifndef XNO_HTTP_H_
#define XNO_HTTP_H_

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
typedef struct XNO_HTTP_VAR
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
}XNO_HTTP_VAR_ST;

typedef void (*httpCplt_cb)(XNO_HTTP_VAR_ST *http_var);
typedef XNO_ERR_STS (*httpClientSetHeader_func)(XNO_HTTP_HDR_ID_EM Header_ID,uint8_t *Header_val);
typedef int (*httpClientStart_func)(uint8_t *ser_ip, uint16_t ser_port);
typedef XNO_HTTP_ERR_CODE_EM (*httpClientSendRequest_func)(int HCID, XNO_HTTP_REQ_METHOD_EM Req_method, uint8_t *URI_string, uint8_t *Data, uint32_t timeout);
typedef XNO_ERR_STS (*httpClientDeleteHeader_func)(XNO_HTTP_HDR_ID_EM Header_ID);
typedef XNO_ERR_STS (*httpCloseConnection_func)(int HCID);

typedef struct XNO_HTTP_FUNC
{
	//Socket callback functions
	httpCplt_cb		                rxCpltCb;
	httpCplt_cb		                errCb;

	httpClientSetHeader_func		httpClientSetHeader;
	httpClientStart_func	        httpClientStart;
	httpClientSendRequest_func      httpClientSendRequest;
	httpClientDeleteHeader_func     httpClientDeleteHeader;
	httpCloseConnection_func		httpCloseConnection;
	//modem UART Interface functions
//	atcmd_send_wait_resp_func		atcmdSendWaitResp;
}XNO_HTTP_FUNC_ST;
typedef struct XNO_HTTP
{
	XNO_HTTP_VAR_ST var;
	XNO_HTTP_FUNC_ST func;
}XNO_HTTP_ST;
//extern void XNO_WIFI_SOCKET_RXTX_INT_HANDLE(XNO_UART_NUM_EM uart_num);
extern XNO_ERR_STS XNO_Http_Init(XNO_HTTP_ST *http_st,
		                         XNO_MODEM_TYPE_EM modemType,
		                         httpCplt_cb rxCpltCb,
		                         httpCplt_cb errCb,
		                         CB_EN_DIS_EM cb_status);
#endif /* XNO_HTTP_H_ */
