/* File: XNO_MQTT.h
* Author: Rajesh
* Company: Eoxys
* Description: APIs for MQTT communication
* Created on:  24-Oct-2023
* Version History
* v1.0 Initial release
*/
#ifndef XNO_MQTT_H_
#define XNO_MQTT_H_

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
typedef struct XNO_MQTT_VAR
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
}XNO_MQTT_VAR_ST;


typedef void (*mqtt_cplt_cb)(XNO_MQTT_VAR_ST *mqtt_var);

/*Socket Create function*/
typedef XNO_ERR_STS (*mqtt_set_config_func)(XNO_MQTT_KEY_ST *key, uint8_t *val);

/*Socket Send function*/
typedef int (*mqtt_connect_to_broker_func)(uint8_t *broker_addr, uint16_t port);

/*Socket Receive function*/
typedef XNO_ERR_STS (*mqtt_client_disconnect_func)(int mqtt_nw_ID);

//*Socket Is Data Avail function*/
typedef XNO_ERR_STS (*mqtt_subscribe_topic_func)(int mqtt_nw_ID, uint8_t *mqtt_topic, XNO_MQTT_QOS_ST *mqtt_qos);

//*Socket Close function*/
typedef XNO_ERR_STS (*mqtt_unsubscribe_topic_func)(int mqtt_nw_ID, uint8_t *mqtt_topic);

typedef XNO_ERR_STS (*mqtt_publish_topic_func)(int mqtt_nw_ID, uint8_t *mqtt_topic, XNO_MQTT_QOS_ST *mqtt_qos, uint8_t *data);

/*Modem ATCMD send and wait resp function*/
//typedef XNO_ATCMD_RESP (*atcmd_send_wait_resp_func)(const uint8_t *atbuf,uint32_t timeout,WIFI_RESP_LINES_STRUCT *resp_lines);

typedef struct XNO_MQTT_FUNC
{
	//Socket callback functions
	mqtt_cplt_cb		            rxCpltCb;
	mqtt_cplt_cb		            errCb;
	//Socket functions
	mqtt_set_config_func		    mqttSetConfig;
	mqtt_connect_to_broker_func     mqttConnectToBroker;
	mqtt_client_disconnect_func     mqttClientDisconnect;
	mqtt_subscribe_topic_func	    mqtt_subscribe_topic_func;
	mqtt_unsubscribe_topic_func	    mqtt_unsubscribe_topic_func;
	mqtt_publish_topic_func	        mqtt_publish_topic_func;
	//modem UART Interface functions
//	atcmd_send_wait_resp_func		atcmdSendWaitResp;

}XNO_MQTT_FUNC_ST;

typedef struct XNO_MQTT
{
	XNO_MQTT_VAR_ST var;
	XNO_MQTT_FUNC_ST func;
}XNO_MQTT_ST;

extern XNO_ERR_STS XNO_Mqtt_Init(XNO_MQTT_ST *mqtt_st,
								XNO_MODEM_TYPE_EM modem_type,
								mqtt_cplt_cb rxCpltCb,
								mqtt_cplt_cb errCb,
								CB_EN_DIS_EM cb_status);
#endif /* XNO_MQTT_H_ */
