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

#ifndef _SYNTIANT_PLATFORM_AFE_H_
#define _SYNTIANT_PLATFORM_AFE_H_

#include "syntiant_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int syntiant_platform_i2s_setup(struct syntiant_platform_app_s *ndp120, int far_field_enable, int aec_enable);
extern int syntiant_platform_afe_setup(struct syntiant_platform_app_s *ndp120, int far_field_enable, int aec_enable);

#ifdef __cplusplus
}
#endif

#endif //_SYNTIANT_PLATFORM_AFE_H_
