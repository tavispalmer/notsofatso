#ifndef __CONFIG_H_
#define __CONFIG_H_

#ifdef WIN32
    // MIN is not defined on windows
    #ifdef _MSVC_VER
        #define MIN min
    #else
        #define MIN(a,b) (((a)<(b))?(a):(b))
    #endif
#else
    // windows macros
    #ifdef __cplusplus
        // c++
        #define __forceinline inline __attribute__((__always_inline__))
    #else
        // c
        #define __forceinline extern __inline__ __attribute__((__always_inline__,__gnu_inline__))
    #endif
#endif

#endif
