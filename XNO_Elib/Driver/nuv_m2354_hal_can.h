/* File			: nuv_m2354_hal_can.h
* Author		: Ravi Kala
* Company		: Eoxys Systems India Private Limited
* Description	: CAN driver implementation
* Created on	:
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_public.h>
#include "NuMicro.h"
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "XNO_COMMON.h"
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 							Local definitions
*----------------------------------------------------------------------------*/
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

#ifndef __NUV_M2354_HAL_HANDLE_CAN_H
#define __NUV_M2354_HAL_HANDLE_CAN_H

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------------------------------------*/
/* CAN Test Mode Constant Definitions                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define    XNO_CAN_NORMAL_MODE   CAN_NORMAL_MODE    /*!< CAN select normal mode */
#define    XNO_CAN_BASIC_MODE    CAN_BASIC_MODE    /*!< CAN select basic mode */

/*---------------------------------------------------------------------------------------------------------*/
/* Message ID Type Constant Definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define    XNO_CAN_STD_ID    CAN_STD_ID    /*!< CAN select standard ID */
#define    XNO_CAN_EXT_ID    CAN_EXT_ID    /*!< CAN select extended ID */

/*---------------------------------------------------------------------------------------------------------*/
/* Message Frame Type Constant Definitions                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define    XNO_CAN_REMOTE_FRAME  CAN_REMOTE_FRAME    /*!< CAN frame select remote frame */
#define    XNO_CAN_DATA_FRAME    CAN_DATA_FRAME      /*!< CAN frame select data frame */

/*---------------------------------------------------------------------------------------------------------*/
/* CAN Baudrate Constant Definitions                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define XNO_CAN_BAUDRATE_50000					(50000UL)
#define XNO_CAN_BAUDRATE_100000					(100000UL)
#define XNO_CAN_BAUDRATE_125000					(125000UL)
#define XNO_CAN_BAUDRATE_250000					(250000UL)
#define XNO_CAN_BAUDRATE_500000					(500000UL)
#define XNO_CAN_BAUDRATE_1000000				(1000000UL)
#define XNO_CAN_DEFAULT_BAUDRATE		       XNO_CAN_BAUDRATE_500000
/*---------------------------------------------------------------------------------------------------------*/
/* Message RX_TX Message objects config Definitions                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define XNO_CAN_1TX_MSGS_1RX_MSGS		0
#define XNO_CAN_4TX_MSGS_28RX_MSGS		1
#define XNO_CAN_8TX_MSGS_24RX_MSGS		2
#define XNO_CAN_16TX_MSGS_16RX_MSGS		3

#define CAN_NUM_MSG_MAX           		0x20

struct XNO_CAN_VAR
{
	CAN_T		         			*interface;        	// Pointer to the CAN peripheral
	XNO_CAN_NUM_EM					num;
	uint32_t           				baudRate;         	// CAN bus baud rate
	uint32_t           				canMode;       		// CN Mode: BASIC/NORMAL MODE
	uint32_t           				timeOutCnt;       	// Timeout counter

	STR_CANMSG_T           			*txCanMsg[CAN_NUM_MSG_MAX];      	// CAN transmit data buf
	CAN_MSG_OBJ_STS    				txCanMsgSts[CAN_NUM_MSG_MAX];      	// CAN transmit data buf sts: EMPTY, READY, SENT
	uint32_t						txMsgNumOffset;
	uint32_t						txMsgNumMax;
	uint32_t						txCounter;
	osSemaphoreId					txLock;				// Semaphore for transmit locking
	osSemaphoreDef_t				txLockDef;			// Definition for transmit semaphore

	STR_CANMSG_T	           		*rxCanMsg[CAN_NUM_MSG_MAX];      	// CAN receive data buf
	CAN_MSG_OBJ_STS    				rxCanMsgSts[CAN_NUM_MSG_MAX];      	// CAN transmit data buf sts: EMPTY, READY, RCVD
	uint32_t						rxMsgNumOffset;
	uint32_t						rxMsgNumMax;
//	uint32_t						rxLen;
//	uint32_t						rxCounter;
	osSemaphoreId					rxLock;				// Semaphore for receive locking
	osSemaphoreDef_t				rxLockDef;			// Definition for receive semaphore
};
typedef struct XNO_CAN_VAR XNO_CAN_VAR_ST;

/**
  * @brief  CAN complete call back function
  * @param  can_var  - Pointer to specified CAN module
  * @param  msg_num  - Message num
  * @param  cb_event - Call back event
  * @retval None
  */
typedef void (*canCplt_cb)(XNO_CAN_VAR_ST *can_var,uint32_t msg_num,CAN_INT_CB_EVENTS cb_event);

/**
  * @brief  CAN BasicMode Transmit function
  * @param  can_var - Pointer to specified CAN module
  * @param  txCanMsg - CAN transmit data buf
  * @retval HAL_StatusTypeDef- error status
  */
typedef HAL_StatusTypeDef (*canBasicModeTransmit_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *txCanMsg);

/**
  * @brief  CAN BasicMode Transmit Interrupt function
  * @param  can_var  - Pointer to specified CAN module
  * @param  txCanMsg - CAN transmit data buf
  * @retval HAL_StatusTypeDef- error status
  */
typedef HAL_StatusTypeDef (*canBasicModeTransmitInt_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *txCanMsg);

/**
  * @brief  CAN BasicMode Receive function
  * @param  can_var  - Pointer to specified CAN module
  * @param  rxCanMsg - CAN receive data buf
  * @retval HAL_StatusTypeDef- error status
  */
typedef HAL_StatusTypeDef (*canBasicModeReceive_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *rxCanMsg);

/**
  * @brief  CAN BasicMode Receive Interrupt function
  * @param  can_var  - Pointer to specified CAN module
  * @param  rxCanMsg - CAN receive data buf
  * @retval HAL_StatusTypeDef- error status
  */
typedef HAL_StatusTypeDef (*canBasicModeReceiveInt_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *rxCanMsg);

/**
  * @brief  CAN NormalMode Transmit function
  * @param  can_var - Pointer to specified CAN module
  * @param  txCanMsgArr - Tx CAN Message Array
  * @param  num_tx_msgs - Number of Tx Messages
  * @retval HAL_StatusTypeDef- error status
  */
typedef HAL_StatusTypeDef (*canNormalModeTransmitList_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *txCanMsgArr, uint32_t num_tx_msgs);

/**
  * @brief  CAN NormalMode Receive function
  * @param  can_var - Pointer to specified CAN module
  * @param  rxCanMsgArr - Rx CAN Message Array
  * @param  rxmsg_filt_id_arr - Rx Message filter ID Array
  * @param  num_rx_msgs - Number of Rx Messages
  * @retval HAL_StatusTypeDef- error status
*/
typedef  HAL_StatusTypeDef (*canNormalModeReceiveList_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *rxCanMsgArr, uint32_t *rxmsg_filt_id_arr, uint32_t num_rx_msgs);

/**
  * @brief  CAN NormalMode Transmit Interrupt function
  * @param  can_var - Pointer to specified CAN module
  * @param  txCanMsgArr - Tx CAN Message Array
  * @param  num_tx_msgs - Number of Tx Messages
  * @retval HAL_StatusTypeDef- error status
*/
typedef HAL_StatusTypeDef (*canNormalModeTransmitListInt_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *txCanMsgArr, uint32_t num_tx_msgs);

/**
  * @brief  CAN NormalMode Receive Interrupt function
  * @param  can_var - Pointer to specified CAN module
  * @param  rxCanMsgArr - Rx CAN Message Array
  * @param  rxmsg_filt_id_arr - Rx Message filter ID Array
  * @param  num_rx_msgs - Number of Rx Messages
  * @retval HAL_StatusTypeDef- error status
*/
typedef  HAL_StatusTypeDef (*canNormalModeReceiveListInt_func)(XNO_CAN_VAR_ST *can_var, STR_CANMSG_T *rxCanMsgArr, uint32_t *rxmsg_filt_id_arr, uint32_t num_rx_msgs);

/**
  * @brief  CAN Recieve Transmit Interrupt Handler
  * @param  can_num - CAN Num
  * @retval None
*/
extern void CAN_RXTX_INT_HANDLER(XNO_CAN_NUM_EM can_num);

struct XNO_CAN_FUNC
{
	canCplt_cb		            			txCpltCb;
	canCplt_cb		            			rxCpltCb;
	canCplt_cb		           				errCb;
	canBasicModeTransmit_func			    canBasicModeTransmit;
	canBasicModeReceive_func		        canBasicModeReceive;
	canBasicModeTransmitInt_func			canBasicModeTransmitInt;
	canBasicModeReceiveInt_func				canBasicModeReceiveInt;
	canNormalModeTransmitList_func	  		canNormalModeTransmitList;
	canNormalModeReceiveList_func	    	canNormalModeReceiveList;
	canNormalModeTransmitListInt_func		canNormalModeTransmitListInt;
	canNormalModeReceiveListInt_func	    canNormalModeReceiveListInt;
};
typedef struct XNO_CAN_FUNC XNO_CAN_FUNC_ST;

struct XNO_CAN
{
	XNO_CAN_VAR_ST var;
	XNO_CAN_FUNC_ST func;
};
typedef struct XNO_CAN XNO_CAN_ST;


extern XNO_ERR_STS XNO_CAN_Init(CAN_T *interface,
						XNO_CAN_ST *CAN,
						XNO_CAN_NUM_EM can_num,
						uint32_t baudRate,
						uint32_t can_mode,
						uint32_t can_rxtx_msg_num_config,
						uint32_t timeOutInSecs,
						canCplt_cb txCpltCb,
						canCplt_cb rxCpltCb,
						canCplt_cb errCb);

extern void CAN_RXTX_INT_HANDLER(XNO_CAN_NUM_EM can_num);

#ifdef __cplusplus
}
#endif
#endif /* __NUV_M2354_HAL_HANDLE_UART_H */


