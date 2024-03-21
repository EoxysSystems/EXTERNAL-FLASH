#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <syntiant_ndp.h>
#include <syntiant_ndp120.h>
#include <ndp120_dsp_fw_state.h>
#include "syntiant_platform_audio.h"
#include "syntiant_platform_iif.h"
#include <ndp120_spi_regs.h> 

// #define DUMP_BYTES_DATA
#define DUMP_BUF_SIZE 64
#define PCM_BUF_SIZE  2048
static uint8_t pcm_data[PCM_BUF_SIZE];


#ifdef DUMP_BYTES_DATA
static void dump_data(uint8_t *buf, uint32_t len)
{
    uint32_t i, pos, remaining_bytes;
    char tmp_buf[DUMP_BUF_SIZE*2 + 1];
    SYNTIANT_TRACE("\r\n");

    remaining_bytes = len;
    pos = 0;
    while(remaining_bytes >= DUMP_BUF_SIZE)
    {
        memset(tmp_buf, 0, sizeof(tmp_buf));
        for (i = 0; i < DUMP_BUF_SIZE; i++)
        {
            sprintf(&tmp_buf[i*2], "%02X", buf[pos]);
            pos++;
        }
        remaining_bytes = remaining_bytes - DUMP_BUF_SIZE;
        SYNTIANT_TRACE(tmp_buf);
    }

    if (remaining_bytes > 0)
    {
        memset(tmp_buf, 0, sizeof(tmp_buf));
        for (i = 0; i < remaining_bytes; i++)
        {
            sprintf(&tmp_buf[i*2], "%02X", buf[pos]);
            pos++;
        }  
        SYNTIANT_TRACE(tmp_buf);
    }
    SYNTIANT_TRACE("\r\n");
}
#endif


void syntiant_platform_audio_wav(int sample_bytes, int total_len, unsigned char *wav_hdr)
{
    struct wav_header_s tmp_wav_hdr;

    tmp_wav_hdr.riff[0] = 'R';
    tmp_wav_hdr.riff[1] = 'I';
    tmp_wav_hdr.riff[2] = 'F';
    tmp_wav_hdr.riff[3] = 'F';
    tmp_wav_hdr.file_size = 36 + total_len;
    tmp_wav_hdr.wave[0] = 'W';
    tmp_wav_hdr.wave[1] = 'A';
    tmp_wav_hdr.wave[2] = 'V';
    tmp_wav_hdr.wave[3] = 'E';
    tmp_wav_hdr.fmt[0] = 'f';
    tmp_wav_hdr.fmt[1] = 'm';
    tmp_wav_hdr.fmt[2] = 't';
    tmp_wav_hdr.fmt[3] = ' ';
    tmp_wav_hdr.fmt_size = 16;
    tmp_wav_hdr.type = 1;
    tmp_wav_hdr.channels = 1;
    tmp_wav_hdr.sample_rate = 16000;
    tmp_wav_hdr.bytes_per_second = 16000 * 1 * sample_bytes;
    tmp_wav_hdr.bytes_per_frame = 1 * sample_bytes;
    tmp_wav_hdr.bits_per_sample = sample_bytes * 8;
    tmp_wav_hdr.data[0] = 'd';
    tmp_wav_hdr.data[1] = 'a';
    tmp_wav_hdr.data[2] = 't';
    tmp_wav_hdr.data[3] = 'a';
    tmp_wav_hdr.data_size = total_len;
    
    if(NULL != wav_hdr){
        memcpy(wav_hdr, (unsigned char *)&tmp_wav_hdr, sizeof(tmp_wav_hdr));
    }

#ifdef DUMP_BYTES_DATA
    SYNTIANT_TRACE("\r\n==== WAV Header ====\r\n");
    dump_data((uint8_t *)&tmp_wav_hdr, sizeof(tmp_wav_hdr));
#endif

    return;
}

static int get_audio_info(struct syntiant_platform_app_s *ndp120)
{
    int s;
    uint32_t adx; 
    unsigned int len;   
    syntiant_ndp120_config_misc_t ndp120_config;

    ndp120_config.set = 0;
    ndp120_config.set1 = 0;
    ndp120_config.get = 0;
    s = syntiant_ndp120_config_misc(ndp120->ndp, &ndp120_config);
    if (syntiant_platform_check_status("syntiant_ndp120_config", s, 1)){
        return s;
    }
    ndp120->pcm_sample_bytes = ndp120_config.audio_sample_size_bytes;

    /* get size of sample */
    adx = ndp120->ndp->d.ndp120.dsp_fw_state_addr +
        (uint32_t)offsetof(ndp120_dsp_fw_base_t, config.aud_samp_size_bytes);
    s = syntiant_ndp_read_block(ndp120->ndp, 1, adx, &len, sizeof(len));
    if (syntiant_platform_check_status("record fetching sample size\n", s, 1)){
        return s;
    }
    ndp120->aud_samp_size_bytes = len;

    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_platform_audio_ww(struct syntiant_platform_app_s *ndp120, unsigned char *buffer, int size)
{
    int s;
    int sample_bytes = 0;
    uint32_t saved_sample_size;
    unsigned int len, total_len, pre_roll;
    ndp120_dsp_audio_sample_annotation_t *annotation;

    s = get_audio_info(ndp120);
    if (syntiant_platform_check_status("get audio info", s, 1)){
        return s;
    }

    if (!ndp120->pcm_sample_bytes) {
        SYNTIANT_TRACE("holding tank not enabled -- can not record\n");
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    /* add annotation size since annotation is enabled */
    saved_sample_size = ndp120->aud_samp_size_bytes + sizeof(ndp120_dsp_audio_sample_annotation_t);
    if(saved_sample_size > PCM_BUF_SIZE){
        SYNTIANT_TRACE("pcm buffer is too small, please set it as %d\r\n", (int)saved_sample_size);
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    SYNTIANT_TRACE("Recording %d bytes(Sample_bytes = %d)\r\n", size, sample_bytes);
    total_len = 0;

    pre_roll = 28 * saved_sample_size;  // 900ms
    s = syntiant_ndp_extract_data(ndp120->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED,
                                       SYNTIANT_NDP_EXTRACT_FROM_NEWEST, NULL, &pre_roll);    
    if(syntiant_platform_check_status("Set wake word pre-roll", s, 1)){
        return s;
    }

    while(total_len < size){
        len = saved_sample_size;
        s = syntiant_ndp_extract_data(ndp120->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED,
                                                   SYNTIANT_NDP_EXTRACT_FROM_UNREAD, pcm_data, &len);
        if (s == SYNTIANT_NDP_ERROR_DATA_REREAD) {
            continue;
        } 
        if (syntiant_platform_check_status("syntiant_ndp_extract_data", s, 1)){
            return SYNTIANT_NDP_ERROR_FAIL;
        }

        if (len > sizeof(ndp120_dsp_audio_sample_annotation_t)){
            len -= sizeof(ndp120_dsp_audio_sample_annotation_t); 
        }
        
        annotation = (ndp120_dsp_audio_sample_annotation_t *)(pcm_data + len);
        len = sizeof(pcm_data) < len ? sizeof(pcm_data) : len;
        if(len && (annotation->src_type == NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO)){
            if(total_len + len >size){
                len = size - total_len;
            }
#ifdef DUMP_BYTES_DATA
            dump_data((uint8_t *)pcm_data, len);
#endif
            if(NULL != buffer){
                memcpy((char *)(buffer + total_len), pcm_data, len);
            }
            total_len += len;
        }
    }

    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_platform_audio_command(struct syntiant_platform_app_s *ndp120, unsigned char *buffer, int size)
{
    int s;
    int sample_bytes = 0;
    uint32_t saved_sample_size;
    unsigned int len, total_len;//, pre_roll;
    ndp120_dsp_audio_sample_annotation_t *annotation;

    s = get_audio_info(ndp120);
    if (syntiant_platform_check_status("get audio info", s, 1)){
        return s;
    }

    if (!ndp120->pcm_sample_bytes) {
        SYNTIANT_TRACE("holding tank not enabled -- can not record\n");
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    /* add annotation size since annotation is enabled */
    saved_sample_size = ndp120->aud_samp_size_bytes + sizeof(ndp120_dsp_audio_sample_annotation_t);
    if(saved_sample_size > PCM_BUF_SIZE){
        SYNTIANT_TRACE("pcm buffer is too small, please set it as %d\r\n", (int)saved_sample_size);
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    SYNTIANT_TRACE("Recording %d bytes(Sample_bytes = %d)\r\n", size, sample_bytes);
    total_len = 0;

    // Read first sample frame
    len = saved_sample_size;
    s = syntiant_ndp_extract_data(ndp120->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED,
                                       SYNTIANT_NDP_EXTRACT_FROM_NEWEST, pcm_data, &len);    
    if(syntiant_platform_check_status("Extract match first package", s, 1)){
        return s;
    }
    if (len > sizeof(ndp120_dsp_audio_sample_annotation_t)){
        len -= sizeof(ndp120_dsp_audio_sample_annotation_t); 
    }
    annotation = (ndp120_dsp_audio_sample_annotation_t *)(pcm_data + len);
    len = sizeof(pcm_data) < len ? sizeof(pcm_data) : len;
    if(len && (annotation->src_type == NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO)){
#ifdef DUMP_BYTES_DATA
        dump_data((uint8_t *)pcm_data, len);
#endif
        if(NULL != buffer){
            memcpy((char *)(buffer + total_len), pcm_data, len);
        }
        total_len += len;
    } else {
        SYNTIANT_TRACE("Match first package type error\r\n");
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    while(total_len < size){
        len = saved_sample_size;
        s = syntiant_ndp_extract_data(ndp120->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED,
                                                   SYNTIANT_NDP_EXTRACT_FROM_UNREAD, pcm_data, &len);
        if (s == SYNTIANT_NDP_ERROR_DATA_REREAD) {
            continue;
        } 
        if (syntiant_platform_check_status("syntiant_ndp_extract_data", s, 1)){
            return SYNTIANT_NDP_ERROR_FAIL;
        }

        if (len > sizeof(ndp120_dsp_audio_sample_annotation_t)){
            len -= sizeof(ndp120_dsp_audio_sample_annotation_t); 
        }
        
        annotation = (ndp120_dsp_audio_sample_annotation_t *)(pcm_data + len);
        len = sizeof(pcm_data) < len ? sizeof(pcm_data) : len;
        if(len && (annotation->src_type == NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO)){
            if(total_len + len >size){
                len = size - total_len;
            }
#ifdef DUMP_BYTES_DATA
            dump_data((uint8_t *)pcm_data, len);
#endif
            if(NULL != buffer){
                memcpy((char *)(buffer + total_len), pcm_data, len);
            }
            total_len += len;
        }
    }

    return SYNTIANT_NDP_ERROR_NONE;
}

int syntiant_platform_audio_record(struct syntiant_platform_app_s *ndp120, unsigned char *buffer, int size)
{
    int s;
    int sample_bytes = 0;
    uint32_t saved_sample_size;
    unsigned int len, total_len;
    ndp120_dsp_audio_sample_annotation_t *annotation;

    s = get_audio_info(ndp120);
    if (syntiant_platform_check_status("get audio info", s, 1)){
        return s;
    }

    if (!ndp120->pcm_sample_bytes) {
        SYNTIANT_TRACE("holding tank not enabled -- can not record\n");
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    /* add annotation size since annotation is enabled */
    saved_sample_size = ndp120->aud_samp_size_bytes + sizeof(ndp120_dsp_audio_sample_annotation_t);  
    if(saved_sample_size > PCM_BUF_SIZE){
        SYNTIANT_TRACE("pcm buffer is too small, please set it as %d\r\n", (int)saved_sample_size);
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    SYNTIANT_TRACE("Recording %d bytes(Sample_bytes = %d)\r\n", size, sample_bytes);
    total_len = 0;

   // Read first sample frame
    len = saved_sample_size;
    s = syntiant_ndp_extract_data(ndp120->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED,
                                       SYNTIANT_NDP_EXTRACT_FROM_OLDEST, pcm_data, &len);    
    if(syntiant_platform_check_status("Extract audio first package", s, 1)){
        return s;
    }
    if (len > sizeof(ndp120_dsp_audio_sample_annotation_t)){
        len -= sizeof(ndp120_dsp_audio_sample_annotation_t); 
    }
    annotation = (ndp120_dsp_audio_sample_annotation_t *)(pcm_data + len);
    len = sizeof(pcm_data) < len ? sizeof(pcm_data) : len;
    if(len && (annotation->src_type == NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO)){
#ifdef DUMP_BYTES_DATA
        dump_data((uint8_t *)pcm_data, len);
#endif
        if(NULL != buffer){
            memcpy((char *)(buffer + total_len), pcm_data, len);
        }
        total_len += len;
    } else {
        SYNTIANT_TRACE("Audio first package type error\r\n");
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    while(total_len < size){
        len = saved_sample_size;
        s = syntiant_ndp_extract_data(ndp120->ndp,
            SYNTIANT_NDP_EXTRACT_TYPE_INPUT_ANNOTATED,
            SYNTIANT_NDP_EXTRACT_FROM_UNREAD, pcm_data, &len);
        if (s == SYNTIANT_NDP_ERROR_DATA_REREAD) {
            continue;
        } 
        if (syntiant_platform_check_status("syntiant_ndp_extract_data", s, 1)){
            return SYNTIANT_NDP_ERROR_FAIL;
        }

        if (len > sizeof(ndp120_dsp_audio_sample_annotation_t)){
            len -= sizeof(ndp120_dsp_audio_sample_annotation_t); 
        }
        
        annotation = (ndp120_dsp_audio_sample_annotation_t *)(pcm_data + len);
        len = sizeof(pcm_data) < len ? sizeof(pcm_data) : len;
        if(len && (annotation->src_type == NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO)){
            if(total_len + len >size){
                len = size - total_len;
            }
#ifdef DUMP_BYTES_DATA
            dump_data((uint8_t *)pcm_data, len);
#endif
            if(NULL != buffer){
                memcpy((char *)(buffer + total_len), pcm_data, len);
            }
            total_len += len;
        }
    }

    return SYNTIANT_NDP_ERROR_NONE;
}
