#ifndef __VLC_STREAM_H_
#define __VLC_STREAM_H_

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_demux.h>

#include "../core/stream.h"

class VlcStream : public Read, public Seek {
private:
    stream_t *stream;

public:
    VlcStream(stream_t *stream) :
        stream(stream) {}

    virtual size_t read(uint8_t *buf, size_t len) {
        vlc_stream_Read(
            this->stream,
            buf,
            len
        );
    }

    virtual uint64_t seek(SeekFrom pos) {
        uint64_t offset;
        switch (pos.type) {
            case SeekFrom::_Start:
                offset = pos.start;
                break;
            case SeekFrom::_End:
                vlc_stream_GetSize(this->stream, &offset);
                offset += pos.end;
                break;
            case SeekFrom::_Current:
                offset = vlc_stream_Tell(this->stream);
                offset += pos.current;
                break;
        }

        vlc_stream_Seek(this->stream, offset);
        return offset;
    }
};

#endif
