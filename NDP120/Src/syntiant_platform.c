/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2018-2020 Syntiant Corporation
 *   All Rights Reserved.
 *
 *  NOTICE:  All information contained herein is, and remains the property of
 *  Syntiant Corporation and its suppliers, if any.  The intellectual and
 *  technical concepts contained herein are proprietary to Syntiant Corporation
 *  and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 *  process, and are protected by trade secret or copyright law.  Dissemination
 *  of this information or reproduction of this material is strictly forbidden
 *  unless prior written permission is obtained from Syntiant Corporation.
 */

#include <assert.h>
#include <XNO_DEBUG_TASK.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <syntiant_ndp.h>
#include <syntiant_ndp120.h>
#include <syntiant_ndp_error.h>
#include <syntiant_ndp_driver.h>
#include <ndp120_dsp_fw_state.h>
#include <ndp120_regs.h>
#include <ndp120_spi_regs.h> 
#include <XNO_UTILS.h>

#include "syntiant_platform.h"
#include "syntiant_platform_iif.h"
#include "syntiant_platform_audio.h"
#include "syntiant_platform_afe.h"
#include "ndp120_bringup_tests.h"

#include "main.h"
#include "NDP_Sample_Demo.h"
//#include "EXT_FLASH.h"

#define LTE_NDP 	0
#define WIFI_NDP	0
#define USE_32768_XTAL
#ifdef APP_TYPE_NEAR_FIELD // For Near Field Demo

char preval[50];
static char payload_print_buf[100];
char payload_data_buf[100];

uint8_t counter=0;
/*
  0:  PLL voltage = 0.9v, input freq = 32768   Hz, system freq = 15360000 Hz
  1:  PLL voltage = 0.9v, input freq = 32768   Hz, system freq = 21504000 Hz
  2:  PLL voltage = 0.9v, input freq = 32768   Hz, system freq = 30720000 Hz
  3:  PLL voltage = 0.9v, input freq = 4096000 Hz, system freq = 15360000 Hz
  4:  PLL voltage = 0.9v, input freq = 4096000 Hz, system freq = 21504000 Hz
  5:  PLL voltage = 0.9v, input freq = 4096000 Hz, system freq = 30720000 Hz
  6:  PLL voltage = 1.0v, input freq = 32768   Hz, system freq = 49152000 Hz
  7:  PLL voltage = 1.0v, input freq = 32768   Hz, system freq = 61440000 Hz
  8:  PLL voltage = 1.0v, input freq = 4096000 Hz, system freq = 49152000 Hz
  9:  PLL voltage = 1.0v, input freq = 4096000 Hz, system freq = 61440000 Hz
  10: PLL voltage = 1.1v, input freq = 32768   Hz, system freq = 76800000 Hz
  11: PLL voltage = 1.1v, input freq = 32768   Hz, system freq = 98304000 Hz
  12: PLL voltage = 1.1v, input freq = 4096000 Hz, system freq = 76800000 Hz
  13: PLL voltage = 1.1v, input freq = 4096000 Hz, system freq = 98304000 Hz
  14: FLL voltage = 0.9v, input freq = 32768   Hz, system freq = 15360000 Hz
  15: FLL voltage = 0.9v, input freq = 32768   Hz, system freq = 16896000 Hz
 *
 */

#define DEAULT_CLOCK_OPTION 1

//#define NUM_OF_CLASS 2 // 1(other class )+target class == 1+1 = 2
//const char *labels[] = {"alexa"};

	 #define NUM_OF_CLASS 12  // 1(other class )+target class == 1+11 = 12
	 const char *labels[] =  {"baby_crying","gun_shot","dog_bark","drilling","car_theft_alarm","jack_hammer","fire_alarm","glass_break","hammer","men_scream","women_scream"};

//	 #define NUM_OF_CLASS 2  // 1(other class )+target class == 1+1 = 2
//	 const char *labels[] =  {"elephant"};
//	 #define NUM_OF_CLASS 2  // 1(other class )+target class == 1+1 = 2
//	 const char *labels[] =  {"mosquito"};

    #if 0  // xiaodu only
        #include "xiaoduxiaodu_341_b0_v8.h"
        #define NN_MODEL_BUF   XIAODUXIAODU_341_B0_V8_MODEL
        #define NN_MODEL_LEN   XIAODUXIAODU_341_B0_V8_MODEL_LEN
        #define NUM_OF_CLASS 2
        const char *labels[] = {"xiaoduxiaodu"};
    #if 0  // xiaodu_10cmds
        #include "xiaodu_ndp120_model406_v66_fsm_ph.h"
        #define NN_MODEL_BUF   XIAODU_NDP120_MODEL406_V66_FSM_PH_MODEL
        #define NN_MODEL_LEN   XIAODU_NDP120_MODEL406_V66_FSM_PH_MODEL_LEN
        #define NUM_OF_CLASS 12
        const char *labels[] = { "xiaoduxiaodu", 
                                "tingzhibofang",
                                "zengdayinliang",
                                "bofangyinyue",
                                "jianxiaoyinliang",
                                "xiayishou",
                                "guaduandianhua",
                                "shezhinaozhong",
                                "chakantianqi",
                                "jietingdianhua",
                                "shangyishou"};
	#endif
    #endif

#endif

#ifdef  APP_TYPE_FAR_FIELD  // For FF w/o AEC
//    #define DEAULT_CLOCK_OPTION  11   // PLL voltage = 1.1v, input freq = 32768   Hz, system freq = 98304000 Hz
	#define DEAULT_CLOCK_OPTION  7     //PLL voltage = 1.0v, input freq = 32768   Hz, system freq = 61440000 Hz

    #include "dsp_firmware_ffnoaec.h"  
    #define  DSP_FW_BUF    DSP_FIRMWARE_FFNOAEC_MODEL
    #define  DSP_FW_LEN    DSP_FIRMWARE_FFNOAEC_MODEL_LEN

    #include "alexa_334ua__v1_ndp120b0.h"
    #define NN_MODEL_BUF   ALEXA_334UA__V1_NDP120B0_MODEL
    #define NN_MODEL_LEN   ALEXA_334UA__V1_NDP120B0_MODEL_LEN
    #define NUM_OF_CLASS 2
    const char *labels[] = {"alexa"};

#endif
#ifdef MCU_RAM_TEST
#include "mcu_test.h"
#endif

#define STRING_LEN 256
#define MAX_LABELS 32

extern ndp120_pll_preset_t ndp120_pll_presets[];
extern ndp120_fll_preset_t ndp120_fll_presets[];
static uint32_t ext_clk_input_freq[] =
       {1536000, 2104000, 15360000, 21504000, 30720000};

struct syntiant_ndp_integration_interfaces_s iif;
struct syntiant_platform_app_s ndp_app;
static uint32_t ndp_fw_size, fw_flash_cnt;
static uint8_t ndp_payload_buf[1200];

int XNO_Load_NDP_FW(struct syntiant_ndp_device_s *ndp, uint32_t fw_sec_start_addr, uint8_t *payload_buf){
	int s, chunksize=0;
	uint32_t i=0;
	uint32_t start=0,end=0,addr=0;
//	SYNTIANT_TRACE("Loading %d bytes of package data\r\n", ndp_fw_len);
	decimal_string_to_int32(&ndp_fw_size, ndp_firm_hdr.ndp_firm_filesize);
	/* reset parser state */
	s = syntiant_ndp_load(ndp, NULL, 0);
	if (s != SYNTIANT_NDP_ERROR_MORE) {
		SYNTIANT_TRACE("Error resetting package load state  %d\r\n", s);
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	fw_flash_cnt = (ndp_fw_size/1024);
	if ((ndp_fw_size % 1024) != 0){
		fw_flash_cnt = fw_flash_cnt + 1;
	}
	for(i=0;i<fw_flash_cnt;i++){
		start = i*1024;//* 1024;
		end = start + 1024;
		if(end > ndp_fw_size){
			end = ndp_fw_size-1;
		}else{
			end = end - 1;
		}
		chunksize = (int)((end - start) + 1);
		addr = start + fw_sec_start_addr;
		Flash_SPI_Read(addr, payload_buf, chunksize); //Read from EXT Flash
		s = syntiant_ndp_load(ndp, payload_buf, chunksize);
	}
	return s;
}

#ifdef ENABLE_NDP_BRINGUP_TEST
static int do_butest(struct syntiant_platform_app_s *ndp120)
{
    int s;
    struct syntiant_ndp_device_s *ndp = ndp120->ndp;
    char *locations[] = NDP120_BRINGUP_TESTS_FAIL_LOCATION_NAMES;
    char *location = NULL;

    s = ndp120_bringup_tests_all(ndp);
    if (s) {
        location = locations[ndp120_bringup_tests_fail_location];
        SYNTIANT_TRACE("failed with %s: %s\n", location,
                        syntiant_ndp_error_name(s));
    }
    return s;
}
#endif

#if 0
static int do_load_package(struct syntiant_ndp_device_s *ndp, void *package, int package_len)
{
    int s;
    SYNTIANT_TRACE("Loading %d bytes of package data\r\n", package_len);

    /* reset parser state */
    s = syntiant_ndp_load(ndp, NULL, 0);
    if (s != SYNTIANT_NDP_ERROR_MORE) {
        SYNTIANT_TRACE("Error resetting package load state  %d\r\n", s);
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    s = syntiant_ndp_load(ndp, package, package_len);
    return s;
}
#endif
int syntiant_platform_poll()
{
    int s,p;
    uint32_t notifications;  
    struct syntiant_platform_app_s *ndp120;
    ndp120 = &ndp_app;

    if(!ndp120->initialized){
        return SYNTIANT_NDP_ERROR_FAIL;
    }
    s = syntiant_ndp_poll(ndp120->ndp, &notifications, 1);
    syntiant_platform_check_status("syntiant_ndp_poll", s, 1);
    return s;   
}

int syntiant_platform_trigger_handler(int *class_index, uint32_t count)
{
    int s,p;
    uint32_t summary;
    uint32_t strength;
    uint32_t confidence;
    uint8_t battary_val;
    float percentage;
    uint32_t notifications;
    *class_index = -1;
    char locstr[50]="";
    char locstr2[100]="";
    char locstr3[50]="";
    char locstr4[50]="";
    char locstr5[50]="";

    struct syntiant_platform_app_s *ndp120;
    ndp120 = &ndp_app;

    if(!ndp120->initialized)
    {
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    s = syntiant_ndp_poll(ndp120->ndp, &notifications, 1);
    if(syntiant_platform_check_status("syntiant_ndp_poll", s, 1))
    {
        return s;
    }
    if (!(notifications & SYNTIANT_NDP_NOTIFICATION_MATCH)) {
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    s = syntiant_ndp_get_match_summary(ndp120->ndp, &summary);
    if(syntiant_platform_check_status("get_match_summary", s, 1))
    {
        return s;
    }
    p = syntiant_ndp_get_match_strength(ndp120->ndp, &strength,60,0);

    if ((summary & NDP120_SPI_MATCH_MATCH_MASK)
        && (NDP120_SPI_MATCHSTS_STATUS_EXTRACT(summary) != (NUM_OF_CLASS - 1))) {
//    if(1){
//        SYNTIANT_TRACE("%s [%d] matched\r\n",
//                        labels[NDP120_SPI_MATCH_WINNER_EXTRACT(summary)],
//                         NDP120_SPI_MATCH_WINNER_EXTRACT(summary));
#if(LTE_NDP==1)
    	string_copy((uint8_t *) locstr, (uint8_t *) labels[NDP120_SPI_MATCH_WINNER_EXTRACT(summary)]);
    	if(strcmp(preval,locstr)==0){
    		counter=1;
			if(is_NDP_timeout_occurred()){
				NDP_Reset_timer_flag();
				XNO_Start_Timer(&NDP_timer_st,TIMEOUT_5_SECS);
				counter=0;
			}
    	}else{
    		if(is_NDP_timeout_occurred()){
    			NDP_Reset_timer_flag();
    			XNO_Start_Timer(&NDP_timer_st,TIMEOUT_5_SECS);
    		}
    		counter=0;
    	}
		Prepare_Device_Date_time();
		string_append(payload_buf,(uint8_t *) locstr);
#if 1
		strcpy(payload_print_buf,payload_buf);
    	XNO_Debug_Printf(payload_print_buf);
#endif
   		strcpy(preval,locstr);
    	if(counter==0){
    		string_append((uint8_t *) payload_buf,"\n");
    		LTE_enqueue_payload(payload_buf);//enqueue the ndp events
    	}

#endif
//<epoch_time>,<macid_without_space>,<detected-1/undetected-0>,<accurary/confident(0 to 100)>,<Battary Per (0 to 100>,<cnt(4byte)>
#if(WIFI_NDP==1)
    	string_copy((uint8_t *) locstr, (uint8_t *) labels[NDP120_SPI_MATCH_WINNER_EXTRACT(summary)]);
    	if(strcmp(preval,locstr)==0){
    		counter=1;
			if(is_NDP_timeout_occurred()){
				NDP_Reset_timer_flag();
				XNO_Start_Timer(&NDP_timer_st,TIMEOUT_5_SECS);
				counter=0;
			}
    	}else{
    		if(is_NDP_timeout_occurred()){
    			NDP_Reset_timer_flag();
    			XNO_Start_Timer(&NDP_timer_st,TIMEOUT_5_SECS);
    		}
    		counter=0;
    	}
		Prepare_Device_Date_time();
		string_append((uint8_t *) locstr2, (uint8_t *) payload_buf);
		string_append((uint8_t *) locstr2, (uint8_t *) "1,");
		confidence = strength;
		if (confidence > 255)
			confidence = 255;
		percentage = ((float)confidence * 100) / 255;
//		Payload = "1696329714,IST,e0693a047222,1,78,4294967295";
		int32_to_string_decimal((uint8_t *)locstr3, percentage);
		string_append(locstr2,(uint8_t *) locstr3);
		string_append((uint8_t *) locstr2, (uint8_t *) ",");
		battary_val =65;
		int32_to_string_decimal((uint8_t *)locstr5, battary_val);
		string_append((uint8_t *) locstr2, (uint8_t *)locstr5);
		string_append((uint8_t *) locstr2, (uint8_t *) ",");
		int32_to_string_decimal((uint8_t *) locstr4, count);
		string_append((uint8_t *) locstr2, (uint8_t *)locstr4);
		string_append((uint8_t *) locstr2, (uint8_t *) "\n");
#if 1
		strcpy(payload_print_buf,locstr2);
		strcpy(payload_data_buf,locstr2);
//		XNO_Debug_Printf(payload_print_buf);
#endif
//		strcpy(preval,locstr);
//		if(counter==0){
//			string_append((uint8_t *) payload_data_buf,"\n");
//			WIFI_enqueue_payload(payload_data_buf);//enqueue the ndp events
//			ringbuf_enqueue_arr(&WIFI_ring_buf_st,&payload_data_buf,40);//enqueue the ndp events
//		ringbuf_enqueue(&WIFI_ring_buf_st,&payload_data_buf);//enqueue the ndp events
//		}

#else
		int32_to_string_decimal((uint8_t *) locstr, count);
		string_append((uint8_t *) locstr, (uint8_t *) ": match-> ");
		string_append((uint8_t *) locstr, (uint8_t *) labels[NDP120_SPI_MATCH_WINNER_EXTRACT(summary)]);
		XNO_Printf(locstr);
#endif
        *class_index = NDP120_SPI_MATCH_WINNER_EXTRACT(summary);
        s = SYNTIANT_NDP_ERROR_NONE;
    } else {
#if(WIFI_NDP==1)
    	Prepare_Device_Date_time();
    			string_append((uint8_t *) locstr2, (uint8_t *) payload_buf);
    			string_append((uint8_t *) locstr2, (uint8_t *) "0,");
    			confidence = strength;
    			if (confidence > 255)
    				confidence = 255;
    			percentage = ((float)confidence * 100) / 255;
    			int32_to_string_decimal((uint8_t *)locstr3, percentage);
    			string_append(locstr2,(uint8_t *) locstr3);
    			string_append((uint8_t *) locstr2, (uint8_t *) ",");
				battary_val =65;
				int32_to_string_decimal((uint8_t *)locstr5, battary_val);
				string_append((uint8_t *) locstr2, (uint8_t *)locstr5);
				string_append((uint8_t *) locstr2, (uint8_t *) ",");
    			string_append((uint8_t *) locstr2, (uint8_t *) "0\n");
    			strcpy(payload_data_buf,locstr2);
    			WIFI_enqueue_payload(payload_data_buf);//enqueue the ndp events
//    			int32_to_string_decimal((uint8_t *) locstr4, count);
//    			string_append((uint8_t *) locstr2, (uint8_t *)locstr4);
//    			string_append((uint8_t *) locstr2, (uint8_t *) "\n");
//#endif
//    	#if 0
    			strcpy(payload_print_buf,locstr2);
//    			XNO_Debug_Printf(payload_print_buf);
    	#endif
        s = SYNTIANT_NDP_ERROR_FAIL;
    }
    
    return s;
}


static 
int convert_clock_option_to_ext_freq(uint8_t clock_option,
                                            uint32_t *input_freq)
{
    int s = SYNTIANT_NDP_ERROR_NONE;

    if (clock_option >= 0 && clock_option <= 13) {
        *input_freq = ndp120_pll_presets[clock_option].input_freq;
    } else if (clock_option >= 14 && clock_option <= 15) {
        *input_freq = ndp120_fll_presets[clock_option - 14].input_freq;
    } else if (clock_option >= 20 && clock_option <= 24) {
        *input_freq = ext_clk_input_freq[clock_option - 20];
    } else {
        s = SYNTIANT_NDP_ERROR_ARG;
    }
    return s;
}

static int
ndp120_config_clock(struct syntiant_platform_app_s *ndp120)
{
    int s = 0;
    syntiant_ndp120_config_clk_src_t config_clk_src;
    syntiant_ndp120_config_clk_pll_t config_clk_pll;
    syntiant_ndp120_config_clk_fll_t config_clk_fll;
    syntiant_ndp120_config_clk_xtal_t config_clk_xtal;
    struct syntiant_ndp_device_s *ndp = ndp120->ndp;
    uint8_t clock_option = ndp120->clock_option;

    if (clock_option <= 13) { /* use PLL */
        if (ndp120->use_xtal) {
            /* enable xtal */
            config_clk_xtal = (syntiant_ndp120_config_clk_xtal_t)
                    { 0, NDP120_CONFIG_SET_CLK_XTAL_OUT |
                      NDP120_CONFIG_SET_CLK_XTAL_OSC, 1, 1 };
            s = syntiant_ndp120_config_clk_xtal(ndp, &config_clk_xtal);
            if(s){
            	syntiant_platform_check_status("syntiant_ndp120_config_clk_xtal", s, 1);
            }
            
            syntiant_platform_sleep_ms(100);

            /* Switch to XTAL pad, select refclk */
            config_clk_src = (syntiant_ndp120_config_clk_src_t)
                    { 0, NDP120_CONFIG_SET_CLK_SRC_REFSEL,
                      1, 0, 0, 0, 0, 0, 0 };
            s = syntiant_ndp120_config_clk_src(ndp, &config_clk_src);
            if(s){
            	syntiant_platform_check_status("syntiant_ndp120_config_clk_src", s, 1);
            }
            config_clk_pll = (syntiant_ndp120_config_clk_pll_t) {
                0, 1, clock_option, 0, 0};
            s = syntiant_ndp120_config_clk_pll(ndp, &config_clk_pll);
            if(s){
            	syntiant_platform_check_status("syntiant_ndp120_config_clk_pll", s, 1);
            	return s;
            }else{
            	XNO_Printf("NDP PLL LOCKED");
            }
        }
        else {
            config_clk_pll = (syntiant_ndp120_config_clk_pll_t) {
                0, 1, clock_option, 0, 0};
            s = syntiant_ndp120_config_clk_pll(ndp, &config_clk_pll);

            config_clk_src = (syntiant_ndp120_config_clk_src_t)
                    { 0, NDP120_CONFIG_SET_CLK_SRC_EXTSEL |
                      NDP120_CONFIG_SET_CLK_SRC_CLKSEL,
                      0, 1, 0, 0, 0, 0, 0};
            s = syntiant_ndp120_config_clk_src(ndp, &config_clk_src);
        }

        syntiant_platform_sleep_ms(500);
        config_clk_pll = (syntiant_ndp120_config_clk_pll_t) {
            1, 0, 0, 0, 0};
        s = syntiant_ndp120_config_clk_pll(ndp, &config_clk_pll);
    } else if ((clock_option <= 15) && (clock_option >= 14)) { /* use FLL */
        if (ndp120->use_xtal) {
            /* enable xtal */
            config_clk_xtal = (syntiant_ndp120_config_clk_xtal_t)
                    { 0, NDP120_CONFIG_SET_CLK_XTAL_OUT |
                      NDP120_CONFIG_SET_CLK_XTAL_OSC, 1, 1 };
            s = syntiant_ndp120_config_clk_xtal(ndp, &config_clk_xtal);
            syntiant_platform_sleep_ms(100);

            /* Switch to XTAL pad, select refclk */
            config_clk_src = (syntiant_ndp120_config_clk_src_t)
                    { 0, NDP120_CONFIG_SET_CLK_SRC_REFSEL,
                        1, 0, 0, 0, 0, 0, 0 };
            s = syntiant_ndp120_config_clk_src(ndp, &config_clk_src);

            config_clk_fll = (syntiant_ndp120_config_clk_fll_t) {
                0, 1, clock_option - 14, 0};
            s = syntiant_ndp120_config_clk_fll(ndp, &config_clk_fll);
        }
        else {
            config_clk_fll = (syntiant_ndp120_config_clk_fll_t) {
                0, 1, clock_option - 14, 0};
            s = syntiant_ndp120_config_clk_fll(ndp, &config_clk_fll);

            config_clk_src = (syntiant_ndp120_config_clk_src_t)
                    { 0, NDP120_CONFIG_SET_CLK_SRC_EXTSEL |
                        NDP120_CONFIG_SET_CLK_SRC_CLKSEL,
                        0, 0, 0, 0, 0, 0, 0 };
            s = syntiant_ndp120_config_clk_src(ndp, &config_clk_src);
        }

        syntiant_platform_sleep_ms(500);
        config_clk_fll = (syntiant_ndp120_config_clk_fll_t) {
            1, 0, 0, 0};
        s = syntiant_ndp120_config_clk_fll(ndp, &config_clk_fll);
    } else { /* external clock */
        config_clk_src = (syntiant_ndp120_config_clk_src_t)
              { 0, NDP120_CONFIG_SET_CLK_SRC_EXTSEL,
                0, 0, 1, 0, 0, 0, 0 };
        s = syntiant_ndp120_config_clk_src(ndp, &config_clk_src);
    }

    return s;
}

static 
void ndp120_default_configs_set(struct syntiant_platform_app_s *ndp120)
{
    ndp120->init_mode = SYNTIANT_NDP_INIT_MODE_RESET;
    ndp120->en_far_field = ENABLE_FAR_FIELD;
    ndp120->en_aec = ENABLE_AEC;

    ndp120->agc_shift_dir = NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT;
    ndp120->agc_shift_cnt = 0;
    ndp120->pdm_freq = 768000;

    ndp120->dnn_input = NDP120_CONFIG_SET_DNN_INPUT_PDM1;
//    ndp120->dnn_input = NDP120_CONFIG_SET_DNN_INPUT_PDM_SUM;
    ndp120->pdm_ndp = NDP120_CONFIG_VALUE_PDM_CLK_MODE_INTERNAL;
//    ndp120->pdm_ndp = NDP120_CONFIG_VALUE_PDM_CLK_MODE_DUAL_INTERNAL;
    ndp120->pdm_in_shift = 13;
    ndp120->pdm_out_shift = 7;

    ndp120->clock_option = DEAULT_CLOCK_OPTION;  
#ifdef USE_32768_XTAL
    ndp120->use_xtal = 1;
#else
    ndp120->use_xtal = 0;
#endif

    // AUD2
    ndp120->aud2_src_type = NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO;
    ndp120->aud2_src_param = 3;
    ndp120->aud2_enable = 0;
    ndp120->aud2_out_mode = NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_SLAVE;

    // I2S Ctl
    ndp120->i2s_framesize = 32;
    ndp120->i2s_samplesize = 16;
    ndp120->i2s_msb = 30;
    ndp120->i2s_negedgeenable = NDP120_CONFIG_VALUE_I2S_EDGE_NEG;
    ndp120->i2s_right_enable = 0;
    ndp120->i2s_left_enable = 1;
    ndp120->i2s_mode = NDP120_CONFIG_VALUE_I2S_MODE_STANDARD;
    ndp120->i2s_pack = 0;
    ndp120->delayed_flop_sensitivity = NDP120_CONFIG_VALUE_I2S_DELAYED_FLOP_SENSITIVITY_NORMAL;

    /* misccfg */
    /* ndp120->i2s_numdelay = 0; */
    ndp120->i2s_zc = 0;
    ndp120->i2s_shift_dir = 1;
    ndp120->i2s_shift_cnt = 0;

    ndp120->audio_sync_enable = 0;
    ndp120->audio_sync_offset = 0;
    ndp120->audio_sync_adj_chan = NDP120_DSP_AUDIO_SYNC_AUD0;
    ndp120->audio_sync_ref_chan = NDP120_DSP_AUDIO_SYNC_AUD1;
    ndp120->pcm_tank_size_in_msec = 0;
    ndp120->aud_samp_size_bytes = 768;
    ndp120->func_samp_size_bytes = 40;
    ndp120->aud_samp_cap = 64;
    ndp120->func_samp_cap = 40;
    ndp120->match_per_frame = 1;
    ndp120->device_type = 0x0;

    convert_clock_option_to_ext_freq(ndp120->clock_option,
                                     &ndp120->input_freq);
}

#if 0
static int get_config(struct syntiant_platform_app_s *ndp120)
{
    int s, i, j;
    char fwver[STRING_LEN] = "";
    char paramver[STRING_LEN] = "";
    char pkgver[STRING_LEN] = "";
    char label_data[STRING_LEN] = "";
    char *labels[MAX_LABELS];
    char loc_str[50]={'\0'};
    struct syntiant_ndp_config_s config;

    memset(&config, 0, sizeof(struct syntiant_ndp_config_s));
    config.firmware_version = fwver;
    config.firmware_version_len = STRING_LEN;
    config.parameters_version = paramver;
    config.parameters_version_len = STRING_LEN;
    config.pkg_version = pkgver;
    config.pkg_version_len = STRING_LEN;
    config.labels = label_data;
    config.labels_len = STRING_LEN;

    s = syntiant_ndp_get_config(ndp120->ndp, &config);
    if(syntiant_platform_check_status("get_config", s, 0))
    {
        return s;
    }

    if (STRING_LEN < config.pkg_version_len) {
        XNO_Debug_Printf("package version string too long");
        return SYNTIANT_NDP_ERROR_FAIL;
    }
    if (STRING_LEN < config.firmware_version_len) {
        XNO_Debug_Printf("firmware version string too long");
        return SYNTIANT_NDP_ERROR_FAIL;
    }
    if (STRING_LEN < config.parameters_version_len) {
        XNO_Debug_Printf("parameter version string too long");
        return SYNTIANT_NDP_ERROR_FAIL;
    }
    if (STRING_LEN < config.labels_len) {
        XNO_Debug_Printf("labels strings too long");
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    /* get pointers to the labels */
    j = 0;
    for (i = 0; i < config.classes; i++) {
        labels[i] = &label_data[j];
        for (; label_data[j]; j++)
            ;
        j++;
    }
    string_append(loc_str,"chip: ");
    string_append(loc_str, config.device_type);
    XNO_Debug_Printf(loc_str);
    memset(&loc_str,0,sizeof(loc_str));
    string_append(loc_str,"package version: ");
    string_append(loc_str, pkgver);
    XNO_Debug_Printf(loc_str);
    memset(&loc_str,0,sizeof(loc_str));
    string_append(loc_str,"firmware version: ");
    string_append(loc_str, fwver);
    XNO_Debug_Printf(loc_str);
    memset(&loc_str,0,sizeof(loc_str));
    string_append(loc_str,"parameters version: ");
    string_append(loc_str, paramver);
    XNO_Debug_Printf(loc_str);
    memset(&loc_str,0,sizeof(loc_str));

    string_append(loc_str,"labels: ");
    for (i = 0; i < config.classes; i++) {
    	string_append(loc_str, labels[i]);
    	string_append(loc_str, " ");
    }
    XNO_Debug_Printf(loc_str);
    memset(&loc_str,0,sizeof(loc_str));
    return s;
}
#endif 

static int do_watch(struct syntiant_platform_app_s *ndp120, int far_field_enable, int aec_enable)
{
    int s;
//    int sample_bytes = 0;
//    syntiant_ndp120_config_misc_t ndp120_config;

    if(far_field_enable && aec_enable){
        s =  syntiant_platform_i2s_setup(ndp120, far_field_enable, aec_enable);
        if(s) {
            SYNTIANT_TRACE("I2S setup failed\r\n");
            return s;
        }
    }

    s = syntiant_platform_afe_setup(ndp120, far_field_enable, aec_enable);
    if(s) {
        SYNTIANT_TRACE("AFE setup failed\r\n");
        return s;
    }

    syntiant_ndp120_init_ring_buffer_pointers(ndp120->ndp, 0);
    return SYNTIANT_NDP_ERROR_NONE;
}
#if 0
static int do_dsp_tank_size_config(struct syntiant_platform_app_s *ndp120)
{
    int s = 0;
    struct syntiant_ndp_device_s *ndp = ndp120->ndp;
    ndp120_dsp_config_t cfg;
    ndp120_dsp_memory_state_t mem_state;
    uint32_t adx, max_winstep_us;
    syntiant_ndp120_config_tank_t tankcfg;

    if (!ndp->d.ndp120.dsp_fw_state_addr) {
         s = SYNTIANT_NDP_ERROR_UNINIT;
         return s;
    }

    adx = ndp->d.ndp120.dsp_fw_state_addr +
          (uint32_t) offsetof(ndp120_dsp_fw_base_t, max_winstep_us);
    s = syntiant_ndp120_read_block(ndp, 1, adx, &max_winstep_us,
                                   sizeof(max_winstep_us));
    if (s){
        return s;
    }

    s = syntiant_ndp120_read_dsp_tank_memory(ndp, &mem_state);
    if (s) {
        return s;
    }

    SYNTIANT_TRACE("=========== dsp memory state prior to config ===========\n");
    SYNTIANT_TRACE("dsp mem_heap_total = %d\ndsp mem_dnn_data_total = %d\n",
           mem_state.mem_heap_total, mem_state.mem_dnn_data_total);
    SYNTIANT_TRACE("dsp algo_mem_use_heap = %d\ndsp algo_mem_use_dnn_data = %d\n",
           mem_state.algo_mem_use_heap, mem_state.algo_mem_use_dnn_data);
    SYNTIANT_TRACE("dsp sample_mem_use_heap = %d\ndsp sample_mem_use_dnn_data = %d\n",
           mem_state.sample_mem_use_heap, mem_state.sample_mem_use_dnn_data);

    memset(&tankcfg, 0, sizeof(tankcfg));
    if (ndp120->pcm_tank_size_in_msec == -1) {
        tankcfg.get = 1;
    } else if (ndp120->pcm_tank_size_in_msec == 0) {
        tankcfg.set = 1;
        tankcfg.sampletank_msec = 0; /* config for maximum size */
    } else if (ndp120->pcm_tank_size_in_msec > 0) {
        tankcfg.set = 1;
        tankcfg.sampletank_msec = ndp120->pcm_tank_size_in_msec;
    }

    s = syntiant_ndp120_config_dsp_tank_memory(ndp, &tankcfg);
    if (s) {
        if ((s == SYNTIANT_NDP_ERROR_NOMEM) &&
            (ndp120->pcm_tank_size_in_msec > 0)) {
            SYNTIANT_TRACE("\nnot enough memory for specified tank size\n");
        } else {
            SYNTIANT_TRACE("\nerror in configuring dsp tank sizes: %d\n", s);
        }
        return s;
    }

    if (ndp120->pcm_tank_size_in_msec == -1) {
        SYNTIANT_TRACE("\ncurrent tank size in msec is %d\n",
               (int)tankcfg.sampletank_msec);
    } else if (!ndp120->pcm_tank_size_in_msec) {
        SYNTIANT_TRACE("\nconfiguring maximum tank size in msec to be %d\n",
        		(int)tankcfg.sampletank_msec);
    } else {
        SYNTIANT_TRACE("\nconfiguring tank size in msec to be %d\n",
        		(int)tankcfg.sampletank_msec);
    }
    
    syntiant_platform_sleep_ms(100);
    s = syntiant_ndp120_read_dsp_tank_memory(ndp, &mem_state);
    if (s) {
        return s;
    }

    SYNTIANT_TRACE("=========== dsp memory state after config ==============\n");
    SYNTIANT_TRACE("dsp mem_heap_total = %d\ndsp mem_dnn_data_total = %d\n",
    		(int) mem_state.mem_heap_total,(int) mem_state.mem_dnn_data_total);
    SYNTIANT_TRACE("dsp algo_mem_use_heap = %d\ndsp algo_mem_use_dnn_data = %d\n",
    		(int) mem_state.algo_mem_use_heap,(int) mem_state.algo_mem_use_dnn_data);
    SYNTIANT_TRACE("dsp sample_mem_use_heap = %d\ndsp sample_mem_use_dnn_data = %d\n",
    		(int)mem_state.sample_mem_use_heap, (int)mem_state.sample_mem_use_dnn_data);

    s = syntiant_ndp120_read_sample_config(ndp, &cfg);
    if (s) {
        SYNTIANT_TRACE("error in reading DSP sample configuration\n");
        return s;
    }

    SYNTIANT_TRACE("max_winstep_us = %d\naud_samp_size_bytes = %d\n"
           "aud_samp_cap = %d\nfunc_samp_size_bytes = %d\n"
           "func_samp_cap = %d\n", 
		   (int) max_winstep_us, (int)cfg.aud_samp_size_bytes,(int) cfg.aud_samp_cap,
		   (int) cfg.func_samp_size_bytes,(int) cfg.func_samp_cap);

    return SYNTIANT_NDP_ERROR_NONE;
}
#endif
int syntiant_platform_init(int spi_speed,  syntiant_spi_transfer_f spi_func)
{
    int s = 0;
    int on;

    struct syntiant_platform_app_s *ndp120;
    struct syntiant_ndp_device_s *ndp = NULL;
    syntiant_ndp120_config_misc_t ndp120_config;

    ndp120 = &ndp_app;
    memset(ndp120, 0, sizeof(struct syntiant_platform_app_s));
    ndp120_default_configs_set(ndp120);
    evb_platform_iif_setup(ndp120, &iif, spi_speed, spi_func);
    SYNTIANT_TRACE("spi speed is %d,  spi_read_delay is %d\r\n",
                   ndp120->evb_io_handle.spi_speed, ndp120->evb_io_handle.spi_read_delay);

    s = syntiant_ndp_init(&ndp120->ndp, &iif, ndp120->init_mode);
    if(syntiant_platform_check_status("ndp_init", s, 1)){
        return s;
    }
    ndp = ndp120->ndp;

#ifdef ENABLE_NDP_BRINGUP_TEST
    s = do_butest(ndp120);
    if(syntiant_platform_check_status("memory test ", s, 1)){
        return s;
    }
    XNO_Printf("NDP BRINGUP TEST SUCCESS");
#endif

    /* configure clocks */
    syntiant_platform_sleep_ms(100);
    s = syntiant_ndp120_get_put_ext_clk_freq(ndp, &ndp120->input_freq);
    SYNTIANT_TRACE("input_freq %d\r\n", (int)ndp120->input_freq);
    if(syntiant_platform_check_status("set main clock", s, 1)){
        return s;
    }

    s = ndp120_config_clock(ndp120);
    if(syntiant_platform_check_status("ndp_clocks_setup", s, 1)){
        return s;
    }

    // enable interrupts
    on = SYNTIANT_NDP_INTERRUPT_DEFAULT;
    s = syntiant_ndp_interrupts(ndp, &on);
    if(syntiant_platform_check_status("interrupts", s, 1))
    {
        return s;
    }
    // Load packages firmware: MCU + DSP firmware + NN model
    /* Check NDP MCU FW available at external flash and load it to NDP */
    if(XNO_Is_NDP_Firm_Avail(NDP_MCU_HDR_START_ADDR)){
		s = XNO_Load_NDP_FW(ndp,NDP_MCU_FW_START_ADDR, ndp_payload_buf);
		if(syntiant_platform_check_status("Load NDP120 MCU FW", s, 1))
		{
			XNO_Printf("ERROR: NDP MCU LOAD FIRMWARE FAILED");
			return s;
		}else{
			XNO_Printf("NDP MCU LOAD FIRMWARE SUCCESS");
		}

    }else{
    	XNO_Printf("ERROR: NDP MCU FW NOT AVAILABLE");
    }
    /* Check NDP DSP FW available at external flash and load it to NDP */
    if(XNO_Is_NDP_Firm_Avail(NDP_DSP_HDR_START_ADDR)){
		s = XNO_Load_NDP_FW(ndp,NDP_DSP_FW_START_ADDR, ndp_payload_buf);
		if(syntiant_platform_check_status("Load NDP120 DSP FW", s, 1))
		{
			XNO_Printf("ERROR: NDP DSP LOAD FIRMWARE FAILED");
			return s;
		}else{
			XNO_Printf("NDP DSP LOAD FIRMWARE SUCCESS");
		}
    }else{
    	XNO_Printf("ERROR: NDP DSP FW NOT AVAILABLE");
    }
    /* Check NDP DNN FW available at external flash and load it to NDP */
    if(XNO_Is_NDP_Firm_Avail(NDP_DNN_HDR_START_ADDR)){
		s = XNO_Load_NDP_FW(ndp,NDP_DNN_FW_START_ADDR, ndp_payload_buf);
		if(syntiant_platform_check_status("Load NDP120 DNN FW", s, 1))
		{
			XNO_Printf("ERROR: NDP DNN LOAD FIRMWARE FAILED");
			return s;
		}else{
			XNO_Printf("NDP DNN LOAD FIRMWARE SUCCESS");
		}

    }else{
    	XNO_Printf("ERROR: NDP DNN FW NOT AVAILABLE");
    }
    /* ping DSP to reset buffers */
    syntiant_ndp120_do_mailbox_req(ndp120->ndp, NDP120_DSP_MB_H2D_PING, NULL);    

    /* configure match-per-frame state */
    memset(&ndp120_config, 0, sizeof(struct syntiant_ndp120_config_misc_s));
    ndp120_config.set = NDP120_CONFIG_SET_MISC_MATCH_PER_FRAME_ON;
    ndp120_config.match_per_frame_on = ndp120->match_per_frame;
    s = syntiant_ndp120_config_misc(ndp, &ndp120_config);
    if (syntiant_platform_check_status("ndp120_config match per frame set", s, 0)) {
        return s;
    }
#ifdef DUMP_FIRMWARE_INFO
    /* Dump firmware status information */
    get_config(ndp120);
#endif

#ifdef SET_MAX_TANK_SIZE
    /* set the maximum tank size  */
    ndp120->pcm_tank_size_in_msec = 0;
    s = do_dsp_tank_size_config(ndp120);
    if (syntiant_platform_check_status("dsp tank size", s, 0)) {
        return s;
    }
#endif

    s = do_watch(ndp120, ndp120->en_far_field, ndp120->en_aec); 
    if(s){
        return s;
    }

    ndp120->initialized = 1;
    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_platform_extract_wakeword(unsigned char *buffer, unsigned char *wav_hdr, int size)
{
    int s;  
    struct syntiant_platform_app_s *ndp120;
    ndp120 = &ndp_app;

    if(!ndp120->initialized){
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    s = syntiant_platform_audio_ww(ndp120, buffer, size);
    if(syntiant_platform_check_status("Record wakeword", s, 1)){
        return s;
    }

    syntiant_platform_audio_wav(2, size, wav_hdr);
    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_platform_extract_command(unsigned char *buffer, unsigned char *wav_hdr, int size)
{
    int s;  
    struct syntiant_platform_app_s *ndp120;
    ndp120 = &ndp_app;

    if(!ndp120->initialized){
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    s = syntiant_platform_audio_command(ndp120, buffer, size);
    if(syntiant_platform_check_status("Record commands", s, 1)){
        return s;
    }

    syntiant_platform_audio_wav(2, size, wav_hdr);
    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_platform_record_audio(unsigned char *buffer, unsigned char *wav_hdr, int size)
{
    int s;  
    struct syntiant_platform_app_s *ndp120;
    ndp120 = &ndp_app;

    if(!ndp120->initialized){
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    s = syntiant_platform_audio_record(ndp120, buffer, size);
    if(syntiant_platform_check_status("Record audio", s, 1)){
        return s;
    }
#if 0
    syntiant_platform_audio_wav(2, size, wav_hdr);
#endif
    return SYNTIANT_NDP_ERROR_NONE;
}

