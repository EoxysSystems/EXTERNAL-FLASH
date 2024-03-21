/* File			: nuv_m2354_hal_master_i2c.h
* Author		: Rajesh
* Company		: Eoxys Systems India Private Limited
* Description	: I2C driver implementation
* Created on	:
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							User library includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_public.h>
#include "NuMicro.h"
#include "cmsis_os.h"
#include "XNO_COMMON.h"


#ifndef INC_NUV_M2354_HAL_I2C_H_
#define INC_NUV_M2354_HAL_I2C_H_

typedef struct XNO_MASTER_I2C_VAR
{
	I2C_T * 			interface;
	XNO_I2C_NUM_EM		num;
	uint32_t 			timeOutCnt;
	uint32_t			busClock;
	uint8_t				slaveAddr;

	uint8_t*			txBuff;
	uint32_t			txLen;
	uint32_t			txCounter;

	uint8_t*			rxBuff;
	uint32_t			rxLen;
	uint32_t			rxCounter;

	osSemaphoreId		Lock;
	osSemaphoreDef_t	LockDef;

}XNO_MASTER_I2C_VAR_ST;

/**
  * @brief  I2C complete call back function
  * @param  I2C_var  - Pointer to specified I2C module
  * @retval None
  */
typedef void (*I2C_cplt_cb)(XNO_MASTER_I2C_VAR_ST *I2C_var);

/**
  * @brief  I2C Transmit function
  * @param  I2C_var  - Pointer to specified I2C module
  * @param  slaveAddr  - Slave address
  * @param  txbuf - Transmit buffer
  * @param  txlen - Transmit length
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*I2C_transmit_func)(XNO_MASTER_I2C_VAR_ST *I2C_var,uint8_t slaveAddr, uint8_t *txbuf, uint32_t txlen);

/**
  * @brief  I2C Receive function
  * @param  I2C_var  - Pointer to specified I2C module
  * @param  slaveAddr  - Slave address
  * @param  rxbuf - Receive buffer
  * @param  rxlen - Receive length
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*I2C_receive_func)(XNO_MASTER_I2C_VAR_ST *I2C_var,uint8_t slaveAddr, uint8_t *rxbuf,  uint32_t rxlen);

/**
  * @brief  I2C Transmit and Receive function
  * @param  I2C_var  - Pointer to specified I2C module
  * @param  slaveAddr  - Slave address
  * @param  txbuf - Transmit buffer
  * @param  txlen - Transmit length
  * @param  rxbuf - Receive buffer
  * @param  rxlen - Receive length
  * @retval HAL_StatusTypeDef - error status
  */
typedef uint32_t (*I2C_multibytes_transmitreceive_func)(XNO_MASTER_I2C_VAR_ST *i2c_var, uint8_t slaveAddr, uint8_t *txbuf, uint32_t tlen,uint8_t *rxbuf, uint32_t rlen);

typedef struct XNO_MASTER_I2C_FUNC
{
	I2C_cplt_cb		            				txCpltCb;
	I2C_cplt_cb		           					rxCpltCb;
	I2C_cplt_cb		            				errCb;
	I2C_transmit_func		    				i2cTransmit;
	I2C_receive_func		   					i2cReceive;
	I2C_multibytes_transmitreceive_func         i2cMultiBytesTransmitReceive;
//	I2C_transmit_it_func	    I2CTransmitInt;
//	I2C_receive_it_func	        I2CReceiveInt;
}XNO_MASTER_I2C_FUNC_ST;

typedef struct XNO_MASTER_I2C
{
	XNO_MASTER_I2C_VAR_ST var;
	XNO_MASTER_I2C_FUNC_ST func;
}XNO_MASTER_I2C_ST;


extern XNO_ERR_STS XNO_MASTER_I2C_Init(I2C_T *interface,
		            		XNO_MASTER_I2C_ST *i2c,
							XNO_I2C_NUM_EM i2c_num,
							uint32_t bus_clock,
							uint32_t timeOutInSecs,
							I2C_cplt_cb txCpltCb,
							I2C_cplt_cb rxCpltCb,
							I2C_cplt_cb errCb);


#endif /* INC_NUV_M2354_HAL_I2C_H_ */
