/*
 * XNO_COMMON.h
 *
 *  Created on: 16-Oct-2023
 *      Author: Rajesh
 */
#include "nuv_m2354_hal_public.h"
#include "stdbool.h"
#include "XNO_DEBUG_TASK.h"

#ifndef XNO_COMMON_H_
#define XNO_COMMON_H_
/*----------------------------------------------------------------------------
* 							Defines
*----------------------------------------------------------------------------*/
#define BLE_ADV_DATA_SIZE                 500
#define MODEM_ATCMD_SIZE				1500
#define STR_MATCHING                      0
#define ATCMD_SIZE   		       100
#define NBIOT_RESP_PAYLOAD_SIZE    	1500
#define NBIOT_RESP_HEADER_SIZE    	50
#define NBIOT_RESP_CSV_PARAM_SIZE     100
#define NBIOT_RESP_NUM_CSV_PARAMS     18
/*----------------------------------------------------------------------------
* 								Task Arguments Struct
*----------------------------------------------------------------------------*/
typedef struct{
	UART_T *interface;
	XNO_UART_NUM_EM uart_num;
	uint32_t baudRate;
	XNO_GPIO_ST gpio_modem_pwr;
	XNO_GPIO_ST gpio_modem_reset;
	XNO_GPIO_ST gpio_modem_pwrkey;
	XNO_GPIO_ST gpio_modem_cts;
	XNO_GPIO_ST gpio_modem_sts;
}XNO_UART_TASK_PARAM_ST;
typedef struct{
	ATCMDS_LIST *atcmd_ext_list;
}XNO_PARSER_TASK_PARAM_ST;
/*----------------------------------------------------------------------------
* 								Modem common protocol struct and enum
*----------------------------------------------------------------------------*/
typedef struct{
	uint8_t BSSID[50];
	uint16_t channel;
	int16_t RSSI;
	uint8_t SSID[50];
}XNO_WIFI_SCAN_RESP;
typedef enum{
	WIFI_NONE_RCVD,
	WIFI_OK_RCVD,
	WIFI_ERROR_RCVD,
	WIFI_ECHO_RCVD,
	WIFI_RESP_RCVD,
	WIFI_TIMEOUT_RCVD,
}XNO_WIFI_ATCMD_RESP_EM;
typedef enum{
	LTE_NONE_RCVD,
	LTE_ECHO_RCVD,
	LTE_ERROR_RCVD,
	LTE_OK_RCVD,
	LTE_RESP_RCVD,
	LTE_EVENT_RCVD,
	LTE_TIMEOUT_RCVD,
	LTE_RESP_KEY_RCVD,
	LTE_HTTP_EVENT_RCVD
}XNO_LTE_ATCMD_RESP_EM;
typedef enum {
	XNO_WLAN_STS_IP,
	XNO_WLAN_STS_RSSI,
	XNO_WLAN_STS_PKT_COUNTERS,
	XNO_WLAN_STS_MAC_ADDR
}XNO_WIFI_STATUS_ID_EM;
typedef enum {
	XNO_LTE_WLAN_STS_IP,
	XNO_LTE_WLAN_STS_RSSI,
	XNO_LTE_WLAN_STS_PKT_COUNTERS,
	XNO_LTE_WLAN_STS_MAC_ADDR
}XNO_LTE_STATUS_ID_EM;
typedef struct{
	uint8_t cid;
	uint8_t PDP_type[10];
	uint8_t APN[20];
	uint8_t PDP_addr[50];
	uint8_t PDP_data_comp;
	uint8_t PDP_header_comp;
	uint8_t ipv4_ctrl;
	uint8_t request_type;
	uint8_t P_CSCF_discovery;
	uint8_t IM_CN_Signalling_Flag_Ind;
	uint8_t NSLPI;
	uint8_t securePCO;
	uint8_t IPv4_MTU_discovery;
	uint8_t Local_Addr_Ind;
	uint8_t Non_IP_MTU_discovery;
	uint8_t Reliable_Data_Service;
}PDP_CONTEXT_ST;
typedef enum{
	CB_DIS,
	CB_EN
}CB_EN_DIS_EM;
typedef enum{
	XNO_SOC_NO_EVENT,
	XNO_SOC_DATA_EVENT,
	XNO_SOC_CLOSE_EVENT,
	XNO_SOC_HTTP_DATA_EVENT,
	XNO_SOC_MQTT_DATA_EVENT,
	XNO_SOC_WAKEUP_EVENT,
	XNO_SOC_TCP_SER_CONN_EVENT,
	XNO_WIFI_SCAN_EVENT,
	XNO_WIFI_DISCONNECTED,
	XNO_BLE_EVENT
}WIFI_EVENT_ID_EM;
//typedef void (*WIFI_RECEIVE_CB_PTR)(uint8_t*);
typedef void (*WIFI_WAKEUP_RECEIVE_CB_PTR)(void);
typedef struct{
	uint8_t active_inerface;
	uint8_t ATCmdBuf[MODEM_ATCMD_SIZE];
	FlagStatus user_on_soc_receive_cb_valid;
	FlagStatus user_on_http_receive_cb_valid;
	FlagStatus user_on_mqtt_receive_cb_valid;
	FlagStatus user_on_ble_receive_cb_valid;
	WIFI_EVENT_ID_EM cur_wifi_event;
	uint16_t cur_wifi_resp_id;
	FlagStatus cur_wifi_resp_with_ok;
//	WIFI_RECEIVE_CB_PTR user_on_receive_cb_ptr;
	WIFI_WAKEUP_RECEIVE_CB_PTR user_wakeup_cb_ptr;
}WIFI_CONTEXT_ST;
typedef struct{
	uint16_t resp_start_idx;
	uint16_t num_resp;
}WIFI_RESP_LINES_ST;
typedef enum{
	SOC_TCP,
	SOC_UDP
}XNO_SOC_PROTOCOL_EM;
typedef enum{
	SOC_DTYPE_ASCII,
	SOC_DTYPE_BINARY
}XNO_SOC_DATATYPE_EM;
extern uint8_t retry_atcmd_cnt;
/*----------------------------------------------------------------------------
* 								MQTT protocol struct and enum
*----------------------------------------------------------------------------*/
typedef enum{
	XNO_MQTT_QOS_INVALID,
	XNO_MQTT_QOS_AT_MOST_ONCE,
	XNO_MQTT_QOS_AT_LEAST_ONCE,
	XNO_MQTT_QOS_EXACTLY_ONCE
}XNO_MQTT_QOS_ST;
typedef enum{
	XNO_MQTT_KEY_INVALID,
	XNO_MQTT_KEY_CLIENT_ID,
	XNO_MQTT_KEY_USR_NAME,
	XNO_MQTT_KEY_PASSWORD,
	XNO_MQTT_KEY_CLEAN_SESSION,
	XNO_MQTT_KEEP_ALIVE_INTERVAL
}XNO_MQTT_KEY_ST;
typedef enum{
	XNO_MQTT_TRANS_NONE=-1,
	XNO_MQTT_TRANS_OVER_TCP,
	XNO_MQTT_TRANS_OVER_TLS,
	XNO_MQTT_TRANS_WEB_SOCKET,
	XNO_MQTT_TRANS_SEC_WEB_SOCKET
}XNO_MQTT_TRANSPORT_EM;

typedef enum{
	XNO_MQTT_CER_NONE,
	XNO_MQTT_NO_CERT_VERIFY,
	XNO_MQTT_CERT_VERIFY
}XNO_MQTT_CERT_VERIFY_EM;

typedef struct{
	uint8_t mqtt_broker_addr[50];
	uint16_t mqtt_port;
	XNO_MQTT_TRANSPORT_EM transport;
	XNO_MQTT_CERT_VERIFY_EM cert_verify;
	uint8_t cert_file_name[100];
	uint8_t web_soc_uri[100];
}XNO_MQTT_CONNECT_ST;

/*----------------------------------------------------------------------------
* 								HTTP protocol struct and enum
*----------------------------------------------------------------------------*/
typedef enum{
	XNO_SECURE_INVALID,
	XNO_NON_SECURE,
	XNO_HTTPS_WO_CER_VALIDATION,
	XNO_HTTPS_WITH_CER_VALIDATION
}XNO_HTTP_SECURE_MODE_EM;
typedef enum{
	XNO_HTTP_GET,
	XNO_HTTP_HEAD,
	XNO_HTTP_POST,
	XNO_HTTP_PUT,
	XNO_HTTP_DEL,
}XNO_HTTP_REQ_METHOD_EM;
typedef enum{
	//Wifi param
	XNO_HTTP_HDRID_INALID,
	XNO_HTTP_HDRID_ALLOW,
	XNO_HTTP_HDRID_AUTH,
	XNO_HTTP_HDRID_CONN_TYPE,
	XNO_HTTP_HDRID_CONTENT_ENCODE,
	XNO_HTTP_HDRID_CONTENT_LENGTH,
	XNO_HTTP_HDRID_CONTENT_RANGE,
	XNO_HTTP_HDRID_CONTENT_TYPE,
	XNO_HTTP_HDRID_COOKIE,
	XNO_HTTP_HDRID_COOKIE2_HDR,
	XNO_HTTP_HDRID_DATE_TIME,
	XNO_HTTP_HDRID_HDR_EXPIRE_DATE_TIME,
	XNO_HTTP_HDRID_USR_EMAIL_ADDR,
	XNO_HTTP_HDRID_DOMAIN_NAME,
	XNO_HTTP_HDRID_RES_MOD_DATE_TIME,
	XNO_HTTP_HDRID_RES_LAST_MOD_DATE_TIME,
	XNO_HTTP_HDRID_REDIRECT_URL,
	XNO_HTTP_HDRID_SPECIFIC_IMPLE,
	XNO_HTTP_HDRID_ONLY_REMOTE_RES,
	XNO_HTTP_HDRID_PREV_PAGE_ADDR,
	XNO_HTTP_HDRID_SERVER_GEN_RESP_ADDR,
	XNO_HTTP_HDRID_SEND_COOKIE,
	XNO_HTTP_HDRID_ENCODE,
	XNO_HTTP_HDRID_CLIENT,
	XNO_HTTP_HDRID_AUTH_METHOD = 34,
	//LTE param
	XNO_HTTP_URL,
	XNO_HTTP_CONNECT_TO,
	XNO_HTTP_RECV_TO,
	XNO_HTTP_CONTENT_TYPE,
	XNO_HTTP_ACCEPT_TYPE,
	XNO_HTTP_SSLCFG,
	XNO_HTTP_USERDATA,
	XNO_HTTP_READMODE
}XNO_HTTP_HDR_ID_EM;
typedef enum{
	XNO_HTTP_ERR_STS_SUCCESS,
	XNO_HTTP_ERR_STS_BAD_REQ,
	XNO_HTTP_ERR_STS_NOT_FOUND,
}XNO_HTTP_ERR_CODE_EM;

/*----------------------------------------------------------------------------
* 								TCP SOCKET protocol struct and enum
*----------------------------------------------------------------------------*/

typedef struct{
	uint8_t connId; /* range 1..6. Socket connection identifier */
	uint8_t txProt;  /* Transmission protocol
	                 	 0: TCP
	                 	 1: UDP */
	uint16_t rPort;  /* range 0..65535. Remote host port to contact */
	uint8_t* IPaddr; /* Address of the remote host */
	uint8_t closureType; /* Socket closure behavior for TCP
	                     	0: Local host hangs up immediately after remote does (default)
	                     	255: Local host closes after an escape sequence (+++) */
	uint16_t lPort; /* range 0..65535. UDP connection local port,ignored for TCP connections */
	uint8_t connMode; /* Connection mode
	                  	 0: Online mode connection (default)
	                     1: Command mode connection */
}XNO_TCP_SKT_DAIL_ST;
/*----------------------------------------------------------------------------
* 								BLE protocol struct and enum
*----------------------------------------------------------------------------*/
typedef struct{
	XNO_WIFI_ATCMD_RESP_EM resp_rcvd;
	uint16_t resp_header;
	uint16_t resp_len;
	uint16_t resp_id;
	uint8_t descr[200];
	uint8_t payload[2000];
}XNO_WIFI_BLE_RESP_ST;
typedef struct{
	XNO_LTE_ATCMD_RESP_EM resp_rcvd;
	uint16_t resp_header;
	uint16_t resp_len;
	uint16_t resp_id;
	uint8_t descr[200];
	uint8_t payload[1500];
}XNO_LTE_BLE_RESP_ST;
typedef enum{
	BLE_NONE_RCVD,
	BLE_ERROR_RCVD,
	BLE_OK_RCVD,
	BLE_TIMEOUT_RCVD,
}XNO_BLE_ATCMD_RESP_EM;
typedef struct{
	uint16_t fast_adv_interval;
	uint16_t fast_adv_duration;
	uint16_t slow_adv_interval;
	uint16_t slow_adv_duration;
}XNO_BLE_ADVCFG_ST;
typedef struct{
	uint32_t scan_period;
	uint16_t fscan_interval;
	uint16_t fscan_window;
	uint16_t bscan_interval;
	uint16_t bscan_window;
	bool scan_filter;
}XNO_BLE_SCANCFG_ST;
typedef struct{
	uint16_t conn_interval;
	uint8_t conn_latency;
	uint16_t conn_timeout;
	bool conn_params_reject;
	uint8_t conn_interval_min;
	uint8_t conn_interval_max;
}XNO_BLE_CONNCFG_ST;
typedef enum{
	BLE_READ,
	BLE_WRITE,
	BLE_READ_WRITE
}XNO_BLE_CHAR_PROPERTY_EM;
typedef enum{
	BLE_PERM_NONE,
	BLE_PERM_READ_WRITE
}XNO_BLE_CHAR_PERMISSION_EM;
typedef enum{
	BLE_16BIT_SERVICE,
	BLE_128BIT_SERVICE
}BLE_SERVICE_TYPE_EM;
typedef struct{
	bool iocap;
	bool OOB;
	bool bondable;
	bool MITM_protection;
	bool secure_conn;
	bool keypress;
	uint8_t min_key_size;
	bool encryption;
}XNO_BLE_SMP_CFG_ST;
typedef enum{
	BLE_INVALID_ADDR_TYPE,
	BLE_PUBLIC_ADDR,
	BLE_RANDOM_ADDR
}XNO_BLE_ADDR_TYPE_EM;
typedef struct{
	uint8_t MAC_addr[20];
	int8_t RSSI;
	XNO_BLE_ADDR_TYPE_EM addr_type;
	uint8_t data_len;    //-------------------------------
	uint8_t data[BLE_ADV_DATA_SIZE];
}XNO_BLE_SCAN_RESP_ST;
typedef enum{
	BLE_INVALID_SERVICE,
	BLE_PRIMARY_SERVICE,
	BLE_SECONDARY_SERVICE,
	BLE_CUSTOME_SERVICE
}XNO_BLE_SER_TYPE_EM;
typedef enum{
	BLE_CHAR_WRITE,
	BLE_CHAR_WRITE_WITH_RESP,
	BLE_CHAR_SIGNED_WRITE
}XNO_BLE_CHAR_WRITE_TYPE;
typedef struct{
	XNO_BLE_SER_TYPE_EM service_type;
	uint16_t start_attr_handle;
	uint16_t end_attr_handle;
	uint8_t ser_uuid_len;
	uint8_t ser_uuid[50];
}XNO_BLE_GATT_SER_ST;
typedef struct{
	uint16_t handle;
	uint8_t property;
	uint8_t value_handle;
	uint8_t char_uuid_len;
	uint8_t char_uuid[50];
}XNO_BLE_GATT_CHAR_ST;
typedef struct{
	uint16_t attr_handle;
	uint8_t char_desc_uuid_len;
	uint8_t char_desc_uuid[50];
}XNO_BLE_GATT_DESC_ST;
typedef enum{
	BLE_NO_EVENT,
	BLE_SCAN,
	BLE_PERI_CONNECTED,
	BLE_PERI_DISCONNECTED,
	BLE_CENTRAL_CONNECTED,
	BLE_CENTRAL_DISCONNECTED,
	BLE_CHAR_WR_REQ,
	BLE_DESCS_WR_REQ,
	BLE_CHAR_RD_REQ,
	BLE_DESCS_RD_REQ,
	BLE_CHAR_NOTIFY_REQ,
	BLE_CHAR_IND_REQ,
	BLE_DISCOVERED_SERVICES,
	BLE_DISCOVERED_CHARS,
	BLE_DISCOVERED_DESCS,
	WIFI_DISCONNECTED
}XNO_BLE_EVENT_EM;
typedef struct BLE_CONTEXT{
	XNO_BLE_ADDR_TYPE_EM ble_addr_type;
	uint8_t active_inerface;
	uint8_t ATCmdBuf[MODEM_ATCMD_SIZE];
	FlagStatus user_on_ble_receive_cb_valid;
	uint8_t filter_type;
	uint8_t filter_data[20];
//	BLE_RECEIVE_CB_PTR user_ble_receive_cb_ptr;
}BLE_CONTEXT_ST;
/*----------------------------------------------------------------------------*/
typedef struct{
	uint8_t dd;
	uint8_t mm;
	uint8_t yy;
}GPS_DATE_ST;
typedef struct{
//	uint8_t latitude[20];
	float latitude;
	uint8_t N_or_S;
//	uint8_t longitude[20];
	float longitude;
	uint8_t E_or_W;
	GPS_DATE_ST date;
	uint8_t UTC_Time[10];
	uint16_t altitude;
	uint16_t speed_over_gnd;
	uint16_t course;
	uint16_t mode;
	uint16_t gps_cnt;
	uint16_t glonass_cnt;
	uint16_t beidou_cnt;
}GPS_INFO_ST;

typedef struct{
	uint8_t GGA;
	uint8_t GLL;
	uint8_t GSA;
	uint8_t GSV;
	uint8_t RMC;
	uint8_t VTG;
	uint8_t ZDA;
	uint8_t ANT;
	uint8_t DHV;
	uint8_t LPS;
	uint8_t res1reserved;
	uint8_t res2reserved;
	uint8_t UTC;
	uint8_t GST;
}GPS_NMEA_ST;

typedef enum{
	GPS = 1,
	BDS,
	GPS_BDS,
	GLONASS,
	GPS_GLONASS,
	BDS_GLONASS,
	GPS_BDS_GLONASS,
}GPS_MODE_TYPE_EM;

typedef struct{
	double latitude;
	double longitude;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint32_t second;
}GNSS_REF_INFO_ST;

typedef struct{
	uint16_t GNSS_ID_ENABLE; /* bit0: GPS L1 C/A, bit1: GLONASS L10F, bit2: BeiDou
                               B1I, bit3: Galileo E1 B/C, bit4: SBAS L1 C/A, bit5:
                               QZSS L1, Default Value: 0x1 (GPS L1 C/A is Enabled) */
	uint8_t NUM_TRACK_CHAN; /* Range: 6 – 20, Default Value: 8 */
	uint8_t NAV_TRACK_MODE; /* 0 = Continuous Tracking, 1 = Power Saving Mode, Default Value: 0 */
	uint8_t NAV_FIX_RATE_HZ; /* Range: 1 – 20 Hz, Default Value: 1 */
	uint16_t NAV_FIX_INTERVAL; /* Range: 3 – 600 seconds, Default Value: 6 0 */
}GNSS_CONFIG_ST;
/*----------------------------------------------------------------------------*/
typedef enum{
	ENO_NO_EVENT,
	ENO_MODEM_EVENT
}MODEM_EVENT_ID;
typedef void (*LTE_EVENT_RECEIVE_CB_PTR)(uint8_t*);
typedef struct{
	MODEM_EVENT_ID cur_LTE_event;
	FlagStatus atcmd_inprogress;
	FlagStatus wait_for_event_inprogress;
	uint8_t LTE_event_on_receive_cb_valid;
	LTE_EVENT_RECEIVE_CB_PTR user_on_receive_cb_ptr;
	FlagStatus data_receive_from_ser;
	uint8_t ATcmd_Buf[ATCMD_SIZE];
	FlagStatus wait_for_http_event_inprogress;
	FlagStatus modem_cpin_ready_rcvd_sts;
	FlagStatus modem_atready_ready_rcvd_sts;
	FlagStatus modem_sms_done_rcvd_sts;
}LTE_CONTEXT_ST;
typedef enum{
	AUTOMATIC=2,
	GSM_ONLY=13,
	WCDMA_ONLY=14,
	LTE_ONLY=38
}MODEM_CELLURE_MODE_EM;
typedef enum{
	MIN_FUN,
	FULL_FUN,
	DIABLE_PHONE = 4,
	FACT_MODE,
	RESET_MODE,
	OFFLINE_MODE
}MODEM_FUNC_MODE_EM;
typedef enum{
	LTE_NW_NOT_REGISTERED_1, //ME is not currently searching a new operator to register
	LTE_NW_REGISTERED,
	LTE_NW_NOT_REGISTERED_2, //ME is currently searching a new operator to register
	LTE_REGISTRATION_DENIED,
	LTE_UNKNOWN,
	LTE_REGISTERED_ROAMING,
	LTE_REGISTERED_SMS_ONLY, //(applicable only when E-UTRAN)
	LTE_EMERGENCY_BEARER_SERVICES_ONLY
}XNO_LTE_NW_REG_STS;
typedef struct{
	char System_Mode[10];
	char Operation_Mode[20];
	char MCC_MNC[10];
	uint16_t TAC;
	uint8_t SCellID[20];
	uint8_t PCellID[5];
	char Frequency_Band[20];
	uint16_t E_UTRA_abs_freq_ch_num;
	uint8_t downlink_BW;
	uint8_t uplink_BW;
	int8_t RSRQ;
	int8_t RSRP;
	int8_t RSSI;
	uint8_t RSSNR;
}UE_SYS_INFO_ST;
typedef struct{
	uint8_t cid;
	uint8_t state;
}LTE_PDP_CXT_STATE_ST;
typedef struct{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	int8_t time_zone;
}LTE_TIME;
typedef enum{
	XNO_LTE_SUCCESS,
	XNO_LTE_NW_FAIL,
	XNO_LTE_NW_NOT_OPENED,
	XNO_LTE_WRONG_PARA,
	XNO_LTE_OPR_NOT_SUPPORTED,
	XNO_LTE_SOC_CRAEATION_FAILED,
	XNO_LTE_SOC_BIND_FAILED,
//	XNO_LTE_TCP_SER_LISTENING,
	XNO_LTE_BUSY,
	XNO_LTE_SOC_OPENED,
	XNO_LTE_TIMEOUT,
	XNO_LTE_DNS_PARSER_FAILED,
	XNO_LTE_UNKNOWN_ERR
}XNO_LTE_ERR_CODE;
/*----------------------------------------------------------------------------*/
typedef enum{
	NBIOT_NONE_RCVD,
	NBIOT_ECHO_RCVD,
	NBIOT_ERROR_RCVD,
	NBIOT_DATA_RCVD,
	NBIOT_OK_RCVD,
	NBIOT_RESP_RCVD,
	NBIOT_EVENT_RCVD,
	NBIOT_TIMEOUT_RCVD,
	NBIOT_DATA_REQ_RCVD,
	NBIOT_NO_CARRIER_RCVD
}NBIOT_ATCMD_RESP;

typedef struct{
	NBIOT_ATCMD_RESP resp_rcvd;
	uint8_t payload[NBIOT_RESP_PAYLOAD_SIZE];
}NBIOT_RESP_STRUCT;

typedef struct{
	uint16_t resp_start_idx;
	uint16_t num_resp;
}NBIOT_RESP_LINES_STRUCT;

typedef struct{
	uint16_t num_args;
	uint8_t resp_header_str[NBIOT_RESP_HEADER_SIZE];
	uint8_t resp_csv_param_str[NBIOT_RESP_NUM_CSV_PARAMS][NBIOT_RESP_CSV_PARAM_SIZE];
}NBIOT_RESP_PARSED_ST;
typedef struct{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	int8_t time_zone;
}NBIOT_TIME;

typedef enum{
	STANDARD,
	OTHER_MODE
}NBIOT_MODE;

typedef struct{
	uint8_t cid;
	uint8_t state;
}NBIOT_PDP_CXT_STATE_ST;

typedef enum{
	NBIOT_NW_NOT_REGISTERED_1, //Not registered, MT is not currently searching an operator to register to
	NBIOT_NW_REGISTERED, //Registered, home network
	NBIOT_NW_NOT_REGISTERED_2, //Not registered, but MT is currently trying to attach or searching an operator to register to
	NBIOT_REGISTRATION_DENIED, //Registration denied
	NBIOT_UNKNOWN, //Unknown (e.g. out of E-UTRAN coverage)
	NBIOT_REGISTERED_ROAMING, //Registered, roaming
	NBIOT_REGISTERED_SMS_ONLY_1, //Registered for ‘SMS only’, home network (not applicable)
	NBIOT_REGISTERED_SMS_ONLY_2, //Registered for ‘SMS only’, roaming (not applicable)
	NBIOT_EMERGENCY_BEARER_SERVICES_ONLY_1, //Attached for emergency bearer services only
	NBIOT_EMERGENCY_BEARER_SERVICES_ONLY_2, //9 Registered for ‘CSFB not preferred’, home network (not applicable)
	NBIOT_CSFB_NOT_PREFERRED, //Registered for ‘CSFB not preferred’, roaming (not applicable)
	NBIOT_PLMN_LOSS=80 //This code is a Sequans’s proprietary notification. Registered, temporary connection lost. A PLMN loss indication is received from RRC.
}XNO_NBIOT_NW_REG_STS;

typedef void (*NBIOT_EVENT_RECEIVE_CB_PTR)(uint8_t*);
typedef struct{
	MODEM_EVENT_ID cur_NBIOT_event;
	FlagStatus atcmd_inprogress;
	uint8_t NBIOT_event_on_receive_cb_valid;
	NBIOT_EVENT_RECEIVE_CB_PTR user_on_receive_cb_ptr;
	uint8_t ATcmd_Buf[100];
	FlagStatus data_receive_from_ser;
	FlagStatus wait_for_event_inprogress;
}NBIOT_CONTEXT_ST;


//extern WIFI_CONTEXT_ST wifi_ctx;
//extern WIFI_RESP_LINES_ST Wifi_resp_lines;
//extern BLE_CONTEXT_ST lib_ble_ctx;
#endif /* LIBRARY_LIB_MODEM_COMMON_H_ */
