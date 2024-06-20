#ifndef __LIBC_FILE_H_
#define __LIBC_FILE_H_

#include <stdio.h>

#include "stream.h"

// File implementation
class File : public Read, public Seek {
private:
    FILE *file;
    File(FILE *file) :
        file(file) {}

public:
    static File open(const char *path) {
        FILE *file = fopen(path, "rb");
        if (!file) {
            throw std::exception();
        }
        return File(file);
    }

    virtual ~File() {
        fclose(this->file);
    }

    virtual size_t read(uint8_t *buf, size_t len) {
        return fread(buf, 1, len, this->file);
    }

    virtual uint64_t seek(SeekFrom pos) {
        int whence;
        long offset;
        switch (pos.type) {
            case SeekFrom::_Start:
                whence = SEEK_SET;
                offset = pos.start;
                break;
            case SeekFrom::_End:
                whence = SEEK_END;
                offset = pos.end;
                break;
            case SeekFrom::_Current:
                whence = SEEK_CUR;
                offset = pos.current;
                break;
        }

        fseek(this->file, offset, whence);
        return ftell(this->file);
    }
};

#endif
