/* FILE NAME:Utils.h
 * DESCRIPTION:This file provides the common defines for the Utils.c file
 * AUTHOR :  Devika,Eoxys Systems India
 * CREATED ON:17-jul-2020
 * CHANGES :
 * VERSION :V022
 * DATE:31/7/2020
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_
/*----------------------------------------------------------------------------
* 							Standard library includes
*----------------------------------------------------------------------------*/
#include <stdint.h>
/*----------------------------------------------------------------------------
* 							User Includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 							Exported variables and functions
*----------------------------------------------------------------------------*/
extern uint16_t string_copy(uint8_t *dststr,uint8_t *srcstr);
extern uint16_t string_copy_selpos(uint8_t *dststr,uint8_t *srcstr,uint16_t stpos,uint16_t len);
extern uint16_t string_copy_selchar(uint8_t *dststr,uint8_t *srcstr,uint8_t stchar,uint16_t len);
extern uint8_t string_cat(uint8_t *srcstr,uint8_t *apstr);
extern uint8_t string_append(uint8_t *srcstr,uint8_t *apstr);
extern uint8_t string_append_CR(uint8_t *srcstr);
extern uint8_t string_append_CRLF(uint8_t *srcstr);
extern uint8_t string_append_nchars(uint8_t *dststr,uint8_t *srcstr, uint16_t len);
extern uint8_t string_compare(uint8_t *refstr,uint8_t *atstr);
extern uint16_t Get_String_len(uint8_t *buf);
extern uint16_t string_copy_with_len(uint8_t *dststr,uint8_t *srcstr,uint16_t len);
extern uint16_t Get_String_len_upto_CR(uint8_t *buf);
extern int string_copy_betweenchars(uint8_t *dststr,uint8_t *srcstr,uint8_t stchar,uint8_t etchar);
extern uint8_t string_compare_firstnchars(uint8_t *srcstr,uint8_t *refstr,uint16_t num);
extern uint8_t string_remove_CR(uint8_t *srcstr);
extern void int8_to_string(uint8_t *dststr,uint8_t num);
extern void int16_to_string(uint16_t *dststr,uint16_t num);
extern void int32_to_string(uint32_t *dststr,uint32_t num);
extern void int8_to_string_hex(uint8_t *dststr,uint8_t num);
extern void int16_to_string_hex(uint8_t *dststr,uint16_t num);
extern void int16_to_string_hex(uint8_t *dststr,uint16_t num);
extern void int32_to_string_hex(uint8_t *dststr,uint32_t num);
extern void int8_to_string_decimal(uint8_t *dststr,uint8_t num);
extern void int16_to_string_decimal(uint8_t *dststr,uint16_t num);
extern void int32_to_string_decimal(uint8_t *dststr,uint32_t num);
extern void decimal_string_to_int8(uint8_t *result,uint8_t *srcstr);
extern int decimal_string_to_int(uint8_t *srcstr);
extern void decimal_string_to_int16(uint16_t *result,uint8_t *srcstr);
extern void decimal_string_to_int32(uint32_t *result,uint8_t *srcstr);
extern void bcd_hex_string_to_int16(uint16_t *result,uint8_t *srcstr);
extern void bcd_hex_string_to_int8(uint8_t *result,uint8_t *srcstr);
extern void bcd_int8_incr(uint8_t *result,uint8_t src);
extern uint8_t day_to_string(uint8_t *dststr,uint8_t weekday);
extern void mon_to_string(uint8_t *dststr,uint8_t month);
extern void conv_datestr_to_hex(uint8_t *yy,uint8_t *mm,uint8_t *dd,uint8_t *sbuf);
extern void conv_timestr_to_hex(uint8_t *hh,uint8_t *mm,uint8_t *ss,uint8_t *sbuf);
extern void int8buf_to_asciibuf(uint8_t *destbuf,uint8_t *srcbuf,uint16_t len);
extern void asciibuf_to_int8buf(uint8_t *destbuf,uint8_t *srcbuf,uint16_t len);
extern uint16_t ascii_intchar_to_int8(uint8_t ch);
extern int Read_Line_from_StringBuf(uint8_t *strbuf,uint8_t *linestr);
extern int Read_Cmd_from_StringBuf(uint8_t *strbuf,uint8_t *cmdstr);
extern int Read_Line_from_StringBuf(uint8_t *strbuf,uint8_t *linestr);
extern int Read_Cmd_from_StringBuf(uint8_t *strbuf,uint8_t *cmdstr);
extern void removeSubstring(char *s, const char *toremove);
#endif /* INC_UTILS_H_ */
