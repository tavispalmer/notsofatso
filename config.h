#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __cplusplus

#include <algorithm>
using std::max, std::min;

constexpr const float &min(const int &a, const float &b) {
    return std::min((float)a, b);
}

#endif

#ifndef _WIN32

#define _POSIX_C_SOURCE 199309L
#include <errno.h>
#include <string.h>
#include <time.h>

#define __fastcall __attribute__((__fastcall__))

#ifdef __cplusplus
# define __forceinline inline __attribute__((__always_inline__))
#else
# define __forceinline extern __inline__ __attribute__((__always_inline__,__gnu_inline__))
#endif

#define __int64 long long

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int UINT;
typedef unsigned int DWORD;

typedef const char *LPCSTR;

int lstrlen(LPCSTR lpString) {
    return strlen(lpString);
}

void Sleep(DWORD dwMilliseconds) {
    if (dwMilliseconds <= 0) {
        return;
    }

    struct timespec ts = {
        dwMilliseconds / 1000L,
        (dwMilliseconds % 1000L) * 1000000L
    };
    
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {}
}

#endif

#endif
