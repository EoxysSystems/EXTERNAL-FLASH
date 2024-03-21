#ifndef __SYNTIANT_PLATFORM_AUDIO_H_
#define __SYNTIANT_PLATFORM_AUDIO_H_

#include "syntiant_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void syntiant_platform_audio_wav(int sample_bytes, int total_len, unsigned char *wav_hdr);
extern int syntiant_platform_audio_ww(struct syntiant_platform_app_s *ndp120, unsigned char *buffer, int size);
extern int syntiant_platform_audio_command(struct syntiant_platform_app_s *ndp120, unsigned char *buffer, int size);
extern int syntiant_platform_audio_record(struct syntiant_platform_app_s *ndp120, unsigned char *buffer, int size);                                 

#ifdef __cplusplus
}
#endif

#endif  /* __SYNTIANT_PLATFORM_AUDIO_H_ */