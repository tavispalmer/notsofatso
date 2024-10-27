#ifndef __CONFIG_H_
#define __CONFIG_H_


// macros for min and max
#ifdef _WIN32
#ifdef _MSC_VER
#define MIN min
#define MAX max
#else
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#endif

#endif
