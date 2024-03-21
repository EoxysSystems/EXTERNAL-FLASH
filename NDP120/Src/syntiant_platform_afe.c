#include <assert.h>
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

#include "syntiant_platform.h"
#include "syntiant_platform_iif.h"
#include "syntiant_platform_afe.h"

static int do_ndp120_flow_setup(struct syntiant_platform_app_s *ndp120, 
                                syntiant_ndp120_flow_setup_t *flow_setup)
{
    int s = 0;

    switch (flow_setup->action) {
        case NDP120_FLOW_SETUP_RESET:
            s = syntiant_ndp120_dsp_flow_setup_reset(&flow_setup->flow_setup);
            break;
        case NDP120_FLOW_SETUP_ADD_RULE:
            s = syntiant_ndp120_dsp_flow_setup_add_rule(&flow_setup->flow_setup,
                &flow_setup->rule, flow_setup->src_type);
            break;
        case NDP120_FLOW_SETUP_APPLY:
            s = syntiant_ndp120_dsp_flow_setup_apply(ndp120->ndp,
                &flow_setup->flow_setup);
            break;
    }
    syntiant_platform_check_status("ndp120_flow_setup", s, 1);

    return s;
}

static int do_flow_pcm_func(struct syntiant_platform_app_s *ndp120,
                 syntiant_ndp120_flow_setup_t *flow_setup,
                 uint32_t src_param, uint32_t func_no)
{
    int s = 0;

    flow_setup->src_type = NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO;
    flow_setup->rule.set_id    = 0;
    flow_setup->rule.src_param = src_param; /* PDM 0 */
    flow_setup->rule.dst_param = func_no; /* func 0 */
    flow_setup->rule.dst_type  = NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION;
    flow_setup->rule.algo_config_index = -1;

    flow_setup->action = NDP120_FLOW_SETUP_ADD_RULE;
    s = do_ndp120_flow_setup(ndp120, flow_setup);
    if (s) {
        SYNTIANT_TRACE("%s: %d: ret %d\n", __func__, __LINE__, s);
        goto out;
    }

out:
    return s;
}

static int do_flow_func_nn(struct syntiant_platform_app_s *ndp120,
                syntiant_ndp120_flow_setup_t *flow_setup,
                uint32_t func_no)
{
    int s = 0;

    flow_setup->src_type = NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION;
    flow_setup->rule.set_id    = 0;
    flow_setup->rule.src_param = func_no; /* func 0 */
    flow_setup->rule.dst_param = 0;
    flow_setup->rule.dst_type  = NDP120_DSP_DATA_FLOW_DST_TYPE_NN;
    flow_setup->rule.algo_config_index = -1;

    flow_setup->action = NDP120_FLOW_SETUP_ADD_RULE;
    s = do_ndp120_flow_setup(ndp120, flow_setup);
    if (s) {
        SYNTIANT_TRACE("%s: %d: ret %d\n", __func__, __LINE__, s);
        goto out;
    }

out:
    return s;
}

static int do_flow_nn_mcu(struct syntiant_platform_app_s *ndp120,
               syntiant_ndp120_flow_setup_t *flow_setup,
               uint32_t nn_num)
{
    int s = 0;

    flow_setup->src_type = NDP120_DSP_DATA_FLOW_SRC_TYPE_NN;
    flow_setup->rule.set_id    = 0;
    flow_setup->rule.src_param = nn_num; /* nn 0 */
    flow_setup->rule.dst_param = 0;
    flow_setup->rule.dst_type  = NDP120_DSP_DATA_FLOW_DST_TYPE_MCU;
    flow_setup->rule.algo_config_index = -1;

    flow_setup->action = NDP120_FLOW_SETUP_ADD_RULE;
    s = do_ndp120_flow_setup(ndp120, flow_setup);
    if (s) {
        SYNTIANT_TRACE("%s: %d: ret %d\n", __func__, __LINE__, s);
        goto out;
    }
out:
    return s;
}

static int do_flow_src_host(struct syntiant_platform_app_s *ndp120,
                 syntiant_ndp120_flow_setup_t *flow_setup,
                 uint8_t src_type, uint8_t src_param,
                 int8_t algo_index, uint8_t dst_param)
{
    int s = 0;

    flow_setup->src_type = src_type;
    flow_setup->rule.set_id    = 0;
    flow_setup->rule.src_param = src_param; /* PDM 0,1, func<N> */
    flow_setup->rule.dst_param = dst_param;
    flow_setup->rule.dst_type  = NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT;
    flow_setup->rule.algo_config_index = algo_index;

    flow_setup->action = NDP120_FLOW_SETUP_ADD_RULE;
    s = do_ndp120_flow_setup(ndp120, flow_setup);
    if (s) {
        SYNTIANT_TRACE("%s: %d: ret %d\n", __func__, __LINE__, s);
        goto out;
    }
out:
    return s;
}

static int do_dsp_flow_setup(struct syntiant_platform_app_s *ndp120, int far_field_enable, int aec_enable)
{
    int s;
    uint32_t input_chnl_map;
    syntiant_ndp120_flow_setup_t driver_flow_setup = {0};

    /* config dsp flow by clearing existing flow and setting up new flow */
    driver_flow_setup.action = NDP120_FLOW_SETUP_RESET;
    s = do_ndp120_flow_setup(ndp120, &driver_flow_setup);
    if (s) {
        SYNTIANT_TRACE("%s: %d: ret %d\n", __func__, __LINE__, s);
        goto out;
    }

    if (far_field_enable && aec_enable) {
        input_chnl_map = 0xf; /* aud0: pdm0, 1; aud1: i2s 0, i2s 1 */
#if 1        
        /* add rule: pcmx:func1 */
        s = do_flow_pcm_func(ndp120, &driver_flow_setup, input_chnl_map, 1);
        if (s) {
            SYNTIANT_TRACE("Can't configure pcm%d-->func1, err:%d\n", (int)input_chnl_map, s);
            goto out;
        } 

        /* add rule: func1:nn0 */
        s = do_flow_func_nn(ndp120, &driver_flow_setup, 1);
        if (s) {
            SYNTIANT_TRACE("Can't set flow rule func1-->nn0, err:%d\n", s);
            goto out;
        }        
#endif           
    } else if (far_field_enable) {
        input_chnl_map = 0x3; /* pdm0, 1 */

        /* add rule: pcmx:func10 */
        s = do_flow_pcm_func(ndp120, &driver_flow_setup, input_chnl_map, 10);
        if (s) {
            SYNTIANT_TRACE("Can't configure pcm%d-->func1, err:%d\n", (int)input_chnl_map, s);
            goto out;
        }

        /* add rule: func10:nn0 */
        s = do_flow_func_nn(ndp120, &driver_flow_setup, 1);
        if (s) {
            SYNTIANT_TRACE("Can't set flow rule func1-->nn0, err:%d\n", s);
            goto out;
        }                           
    } else {
        input_chnl_map = 0x1; /* pdm0 */
        
        /* add rule: pcm0:func0 */
        s = do_flow_pcm_func(ndp120, &driver_flow_setup, input_chnl_map, 0);
        if (s) {
            SYNTIANT_TRACE("Can't configure pcm%d-->func1, err:%d\n", (int)input_chnl_map, s);
            goto out;
        }   

        /* add rule: func0:nn0 */
        s = do_flow_func_nn(ndp120, &driver_flow_setup, 0);
        if (s) {
            SYNTIANT_TRACE("Can't set flow rule func1-->nn0, err:%d\n", s);
            goto out;
        }            
    }

    /* add rule: nn0:mcu */
    s = do_flow_nn_mcu(ndp120, &driver_flow_setup, 0);
    if (s) {
        SYNTIANT_TRACE("Can't set flow rule nn0-->mcu, err:%d\n", s);
        goto out;
    }

    /* Below rules are used for recording audio */
    if (far_field_enable && aec_enable) {
        /* PCM3 -> host_ext_audio */
        s = do_flow_src_host(ndp120, &driver_flow_setup,
                             NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
                             3, -1,
                             NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO);
        if (s) {
            SYNTIANT_TRACE("Couldn't configure flows for pcm3-->host_ext_audio, error:%d\n", s);
            goto out;
        }
        /* PCM12 -> host_ext_audio */
        s = do_flow_src_host(ndp120, &driver_flow_setup,
                             NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
                             12, -1,
                             NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO);
        if (s) {
            SYNTIANT_TRACE("Couldn't configure flows for pcm12-->host_ext_audio, error:%d\n", s);
            goto out;
        }
#if 1
        /* add rule: func1:host_ext_audio */
        s = do_flow_src_host(ndp120, &driver_flow_setup,
                             NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION, 1, -1,
                             NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO);
        if (s) {
            SYNTIANT_TRACE("Can't configure func1-->host_ext_audio, err:%d\n", s);
            goto out;
        }
#endif
    } else if(far_field_enable) {
        /* PCM3 -> host_ext_audio */
        s = do_flow_src_host(ndp120, &driver_flow_setup,
                             NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
                             3, -1,
                             NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO);
        if (s) {
            SYNTIANT_TRACE("Couldn't configure flows for pcm3-->host_ext_audio, error:%d\n", s);
            goto out;
        }

        /* add rule: func10:host_ext_audio */
        s = do_flow_src_host(ndp120, &driver_flow_setup,
                             NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION, 10, -1,
                             NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO);
        if (s) {
            SYNTIANT_TRACE("Can't configure func10-->host_ext_audio, err:%d\n", s);
            goto out;
        }
    } else {
        /* PCM1 -> host_ext_audio */
        s = do_flow_src_host(ndp120, &driver_flow_setup,
                             NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
                             1, -1,
                             NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO);
        if (s) {
            SYNTIANT_TRACE("Couldn't configure flows for pcm1-->host_ext_audio, error:%d\n", s);
            goto out;
        }

#if 0  // Don't need this now 
       /* add rule: func0:host_ext_audio */
        s = do_flow_src_host(ndp120, &driver_flow_setup,
                             NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION, 1, -1,
                             NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO);
        if (s) {
            SYNTIANT_TRACE("Can't configure func10-->host_ext_audio, err:%d\n", s);
            goto out;
        }   
#endif              
    }

    /* save rules in DSP memory */
    driver_flow_setup.action = NDP120_FLOW_SETUP_APPLY;
    s = do_ndp120_flow_setup(ndp120, &driver_flow_setup);
    if (s) {
        SYNTIANT_TRACE("%s: %d: ret %d\n", __func__, __LINE__, s);
        goto out;
    }

    /* DSP ping */
    s = syntiant_ndp120_do_mailbox_req(ndp120->ndp, NDP120_DSP_MB_H2D_PING, NULL);
    if (s) {
        SYNTIANT_TRACE("error in sending DSP ping: %d\n", s);
        goto out;
    }

#ifdef DUMP_DSP_FLOW
    ndp120_dsp_data_flow_setup_t *flow_setup = &driver_flow_setup.flow_setup;
    syntiant_ndp120_dsp_flow_setup_get(ndp120->ndp, flow_setup);
    syntiant_ndp120_dsp_flow_setup_dump(flow_setup);    
#endif 

out:
    return s;
}


static int ndp120_config_decimation(struct syntiant_platform_app_s *ndp120, 
        int mic, int pdm_in_shift, int pdm_out_shift)
{
    int s = 0;
    syntiant_ndp120_config_decimation_t config_decimation =
        {0, NDP120_CONFIG_SET_DECIMATION_INSHIFT |
            NDP120_CONFIG_SET_DECIMATION_OUTSHIFT,
            mic, pdm_in_shift, pdm_out_shift};

    s = syntiant_ndp120_config_decimation(ndp120->ndp, &config_decimation);

    return s;
}

static int ndp120_config_gain(struct syntiant_platform_app_s *ndp120, int mic, int agc_shift_dir,
        int shift_dir_val, int agc_shift_cnt, int dcremoval, int fine_grain_mul, int zc)
{
    int s = 0;
    syntiant_ndp120_config_gain_t config_gain;

    memset(&config_gain, 0, sizeof(config_gain));

    config_gain.set = NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT;
    config_gain.agcshiftcnt = agc_shift_cnt;

    if (dcremoval) {
        config_gain.set |= NDP120_CONFIG_SET_GAIN_DCREMOVALMODE;
        config_gain.dcremovalmode = NDP120_CONFIG_VALUE_GAIN_DCREMOVALMODE_ON;
    }
    if (agc_shift_dir) {
        config_gain.set |= NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR;
        config_gain.agcshiftdir = shift_dir_val;
    }
    if (fine_grain_mul) {
        config_gain.set |= NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL;
        config_gain.agcfinegrainmul = fine_grain_mul;
    }
    /* apply only when zero change is disabled, HW default is enabled mode */
    if (!zc) {
        config_gain.set |= NDP120_CONFIG_SET_GAIN_ZCGAINCHANGE;
        config_gain.zcgainchange = zc;
    }
    config_gain.mic = mic;

    s = syntiant_ndp120_config_gain(ndp120->ndp, &config_gain);

    return s;
}

static int do_pdm_clock_config(struct syntiant_platform_app_s *ndp120, int aud_intf, int freq,
        int mode, int clock_mode)
{
    int s = 0;
    syntiant_ndp120_config_pdm_t *ndp120_pdm_config, config_pdm;

    memset(&config_pdm, 0, sizeof(config_pdm));
    ndp120_pdm_config = &config_pdm;
    ndp120_pdm_config->get = 0;
    ndp120_pdm_config->interface = aud_intf;
    ndp120_pdm_config->pdm_rate = freq;
    ndp120_pdm_config->sample_rate = 16000;
    ndp120_pdm_config->clk_mode = clock_mode;
    ndp120_pdm_config->mode = mode;
    ndp120_pdm_config->set = NDP120_CONFIG_SET_PDM_PDM_RATE |
                             NDP120_CONFIG_SET_PDM_SAMPLE_RATE |
                             NDP120_CONFIG_SET_PDM_CLK_MODE |
                             NDP120_CONFIG_SET_PDM_MODE;

    s = syntiant_ndp120_config_pdm(ndp120->ndp, ndp120_pdm_config);
    syntiant_platform_check_status("120 config pdm set", s, 1);

    return s;
}

static int do_pdm_channels_setup(struct syntiant_platform_app_s *ndp120, int far_field_enable)
{
    int s = 0;

   /* configure decimation: mic 0 */
    s = ndp120_config_decimation(ndp120, 0, ndp120->pdm_in_shift, ndp120->pdm_out_shift);
    if (s) {
        SYNTIANT_TRACE("configure mic 0 decimation, err:%d\r\n", s);
        goto out;
    }                                 

    /* configure gain: mic 0 */
//    s = ndp120_config_gain(ndp120, 0, 1, NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT, 0, 1, 0x196b, 1);
    s = ndp120_config_gain(ndp120, 0, 1, NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT, 0, 1, 0x2100, 1);
    if (s) {
        SYNTIANT_TRACE("configure mic 0 gain, err: %d\r\n", s);
        goto out;
    } 

    if(far_field_enable) {
        /* printf("configure PDM aud0 mic 1 decimation\n"); */
        s = ndp120_config_decimation(ndp120, 1, ndp120->pdm_in_shift, ndp120->pdm_out_shift);
        if (s) {
            SYNTIANT_TRACE("configure mic 1 decimation, err:%d\n", s);
            goto out;
        }

        /* printf("configure PDM aud0 mic 1 gain\n"); */
        s = ndp120_config_gain(ndp120, 1, 1, NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT, 0, 1, 0x2000, 1);
        if (s) {
            SYNTIANT_TRACE("configure mic 1 gain, err: %d\r\n", s);
            goto out;
        }

        s = do_pdm_clock_config(ndp120, 0, ndp120->pdm_freq,
                                NDP120_CONFIG_VALUE_PDM_MODE_STEREO,
                                NDP120_CONFIG_VALUE_PDM_CLK_MODE_DUAL_INTERNAL);
        if(s) {
            SYNTIANT_TRACE("pdm clock configure, err:%d\n", s);
            goto out;
        }
    } else {
        s = do_pdm_clock_config(ndp120, 0, ndp120->pdm_freq,
//                                NDP120_CONFIG_VALUE_PDM_MODE_LEFT,
								NDP120_CONFIG_VALUE_PDM_MODE_STEREO,
                                NDP120_CONFIG_VALUE_PDM_CLK_MODE_DUAL_INTERNAL);
//        						NDP120_CONFIG_VALUE_PDM_CLK_MODE_INTERNAL);
        if(s) {
            SYNTIANT_TRACE("pdm clock configure, err:%d\n", s);
            goto out;
        }
    }

out:
    return s;
}

//static int do_i2s_channels_setup(struct syntiant_platform_app_s *ndp120, int far_field_enable)
//{
//    int s = 0;
//    if(far_field_enable){
//		/* configure gain: mic 0 */
//		s = ndp120_config_gain(ndp120, 0, ndp120->i2s_shift_dir, NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT,
//							   ndp120->i2s_shift_cnt, 1, 0x196b, ndp120->i2s_zc);
//		if (s) {
//			SYNTIANT_TRACE("configure mic 0 gain, err: %d\r\n", s);
//			goto out;
//		}
//    }
//out:
//    return s;
//}

static int do_audio_sync(struct syntiant_platform_app_s *ndp120)
{
    int s = 0;
    ndp120_dsp_audio_sync_config_t cfg;

    memset(&cfg, 0, sizeof(cfg));

    cfg.mode = NDP120_DSP_AUDIO_SYNC_MODE_ALIGN;
    cfg.ref_chan = (ndp120_dsp_audio_sync_channel_t) ndp120->audio_sync_ref_chan;
    cfg.adj_chan = (ndp120_dsp_audio_sync_channel_t) ndp120->audio_sync_adj_chan;
    cfg.sample_count_offset = ndp120->audio_sync_offset;

    s = syntiant_ndp120_write_audio_sync_config(ndp120->ndp, &cfg);
    if (s) {
        SYNTIANT_TRACE("error in config for aud2 sync: %d\n", s);
    }
    return s;
}

static int do_ndp120_i2s_setup(struct syntiant_platform_app_s *ndp120, uint8_t intf)
{
    int s = 0;
    syntiant_ndp120_config_i2s_t ndp120_config_i2s, *config;

    config = &ndp120_config_i2s;

    memset(config, 0, sizeof(*config));
    config->set =
      NDP120_CONFIG_SET_I2S_MODE |
      NDP120_CONFIG_SET_I2S_FRAMESIZE |
      NDP120_CONFIG_SET_I2S_SAMPLESIZE |
      NDP120_CONFIG_SET_I2S_MSB_INDEX |
      NDP120_CONFIG_SET_I2S_PACKED |
      NDP120_CONFIG_SET_I2S_EDGE |
      NDP120_CONFIG_SET_I2S_DELAYED_FLOP_SENSITIVITY |
      NDP120_CONFIG_SET_I2S_AUDIO_OUT_FS_EXT_ENABLE |
      NDP120_CONFIG_SET_I2S_LEFTCHENABLE |
      NDP120_CONFIG_SET_I2S_RIGHTCHENABLE |
      /* NDP120_CONFIG_SET_I2S_AUD2CLKOUTNEEDED | */
      NDP120_CONFIG_SET_I2S_AUD2_OUT_MODE |
      NDP120_CONFIG_SET_I2S_AUD2_SRC_TYPE |
      NDP120_CONFIG_SET_I2S_AUD2_SRC_PARAM ;

    /* printf("applying fs: %d ss:%d edge: %d mode: %d msb: %d rch:%d "
                   "lch:%d delay: %d\n", ndp120->i2s_framesize,
            ndp120->i2s_samplesize, ndp120->i2s_negedgeenable,
            ndp120->i2s_mode, ndp120->i2s_msb, ndp120->i2s_right_enable,
            ndp120->i2s_left_enable,  ndp120->delayed_flop_sensitivity); */
    config->freq = 16000;
    config->edge = ndp120->i2s_negedgeenable;     // NDP120_CONFIG_VALUE_I2S_EDGE_POS
    config->mode = ndp120->i2s_mode;              // NDP120_CONFIG_VALUE_I2S_MODE_STANDARD
    config->framesize = ndp120->i2s_framesize;    // 16
    config->samplesize = ndp120->i2s_samplesize;  // 16
    config->msb_index = ndp120->i2s_msb;          // 30
    config->delayed_flop_sensitivity = ndp120->delayed_flop_sensitivity;   // NDP120_CONFIG_VALUE_I2S_DELAYED_FLOP_SENSITIVITY_NORMAL
    config->packed = ndp120->i2s_pack;                 // 0 : false
    config->rightchenable = ndp120->i2s_right_enable;  // 1 : enable
    config->leftchenable = ndp120->i2s_left_enable;    // 1 : enable 

    if (intf == NDP120_DSP_AUDIO_SYNC_AUD2) {
    config->aud2_out_mode = ndp120->aud2_out_mode;     // NDP120_CONFIG_VALUE_I2S_AUD2_OUT_MODE_SLAVE
    /* config->aud2clkoutneeded = 1; */
    config->audio_out_fs_ext_enable = 1;
    config->aud2_src_type = NDP120_CONFIG_VALUE_I2S_AUD2_SRC_TYPE_PCM_AUDIO;
    config->aud2_src_param = 3; /* TODO: remove hard coding */
    }
    config->interface = intf;

    s = syntiant_ndp120_config_i2s(ndp120->ndp, config);
    if (s) {
        SYNTIANT_TRACE("error in config for aud%d: %d\n", intf, s);
    }
    return s;
};


int syntiant_platform_i2s_setup(struct syntiant_platform_app_s *ndp120, int far_field_enable, int aec_enable)
{
    int s = 0;
    
    (void)aec_enable;
    if(!far_field_enable){
        goto out;  // For no-FF, no need to set the i2s
    }

    /* audio sync config */
    if (ndp120->audio_sync_enable) {
        SYNTIANT_TRACE("configure audio sync\n");
        s = do_audio_sync(ndp120);
        if (s) {
            SYNTIANT_TRACE("error in configuring audio sync: %d\n", s);
            goto out;
        }
    }

    /* DSP ping */
    s = syntiant_ndp120_do_mailbox_req(ndp120->ndp, NDP120_DSP_MB_H2D_PING, NULL);
    if (s) {
        SYNTIANT_TRACE("error in sending DSP ping:  %d\n", s);
        goto out;
    }

    /* AUD1 slave */
    s = do_ndp120_i2s_setup(ndp120, NDP120_DSP_AUDIO_SYNC_AUD1);
    if (s) {
        SYNTIANT_TRACE("error in AUD1 slave:  %d\n", s);
        goto out;
    }

out:
    return s;
}

int syntiant_platform_afe_setup(struct syntiant_platform_app_s *ndp120, int far_field_enable, int aec_enable)
{
    int s = 0;
    printf("far %d  aec %d\r\n", far_field_enable, aec_enable);
    s = do_dsp_flow_setup(ndp120, far_field_enable, aec_enable);
    if(s){
        SYNTIANT_TRACE("DSP Flow Setup, err:%d\r\n", s);
        goto out;
    }

#if 0
    s = do_i2s_channels_setup(ndp120, far_field_enable);
    if(s){
        SYNTIANT_TRACE("I2S Channels Setup, err:%d\r\n", s);
        goto out;
    }
#endif
#if 1
    s = do_pdm_channels_setup(ndp120, far_field_enable);
    if(s){
        SYNTIANT_TRACE("PDM Channels Setup, err:%d\r\n", s);
        goto out;
    }
#endif
out:
    return s;
}
