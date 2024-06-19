

#include "vlc_stream.h"

size_t VlcStream::read(uint8_t *buf, size_t len) {
    vlc_stream_Read(
        this->stream,
        buf,
        len
    );
}