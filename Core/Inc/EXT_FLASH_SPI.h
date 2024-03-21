/* File: EXT_Flash_SPI.h
 * Author: Alsufiyan
 * Company: Eoxys
 * Description: Required defines for external flashing
 * Created on: 29-Jul-2022
 * Version History
 * v1.0 Initial release
 */
#ifndef INC_EXT_FLASH_SPI_H_
#define INC_EXT_FLASH_SPI_H_
/*----------------------------------------------------------------------------
 * 						User Includes
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include "nuv_m2354_hal_public.h"
/*----------------------------------------------------------------------------
 * 								Defines
 *----------------------------------------------------------------------------*/
#define FW_DATA_BUF_SIZE 	520

#define FLASH_SPI_BUFSIZE   		  2000
#define FLASH_SPI_MFG_ID 			  0xEF

/* Identification Operations */
#define READ_ID_CMD1                         0xAB
#define READ_ID_CMD2                         0x90

/* Read Operations */
#define NORMAL_READ_CMD                      0x03
#define FAST_READ_CMD                        0x0B
//(2 x I/O read command)
#define DUAL_INOUT_READ_CMD					 0xBB
#define DUAL_OUT_READ_CMD 					 0x3B
#define QUAD_INOUT_READ_CMD                  0xEB
#define QUAD_OUT_READ_CMD                    0x6B

/* Program Operations */
#define PAGE_PROG_CMD                        0x02
#define READ_CFG_REG_CMD                     0x15
#define QUAD_IN_PAGE_PROG_CMD                0x32
#define QUAD_PAGE_PROG_CMD                   0x38

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Erase Operations */
#define SECTOR_ERASE_CMD                     0x20
#define BLOCK_ERASE_32KB					 0x52
#define BLOCK_ERASE_64KB					 0xD8
#define CHIP_ERASE_CMD                       0x60 //or 0xC7

/* Register Operations */
#define READ_STATUS_REG_CMD                  0x05
#define WRITE_STATUS_REG_CMD                 0x01
#define FACTORY_MODE_ENABLE					 0x41

/* Reset Operations */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99

#define READ_SFDP 							 0x5A
#define SUSPEND_PROG_OR_ERASE                0x75 //or B0
#define RESUME_PROG_OR_ERASE                 0x7A //or 30
#define DEEP_POWER_DOWN						 0xB9
#define SET_BURST_LEN						 0xC0
#define READ_FACTORY_RESET_REG				 0x44
/*----------------------------------------------------------------------------
 * 								Typedefs variables
 *----------------------------------------------------------------------------*/
typedef enum {
	FW_NONE,
	FW_LDR_BIN,
	FW_BTLD_BIN,
	FW_MAIN_BIN,
	FW_NDP_DSP_BIN,
	FW_NDP_MCU_BIN,
	FW_NDP_DNN_BIN,
	FW_MODEM_BIN,
	FW_GPS_BIN
}FW_PID_TYPE;
typedef enum {
	FW_FORMAT_NONE,
	FW_FORMAT_BIN,
	FW_FORMAT_ASCII
}FW_FORMAT_ST;

typedef struct {
	uint8_t fw_filepid[50];
	uint8_t fw_filename[100];
	uint8_t fw_fileformat[50];
	uint8_t fw_filesize[50];
	uint8_t fw_version[50];
	uint8_t fw_rel_date[100];
}FW_HEADER_ST;

typedef struct{
	FW_PID_TYPE fw_pid;
	FW_FORMAT_ST fw_format;
	uint32_t fw_start_addr;
	uint16_t fw_len;
	uint8_t fw_data_buff[FW_DATA_BUF_SIZE];
	uint16_t fw_data_crc;
	FW_HEADER_ST fw_header;
	uint32_t baudrate;
}FW_FILE_ST;
/*----------------------------------------------------------------------------
 * 								Exported variables
 *----------------------------------------------------------------------------*/
//extern uint32_t addr_count;
/*----------------------------------------------------------------------------
 * 						Exported function Declarations
 *----------------------------------------------------------------------------*/
extern XNO_ERR_STS W25Q_SPI_Read(uint32_t addr, uint8_t *flash_rx_data, uint32_t len);
extern XNO_ERR_STS W25Q_SPI_Write(uint32_t addr, uint8_t *wrdata, uint32_t len);
extern XNO_ERR_STS W25Q_Read_RDSR(uint8_t *rdsr_buf);
extern XNO_ERR_STS W25Q_Read_WRSR(uint8_t *rdsr_buf);
extern XNO_ERR_STS W25Q_Read_CHIPID();
extern XNO_ERR_STS W25Q_WREN();
extern XNO_ERR_STS W25Q_Chip_Erase();
extern XNO_ERR_STS W25Q_Sector_Erase(uint32_t sec_start_addr);
extern XNO_ERR_STS W25Q_RDWR_test();
extern XNO_ERR_STS W25Q_test();
extern XNO_ERR_STS W25Q_Erase_Sectors(uint32_t sec_start_addr, uint16_t num_sectors);
extern XNO_ERR_STS W25Q_FW_Data_Write(uint32_t fw_start_addr,uint8_t *fw_buf,uint32_t fw_len);
extern XNO_ERR_STS W25Q_FW_Hdr_Write(uint32_t hdr_start_addr,FW_FILE_ST fw_file_st);

#endif /* INC_EXT_FLASH_SPI_H_ */
