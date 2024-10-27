#ifndef __STREAM_HPP_
#define __STREAM_HPP_

// typedefs
#include <stdio.h>

class Stream {
public:
    virtual ssize_t read(void *buf, size_t count) = 0;
    virtual off_t seek(off_t offset, int whence) = 0;
};

#endif
