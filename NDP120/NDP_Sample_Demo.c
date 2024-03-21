/* File: NDP_Sample_Demo.c
* Author: Alsufiyan
* Company: Eoxys
* Description: example code for flashing NDP and header update for appropriate firmware region
* Created on: 01-Aug-2022
* Version History
* v1.0 Initial release
*/
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <string.h>
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
#include "main.h"
#include "NDP_Sample_Demo.h"
#include "XNO_UTILS.h"
#include "XNO_DEBUG_TASK.h"
/*----------------------------------------------------------------------------
* 							Local definitions
*----------------------------------------------------------------------------*/
#define FL_NDP_FW_LOC_BUF_SIZE 		1200
#define FL_NDP_FW_PARAM_STR_SIZE 	100
/*----------------------------------------------------------------------------
* 							Local variables
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/
typedef enum {
	NDP_MCU,
	NDP_DSP,
	NDP_DNN
}NDP_FW_AVAIL;
/*----------------------------------------------------------------------------
							Global variables
*----------------------------------------------------------------------------*/
NDP_FW_AVAIL fw_status;
FlagStatus ndp_firm_available = RESET;
uint8_t Fl_ndp_fw_loc_buf[FL_NDP_FW_LOC_BUF_SIZE];
uint8_t Firm_HDR_ParamStr[5][FL_NDP_FW_PARAM_STR_SIZE];
uint8_t LineStr[FL_NDP_FW_PARAM_STR_SIZE];
uint8_t CmdStr[FL_NDP_FW_PARAM_STR_SIZE];
NDP_FIRM_HEADER ndp_firm_hdr;
/*----------------------------------------------------------------------------
* 							Local functions
*----------------------------------------------------------------------------*/
static void Set_NDP_FirmHDR(uint8_t *file_type,uint8_t *file_name, uint8_t *file_size, uint8_t *file_version,uint8_t *rel_date){
	strcpy((char*)ndp_firm_hdr.ndp_firm_filetype,(char*)file_type);
	strcpy((char*)ndp_firm_hdr.ndp_firm_filename,(char*)file_name);
	strcpy((char*)ndp_firm_hdr.ndp_firm_filesize,(char*)file_size);
	strcpy((char*)ndp_firm_hdr.ndp_firm_version,(char*)file_version);
	strcpy((char*)ndp_firm_hdr.ndp_firm_rel_date,(char*)rel_date);
}

static uint8_t Parse_CSV_Param_Str(uint8_t *text, uint8_t (*out_param_str)[FL_NDP_FW_PARAM_STR_SIZE])
{
	 uint16_t i=0;
	 uint16_t j=0;
	 uint16_t comma_cnt=0;
	 uint16_t pidx=0;
	 uint8_t out_num_param_str;

	 if(text[i]=='\r'){
		 return 0;
	 }
	 while(text[i]!='\r' && i<ATCMD_PARAM_STR_SIZE)
	 {
		if(text[i] == ','){
			comma_cnt++;
		}
		i++;
	 }
	 i=0;
	 //Extract Params based on ','
	 for(pidx=0;pidx < comma_cnt; pidx++){
		 j=0;
		 while(text[i]!=',' && i<ATCMD_PARAM_STR_SIZE)
		 {
			 if(text[i]!='\"'){
				 out_param_str[pidx][j]=text[i];
				 j++;
			 }
			 i++;
		 }
		 if(j<ATCMD_PARAM_STR_SIZE){
			 out_param_str[pidx][j]=0x0;

		  }else{
			XNO_Printf("ATParamStr len exceeds");
		  }
		 i++;
	 }
	 //Extract Last Param based on '\r'
	 j=0;
	 while(text[i]!='\r'&& i<ATCMD_PARAM_STR_SIZE)
	 {
		 if(text[i]!='\"'){
			 out_param_str[pidx][j]=text[i];
			 j++;
		 }
		 i++;
	 }
	 if(j<ATCMD_PARAM_STR_SIZE){
		 out_param_str[pidx][j]=0x0;
	 }else{
		 XNO_Printf("ATResp len exceeds");
	 }
	 out_num_param_str = comma_cnt+1;
	 return out_num_param_str;
}
static void Update_NDP_FirmHDR_struct(uint8_t *cmdstr,uint8_t *paramstr){
	uint8_t num_str;
	if(strcmp("%FIRM_HDR:",(const char *)cmdstr)==0){
		num_str = Parse_CSV_Param_Str(paramstr,(uint8_t (*)[])Firm_HDR_ParamStr);
		if(num_str == 5){
			Set_NDP_FirmHDR(&Firm_HDR_ParamStr[0][0],&Firm_HDR_ParamStr[1][0], &Firm_HDR_ParamStr[2][0],
				&Firm_HDR_ParamStr[3][0], &Firm_HDR_ParamStr[4][0]);
	    }else{
	    	XNO_Printf("ERROR WHILE UPDATING SWVER STRUCT OF FIRMWARE　NAME");
		}
	}else{
		XNO_Printf("NO MATCH WHILE UPDATING HEADER CONFIG STRUCT");
	}
}
static int Parse_NDP_FirmHDR_struct(uint8_t *str_buf)
{
	uint16_t param,nxt_idx;
	int len=0,ret;
	nxt_idx=0;

	for(param=0;param<2;param++){
		len=Read_Line_from_StringBuf(&str_buf[nxt_idx],LineStr);
		if(len<0){
			XNO_Printf("UNABLE TO READ LINE FROM FLASH FW STS CONFIG STRING");
			ret = -1;
			break;
		}else{
			nxt_idx = nxt_idx + len;
		}
		if(param>0){
			len=Read_Cmd_from_StringBuf(LineStr,CmdStr);

			if(len>0){
				Update_NDP_FirmHDR_struct(CmdStr,&LineStr[len]);
			}
		}
	}
	return ret;
}

static void EXT_Flash_NDP_Read_HDR(uint32_t ndp_fw_hdr_start_addr){

	Flash_SPI_Read(ndp_fw_hdr_start_addr, Fl_ndp_fw_loc_buf, 1000);
	Read_Line_from_StringBuf(Fl_ndp_fw_loc_buf, LineStr);
	if((Fl_ndp_fw_loc_buf[0]=='D') && (Fl_ndp_fw_loc_buf[1]=='O')
		&&(Fl_ndp_fw_loc_buf[2]=='N') &&(Fl_ndp_fw_loc_buf[3]=='E')){
			Parse_NDP_FirmHDR_struct(Fl_ndp_fw_loc_buf);
			ndp_firm_available = SET;
	}else{
			ndp_firm_available = RESET;
	}
}
/*----------------------------------------------------------------------------
* 							Global functions
*----------------------------------------------------------------------------*/
FlagStatus XNO_Is_NDP_Firm_Avail(uint32_t fw_hdr_start_addr){
	EXT_Flash_NDP_Read_HDR(fw_hdr_start_addr);
	return ndp_firm_available;
}
