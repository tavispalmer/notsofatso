#ifndef __LIBC_FILE_H_
#define __LIBC_FILE_H_

#include <stdio.h>

#include "stream.h"

// File implementation
class LibcFile : public Read, public Write, public Seek {
private:
    FILE *file;
    LibcFile(FILE *file) :
        file(file) {}

public:
    static LibcFile open(const char *path) {
        return LibcFile(fopen64(path, "rb"));
    }

    static LibcFile create(const char *path) {
        return LibcFile(fopen64(path, "wb"));
    }

    virtual ~LibcFile() {
        fclose(this->file);
    }

    virtual size_t read(uint8_t *buf, size_t len) {
        return fread(buf, 1, len, this->file);
    }

    virtual size_t write(const uint8_t *buf, size_t len) {
        return fwrite(buf, 1, len, this->file);
    }

    virtual uint64_t seek(SeekFrom pos) {
        int whence;
        _off64_t offset;
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

        fseeko64(this->file, offset, whence);
        return ftello64(this->file);
    }
};

#endif
