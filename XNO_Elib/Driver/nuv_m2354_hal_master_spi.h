/* File			: nuv_m2354_hal_master_spi.h
* Author		: Rajesh
* Company		: Eoxys Systems India Private Limited
* Description	: SPI driver implementation
* Created on	: 24-Oct-2023
* Version 		: V11
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_public.h>
#include "NuMicro.h"
#include "cmsis_os.h"
/*----------------------------------------------------------------------------
* 							Types Declarations
*----------------------------------------------------------------------------*/
#define SPI_MAX_CS  8
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
#ifndef DRIVERS_INC_NUV_M2354_HAL_SPI_H_
#define DRIVERS_INC_NUV_M2354_HAL_SPI_H_
#ifdef __cplusplus
extern "C" {
#endif

typedef struct XNO_MASTER_SPI_VAR
{
	SPI_T 			    	*interface;
	XNO_SPI_NUM_EM			spiNum;
	XNO_SPI_MODE_SELECT_EM  spiMode; //MODE_0, MODE_1, MODE_2, MODE_3
	XNO_SPI_DATA_WIDTH_EM 	dataWidth;
	uint32_t            	busClock;

	XNO_GPIO_ST       *chipSelect[SPI_MAX_CS];
	uint32_t            addrOfCs[SPI_MAX_CS];
	uint32_t			 chipSelectNum;
	uint8_t				*txBuff;
	uint32_t			txLen;
	uint32_t			txCounter;

	uint8_t				*rxBuff;
	uint32_t			rxLen;
	uint32_t			rxCounter;

	osSemaphoreId		txrxLock;
	osSemaphoreDef_t	txrxLockDef;
}XNO_MASTER_SPI_VAR_ST;

/**
  * @brief  SPI complete call back function
  * @param  spi_var  - Pointer to specified SPI module
  * @retval None
  */
typedef void (*spi_cplt_cb)(XNO_MASTER_SPI_VAR_ST *spi_var);

/**
  * @brief  SPI Transmit function
  * @param  spi_var  - Pointer to specified SPI module
  * @param  cs_num  - Chip select number
  * @param  tx_data - Transmit data
  * @param 	DataCount - Data count
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*spi_transmit_func)(XNO_MASTER_SPI_VAR_ST *spi_var, uint32_t cs_num, uint8_t* tx_data, uint32_t DataCount);

/**
  * @brief  SPI Receive function
  * @param  spi_var  - Pointer to specified SPI module
  * @param  cs_num  - Chip select number
  * @param  tx_buf - Pointer to transmit buffer
  * @param 	rx_buf - Pointer to receive buffer
  * @param 	len - length of the data
  * @retval HAL_StatusTypeDef - error status
  */
typedef HAL_StatusTypeDef (*spi_transmit_receive_func)(XNO_MASTER_SPI_VAR_ST *spi_var, uint32_t cs_num,uint8_t *tx_buf, uint8_t *rx_buf, uint32_t len);

typedef struct XNO_SPI_FUNC
{
	spi_cplt_cb		        	errCb;
	spi_transmit_func			spiTransmit;
	spi_transmit_receive_func	spiTransmitReceive;
//	spi_transmit_receive_func	spiTransmitReceive;
}XNO_MASTER_SPI_FUNC_ST;

typedef struct XNO_SPI
{
	XNO_MASTER_SPI_VAR_ST var;
	XNO_MASTER_SPI_FUNC_ST func;
}XNO_MASTER_SPI_ST;

extern XNO_ERR_STS XNO_SPI_Init(SPI_T *interface,
                               XNO_SPI_NUM_EM spiNum,
			                   XNO_MASTER_SPI_ST *spi,
			                   XNO_SPI_MODE_SELECT_EM spiMode,
			                   XNO_SPI_DATA_WIDTH_EM dataWidth,
			                   uint32_t busClock,
							   XNO_GPIO_ST *chipSelect,
			                   uint32_t chipSelectNum,
			                   spi_cplt_cb errCb);

#ifdef __cplusplus
}
#endif
#endif /* DRIVERS_INC_NUV_M2354_HAL_SPI_H_ */
