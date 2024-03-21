/* File: ATcmds.c
 *
 *  Created on: 21-may-2023
 *      Author: Prabhakar, Rajesh
 */
/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
#include "nuv_m2354_hal_wdt.h"
#include <string.h>
/*----------------------------------------------------------------------------
* 								User Includes
*----------------------------------------------------------------------------*/
#include "ATcmds.h"
#include "XNO_UTILS.h"
#include "cmsis_os.h"
#include "XNO_DEBUG_TASK.h"
#include "User_Config.h"
#include <XNO_IPC_MESG.h>
/*----------------------------------------------------------------------------
* 								 variables
*----------------------------------------------------------------------------*/
//static uint8_t LOADER_FW=0,BTLD_FW=0,MAIN_FW=1,NDPMCU_FW = 0,NDPDSP_FW = 0,MODEM_FW = 0,GPS_FW = 0,NDPDNN_FW = 0;
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/

WIFI_AP_CFG wifi_ap_cfg = {
//	"EoxysSystems",
//	"eoxys@110"
	"eoxys act",
	"&6%5$4#3"
};

DEVINFO devinfo = {
	.dev_name = "Xeno"
};
typedef struct{
	uint32_t fw_start_addr;
	uint16_t fw_len;
	uint16_t fw_data_crc;
	uint32_t baudrate;
}FW_FILE_ST;
FW_FILE_ST fw_st;
const ATCMDS_LIST atcmd_extlist[] = {
	//version command
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_SWVER,
		.atcmd_str = "SWVER",
		.atcmd_testinfo_str[0] = "AT%SWVER",
		.atcmd_testinfo_str[1] = "END"
	},

	//Device version command
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_DEVID,
		.atcmd_str = "DEVID",
		.atcmd_testinfo_str[0] = "AT%DEVID",
		.atcmd_testinfo_str[1] = "END"
	},

	//wifi AP config command
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_WIFIAPCFG,
		.atcmd_str = "WIFIAPCFG",
		.atcmd_testinfo_str[0] = "AT%WIFIAPCFG=<APN Name>,<Password>",
		.atcmd_testinfo_str[1] = "END"
	},
	//Devinfo command
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_DEVINFO,
		.atcmd_str = "DEVINFO",
		.atcmd_testinfo_str[0] = "AT%DEVINFO",
		.atcmd_testinfo_str[1] = "END"
	},
	// AT%DBGBAUD=<Baudrate>
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_DBG_BAUDRATE,
		.atcmd_str = "DBGBAUD",
		.atcmd_testinfo_str[0] = "AT%DBGBAUD=<Baudrate>",
		.atcmd_testinfo_str[1] = "END"
	},
	// AT%BYPMDMUART
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_BYP_MDM_UART,
		.atcmd_str = "BYPMDMUART",
		.atcmd_testinfo_str[0] = "AT%BYPMDMUART",
		.atcmd_testinfo_str[1] = "END"
	},
	// AT command mode exit
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_EXIT,
		.atcmd_str = "EXIT",
		.atcmd_testinfo_str[0] = "AT%EXIT",
		.atcmd_testinfo_str[1] = "END"
	},
	//End of List
	{
		.ATCmd_Id = (int16_t)EX_ATCMD_END,
		.atcmd_str = "",
		.atcmd_testinfo_str[0] = "END"
	},

};
uint16_t atcmd_extlist_len = sizeof(atcmd_extlist)/sizeof(atcmd_extlist[0]);
/*----------------------------------------------------------------------------
* 								Defines
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Macros
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Static global variables
*----------------------------------------------------------------------------*/
static uint8_t locstr[100];
static uint8_t Print_buf[1000];
static uint8_t wait_for_resp;
/*----------------------------------------------------------------------------
* 								Global Variables
*----------------------------------------------------------------------------*/
FlagStatus at_cmd_mode_active = RESET;
FW_INFO app_swver = { "XENO Example", "12", "29-Nov-2023" };
DEV_ID XENO_device_id = XENO_WIFI_ML;
/*----------------------------------------------------------------------------
* 								Static Functions
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Global Functions
*----------------------------------------------------------------------------*/
FlagStatus Is_at_cmd_mode_active(){
	return at_cmd_mode_active;
}
void execute_escchar_cmd_ext(uint8_t rchar){
	at_cmd_mode_active = SET;
	XNO_Printf("ENTERING INTO AT COMMAND MODE");
}
void execute_atprop_cmd_ext(ATCMD_RCVD *atcmd_rcvd){
	uint32_t reg_addr=0;
	uint8_t pid=0;
	uint8_t flash_type=0;
	uint8_t sts=0;
	ATCMD_EXT_MASTER_ID atcmd_ext_id = (ATCMD_EXT_MASTER_ID) atcmd_rcvd->ATCmd_Id;
	switch(atcmd_ext_id){
	case EX_ATCMD_DEVID:
			if(strcmp((char *)(atcmd_rcvd->ATCmd),"DEVID")==0){
				if(atcmd_rcvd->ATCmd_Type==ATTYPE_TEST){
					XNO_Printf("AT%DEVID");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_READ){
					XNO_Printf("ERROR");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_WRITE){
					XNO_Printf("ERROR");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_EXECUTE){
					switch(XENO_device_id){
						case XENO_WIFI :
							XNO_Printf("DEVICE ID:XENO WIFI");
							break;
						case XENO_WIFI_ML :
							XNO_Printf("DEVICE ID:XENO WIFI ML");
							break;
						case XENO_NBIOT_ML :
							XNO_Printf("DEVICE ID:XENO NBIOT ML");
							break;
						case XENO_LTE :
							XNO_Printf("DEVICE ID:XENO LTE");
							break;
						case XENO_LTE_ML :
							XNO_Printf("DEVICE ID:XENO LTE ML");
							break;
						default :
							XNO_Printf("DEVICE ID:UNKNOWN DEVICE");
							break;
					}
					XNO_Printf("OK");
				}else{
					XNO_Printf("ERROR");
				}
			}
			break;
		case EX_ATCMD_SWVER:
			if(strcmp((char *)(atcmd_rcvd->ATCmd),"SWVER")==0){
					if(atcmd_rcvd->ATCmd_Type==ATTYPE_TEST){
						XNO_Printf("AT%SWVER");
					}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_READ){
						XNO_Printf("ERROR");
					}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_WRITE){

						XNO_Printf("ERROR");
					}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_EXECUTE){
						string_copy(Print_buf,(uint8_t *)"SWVER-FIRMNAME: ");
						string_append(Print_buf,app_swver.firm_name);
						string_append_CRLF(Print_buf);
						XNO_Printf((char *)Print_buf);
						string_copy(Print_buf,(uint8_t *)"SWVER-NUM: ");
						string_append(Print_buf,app_swver.version);
						XNO_Printf((char *)Print_buf);
						string_copy(Print_buf,(uint8_t *)"SWVER-DATE: ");
						string_append(Print_buf,app_swver.reldate);
						string_append_CRLF(Print_buf);
						XNO_Printf((char *)Print_buf);
						XNO_Printf("OK");
					}else{
						XNO_Printf("ERROR");
					}
				}
			break;
		case EX_ATCMD_WIFIAPCFG:
			if(strcmp((char *)(atcmd_rcvd->ATCmd),"WIFIAPCFG")==0){
				if(atcmd_rcvd->ATCmd_Type==ATTYPE_TEST){
					XNO_Printf("AT%WIFIAPCFG=<APN Name>,<Password>");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_READ){
					string_copy(Print_buf,(uint8_t *)"%WIFIAP-NAME:");
					string_append(Print_buf,wifi_ap_cfg.apn);
					string_append_CRLF(Print_buf);
					XNO_Printf((char *)Print_buf);
					string_copy(Print_buf,(uint8_t *)"%WIFIAP-PWD:");
//					string_append(Print_buf,wifi_ap_cfg.password);
					string_append(Print_buf,"********");
					XNO_Printf((char *)Print_buf);
					XNO_Printf("OK");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_WRITE){
					if(atcmd_rcvd->Num_Args==2){
						strcpy((char*)wifi_ap_cfg.apn,(char*)atcmd_rcvd->ATCmdParamEachStr[0]);
						strcpy((char*)wifi_ap_cfg.password,(char*)atcmd_rcvd->ATCmdParamEachStr[1]);
						XNO_Printf((char*)wifi_ap_cfg.apn);
						XNO_Printf((char*)wifi_ap_cfg.password);
						XNO_Printf("OK");
					}else{
						XNO_Printf("ERROR");
					}
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_EXECUTE){
					XNO_Printf("ERROR");
				}else{
					XNO_Printf("ERROR");
				}
			}
			break;
		case EX_ATCMD_DEVINFO:
			if(strcmp((char *)(atcmd_rcvd->ATCmd),"DEVINFO")==0){
				if(atcmd_rcvd->ATCmd_Type==ATTYPE_TEST){
					XNO_Printf("AT%DEVINFO");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_READ){
					XNO_Printf("ERROR");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_WRITE){
					XNO_Printf("ERROR");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_EXECUTE){
					string_copy(Print_buf,(uint8_t *)"%DEVINFO-DEVNAME:");
					string_append(Print_buf,devinfo.dev_name);
					string_append_CRLF(Print_buf);
					XNO_Printf((char *)Print_buf);
					string_copy(Print_buf,(uint8_t *)"%DEVINFO-IPADDR:");
					string_append(Print_buf,devinfo.ip_addr);
					string_append_CRLF(Print_buf);
					XNO_Printf((char *)Print_buf);
					string_copy(Print_buf,(uint8_t *)"%DEVINFO-MACID:");
					string_append(Print_buf,devinfo.mac_id);
					XNO_Printf((char *)Print_buf);
					XNO_Printf("OK");
				}else{
					XNO_Printf("ERROR");
					break;
				}
			}
			break;
		case EX_ATCMD_EXIT:
			if(strcmp((char *)(atcmd_rcvd->ATCmd),"EXIT")==0){
				if(atcmd_rcvd->ATCmd_Type==ATTYPE_TEST){
					XNO_Printf("AT%EXIT");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_READ){
					XNO_Printf("ERROR");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_WRITE){
					XNO_Printf("ERROR");
				}else if(atcmd_rcvd->ATCmd_Type==ATTYPE_EXECUTE){
					at_cmd_mode_active = RESET;
//					Write_UserCfg_InternalFlash();
					XNO_Printf("%EXIT: COMING OUT OF ATCMD MODE. DO DEVICE RESET");
					XNO_Printf("OK");
					XNO_Printf("OK");
					osDelay(2000);
					XNO_WDG_Init();
					osDelay(1000);
				}else{
					XNO_Printf("ERROR");
				}
			}
			break;
		default:
			XNO_Printf("ERROR: INVALID AT CMD");
			break;
	}
}
