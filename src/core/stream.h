#ifndef __STREAM_H_
#define __STREAM_H_

#include <stdint.h>

// based on Rust File API

class SeekFrom {
public:
    enum Type { _Start, _End, _Current } type;
    union {
        uint64_t start;
        int64_t end;
        int64_t current;
    };

private:
    SeekFrom(Type type, uint64_t val) :
        type(type),
        start(val) {}
    
    SeekFrom(Type type, int64_t val) :
        type(type),
        end(val) {}

public:
    static SeekFrom Start(uint64_t start) {
        return SeekFrom(_Start, start);
    }

    static SeekFrom End(int64_t end) {
        return SeekFrom(_End, end);
    }

    static SeekFrom Current(int64_t current) {
        return SeekFrom(_Current, current);
    }
};

class Read {
public:
    virtual size_t read(uint8_t *buf, size_t len) = 0;
};

class Seek {
public:
    virtual uint64_t seek(SeekFrom pos) = 0;
};

#endif
