/* File      : main.c
* Author     : Rajesh
* Company    : Eoxys
* Description:
* Created on : 28-Nov-2023
* Version    : 13
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <stdio.h>
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include "NuMicro.h"
#include "cmsis_os.h"

#include <nuv_m2354_hal_msp.h>
#include <nuv_m2354_hal_uart.h>

#include "ATcmds.h"
#include "main.h"
#include "XNO_OS_TIMERS.h"
#include "PIN_CONFIG.h"

#include "XNO_DEBUG_TASK.h"
#include "XNO_LTE_TASK.h"

#include "SENSOR_Task.h"
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
S_RTC_TIME_DATA_T sWriteRTC;
osThreadId TaskHandle1, TaskHandle2, TaskHandle3, TaskHandle4,TaskHandle5,TaskHandle6;
/*----------------------------------------------------------------------------
* 							Local definitions
*----------------------------------------------------------------------------*/
XNO_UART_TASK_PARAM_ST dbg_uart_task_param_st;
XNO_UART_TASK_PARAM_ST modem_uart_task_param_st;
XNO_PARSER_TASK_PARAM_ST dbg_parser_task_param_st;
XNO_OSTIMER_ST NDP_timer_st,wifi_timer_st;
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
#define fl_firm_sts_data = 0x77000
uint8_t __attribute__((section(".flash_d")))  flash_firmsts_data[2048]= "DONE\r\n%FIRM_NAME:DemoApplication\r\n%SWVER:V10\r\n%DATE:22:04:25\r\n";

void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
}
void Reset_NDP120(void){
	PB9 = 0;
	osDelay(1000);
	PB9 = 1;
	osDelay(100);
}
int main()
{
	/* Unlock protected registers */
	SYS_UnlockReg();
	/* Init System, peripheral clock and multi-function I/O */
	XNO_Extclk_sys_init();
//	wifi_flash_init();
	/* Lock protected registers */
	SYS_LockReg();

	Board_GPIO_init();

	osThreadDef(Task_1, DBG_UART_Task, osPriorityNormal, 0, 512);
	dbg_uart_task_param_st.interface = UART0;
	dbg_uart_task_param_st.uart_num  = _UART0;
	dbg_uart_task_param_st.baudRate  = XNO_UART_BAUDRATE_115200;
	dbg_uart_task_param_st.gpio_modem_cts.gpioPort  = XNO_GPIO_PORT_NONE;
	dbg_uart_task_param_st.gpio_modem_cts.gpioPin  = XNO_GPIO_PIN_NONE;
	dbg_uart_task_param_st.gpio_modem_pwr.gpioPort  = XNO_GPIO_PORT_NONE;
	dbg_uart_task_param_st.gpio_modem_pwr.gpioPin  = XNO_GPIO_PIN_NONE;
	dbg_uart_task_param_st.gpio_modem_pwrkey.gpioPort  = XNO_GPIO_PORT_NONE;
	dbg_uart_task_param_st.gpio_modem_pwrkey.gpioPin  = XNO_GPIO_PIN_NONE;
	dbg_uart_task_param_st.gpio_modem_reset.gpioPort  = XNO_GPIO_PORT_NONE;
	dbg_uart_task_param_st.gpio_modem_reset.gpioPin  = XNO_GPIO_PIN_NONE;
	TaskHandle1 = osThreadCreate(osThread(Task_1),&dbg_uart_task_param_st);

	osThreadDef(Task_2, DBG_Parser_Task, osPriorityNormal, 0, 512);
	dbg_parser_task_param_st.atcmd_ext_list = atcmd_extlist;
	TaskHandle2 = osThreadCreate(osThread(Task_2), &dbg_parser_task_param_st);

	osThreadDef(Task_3, LTE_UART_Task, osPriorityNormal, 0, 1024);
	modem_uart_task_param_st.interface = UART1;
	modem_uart_task_param_st.uart_num  = _UART1;
	modem_uart_task_param_st.baudRate  = XNO_UART_BAUDRATE_115200;
//	modem_uart_task_param_st.baudRate  = XNO_UART_BAUDRATE_230400;
	modem_uart_task_param_st.gpio_modem_cts.gpioPort  = XNO_GPIO_PORT_A;
	modem_uart_task_param_st.gpio_modem_cts.gpioPin  = XNO_GPIO_PIN_1;
	modem_uart_task_param_st.gpio_modem_pwr.gpioPort  = XNO_GPIO_PORT_B;
	modem_uart_task_param_st.gpio_modem_pwr.gpioPin  = XNO_GPIO_PIN_7;//7
	modem_uart_task_param_st.gpio_modem_pwrkey.gpioPort  = XNO_GPIO_PORT_B;
	modem_uart_task_param_st.gpio_modem_pwrkey.gpioPin  = XNO_GPIO_PIN_5;
	modem_uart_task_param_st.gpio_modem_reset.gpioPort  = XNO_GPIO_PORT_B;
	modem_uart_task_param_st.gpio_modem_reset.gpioPin  = XNO_GPIO_PIN_8;
	modem_uart_task_param_st.gpio_modem_sts.gpioPort  = XNO_GPIO_PORT_B;
	modem_uart_task_param_st.gpio_modem_sts.gpioPin  = XNO_GPIO_PIN_1;
	TaskHandle3 = osThreadCreate(osThread(Task_3),&modem_uart_task_param_st);

	osThreadDef(Task_5,SENSOR_Task, osPriorityNormal, 0, 512);
	TaskHandle5 = osThreadCreate(osThread(Task_5), NULL);

	osKernelStart();
	while(1);
}

