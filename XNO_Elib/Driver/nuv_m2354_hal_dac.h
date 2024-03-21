/* File			: nuv_m2354_hal_dac.h
* Author		: Rajesh,Sujit
* Company		: Eoxys Systems India Private Limited
* Description	: DAC driver implementation
* Created on	: 14-Nov-2023
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_public.h>
#include "NuMicro.h"
#include "cmsis_os.h"
#include "XNO_COMMON.h"
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
#define DAC_MAX_NUM_MODULES				2
/*----------------------------------------------------------------------------
* 							Local definitions
*----------------------------------------------------------------------------*/
#ifndef DRIVERS_INC_NUV_M2354_HAL_DAC_H_
#define DRIVERS_INC_NUV_M2354_HAL_DAC_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef struct XNO_DAC_VAR
{
	DAC_T* 				        interface;
	XNO_DAC_NUM_EM 				num;
	uint32_t             		u32ch; //Not used in M2354 DAC series
	XNO_DAC_TRIGGER_SOURSE_EM	trgSrc; // Trigger source
	uint32_t 					timeOutCnt;
	uint8_t             		enabledDacModules[DAC_MAX_NUM_MODULES];

	uint32_t					*rdData;
	uint32_t					rdlen;

	uint32_t					*wrData;
	uint32_t					wrlen;

	osSemaphoreId				dacLock;
	osSemaphoreDef_t			dacLockDef;

}XNO_DAC_VAR_ST;

/**
  * @brief  DAC complete call back function
  * @param  dac_var  - Pointer to specified DAC module
  * @retval None
  */
typedef void (*dacCplt_cb)(XNO_DAC_VAR_ST *dac_var);

/**
  * @brief  DAC Write Function
  * @param  dac_var  - Pointer to specified DAC module
  * @param  u32ch  - Not used in M2354 DAC series
  * @param  wrdata - Write data for conversion
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*dacWrite_func)(XNO_DAC_VAR_ST *dac_var, uint32_t u32ch, uint32_t wrdata);

/**
  * @brief  DAC Read Function
  * @param  dac_var  - Pointer to specified DAC module
  * @param  u32ch  - Not used in M2354 DAC series
  * @param  rddata - Read data after conversion
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*dacRead_func)(XNO_DAC_VAR_ST *dac_var,uint32_t channel, uint32_t *rddata);

/**
  * @brief  DAC Interrupt Write Function
  * @param  dac_var  - Pointer to specified DAC module
  * @param  u32ch  - Not used in M2354 DAC series
  * @param  wrdata - Write data for conversion
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*dacWriteInt_func)(XNO_DAC_VAR_ST *dac_var, uint32_t u32ch, uint32_t wrdata);

/**
  * @brief  DAC Interrupt Read Function
  * @param  dac_var  - Pointer to specified DAC module
  * @param  u32ch  - Not used in M2354 DAC series
  * @param  rddata - Read data after conversion
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*dacReadInt_func)(XNO_DAC_VAR_ST *dac_var, uint32_t u32ch, uint32_t rddata);

/**
  * @brief  Start the DAC conversion.
  * @param  dac_var  - Pointer to specified DAC module
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*dacStartConv_func)(XNO_DAC_VAR_ST *dac_var);


/**
  * @brief  DAC Set Delay Time Function
  * @param  dac_var  - Pointer to specified DAC module
  * @param  u32Delay - Decides the DAC conversion settling time
  * @retval HAL_StatusTypeDef - error status
  */

typedef HAL_StatusTypeDef (*dacSetDelayTime_func)(XNO_DAC_VAR_ST *dac_var, uint32_t u32Delay);

/**
  * @brief  DAC Close Function
  * @param  dac_var  - Pointer to specified DAC module
  * @param  u32ch  - Not used in M2354 DAC series
  * @retval None
*/
typedef void (*dacClose_func)(XNO_DAC_VAR_ST *dac_var, uint32_t u32ch);

typedef struct XNO_DAC_FUNC
{
	dacCplt_cb       			errCb;
	dacWrite_func			dacWrite;
	dacRead_func 			dacRead;
	dacWriteInt_func		dacWriteInt;
	dacReadInt_func 		dacReadInt;
	dacSetDelayTime_func 	dacSetDelayTime;
	dacStartConv_func		dacStartConv;
	dacClose_func           dacClose;
}XNO_DAC_FUNC_ST;

 typedef struct XNO_DAC
{
	XNO_DAC_VAR_ST var;
	XNO_DAC_FUNC_ST func;
}XNO_DAC_ST;

extern XNO_ERR_STS XNO_DAC_Init(DAC_T *interface,
        	XNO_DAC_NUM_EM dac_num,
			XNO_DAC_ST *dac,
			uint32_t channel,
			XNO_DAC_TRIGGER_SOURSE_EM trgSrc,
			uint32_t timeOutInSecs,
			dacCplt_cb errCb);

#ifdef __cplusplus
}
#endif
#endif /* DRIVERS_INC_NUV_M2354_HAL_DAC_H_ */
