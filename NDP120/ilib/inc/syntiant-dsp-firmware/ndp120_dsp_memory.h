
/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2018-2021 Syntiant Corporation
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
#ifndef NDP120_DSP_MEMORY_H_
#define NDP120_DSP_MEMORY_H_

#ifndef __KERNEL__
#include <stdint.h>
#include <stddef.h>
#endif

typedef enum {
    NDP120_DSP_MEM_TYPE_HEAP = 0,
    NDP120_DSP_MEM_TYPE_DNN_DATA = 1
} ndp120_dsp_mem_type_t;

typedef struct {
    uint32_t size;
    uint8_t pattern[12];
} ndp120_dsp_memory_block_header_t;

/* Round up to multiple of 16 */
#define NDP120_DSP_MEM_ALLOC_TRUE_SIZE(req_size) ((req_size + 15) / 16 * 16 + sizeof(ndp120_dsp_memory_block_header_t)) 

typedef struct {
    uint32_t mem_heap_total;
    uint32_t mem_dnn_data_total;

    uint32_t algo_mem_use_heap;
    uint32_t algo_mem_use_dnn_data;

    uint32_t sample_mem_use_heap;
    uint32_t sample_mem_use_dnn_data;

    uint32_t mem_heap_base;
    uint32_t mem_dnn_data_base;
} ndp120_dsp_memory_state_t;

#endif
