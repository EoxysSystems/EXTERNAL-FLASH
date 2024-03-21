/* File: XNO_BLE.h
* Author: Rajesh
* Company: Eoxys
* Description: APIs for BLE communication
* Created on:  28-Oct-2023
* Version History
* v1.0 Initial release
*/
#ifndef XNO_BLE_H_
#define XNO_BLE_H_

/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								User Includes
*----------------------------------------------------------------------------*/
#include <XNO_COMMON.h>
#include <XNO_MODEM.h>
//#include "BLE.h"
/*----------------------------------------------------------------------------
* 								Defines
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Macros
*----------------------------------------------------------------------------*/
#define MAX_BLE_CONN      10


#define GATT_PROP_BCAST                  0x01//1 //!< Permits broadcasts of the Characteristic Value
#define GATT_PROP_READ                   0x02//2 //!< Permits reads of the Characteristic Value
#define GATT_PROP_WRITE_NO_RSP           0x04//4 //!< Permits writes of the Characteristic Value without response
#define GATT_PROP_WRITE                  0x08//8 //!< Permits writes of the Characteristic Value with response
#define GATT_PROP_NOTIFY                 0x10//16 //!< Permits notifications of a Characteristic Value without acknowledgement
#define GATT_PROP_INDICATE               0x20//32 //!< Permits indications of a Characteristic Value with acknowledgement
#define GATT_PROP_AUTHEN                 0x40//64//!< Permits signed writes to the Characteristic Value
#define GATT_PROP_EXTENDED               0x80//128//!< Additional characteristic properties are defined in the Characteristic Extended Properties Descriptor
/*----------------------------------------------------------------------------
* 								Exported Variables
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Exported Functions
*----------------------------------------------------------------------------*/
typedef struct XNO_BLE_VAR
{
	XNO_MODEM_TYPE_EM   modem_type;
	uint8_t*			txBuff;
	uint32_t			txLen;
	osSemaphoreId		txLock;
	osSemaphoreDef_t	txLockDef;

	uint8_t*			rxBuff;
	uint32_t			rxLen;
	uint32_t			rxCbEn;
	osSemaphoreId		rxLock;
	osSemaphoreDef_t	rxLockDef;
}XNO_BLE_VAR_ST;

/*BLE Config function*/
//typedef void (*bleCplt_cb)(XNO_BLE_EVENT_EM event,XNO_BLE_VAR_ST *ble_var);
typedef void (*bleCplt_cb)(XNO_BLE_EVENT_EM event,uint8_t *ble_var);
typedef void (*bleErrorCplt_cb)(XNO_BLE_VAR_ST *ble_var);

typedef XNO_ERR_STS (*bleStackInit_func)();
typedef XNO_ERR_STS (*bleGlobalConfig_func)(uint8_t *mac_addr, XNO_BLE_ADDR_TYPE_EM ble_addr_type, uint8_t *dev_name);
/*BLE TxPower Config function*/
typedef XNO_ERR_STS (*bleTxPowerConfig_func)(uint32_t txpower);
/*BLE Peripheral Advertisement Config function*/
typedef XNO_ERR_STS (*blePeriAdvConfig_func)(XNO_BLE_ADVCFG_ST ble_advcfg);
typedef XNO_ERR_STS (*bleScanConfig_func)(XNO_BLE_SCANCFG_ST ble_scancfg);
typedef XNO_ERR_STS (*bleConnConfig_func)(XNO_BLE_CONNCFG_ST ble_conncfg);

/*BLE Peripheral Start Advertisement function*/
typedef XNO_ERR_STS (*blePeriStartAdv_func)(uint8_t *adv_databuf,uint8_t adv_len);
typedef XNO_ERR_STS (*blePeriStopAdv_func)();

typedef XNO_ERR_STS (*bleGattCreateService_func)(BLE_SERVICE_TYPE_EM ser_type,uint8_t *ser_uuid);
typedef XNO_ERR_STS (*bleGattAddService_func)(BLE_SERVICE_TYPE_EM ser_type,uint8_t *ser_uuid);
typedef XNO_ERR_STS (*bleGattAddChar_func)(BLE_SERVICE_TYPE_EM ser_type,uint8_t *ser_uuid, uint16_t char_uuid, uint8_t properties, XNO_BLE_CHAR_PERMISSION_EM permission);

typedef XNO_ERR_STS (*bleGattRemoveService_func)(BLE_SERVICE_TYPE_EM ser_type,uint8_t *ser_uuid);
typedef XNO_ERR_STS (*bleGattDescrptionToChar_func)(uint16_t char_uuid,uint16_t desc_uuid, uint8_t properties, XNO_BLE_CHAR_PERMISSION_EM permission);

typedef XNO_ERR_STS (*bleGattServerStart_func)();
typedef XNO_ERR_STS (*bleGattServerStop_func)();

typedef XNO_ERR_STS (*bleGattCharNotify_func)(uint8_t *char_uuid,uint8_t *notify_data);
typedef XNO_ERR_STS (*bleGattCharIndicate_func)(uint8_t *char_uuid,uint8_t *indication_data);
typedef XNO_ERR_STS (*bleGattCharSendReadReq_func)(uint8_t *char_uuid,uint8_t *rd_req_data);
typedef XNO_ERR_STS (*bleGattCharAckWriteReq_func)(uint8_t *char_uuid, uint16_t data_len);
typedef XNO_ERR_STS (*bleGattDescSendReadReq_func)(uint8_t *desc_uuid,uint8_t *rd_req_data);
typedef XNO_ERR_STS (*bleGattDescAckWriteReq_func)(uint8_t *desc_uuid, uint16_t data_len, uint8_t *wr_req_data);

typedef XNO_ERR_STS (*bleGattReadCharValue_func)(uint16_t conn_id, uint16_t attr_handle, uint8_t *rd_data);
typedef XNO_ERR_STS (*bleGattWriteCharValue_func)(uint16_t conn_id, uint16_t attr_handle, uint8_t *wr_data);

//typedef XNO_ERR_STS (*bleCentralScanStart_func)(uint8_t *scan_result_buf);
typedef XNO_ERR_STS (*bleCentralScanStart_func)();

typedef int (*bleConnect_func)(uint8_t *peer_addr, XNO_BLE_ADDR_TYPE_EM peer_addr_type);
typedef XNO_ERR_STS (*bleDisconnect_func)(uint16_t conn_id);

typedef XNO_ERR_STS (*bleGattDiscoverServices_func)(uint16_t conn_id);
typedef XNO_ERR_STS (*bleGattDiscoverServicesChar_func)(uint16_t conn_id, uint16_t serv_attr_start_handle,uint16_t serv_attr_end_handle);
typedef XNO_ERR_STS (*bleGattDiscoverCharDesc_func)(uint16_t conn_id, uint16_t char_attr_start_handle,uint16_t char_attr_end_handle);

typedef XNO_ERR_STS (*bleConfigSmp_func)(XNO_BLE_SMP_CFG_ST smp_cfg);
typedef XNO_ERR_STS (*bleAuthSmp_func)(uint16_t conn_id, XNO_BLE_SMP_CFG_ST smp_cfg);
typedef XNO_ERR_STS (*bleConfigFota_func)(uint8_t *service_id, uint16_t char_id);
typedef struct XNO_BLE_FUNC
{
	//BLE Network functions
	bleCplt_cb                          rxCpltCb;
	bleErrorCplt_cb                          errCb;

	bleStackInit_func                  	bleStackInit;

	bleGlobalConfig_func		        bleGlobalConfig;
	bleTxPowerConfig_func		        bleTxPowerConfig;
	blePeriAdvConfig_func	         	blePeriAdvConfig;
	bleScanConfig_func                 	bleScanConfig;
	bleConnConfig_func                 	bleConnConfig;

	blePeriStartAdv_func		        blePeriStartAdv;
	blePeriStopAdv_func		         	blePeriStopAdv;

	bleGattCreateService_func	     	bleGattCreateService;
	bleGattAddService_func		     	bleGattAddService;
	bleGattAddChar_func		         	bleGattAddChar;

	bleGattRemoveService_func	     	bleGattRemoveService;
	bleGattDescrptionToChar_func	 	bleGattDescrptionToChar;

	bleGattServerStart_func	         	bleGattServerStart;
	bleGattServerStop_func	         	bleGattServerStop;

	bleGattCharNotify_func	         	bleGattCharNotify;
	bleGattCharIndicate_func	        bleGattCharIndicate;
	bleGattCharSendReadReq_func	 		bleGattCharSendReadReq;
	bleGattCharAckWriteReq_func	 		bleGattCharAckWriteReq;
	bleGattDescSendReadReq_func     	bleGattDescSendReadReq;
	bleGattDescAckWriteReq_func	 		bleGattDescAckWriteReq;

	bleGattReadCharValue_func	     	bleGattReadCharValue;
	bleGattWriteCharValue_func	     	bleGattWriteCharValue;

	bleCentralScanStart_func	        bleCentralScanStart;

	bleConnect_func	                 	bleConnect;
	bleDisconnect_func	                bleDisconnect;

	bleGattDiscoverServices_func	    bleGattDiscoverServices;
	bleGattDiscoverServicesChar_func 	bleGattDiscoverServicesChar;
	bleGattDiscoverCharDesc_func	 	bleGattDiscoverCharDesc;

	bleConfigSmp_func	                bleConfigSmp;
	bleAuthSmp_func	                 	bleAuthSmp;
	bleConfigFota_func	             	bleConfigFota;
}XNO_BLE_FUNC_ST;

typedef struct XNO_BLE{
	XNO_BLE_VAR_ST var;
	XNO_BLE_FUNC_ST func;
}XNO_BLE_ST;
extern XNO_ERR_STS XNO_Ble_Init(XNO_BLE_ST *ble_st,
		                        XNO_MODEM_TYPE_EM modemType,
								bleCplt_cb rxCpltCb,
								bleErrorCplt_cb errCb,
								FlagStatus cb_status);

extern void ble_receive_cb(XNO_BLE_EVENT_EM event,uint8_t *ble_data,uint32_t data_len);
#endif /* XNO_BLE_H_ */
