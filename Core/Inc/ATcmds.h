/* File: ATcmds.h
 *
 *  Created on: 21-may-2023
 *      Author: Prabhakar, Rajesh
 */
#ifndef INC_ATCMDS_H_
#define INC_ATCMDS_H_
/*----------------------------------------------------------------------------
 * 						       User Includes
 *----------------------------------------------------------------------------*/
#include "XNO_DEBUG_TASK.h"
#include "ATcmds.h"
/*----------------------------------------------------------------------------
 * 								Typedefs
 *----------------------------------------------------------------------------*/
typedef enum {
	EX_ATCMD_INVALID,
	EX_ATCMD_SWVER,
	EX_ATCMD_WIFIAPCFG,
//	EX_ATCMD_TCPSERCFG,
	EX_ATCMD_FOTASERCFG,
	EX_ATCMD_FOTA_ENABLE,
	EX_ATCMD_DEVINFO,
	EX_ATCMD_DEVID,
	EX_ATCMD_PKCE,
	EX_ATCMD_FWDNLD,
	EX_ATCMD_FWERASE,
	EX_ATCMD_FWHDR,
	EX_ATCMD_FWSTS,
	EX_ATCMD_NDPCHIPID,
	EX_ATCMD_DBG_BAUDRATE,
	EX_ATCMD_BYP_MDM_UART,
	EX_ATCMD_EXIT,
	EX_ATCMD_END = -1
} ATCMD_EXT_MASTER_ID;

typedef struct {
	uint8_t dev_name[50];
	uint8_t ip_addr[20];
	uint8_t mac_id[20];
} DEVINFO;

typedef struct {
	uint8_t apn[50];
	uint8_t password[50];
} WIFI_AP_CFG;

/*----------------------------------------------------------------------------
 * 								Exported variables
 *----------------------------------------------------------------------------*/
extern const ATCMDS_LIST atcmd_extlist[];
extern WIFI_AP_CFG wifi_ap_cfg;
extern DEVINFO devinfo;
extern uint16_t atcmd_extlist_len;
/*----------------------------------------------------------------------------
 * 						Exported function Declarations
 *----------------------------------------------------------------------------*/
extern FlagStatus Is_at_cmd_mode_active();

#endif /* INC_ATCMDS_H_ */
