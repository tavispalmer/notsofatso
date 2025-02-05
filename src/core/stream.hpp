#ifndef __STREAM_HPP_
#define __STREAM_HPP_

// typedefs
#include <stdio.h>

#ifdef _MSC_VER
#ifdef _WIN64
typedef long long off_t;
typedef long long ssize_t;
#else
typedef int off_t;
typedef int ssize_t;
#endif
#endif

class Stream {
public:
    virtual ssize_t read(void *buf, size_t count) = 0;
    virtual off_t seek(off_t offset, int whence) = 0;
};

#endif
