/* File      : main.h
* Author     : Rajesh
* Company    : Eoxys
* Description:
* Created on : 28-Nov-2023
* Version    : 13
*/
#ifndef __MAIN_H
#define __MAIN_H
/*----------------------------------------------------------------------------
 * 							Standard library includes
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cmsis_os.h"

#include "XNO_OS_TIMERS.h"
//#include "MCU_Firmware_Flash.h"
//#include "fw_flash_state_machine.h"
/*----------------------------------------------------------------------------
 * 							Types Declarations
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * 							Local definitions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * 							Global definitions
 *----------------------------------------------------------------------------*/
#define	EXT_FLASH_SECTOR_SIZE	4096

/*NDP FW Header Start address*/
#define NDP_MCU_HDR_START_ADDR 0x000000UL
#define NDP_DSP_HDR_START_ADDR 0x018000UL
#define NDP_DNN_HDR_START_ADDR 0x080000UL

/*NDP FW Start address*/
#define NDP_MCU_FW_START_ADDR 0x001000UL
#define NDP_DSP_FW_START_ADDR 0x019000UL
#define NDP_DNN_FW_START_ADDR 0x081000UL

/*BTLD Header Start address*/
#define BTLD_HDR_START_ADDR 0x180000UL

/*BTLD Start address*/
#define BTLD_START_ADDR 0x181000UL

/*GPS FW Header Start address*/
#define GPS_FW_HDR_START_ADDR 0x200000UL

/*GPS FW Start address*/
#define GPS_FW_START_ADDR 0x201000UL

/*----------------------------------------------------------------------------
 * 							static global variables
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * 							Global variables
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * 							Local functions
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * 							Exported functions
 *----------------------------------------------------------------------------*/
extern XNO_OSTIMER_ST NDP_timer_st,wifi_timer_st;
extern void Error_Handler(void);

extern S_RTC_TIME_DATA_T sWriteRTC;
extern void Reset_NDP120(void);
/* Private defines -----------------------------------------------------------*/

/* GPIO PIN -----------------------------------------------------------*/

#ifndef __weak
#define __weak  __attribute__((weak))
#endif

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

#endif
