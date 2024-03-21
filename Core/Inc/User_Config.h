/* File: User_Config.h
* Author:Prabhakar,Roopa
* Company: Eoxys
* Description: This file provides the user configurations
* Created on: 20-Jun-2021
* Version History
* v1.0 Initial release
*/
#ifndef INC_USER_CONFIG_H_
#define INC_USER_CONFIG_H_
/*----------------------------------------------------------------------------
* 						User Includes
*----------------------------------------------------------------------------*/
#include "main.h"
#include "XNO_DEBUG_TASK.h"
#include "XNO_WIFI_TASK.h"
#include "ATcmds.h"
#include "XNO_SOCKET.h"
/*----------------------------------------------------------------------------
* 						Global definitions
*----------------------------------------------------------------------------*/
#define BUF_SIZE_50					50
#define BUF_SIZE_100				100
#define LINE_STR_LEN_200        	200
#define ACCESS_TOKEN_BUF_LEN		600
#define FLASH_USR_CNFG_STR_BUF_LEN	2048
/*----------------------------------------------------------------------------
* 						Types Declarations
*----------------------------------------------------------------------------*/
typedef enum{
	XENO_WIFI,
	XENO_WIFI_ML,
	XENO_NBIOT_ML,
	XENO_LTE,
	XENO_LTE_ML,
}DEV_ID;


typedef struct{
	uint8_t firm_name[BUF_SIZE_50];
	uint8_t version[BUF_SIZE_50];
	uint8_t reldate[BUF_SIZE_50];
}FW_INFO;

typedef struct{
	uint8_t device_name[BUF_SIZE_50];
}DEV_INFO;

typedef struct{
	uint8_t  name[BUF_SIZE_50];
	uint8_t  content[BUF_SIZE_50*15];
}FOTA_CERT_ST;

//typedef struct{
//	uint8_t	IP[50];
//	uint16_t port;
//}FOTA_SER_CFG;
typedef struct{
	uint8_t ip[50];
	uint16_t port;
	uint8_t	url[100];
}MODEM_SER_CFG;
//typedef enum{
//	FOTA_NONE,
//	FOTA_NDP_MCU,
//	FOTA_NDP_DSP,
//	FOTA_NDP_DNN,
//	FOTA_HOST_MCU,
//	FOTA_STM,
//}FOTA_FW_TYPE;

typedef struct{
	uint8_t  host_ipadr[BUF_SIZE_50];
	uint8_t  device_id[BUF_SIZE_50];
//	uint8_t  code_verifier[SHA256_INPUT_BUF_SIZE];
	uint8_t  authorization_code[BUF_SIZE_50];
	uint8_t  access_token[ACCESS_TOKEN_BUF_LEN];
	uint8_t  Cookie[BUF_SIZE_50];
//	uint8_t  code_challenge[SHA256_HASH_SIZE * 2 + 1];
	uint8_t  fota_upd_sts[BUF_SIZE_50];
	uint8_t  fota_upd_sts_resp[BUF_SIZE_50];
	uint32_t range_start;
	uint32_t chunk_size;
	uint8_t  range_start_str[BUF_SIZE_50];
	uint8_t  range_end_str[BUF_SIZE_50];
	uint8_t  chunk_size_str[BUF_SIZE_50];
	uint8_t  host_firmreq_url[BUF_SIZE_50*10];
	uint8_t  FOTA_Cmd_Payload[WIFI_ATCMD_BUF_SIZE];
	uint8_t  FOTA_Resp_Payload[WIFI_RESP_BUF_SIZE];
	uint8_t  dev_accsess_sts[BUF_SIZE_50];
	uint8_t  fota_upd_check_sts[BUF_SIZE_50];
	uint8_t  fota_file_size_str[BUF_SIZE_50];
	uint32_t fota_file_size;
//	FOTA_ACTIVE_REQ fota_active_req;
	MODEM_SER_CFG fota_ser_cfg;
//	FW_PID_TYPE fota_fw_type;
	uint8_t fota_enable_sts[5];
	uint16_t STM_frame_crc;
	FlagStatus flash_from_bkup_to_main_inprogress;
	uint8_t pkce_secret[50];
}FOTA_SERVER_ST;

/*----------------------------------------------------------------------------
* 						Exported variables
*----------------------------------------------------------------------------*/
#define _fl_user_cfg_data_start 0x00037800
#define _fl_user_cfg_data_end 0x00037FFF
extern FOTA_SERVER_ST fota_server;
extern FW_INFO fota_stm_fw_info,fota_loder_fw_info,fota_boot_fw_info,fota_ndpmcu_fw_info, fota_ndpdsp_fw_info,fota_ndpdnn_fw_info, fota_modem_fw_info,fota_main_fw_info,fota_gps_fw_info;
extern XNO_TCP_SKT_DAIL_ST tcp_ser_cfg;
//extern FW_INFO fota_stm_fw_info, fota_fw_info;
/*----------------------------------------------------------------------------
* 								Exported function Declarations
*----------------------------------------------------------------------------*/
extern uint8_t Write_UserCfg_InternalFlash();
extern void Read_Update_UserCfg_InternalFlash();
extern ITStatus Is_UserCfg_Avail_Flash();
extern FlagStatus Is_access_token_available();
#endif /* INC_USER_CONFIG_H_ */
