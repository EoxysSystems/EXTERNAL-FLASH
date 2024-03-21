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

#ifndef _SYNTIANT_PLATFORM_H_
#define _SYNTIANT_PLATFORM_H_

#include <syntiant_ndp_error.h>
#include <syntiant_ndp_driver.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENABLE_NDP_BRINGUP_TEST
#define ENABLE_NDP_MEMORY_TEST

// #define DUMP_FIRMWARE_INFO
// #define SET_MAX_TANK_SIZE
#define DUMP_DSP_FLOW

#define APP_TYPE_NEAR_FIELD

#ifdef  APP_TYPE_FAR_FIELD 
#define ENABLE_FAR_FIELD        1
#define ENABLE_AEC              0
#else
#define ENABLE_FAR_FIELD        0
#define ENABLE_AEC              1
#endif

/* assuming little endian, and structure with no padding */
struct wav_header_s {
    char riff[4];
    uint32_t file_size;
    char wave[4];
    char fmt[4];
    uint32_t fmt_size;
    uint16_t type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t bytes_per_second;
    uint16_t bytes_per_frame;
    uint16_t bits_per_sample;
    char data[4];
    uint32_t data_size;
};

/**
 * @brief exchange bytes with NDP device integration interface
 *
 * @param out bytes to send, or NULL if '0' bytes should be sent
 * @param in bytes to receive, or NULL if received bytes should be ignored
 * @param count number of bytes to exchange
 * @return a @c SYNTIANT_NDP_ERROR_* code
 */
typedef int (*syntiant_spi_transfer_f)(void *out, void *in, unsigned int count);

/* structure to hold spi integated interfaces */
struct syntiant_platform_evb_io_s {
    int spi_speed;
    int spi_read_delay;
    syntiant_spi_transfer_f spi_txrx;
};

struct syntiant_platform_app_s {
    int init_mode;
    uint8_t device_type;
    uint8_t clock_option;
    uint8_t en_far_field;
    uint8_t en_aec;

    int agc_shift_dir;
    int agc_shift_cnt;
    int pdm_freq;
    uint32_t input_freq;
    int dnn_input;
    int pdm_ndp;
    int pdm_in_shift;
    int pdm_out_shift;

    int aud2_src_type;
    int aud2_src_param;
    int aud2_enable;

    int aud2_out_mode;

    int i2s_framesize;
    int i2s_samplesize;
    int i2s_msb;
    int i2s_right_enable;
    int i2s_left_enable;
    int i2s_mode;
    int i2s_pack;
    int i2s_negedgeenable;
    int delayed_flop_sensitivity;

    /* int i2s_numdelay; */
    int i2s_zc;
    int i2s_shift_dir;
    int i2s_shift_cnt;

    int audio_sync_enable;
    int audio_sync_ref_chan;
    int audio_sync_adj_chan;
    int audio_sync_offset;

    /* dsp tank size related */
    int32_t pcm_tank_size_in_msec;
    uint32_t aud_samp_size_bytes;   // bytes of sample frame (30ms, or 24ms......)
    uint32_t func_samp_size_bytes;
    uint32_t aud_samp_cap;
    uint32_t func_samp_cap;
    int use_xtal;
    int match_per_frame;
    int initialized;
    int pcm_sample_bytes;          // should be 32 bytes per sample
    struct syntiant_platform_evb_io_s evb_io_handle;
    struct syntiant_ndp_device_s *ndp;
};
extern char payload_data_buf[100];
extern FlagStatus start_wifi;
extern void NDP_Demo();
extern int syntiant_platform_init(int spi_speed,  syntiant_spi_transfer_f spi_func);
extern int syntiant_platform_trigger_handler(int *class_index, uint32_t count);
extern int syntiant_platform_poll();
extern int syntiant_platform_extract_wakeword(unsigned char *buffer, unsigned char *wav_hdr, int size);
extern int syntiant_platform_extract_command(unsigned char *buffer, unsigned char *wav_hdr, int size);
extern int syntiant_platform_record_audio(unsigned char *buffer, unsigned char *wav_hdr, int size);
#ifdef __cplusplus
}
#endif

#endif //_SYNTIANT_PLATFORM_H_
