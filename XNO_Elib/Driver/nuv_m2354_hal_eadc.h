/* File			: nuv_m2354_hal_eadc.h
* Author		: Ravi Kala
* Company		: Eoxys Systems India Private Limited
* Description	: EADC driver implementation
* Created on	:
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_public.h>
#include "NuMicro.h"
#include "cmsis_os.h"
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Local definitions
*----------------------------------------------------------------------------*/

#define EADC_MAX_NUM_MODULES				19
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
#ifndef __NUV_M2354_HAL_HANDLE_EADC_H_
#define __NUV_M2354_HAL_HANDLE_EADC_H_

#ifdef __cplusplus
extern "C" {
#endif
/**
  * @\struct  typedef struct XNO_EADC_VAR XNO_EADC_VAR_ST;
  * @brief  Structure for EADC
  * @param  interface  - EADC peripheral interface pointer
  * @param  adc_num  - EADC module number
  * @param  adcClockDivider -  EADC Clock Driver
  * @retval timeOutCnt - SDC timeOutCnt = SystemCoreClock*timeOutInSecs;
  * @retval resolution - Resolution setting (e.g., 12-bit, 16-bit)
  * @retval referenceVolt - Voltage reference selection (e.g., internal, external)
  * @retval enabledAdcModules[EADC_MAX_NUM_MODULES] -
  * @retval adcData[EADC_MAX_NUM_MODULES] -
  * @retval triggerSrc[EADC_MAX_NUM_MODULES] - Trigger source for EADC conversion (e.g., software, external)
  * @retval adcChs[EADC_MAX_NUM_MODULES] -
  * @retval adcLock -  Semaphore for result synchronization
  * @retval adcLockDef - Semaphore definition for result synchronization
*/
typedef struct XNO_EADC_VAR
{
    EADC_T*             			interface;
    XNO_EADC_NUM_EM     			adc_num;
    uint32_t 						adcClockDivider;
    uint32_t 						timeOutCnt;

    uint32_t            			resolution;
    uint32_t            			referenceVolt;

    uint8_t             			enabledAdcModules[EADC_MAX_NUM_MODULES];
    uint32_t						adcData[EADC_MAX_NUM_MODULES];
    XNO_EADC_TRIGGER_SOURSE_EM      triggerSrc[EADC_MAX_NUM_MODULES];
    uint32_t      					adcChs[EADC_MAX_NUM_MODULES];
    //    uint32_t            			triggerMode;   			 // Trigger mode (e.g., single-shot, continuous)

    osSemaphoreId       			adcLock;
    osSemaphoreDef_t    			adcLockDef;
}XNO_EADC_VAR_ST;

/**
  * @brief  EADC error call back function
  * @param  eadc_var  - Pointer to specified DAC module
  * @retval None
*/
typedef void (*eadc_err_cb)(XNO_EADC_VAR_ST *eadc_var);

/**
  * @brief  EADC complete call back function
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  adc_data  - EADC data for conversion
  * @retval None
  */
typedef void (*eadc_cplt_cb)(XNO_EADC_VAR_ST *eadc_var,uint32_t adc_data);

/**
  * @brief  Configure the sample control logic module.
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @param  trigger_src  - Decides the trigger source
  * @param	eadc_ch_num - Specifies the sample module channel
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef(*eadc_Config_SampleModule_func)(XNO_EADC_VAR_ST *eadc_var, XNO_EADC_NUM_EM eadc_mod_num, XNO_EADC_TRIGGER_SOURSE_EM trigger_src, uint32_t eadc_ch_num);

/**
  * @brief  Start the A/D conversion
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef(*eadc_StartConv_func)(XNO_EADC_VAR_ST *eadc_var, XNO_EADC_NUM_EM eadc_mod_num);

/**
  * @brief  Checks if there's any data available
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @retval HAL_StatusTypeDef - error status
*/
typedef HAL_StatusTypeDef(*eadc_IsDataAvail_func)(XNO_EADC_VAR_ST *eadc_var, XNO_EADC_NUM_EM eadc_mod_num);

/**
  * @brief  EADC read data function
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @param  adc_data_buf -  Pointer to the ADC data buffer
  * @retval HAL_StatusTypeDef - error status
*/
typedef HAL_StatusTypeDef(*eadc_ReadData_func)(XNO_EADC_VAR_ST *eadc_var, XNO_EADC_NUM_EM eadc_mod_num,uint32_t *adc_data_buf);

/**
  * @brief  EADC read data interrupt function
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @param  adc_data_buf -  Pointer to the ADC data buffer
  * @retval HAL_StatusTypeDef - error status
*/
typedef HAL_StatusTypeDef(*eadc_ReadData_Int_func)(XNO_EADC_VAR_ST *eadc_var, XNO_EADC_NUM_EM eadc_mod_num,uint32_t *adc_data_buf);
/**
  * @brief  EADC read data interrupt function
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @param  adc_data_buf -  Pointer to the ADC data buffer
  * @retval HAL_StatusTypeDef - error status
*/
typedef struct XNO_EADC_FUNC
{
	eadc_err_cb		        		errCb;
	eadc_cplt_cb					adcCpltCb;
	eadc_Config_SampleModule_func   eadcConfigSampleModule;
	eadc_StartConv_func             eadcStartConv;
	eadc_IsDataAvail_func           eadcIsDataAvail;
	eadc_ReadData_func              eadcReadData;
	eadc_ReadData_Int_func          eadcReadDataInt;
}XNO_EADC_FUNC_ST;
/**
  * @brief  EADC read data interrupt function
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @param  adc_data_buf -  Pointer to the ADC data buffer
  * @retval HAL_StatusTypeDef - error status
*/
typedef struct XNO_EADC
{
	XNO_EADC_VAR_ST var;
	XNO_EADC_FUNC_ST func;
}XNO_EADC_ST;
/**
  * @brief  EADC read data interrupt function
  * @param  eadc_var  - Pointer to specified DAC module
  * @param  eadc_mod_num  - Decides the sample module number
  * @param  adc_data_buf -  Pointer to the ADC data buffer
  * @retval HAL_StatusTypeDef - error status
*/
extern XNO_ERR_STS XNO_EADC_Init(EADC_T *interface,
								XNO_EADC_NUM_EM eadc_num,
								XNO_EADC_ST *eadc,
								XNO_EADC_MODE_EM adc_mode,
								uint32_t adcClockDivider,
								eadc_cplt_cb errCb);

#ifdef __cplusplus
}
#endif
#endif
