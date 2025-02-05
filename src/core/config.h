#ifndef __CONFIG_H_
#define __CONFIG_H_

// don't use register
#ifndef _MSC_VER
#define register
#endif

// macros for min and max
#ifdef _WIN32
#ifdef _MSC_VER
#define MIN min
#define MAX max
#else
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#else

#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 199309L
#define _POSIX_C_SOURCE 199309L
#endif
// memset
#include <string.h>
// min and max
#include <sys/param.h>
// nanosleep
#include <time.h>

// windows attributes
#ifdef __cplusplus
#define __forceinline inline __attribute__((__always_inline__))
#else
#define __forceinline extern __inline__ __attribute__((__always_inline__,__gnu_inline__))
#endif

#ifdef __i686__
#define __fastcall __attribute__((__fastcall__))
#else
#define __fastcall
#endif

#define __int64 long long

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef unsigned int DWORD;

typedef char CHAR;
typedef const CHAR *LPCSTR;

__forceinline void Sleep(DWORD dwMilliseconds) {
    // rust Thread::sleep()
    long secs = dwMilliseconds / 1000U;
    long nsecs = ((long)dwMilliseconds % 1000L) * 1000000L;

    while (secs > 0 || nsecs > 0) {
        struct timespec ts = {
            .tv_sec  = secs,
            .tv_nsec = nsecs,
        };
        if (nanosleep(&ts, &ts) == -1) {
            secs = ts.tv_sec;
            nsecs = ts.tv_nsec;
        } else {
            secs = 0;
            nsecs = 0;
        }
    }
}

__forceinline int lstrlen(LPCSTR lpString) {
    return strlen(lpString);
}

#endif

#endif
