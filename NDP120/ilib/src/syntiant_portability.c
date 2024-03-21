/*
 * SYNTIANT CONFIDENTIAL
 *
 * _____________________
 *
 * Copyright (c) 2017-2020 Syntiant Corporation
 * All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains the property of
 * Syntiant Corporation and its suppliers, if any.  The intellectual and
 * technical concepts contained herein are proprietary to Syntiant Corporation
 * and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law.  Dissemination
 * of this information or reproduction of this material is strictly forbidden
 * unless prior written permission is obtained from Syntiant Corporation.
 *
 */
//#include <sys/time.h>
#include <syntiant_portability.h>

#include <main.h>


int gettime(struct timeval * tp)
{
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

//    SYSTEMTIME  system_time;
//    FILETIME    file_time;
//    uint64_t    time;

//    GetSystemTime( &system_time );
//    SystemTimeToFileTime( &system_time, &file_time );
//    time =  ((uint64_t)file_time.dwLowDateTime )      ;
//    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((5000) / 10000000L);
    tp->tv_usec = (long) (5000 * 1000);
    return 0;
}

int ERROR_PRINTF(const char * fmt, ...){
    int ret;
    va_list args;
    va_start(args, fmt);
    #ifdef __KERNEL__
    ret = vprintk(fmt, args);
    #else
    ret = vfprintf(stderr, fmt, args) + fprintf(stderr, "\n");
    #endif
    va_end(args);
    return ret;
}

#if defined(__KERNEL__)
int syntiant_get_ms_time(syntiant_ms_time *ms_time)
{
    ktime_get_ts64(ms_time);
    return 0;
}

unsigned long syntiant_get_ms_elapsed(syntiant_ms_time *ms_time)
{
    syntiant_ms_time now_time, diff_time;
    syntiant_get_ms_time(&now_time);
    diff_time = timespec64_sub(now_time, *ms_time);
    return (unsigned long)diff_time.tv_sec * 1000 +
           (unsigned long)diff_time.tv_nsec / 1000000;
}

#elif defined(__linux__)
int syntiant_get_ms_time(syntiant_ms_time *ms_time)
{
    return gettimeofday(ms_time, NULL);
}

unsigned long syntiant_get_ms_elapsed(syntiant_ms_time *ms_time)
{
    syntiant_ms_time now_time, diff_time;

    syntiant_get_ms_time(&now_time);
    timersub(&now_time, ms_time, &diff_time);

    return (unsigned long)diff_time.tv_sec * 1000 +
           (unsigned long)diff_time.tv_usec / 1000;
}

#else
/* customer platform implementation here */
int syntiant_get_ms_time(syntiant_ms_time *ms_time)
{
	return gettime(ms_time);
}

unsigned long syntiant_get_ms_elapsed(syntiant_ms_time *ms_time)
{
    syntiant_ms_time now_time, diff_time;

    syntiant_get_ms_time(&now_time);
    timersub(&now_time, ms_time, &diff_time);

    return (unsigned long)diff_time.tv_sec * 1000 +
           (unsigned long)diff_time.tv_usec / 1000;
}

#endif



