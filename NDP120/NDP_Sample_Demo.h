/* File: NDP_Sample_Demo.h
* Author: Devika
* Company: Eoxys
* Description:
* Created on: 01-Aug-2022
* Version History
* v1.0 Initial release
*/
#ifndef INC_NDP_SAMPLE_DEMO_H_
#define INC_NDP_SAMPLE_DEMO_H_
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/
typedef struct {
	uint8_t ndp_firm_rel_date[20];
	uint8_t ndp_firm_version[10];
	uint8_t ndp_firm_filetype[10];
	uint8_t ndp_firm_filename[50];
	uint8_t ndp_firm_filesize[10];
}NDP_FIRM_HEADER;
/*----------------------------------------------------------------------------
* 							Exported variables
*----------------------------------------------------------------------------*/
extern NDP_FIRM_HEADER ndp_firm_hdr;
/*----------------------------------------------------------------------------
* 							Exported functions
*----------------------------------------------------------------------------*/
extern FlagStatus XNO_Is_NDP_Firm_Avail(uint32_t fw_hdr_start_addr);
extern void NDP_Demo_Task(void const * argument);
#endif /* INC_NDP_SAMPLE_DEMO_H_ */
/* File: NDP_Sample_Demo.h
* Author: Devika
* Company: Eoxys
* Description:
* Created on: 01-Aug-2022
* Version History
* v1.0 Initial release
*/
#ifndef INC_NDP_SAMPLE_DEMO_H_
#define INC_NDP_SAMPLE_DEMO_H_
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/
typedef struct {
	uint8_t ndp_firm_rel_date[20];
	uint8_t ndp_firm_version[10];
	uint8_t ndp_firm_filetype[10];
	uint8_t ndp_firm_filename[50];
	uint8_t ndp_firm_filesize[10];
}NDP_FIRM_HEADER;
/*----------------------------------------------------------------------------
* 							Exported variables
*----------------------------------------------------------------------------*/
extern NDP_FIRM_HEADER ndp_firm_hdr;
/*----------------------------------------------------------------------------
* 							Exported functions
*----------------------------------------------------------------------------*/
extern FlagStatus XNO_Is_NDP_Firm_Avail(uint32_t fw_hdr_start_addr);
extern void NDP_Demo_Task(void const * argument);
#endif /* INC_NDP_SAMPLE_DEMO_H_ */
