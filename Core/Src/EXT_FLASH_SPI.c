/* File: EXT_Flash_SPI.c
* Author: Ravikala
* Company: Eoxys
* Description:
* Created on:  14-02-2024
* Version History
* v1.0 Initial release
*/
/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
#include <nuv_m2354_hal_master_spi.h>
#include <string.h>
/*----------------------------------------------------------------------------
* 								User Includes
*----------------------------------------------------------------------------*/
#include "XNO_DEBUG_TASK.h"
#include "XNO_UTILS.h"
#include "cmsis_os.h"
#include "EXT_FLASH_SPI.h"
#include "SENSOR_Task.h"
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Defines
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Macros
*----------------------------------------------------------------------------*/
#define FL_NDP_FW_PARAM_STR_SIZE 	100
/*----------------------------------------------------------------------------
* 								Static global variables
*----------------------------------------------------------------------------*/
static uint8_t W25Q_TxBuf[FLASH_SPI_BUFSIZE] = {0};
static uint8_t W25Q_RxBuf[FLASH_SPI_BUFSIZE] = {0};
static uint8_t rdsr_buff[10] = {0};
static uint8_t Firm_HDR_ParamStr[6][FL_NDP_FW_PARAM_STR_SIZE];
static uint8_t LineStr[FL_NDP_FW_PARAM_STR_SIZE];
static uint8_t CmdStr[FL_NDP_FW_PARAM_STR_SIZE];

/*----------------------------------------------------------------------------
* 								Global Variables
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
* 								Static Functions
*----------------------------------------------------------------------------*/
//static uint8_t W25Q_write_reg(uint8_t addr, uint8_t *data,uint8_t tlen){
//	uint8_t ret = 0;
//	uint32_t len;
//	len = (uint32_t)tlen;
//	uint8_t W25QLocBuf[10] = {0x00};
//	W25QLocBuf[0] = addr;
//	for(uint8_t i=1;i<tlen;i++){
//		W25QLocBuf[i] = data[i-1];
//	}
//	if(W25Q_FLASH.func.spiTransmit(&W25Q_FLASH.var, W25Q_FLASH_CS, W25QLocBuf,len)!= XNO_OK){
//		ret= -1;
//	}
//	return ret;
//}
//static uint8_t W25Q_read_reg(uint8_t addr, uint8_t *Wt_data, uint8_t *rd_data, uint8_t rlen){
//	uint8_t ret = 0;
//	uint32_t len;
//	len = (uint32_t)rlen;
//	uint8_t W25QLocBuf[10] = {0x00};
//	W25QLocBuf[0] = addr;
//	for(uint8_t i=1;i<rlen;i++){
//		W25QLocBuf[i] = Wt_data[i-1];
//	}
//	if(W25Q_FLASH.func.spiTransmitReceive(&W25Q_FLASH.var,W25Q_FLASH_CS, W25QLocBuf, W25Q_RxBuf,len)!= XNO_OK){
//			ret= -1;
//		}
//
//	return ret;
//}

/* External Flash Read Status Register */
XNO_ERR_STS W25Q_Read_RDSR(uint8_t *rdsr_buf){

//	uint32_t tx_len = 0;
	uint32_t len = 0;
	XNO_ERR_STS status = XNO_OK;
	W25Q_TxBuf[0] = READ_STATUS_REG_CMD; // Read status register command
	W25Q_TxBuf[1] = 0xFF;
	len = 2;
//	if(W25Q_read_reg(0x05,W25Q_TxBuf,rdsr_buf,len) != XNO_OK) {
if(W25Q_FLASH.func.spiTransmitReceive(&W25Q_FLASH.var,W25Q_FLASH_CS, (uint8_t*)W25Q_TxBuf, (uint8_t *)rdsr_buf,len)  != XNO_OK){
		status = XNO_ERROR;
		return status;
	}
	return status;
}
/* External Flash Write Status Register */
XNO_ERR_STS W25Q_Read_WRSR(uint8_t *rdsr_buf){

//	uint32_t tx_len = 0;
	uint32_t len = 0;
	XNO_ERR_STS status = XNO_OK;
	W25Q_TxBuf[0] = WRITE_STATUS_REG_CMD; //operation commands
	W25Q_TxBuf[1] = 0xFF;
	len = 2;
//	if(W25Q_read_reg(0x05,W25Q_TxBuf,rdsr_buf,len) != XNO_OK) {
	if(W25Q_FLASH.func.spiTransmitReceive(&W25Q_FLASH.var,W25Q_FLASH_CS, (uint8_t*)W25Q_TxBuf, (uint8_t *)rdsr_buf,len)  != XNO_OK){
		status = XNO_ERROR;
		return status;
	}
	return status;
}
/* External Flash wait for Write Enable */
static void wait_for_WREN(){
	// Read RDSR reg. Wait till WREN is set
	uint8_t sts = 0;
	do{
		osDelay(10);
		W25Q_Read_RDSR((uint8_t *)rdsr_buff);
		sts = (rdsr_buff[1] & 0x02);
		if(sts == 0x02){
			break;
		}
	 }while(1);
}

/* External Flash wait for Write In Progress */
static void wait_for_WIP(){
	//Read RDSR reg. Wait till WIP becomes 0
	uint8_t sts = 0;
	do{
		osDelay(10);
		W25Q_Read_RDSR((uint8_t *)rdsr_buff);
		sts = (rdsr_buff[1] & 0x01);
		if(sts != 0x01){
			osDelay(1);
			break;
		}
	 }while(1);
}
/*----------------------------------------------------------------------------
* 								Exported Functions
*----------------------------------------------------------------------------*/
/* External Flash Write Enable */
XNO_ERR_STS W25Q_WREN(){

	XNO_ERR_STS status = XNO_OK;

	W25Q_TxBuf[0] = WRITE_ENABLE_CMD;
	W25Q_TxBuf[1] = 0xff;

	W25Q_FLASH.func.spiTransmit(&W25Q_FLASH.var,W25Q_FLASH_CS,(uint8_t*)W25Q_TxBuf,1);

	wait_for_WREN(); //Read RDSR reg. Wait till WREN is set
	return status;
}
XNO_ERR_STS W25Q_SPI_Read(uint32_t addr, uint8_t *flash_rx_data, uint32_t len){

	 XNO_ERR_STS status = XNO_OK;
	 uint16_t i=0;
	 uint16_t tx_len;
	 tx_len = len;
	 wait_for_WIP();
	 if(tx_len > 260){
		 XNO_Printf("EXT Flash LEN ERROR");
		status = XNO_ERROR;
		return status;
	 }
	 W25Q_TxBuf[0] = NORMAL_READ_CMD; //operation command
	 W25Q_TxBuf[1] = (uint8_t)(addr >> 16);
	 W25Q_TxBuf[2] = (uint8_t)(addr >> 8);
	 W25Q_TxBuf[3] = (uint8_t) addr;
	 tx_len += 4;
	 for(i=4;i<tx_len;i++)
	 {
		 W25Q_TxBuf[i]=0xFF; //Fill TX-buffer with dummy data
	 }
	 W25Q_FLASH.func.spiTransmitReceive(&W25Q_FLASH.var,W25Q_FLASH_CS,(uint8_t*)W25Q_TxBuf,(uint8_t *)W25Q_RxBuf,tx_len);
	 memcpy(flash_rx_data,&W25Q_RxBuf[4],len);
//	for(uint16_t i=4;i<len;i++){
//		flash_rx_data[i-4]=W25Q_RxBuf[i];
//	}
	 return status;
}
/* External Flash SPI Write */
XNO_ERR_STS W25Q_SPI_Write(uint32_t addr, uint8_t *wrdata, uint32_t len){

	XNO_ERR_STS status = XNO_OK;

	if(len > 260){
		XNO_Printf("EXT Flash LEN ERROR");
		status = XNO_ERROR;
		return status;
	}
	W25Q_WREN(); //Enable write operations
	W25Q_TxBuf[0] = PAGE_PROG_CMD;
	W25Q_TxBuf[1] = (uint8_t)(addr >> 16);
	W25Q_TxBuf[2] = (uint8_t)(addr >> 8);
	W25Q_TxBuf[3] = (uint8_t) addr;
//    for(uint16_t i=4;i<(len+4);i++){
//    	W25Q_TxBuf[i]=wrdata[i-4];
//    }
	memcpy(&W25Q_TxBuf[4], wrdata, len);

	if(W25Q_FLASH.func.spiTransmit(&W25Q_FLASH.var,W25Q_FLASH_CS,(uint8_t*)W25Q_TxBuf,(len+4)) != XNO_OK){
		status = XNO_ERROR;
		return status;
	}
	wait_for_WIP();

	return status;
}
/* External Flash Read Chip ID */
XNO_ERR_STS W25Q_Read_CHIPID(){

	XNO_ERR_STS ret = XNO_OK;
	uint8_t Flash_ChipID = 0;
	uint32_t rx_buf = 0;
	W25Q_TxBuf[0] = READ_ID_CMD2; //operation commands
	W25Q_TxBuf[1] = 0x00;
	W25Q_TxBuf[2] = 0x00;
	W25Q_TxBuf[3] = 0x00;
	W25Q_TxBuf[4] = 0xFF;
	W25Q_TxBuf[5] = 0xFF;
//	W25Q_read_reg(0x90,W25Q_TxBuf,W25Q_RxBuf,6);
	W25Q_FLASH.func.spiTransmitReceive(&W25Q_FLASH.var,W25Q_FLASH_CS,(uint8_t*)W25Q_TxBuf,(uint8_t *)W25Q_RxBuf,6);
	Flash_ChipID = W25Q_RxBuf[4];
	if(FLASH_SPI_MFG_ID == Flash_ChipID){
		ret = XNO_OK;
	}else{
		ret = XNO_ERROR;
	}
	return ret;
}

/* External Flash Chip Erase */
XNO_ERR_STS W25Q_Chip_Erase(){
	XNO_ERR_STS status = XNO_OK;
//	uint32_t addr = sector << 12;
	if(W25Q_WREN()==XNO_OK){
		// Read RDSR reg. Wait till WREN is set
		W25Q_TxBuf[0] = CHIP_ERASE_CMD;
		//	Flash_SPI_ChipSelect(0); //CS low
		W25Q_FLASH.func.spiTransmit(&W25Q_FLASH.var,W25Q_FLASH_CS,(uint8_t*)W25Q_TxBuf,1);
		//	Flash_SPI_ChipSelect(1); //CS high
		wait_for_WIP();//Read RDSR reg. Wait till WIP becomes 0
		osDelay(50000);
	}else{
		status = XNO_ERROR;
	}
	return status;
}


/* External Flash Sector Erase */
XNO_ERR_STS W25Q_Sector_Erase(uint32_t sec_start_addr){

	XNO_ERR_STS status = XNO_OK;
	//Each sector is 4096 bytes. Multiply with 4096
//	uint32_t addr = sector << 12;
	W25Q_WREN();// Read RDSR reg. Wait till WREN is set
	W25Q_TxBuf[0] = SECTOR_ERASE_CMD;
	W25Q_TxBuf[1] = (uint8_t)(sec_start_addr >> 16);
	W25Q_TxBuf[2] = (uint8_t)(sec_start_addr >> 8);
	W25Q_TxBuf[3] = (uint8_t) sec_start_addr;
//	Flash_SPI_ChipSelect(0); //CS low
	W25Q_FLASH.func.spiTransmit(&W25Q_FLASH.var,W25Q_FLASH_CS,(uint8_t*)W25Q_TxBuf,4);
//	if( SPI_Transmit(&hspi3,(uint8_t*)W25Q_TxBuf,4)!= XNO_OK)
//	{
//		status = XNO_ERROR;
//		Flash_SPI_ChipSelect(1); //CS high
//		return status;
//	}
//	Flash_SPI_ChipSelect(1); //CS high
	wait_for_WIP();//Read RDSR reg. Wait till WIP becomes 0

	return status;
}

XNO_ERR_STS W25Q_Erase_Sectors(uint32_t sec_start_addr, uint16_t num_sectors){
	XNO_ERR_STS ret=XNO_OK;
	uint16_t i=0;

	for(i=0;i<num_sectors;i++){
		if(W25Q_Sector_Erase(sec_start_addr) == XNO_OK){
			sec_start_addr = sec_start_addr + 4096;
		}else{
			ret = XNO_ERROR;
			XNO_Printf("EXT Flash Erase Failed");
			break;
		}
	}
	return ret;
}


XNO_ERR_STS W25Q_RDWR_test(){

	uint16_t i=0;
	XNO_ERR_STS ret=XNO_OK;
	uint8_t W25Q_Loc1Buf[] = "Hello World!";
	uint8_t W25Q_Loc2Buf[100];
	uint32_t len=0;
	char LocStr[100];
	char StrBuff[100];
	len = strlen(W25Q_Loc1Buf);
//	len = sizeof(W25Q_LocBuf);
	W25Q_Erase_Sectors(0x3,2);
//	W25Q_Sector_Erase(0x3);
	//EXT_Flash_Chip_Erase();
	osDelay(1000);

//	W25Q_SPI_Write(0x3, W25Q_TxBuf, len);
	W25Q_SPI_Write(0x3, W25Q_Loc1Buf, len);
	osDelay(1000);
	W25Q_SPI_Read(0x3, W25Q_Loc2Buf, len);
	// Compare the buffers
	if (memcmp(W25Q_Loc1Buf, W25Q_Loc2Buf, len) != 0) {
	    ret = XNO_ERROR;
	}
	if(ret == XNO_OK){
		XNO_Printf("EXT Flash RDWR Test: SUCCESS");
		strcpy(StrBuff, "The Sting Written in flash memory is: ");
		sprintf(LocStr, "%s", W25Q_Loc2Buf);
		strcat(StrBuff, LocStr);
		XNO_Printf(StrBuff);
		XNO_Printf("");

	}else{
		XNO_Printf("EXT Flash RDWR Test: FAILED");
	}
	return ret;
}

XNO_ERR_STS W25Q_test(){
	XNO_ERR_STS ret=XNO_OK;
	W25Q_Read_RDSR(rdsr_buff);
	int8_to_string_hex(&rdsr_buff[2], rdsr_buff[1]);
//	XNO_Printf("EXT FLASH SR:");
//	XNO_Printf((char*)&rdsr_buff[2]);
	if(W25Q_Read_CHIPID() != XNO_OK){
		ret = XNO_ERROR;
	}
//	if(W25Q_RDWR_test() != XNO_OK){
//		ret = XNO_ERROR;
//	}

	return ret;
}

XNO_ERR_STS W25Q_FW_Data_Write(uint32_t fw_start_addr,uint8_t *fw_buf,uint32_t fw_len){
	uint16_t i=0;
	XNO_ERR_STS ret=XNO_OK;
	// sector start address is the macro defined and incremented by fw_len
	if(W25Q_SPI_Write(fw_start_addr, fw_buf, fw_len)==XNO_OK){
		if(W25Q_SPI_Read(fw_start_addr, W25Q_RxBuf, fw_len)==XNO_OK){
			osDelay(10);
			for(i=0;i<fw_len;i++){
				if(fw_buf[i] != W25Q_RxBuf[i]){
					ret=XNO_ERROR;
				}
			}
		}else{
			ret=XNO_ERROR;
		}
	}else{
		ret=XNO_ERROR;
	}
	return ret;
}

XNO_ERR_STS W25Q_FW_Hdr_Write(uint32_t hdr_start_addr,FW_FILE_ST fw_st){

	uint8_t i=0;
	XNO_ERR_STS ret=XNO_OK;
	uint16_t len=0;

	strcpy((char *)W25Q_TxBuf,"DONE\r\n%FIRM_HDR:");
	strcat((char *)W25Q_TxBuf,(char *)fw_st.fw_header.fw_filepid);
	strcat((char *)W25Q_TxBuf,",");
	strcat((char *)W25Q_TxBuf,(char *)fw_st.fw_header.fw_fileformat);
	strcat((char *)W25Q_TxBuf,",");
	strcat((char *)W25Q_TxBuf,(char *)fw_st.fw_header.fw_filename);
	strcat((char *)W25Q_TxBuf,",");
	strcat((char *)W25Q_TxBuf,(char *)fw_st.fw_header.fw_filesize);
	strcat((char *)W25Q_TxBuf,",");
	strcat((char *)W25Q_TxBuf,(char *)fw_st.fw_header.fw_version);
	strcat((char *)W25Q_TxBuf,",");
	strcat((char *)W25Q_TxBuf,(char *)fw_st.fw_header.fw_rel_date);
	strcat((char *)W25Q_TxBuf,"\r\n");
	len = strlen((char*)W25Q_TxBuf);

	// sector start address is the macro defined and incremented by fw_len
	W25Q_SPI_Write(hdr_start_addr, W25Q_TxBuf, len);
	W25Q_SPI_Read(hdr_start_addr, W25Q_RxBuf, len);
	for(i=0;i<len;i++){
		if(W25Q_TxBuf[i] != W25Q_RxBuf[i]){
			ret = XNO_ERROR;
		}
	}
	return ret;
}

static uint8_t Parse_CSV_Param_Str(uint8_t *text,
		uint8_t (*out_param_str)[FL_NDP_FW_PARAM_STR_SIZE]) {
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t comma_cnt = 0;
	uint16_t pidx = 0;
	uint8_t out_num_param_str;

	if (text[i] == '\r') {
		return 0;
	}
	while (text[i] != '\r' && i < ATCMD_PARAM_STR_SIZE) {
		if (text[i] == ',') {
			comma_cnt++;
		}
		i++;
	}
	i = 0;
	//Extract Params based on ','
	for (pidx = 0; pidx < comma_cnt; pidx++) {
		j = 0;
		while (text[i] != ',' && i < ATCMD_PARAM_STR_SIZE) {
			if (text[i] != '\"') {
				out_param_str[pidx][j] = text[i];
				j++;
			}
			i++;
		}
		if (j < ATCMD_PARAM_STR_SIZE) {
			out_param_str[pidx][j] = 0x0;

		} else {
			XNO_Printf("ATParamStr len exceeds");
		}
		i++;
	}
	//Extract Last Param based on '\r'
	j = 0;
	while (text[i] != '\r' && i < ATCMD_PARAM_STR_SIZE) {
		if (text[i] != '\"') {
			out_param_str[pidx][j] = text[i];
			j++;
		}
		i++;
	}
	if (j < ATCMD_PARAM_STR_SIZE) {
		out_param_str[pidx][j] = 0x0;
	} else {
		XNO_Printf("ATResp len exceeds");
	}
	out_num_param_str = comma_cnt + 1;
	return out_num_param_str;
}
static void W25Q_Extract_Cmd_Params(FW_FILE_ST *fw_st,uint8_t *cmdkey,uint8_t *cmdstr, uint8_t *paramstr) {
	uint8_t num_str;
	if (strcmp(cmdkey, (const char*) cmdstr) == 0) {
		num_str = Parse_CSV_Param_Str(paramstr,
				(uint8_t (*)[]) Firm_HDR_ParamStr);
		if (num_str == 5) {
			strcpy((char*) fw_st->fw_header.fw_filepid, (char*) Firm_HDR_ParamStr[0]);
			strcpy((char*) fw_st->fw_header.fw_fileformat, (char*) Firm_HDR_ParamStr[1]);
			strcpy((char*) fw_st->fw_header.fw_filename, (char*) Firm_HDR_ParamStr[2]);
			strcpy((char*) fw_st->fw_header.fw_filesize, (char*) Firm_HDR_ParamStr[3]);
			strcpy((char*) fw_st->fw_header.fw_version, (char*) Firm_HDR_ParamStr[4]);
			strcpy((char*) fw_st->fw_header.fw_rel_date, (char*) Firm_HDR_ParamStr[5]);
		} else {
			XNO_Printf(
					"ERROR WHILE UPDATING SWVER STRUCT OF FIRMWARE　NAME");
		}
	} else {
		XNO_Printf("NO MATCH WHILE UPDATING HEADER CONFIG STRUCT");
	}
}

XNO_ERR_STS W25Q_Parse_Firm_Header(FW_FILE_ST *fw_st,uint8_t *str_buf) {
	XNO_ERR_STS ret=XNO_OK;
	uint16_t param, nxt_idx;
	int len = 0;
	nxt_idx = 0;

	for (param = 0; param < 2; param++) {
		len = Read_Line_from_StringBuf(&str_buf[nxt_idx], LineStr);
		if (len < 0) {
			XNO_Printf(
					"UNABLE TO READ LINE FROM FLASH FW STS CONFIG STRING");
			ret = XNO_ERROR;
			break;
		} else {
			nxt_idx = nxt_idx + len;
		}
		if (param > 0) {
			len = Read_Cmd_from_StringBuf(LineStr, CmdStr);

			if (len > 0) {
				W25Q_Extract_Cmd_Params(fw_st,"%FIRM_HDR:",CmdStr, &LineStr[len]);
			}
		}
	}
	return ret;
}

FlagStatus W25Q_Read_Firm_Header(FW_FILE_ST *fw_st, uint32_t fw_hdr_start_addr) {
	FlagStatus ret;
	XNO_ERR_STS sts;
	sts = W25Q_SPI_Read(fw_hdr_start_addr, W25Q_RxBuf, 1000);
//	Read_Line_from_StringBuf(Fl_ndp_fw_loc_buf, LineStr);
	if ((sts==XNO_OK) && (W25Q_RxBuf[0] == 'D') && (W25Q_RxBuf[1] == 'O')
			&& (W25Q_RxBuf[2] == 'N') && (W25Q_RxBuf[3] == 'E')){
		if(W25Q_Parse_Firm_Header(fw_st,W25Q_RxBuf)==XNO_OK){
			ret = SET;
		}else{
			ret = RESET;
		}
	} else {
		ret = RESET;
	}
	return ret;
}
FlagStatus W25Q_Is_Firm_Avail(uint32_t fw_hdr_start_addr) {
	FlagStatus ret;
	W25Q_SPI_Read(fw_hdr_start_addr, W25Q_RxBuf, 1000);
	//	Read_Line_from_StringBuf(Fl_ndp_fw_loc_buf, LineStr);
	if ((ret==XNO_OK) && (W25Q_RxBuf[0] == 'D') && (W25Q_RxBuf[1] == 'O')
			&& (W25Q_RxBuf[2] == 'N') && (W25Q_RxBuf[3] == 'E')){
		ret = SET;
	}else{
		ret = RESET;
	}
	return ret;
}
