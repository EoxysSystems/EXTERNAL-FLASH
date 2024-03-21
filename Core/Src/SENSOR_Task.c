/* File      : sensor_Task.c
* Author     : Rajesh
* Company    : Eoxys Systems India Pvt Ltd
* Description:
* Created on : 29-Dec-2023
* Version    : 13
*/
/*----------------------------------------------------------------------------
							Standard library includes
*----------------------------------------------------------------------------*/

#include <nuv_m2354_hal_eint.h>
#include <nuv_m2354_hal_public.h>
#include "SENSOR_Task.h"
#include "XNO_UTILS.h"
#include "XNO_OS_TIMERS.h"
#include "EXT_FLASH_SPI.h"
/*----------------------------------------------------------------------------
* 								Typedef
*----------------------------------------------------------------------------*/
typedef enum{
	SEN_SM_IDLE,
	SEN_SM_W25Q,
	SEN_DATA_SEND
}SEN_SM_EM;
/*----------------------------------------------------------------------------
* 								local variable
*----------------------------------------------------------------------------*/
static SEN_SM_EM cur_sen_sm_state = SEN_SM_IDLE;
static SEN_SM_EM nex_sen_sm_state = SEN_SM_IDLE;
static XNO_OSTIMER_ST SEN_timer_st;
static uint8_t ipcmesg_sen_obj[250];
static uint8_t wait_for_resp = 0;
uint8_t dev_mac_id[16];
/*----------------------------------------------------------------------------
* 								Global variable
*----------------------------------------------------------------------------*/
XNO_MASTER_SPI_ST W25Q_FLASH;
XNO_GPIO_ST W25Q_CS;
XNO_IPCMESG_ST ipcmesg_sen;
/*----------------------------------------------------------------------------
* 								CB function
*----------------------------------------------------------------------------*/
void SPI_ErrorCallback(XNO_MASTER_SPI_VAR_ST *spi_var)
{

}
void SEN_timer_cb(){
	SEN_timer_st.var.timeout_flag = SET;
}
/*----------------------------------------------------------------------------
* 								global function
*----------------------------------------------------------------------------*/
void sen_SPI_init(void){
	XNO_ERR_STS ret = XNO_NONE;
	W25Q_CS.gpioPort = XNO_GPIO_PORT_C;
	W25Q_CS.gpioPin = XNO_GPIO_PIN_0;

	ret = XNO_SPI_Init(SPI1, _SPI1, &W25Q_FLASH, XNO_SPI_MODE_0, XNO_SPI_DATABIT_8, XNO_SPI_BUS_CLOCK_16M, &W25Q_CS, 1, SPI_ErrorCallback);
	if(ret == XNO_OK){
		XNO_Printf("SPI_1 Init Success");
	}else{
		XNO_Printf("SPI_1 Init Failed");
	}
	XNO_OSTIMER_Init(&SEN_timer_st, TIMER_PERIODIC,SEN_timer_cb);
	SEN_timer_st.func.startTimer(&SEN_timer_st.var,TIMEOUT_1_SECS);
}

void sensor_SM(){
	switch(cur_sen_sm_state){
		case SEN_SM_IDLE:
			if(SEN_timer_st.var.timeout_flag == SET){
				SEN_timer_st.var.timeout_flag = RESET;
				nex_sen_sm_state = SEN_SM_W25Q;
			}
			break;
		case SEN_SM_W25Q:
			W25Q_RDWR_test();
			nex_sen_sm_state = SEN_SM_IDLE;
			break;
		case SEN_DATA_SEND:
			ipcmesg_sen.func.sendReqWithDataObj(&ipcmesg_sen.var,(uint32_t)IPC_REQ_HTTP_DATA_SEND,ipcmesg_sen_obj);
			wait_for_resp = 100;
			while(wait_for_resp){
				if(ipcmesg_sen.func.isRespAvailable(&ipcmesg_sen.var)){
					wait_for_resp=0;
					if(ipcmesg_sen.var.cmd_resp == IPC_RESP_DONE){
						XNO_Printf("SEND OK");
						ipcmesg_sen.func.reset(&ipcmesg_sen.var);
						nex_sen_sm_state = SEN_SM_IDLE;
						break;
					}else{
						XNO_Printf("SEND ERROR");
						ipcmesg_sen.func.reset(&ipcmesg_sen.var);
						nex_sen_sm_state = SEN_SM_IDLE;
						break;
					}
				}
				if(wait_for_resp == 1){
					wait_for_resp=0;
					XNO_Printf("IPC RESP RECIVE TIMEOUT");
					nex_sen_sm_state = SEN_SM_IDLE;
					break;
				}
				wait_for_resp--;
				osDelay(10);
			}
			break;
		default:
			XNO_Printf("ERROR: INVALID SENSOR SM STATE");
			break;
	}
 cur_sen_sm_state = nex_sen_sm_state;
}
void sensor_init(){
	XNO_ERR_STS ret = XNO_NONE;
	uint8_t sts = 0;
	if(W25Q_test() == 1){
		XNO_Printf("W25Q EXTERNAL FLASH Detection Successful");
	}else{
		XNO_Printf("ERROR: W25Q EXTERNAL FLASH Detection Failed");
	}
}

void SENSOR_Task(void const * argument){
	sen_SPI_init();
	sensor_init();
	for(;;){
		sensor_SM();
		osDelay(10);
	}
}
