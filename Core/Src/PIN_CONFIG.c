
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
#include "NuMicro.h"
#include "main.h"
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/

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
/*----------------------------------------------------------------------------*/
void LTE_Modem_GPIO_init() {
	/* Modem PWR EN */
	GPIO_SetMode(PB, BIT7, GPIO_MODE_OUTPUT);
	/* Modem RST */
	GPIO_SetMode(PB, BIT8, GPIO_MODE_OUTPUT);
	/* Modem PWR KEY */
	GPIO_SetMode(PB, BIT5, GPIO_MODE_OUTPUT);
	/* LTE Status */
	GPIO_SetMode(PB, BIT1, GPIO_MODE_INPUT);
	/* LTE CTS */
	GPIO_SetMode(PA, BIT1, GPIO_MODE_INPUT);
//	/* LTE RTS */
//	GPIO_SetMode(PB, BIT8, GPIO_MODE_INPUT);
    PB8 = 0;
    PB7 = 1;
    PB7 = 0;
	/* Modem UART3 TXD and RXD Pins*/
    SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA3MFP_Msk | SYS_GPA_MFPL_PA2MFP_Msk);
    SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA3MFP_UART1_TXD | SYS_GPA_MFPL_PA2MFP_UART1_RXD);
}
void LTE_EVK_GPIO_Init(){
	    /* Set multi-function pins for SPI1 */
	    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC3MFP_Msk | SYS_GPC_MFPL_PC2MFP_Msk | SYS_GPC_MFPL_PC1MFP_Msk);
	    SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC3MFP_SPI1_MISO | SYS_GPC_MFPL_PC2MFP_SPI1_MOSI | SYS_GPC_MFPL_PC1MFP_SPI1_CLK);

        /* Set multi-function pins for NSS0 */
	    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC0MFP_Msk);
	    SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC0MFP_GPIO);
	    GPIO_SetMode(PC, BIT0, GPIO_MODE_OUTPUT);

	    /* Set multi-function pins for GNSS MODEM CS */
//	    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA13MFP_Msk);
//	    SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA13MFP_GPIO);
//	    GPIO_SetMode(PA, BIT13, GPIO_MODE_OUTPUT);

}

/************************************************************************************************************************************/
void NDP_GPIO_init()
{
	/* NDP FLASH CS */
	GPIO_SetMode(PB, BIT3, GPIO_MODE_OUTPUT);

	/* NDP RST */
	GPIO_SetMode(PB, BIT9, GPIO_MODE_OUTPUT);

	/* NDP INT */
	SYS->GPA_MFPL &= ~(SYS_GPA_MFPL_PA6MFP_Msk);
	SYS->GPA_MFPL |= (SYS_GPA_MFPL_PA6MFP_INT0);
	GPIO_SetMode(PA, BIT6, GPIO_MODE_INPUT);
//	NVIC_EnableIRQ(EINT0_IRQn);

	/* NDP CS */
	GPIO_SetMode(PB, BIT2, GPIO_MODE_OUTPUT);

	/* NDP SPI */
    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA10MFP_Msk | SYS_GPA_MFPH_PA9MFP_Msk | SYS_GPA_MFPH_PA8MFP_Msk);
    SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA10MFP_SPI2_CLK | SYS_GPA_MFPH_PA9MFP_SPI2_MISO | SYS_GPA_MFPH_PA8MFP_SPI2_MOSI);

	PB2 = 1;
	PB3 = 1;
	PB9 = 1;
}
/********************************************************************************************************************************/
void DBG_GPIO_init() {
	/* Debug UART0 TXD and RXD Pins*/
	SYS->GPD_MFPL &= ~(SYS_GPD_MFPL_PD3MFP_Msk | SYS_GPD_MFPL_PD2MFP_Msk);
	SYS->GPD_MFPL |= (SYS_GPD_MFPL_PD3MFP_UART0_TXD	| SYS_GPD_MFPL_PD2MFP_UART0_RXD);
}
/********************************************************************************************************/
void GNSS_GPIO_init() {
	/* GNSS UART4 TXD and RXD Pins*/
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB11MFP_Msk | SYS_GPB_MFPH_PB10MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB11MFP_UART4_TXD | SYS_GPB_MFPH_PB10MFP_UART4_RXD);

    /* Set multi-function pins for SPI1 */
//	    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC3MFP_Msk | SYS_GPC_MFPL_PC2MFP_Msk | SYS_GPC_MFPL_PC1MFP_Msk);
//	    SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC3MFP_SPI1_MISO | SYS_GPC_MFPL_PC2MFP_SPI1_MOSI | SYS_GPC_MFPL_PC1MFP_SPI1_CLK);
    /* Set multi-function pins for GNSS MODEM CS */
	    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA13MFP_Msk);
	    SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA13MFP_GPIO);
	    GPIO_SetMode(PA, BIT13, GPIO_MODE_OUTPUT);

}
/********************************************************************************************************/
void Board_GPIO_init(){
	//Debug Interface Pin mapping
	DBG_GPIO_init();
	//Modem Interface Pin mapping
	LTE_Modem_GPIO_init();
	LTE_EVK_GPIO_Init();
}
