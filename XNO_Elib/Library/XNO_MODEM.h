/* File: XNO_MODEM.h
* Author: Prabhakar, Rajesh
* Company: Eoxys
* Description: Modem Power, Reset and Common functions
* Created on:  12-Oct-2023
* Version History
* v1.0 Initial release
*/
#ifndef XNO_MODEM_H_
#define XNO_MODEM_H_

/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <cmsis_os.h>
#include <rtc.h>
#include <XNO_COMMON.h>

/*----------------------------------------------------------------------------
* 								User Includes
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Macros
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/
typedef enum {
	MODEM_NONE,
	MODEM_WIFI,
	MODEM_LTE,
	MODEM_NBIOT,
	MODEM_LORA
} XNO_MODEM_TYPE_EM;
typedef struct XNO_MODEM_VAR
{
	XNO_MODEM_TYPE_EM   modemType;
	osSemaphoreId		modemLock;
	osSemaphoreDef_t	modemLockDef;
}XNO_MODEM_VAR_ST;

/*----------------------------------------------------------------------------
* 								Exported Variables
*----------------------------------------------------------------------------*/
extern uint8_t retry_atcmd_cnt;
/*----------------------------------------------------------------------------
* 								Exported Functions
*----------------------------------------------------------------------------*/
/*Modem Power Init function*/
typedef XNO_ERR_STS (*modemPowerInit_func)(void);
/*Modem Power ON function*/
typedef XNO_ERR_STS (*modemPowerOn_func)(void);
/*Modem Power OFF function*/
typedef XNO_ERR_STS (*modemPowerOff_func)(void);
/*Modem Reset function*/
typedef XNO_ERR_STS (*modemReset_func)(void);
/*Modem Sleep function*/
typedef XNO_ERR_STS (*modemSleep_func)(uint32_t sleepTime, void (*user_wakeup_cb)(void));
/*Modem Wakeup function*/
typedef XNO_ERR_STS (*modemWakeup_func)(void);
/*Modem ATCMD send and wait resp function*/
//typedef XNO_ATCMD_RESP (*atcmd_send_wait_resp_func)(const uint8_t *atbuf,uint32_t timeout,WIFI_RESP_LINES_STRUCT *resp_lines);
typedef struct XNO_MODEM_FUNC
{
	//Modem Power, Reset Sleep and Wakeup functions
	modemPowerInit_func           modemPowerInit;
	modemPowerOn_func		      modemPowerOn;
	modemPowerOff_func	          modemPowerOff;
	modemReset_func		          modemReset;
	modemSleep_func               modemSleep;
	modemWakeup_func			  modemWakeup;
}XNO_MODEM_FUNC_ST;

/*Modem specific functions*/

/*WiFi AP Hostname function*/
typedef XNO_ERR_STS (*wifiApSetHostname_func)(uint8_t *hname);
/*WiFi AP Connect function*/
typedef XNO_ERR_STS (*wifiApConnect_func)(uint8_t *ssid, uint8_t *password);
/*WiFi AP Disconnect function*/
typedef XNO_ERR_STS (*wifiApDisconnect_func)(void);
/*WiFi WIFI Scan function*/
typedef XNO_ERR_STS (*wifiScan_func)(uint8_t *ssid, uint8_t *result);
/*WiFi Get WLAN Param Sts function*/
typedef XNO_ERR_STS (*wifiGetWlanParam_func)(XNO_WIFI_STATUS_ID_EM wlan_sts_id, uint8_t* param_result);
/*WiFi WIFI Set IPV4 function*/
typedef XNO_ERR_STS (*wifiSetIpv4Addr_func)(uint32_t ipv4_addr, uint32_t netmask, uint32_t gw, uint32_t dns);
/*WiFi NW specific functions*/
typedef XNO_ERR_STS (*wifiGetNtpTimestamp_func)(S_RTC_TIME_DATA_T *time_st);

typedef struct XNO_WIFINW_FUNC
{
	//WiFi Network functions
	wifiApSetHostname_func	wifiApSetHostname;
	wifiApConnect_func		wifiApConnect;
	wifiApDisconnect_func	    wifiApDisconnect;
	wifiScan_func		        wifiScan;
	wifiGetWlanParam_func   	wifiGetWlanParam;
	wifiSetIpv4Addr_func		wifiSetIpv4Addr;
	wifiGetNtpTimestamp_func  wifiGetNtpTimestamp;
}XNO_WIFINW_FUNC_ST;


typedef XNO_ERR_STS (*lteGetSimcomSts_func)(void);
typedef XNO_ERR_STS (*lteGetSIMCardSts_func)(uint8_t *sim_rdy_sts);
typedef XNO_ERR_STS (*lteGetPreferredMode_func)(MODEM_CELLURE_MODE_EM *LTE_mode);
typedef XNO_ERR_STS (*lteSetPreferredMode_func)(MODEM_CELLURE_MODE_EM LTE_mode);
typedef XNO_ERR_STS (*lteGetSignalQuality_func)(uint8_t *rssi, uint8_t *bit_err_rate);
typedef XNO_ERR_STS (*lteSetFunctionQuality_func)(MODEM_FUNC_MODE_EM *fun_quality);
typedef XNO_ERR_STS (*lteGetMENetworkRegSts_func)(XNO_LTE_NW_REG_STS *nw_reg_sts);
typedef XNO_ERR_STS (*lteGetMTNetworkRegSts_func)(XNO_LTE_NW_REG_STS *nw_reg_sts);
typedef XNO_ERR_STS (*lteGetUESystemInfo_func)(UE_SYS_INFO_ST *system_info);
typedef XNO_ERR_STS (*lteSetPDPContext_func)(PDP_CONTEXT_ST *pdp_ctx_st);
typedef XNO_ERR_STS (*lteGetPDPContext_func)(PDP_CONTEXT_ST *pdp_ctx_st);
typedef XNO_ERR_STS (*ltePDPSetContextState_func)( uint8_t state, uint8_t context_ID);
typedef XNO_ERR_STS (*ltePDPGetContextState_func)(LTE_PDP_CXT_STATE_ST *pdp_state);
typedef XNO_ERR_STS (*lteReadCurrentTime_func)(LTE_TIME *time);
typedef XNO_ERR_STS (*lteNTPTimeUpdate_func)();
typedef XNO_ERR_STS (*lteAutomaticTimeUpdate_func)(uint8_t Mode);
//typedef XNO_ERR_STS lteSet_GPIO_Value_func( uint8_t gpio_num, GPIO_VALUE_EM val);
//typedef XNO_ERR_STS lteGet_GPIO_Value_func( uint8_t gpio_num,uint8_t gpio_val);
//typedef XNO_ERR_STS lteGet_sim_uim_func(uint8_t level);
//typedef XNO_ERR_STS lteSet_sim_uim_func(uint8_t level);
//typedef XNO_ERR_STS lteGet_uimhost_func(uint8_t level);
//typedef XNO_ERR_STS lteSet_uimhost_func(uint8_t level);
typedef XNO_ERR_STS (*lteSaveUserSettingsToME_func)();
typedef XNO_ERR_STS (*lteGetCICCIDFromSim_func)(uint8_t *ciccid);
typedef XNO_ERR_STS (*lteGetOperatorSelection_func)();
typedef XNO_ERR_STS (*lteGetNWSystemMode_func)();
typedef void (*lteUARTReschedule_func)(uint32_t baudrate);
typedef XNO_ERR_STS (*lteSetBaudrate_func)(uint32_t baud_rate);
typedef XNO_ERR_STS (*lteGetBaudrate_func)(uint32_t *baud_rate);
typedef XNO_ERR_STS (*lteModuleReset_func)();
typedef XNO_ERR_STS (*lteTestAatcmd_func)();
typedef struct XNO_LTENW_FUNC
{
	lteGetSimcomSts_func 				lteGetSimcomSts;
	lteGetSIMCardSts_func 				lteGetSIMCardSts;
	lteGetPreferredMode_func			lteGetPreferredMode;
	lteSetPreferredMode_func			lteSetPreferredMode;
	lteGetSignalQuality_func            lteGetSignalQuality;
	lteSetFunctionQuality_func          lteSetFunctionQuality;
	lteGetMENetworkRegSts_func          lteGetMENetworkRegSts;
	lteGetMTNetworkRegSts_func          lteGetMTNetworkRegSts;
	lteGetUESystemInfo_func             lteGetUESystemInfo;
	lteSetPDPContext_func               lteSetPDPContext;
	lteGetPDPContext_func               lteGetPDPContext;
	ltePDPSetContextState_func          ltePDPSetContextState;
	ltePDPGetContextState_func          ltePDPGetContextState;
	lteReadCurrentTime_func             lteReadCurrentTime;
	lteNTPTimeUpdate_func               lteNTPTimeUpdate;
	lteAutomaticTimeUpdate_func         lteAutomaticTimeUpdate;
	lteSaveUserSettingsToME_func    	lteSaveUserSettingsToME;
	lteGetCICCIDFromSim_func         	lteGetCICCIDFromSim;
	lteGetOperatorSelection_func     	lteGetOperatorSelection;
	lteGetNWSystemMode_func          	lteGetNWSystemMode;
	lteUARTReschedule_func             	lteUARTReschedule;
	lteSetBaudrate_func                 lteSetBaudrate;
	lteGetBaudrate_func                 lteGetBaudrate;
	lteModuleReset_func                 lteModuleReset;
	lteTestAatcmd_func                  lteTestAatcmd;
}XNO_LTENW_FUNC_ST;
typedef struct XNO_MODEM
{
	XNO_MODEM_VAR_ST var;
	XNO_MODEM_FUNC_ST func;
	XNO_WIFINW_FUNC_ST wifi;
//	XNO_CELLNW_FUNC_ST cellular;
	XNO_LTENW_FUNC_ST lte;
//	XNO_LORANW_FUNC_ST lora;
}XNO_MODEM_ST;

extern XNO_ERR_STS XNO_Modem_Init(XNO_MODEM_ST *modem_st,
						   XNO_MODEM_TYPE_EM modem_type);
extern uint8_t retry_atcmd(void);
//extern void XNO_WIFI_SOCKET_RXTX_INT_HANDLE(XNO_UART_NUM_EM uart_num);
#endif /* XNO_MODEM_H_ */
