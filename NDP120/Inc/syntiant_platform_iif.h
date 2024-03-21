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

#ifndef _SYNTIANT_PLATFORM_IIF_H_
#define _SYNTIANT_PLATFORM_IIF_H_

#include "syntiant_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 1
#define SYNTIANT_TRACE(...)      do {printf(__VA_ARGS__);fflush(stdout);}while(0)
#else
#define SYNTIANT_TRACE(...)
#endif

extern void syntiant_platform_sleep_ms(int ms);
extern int evb_platform_transfer(void *d, int mcu, uint32_t addr, 
                                 void *out, void *in, unsigned int count);
extern int evb_platform_iif_setup(void *d, struct syntiant_ndp_integration_interfaces_s *iif, 
                                  int spi_speed,  syntiant_spi_transfer_f spi_func);
extern int syntiant_platform_check_status(char *message, int s, int do_exit);

#ifdef __cplusplus
}
#endif

#endif //_SYNTIANT_PLATFORM_IIF_H_
