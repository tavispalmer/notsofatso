#include <stdlib.h>

extern "C" {
    // poll not defined for some reason on windows
    struct pollfd;
    int poll(struct pollfd *fds, unsigned nfds, int timeout);

    #define MODULE_STRING "nsf"
    const char vlc_module_name[] = MODULE_STRING;

    #define MODULE_NAME 3_0_0f
}

// void *operator new(size_t count) {
//     return malloc(count);
// }

// void operator delete(void *ptr) {
//     free(ptr);
// }

#include <vlc_common.h>
#include <vlc_input.h>
#include <vlc_demux.h>
#include <vlc_plugin.h>

#include "core/NSF_File.h"
#include "core/NSF_Core.h"

#include "core/stream.hpp"

class VlcStream : public Stream {
private:
    stream_t *stream;
public:
    VlcStream(stream_t *stream) :
        stream(stream) {}

    virtual ssize_t read(void *buf, size_t count) {
        return vlc_stream_Read(stream, buf, count);
    }

    virtual off_t seek(off_t offset, int whence) {
        switch (whence) {
            case SEEK_SET:
                if (!vlc_stream_Seek(stream, offset)) {
                    return vlc_stream_Tell(stream);
                }
                break;
            case SEEK_CUR:
                if (!vlc_stream_Seek(stream, vlc_stream_Tell(stream) + offset)) {
                    return vlc_stream_Tell(stream);
                }
                break;
            case SEEK_END:
                uint64_t size;
                vlc_stream_GetSize(stream, &size);
                if (!vlc_stream_Seek(stream, size + offset)) {
                    return vlc_stream_Tell(stream);
                }
                break;
        }
        return -1;
    }
};

struct demux_sys_t {
    CNSFCore *nsf;

    es_out_id_t *es;
    date_t pts;
};

static int Demux(demux_t *);
static int Control(demux_t *, int, va_list);

static int Open(vlc_object_t *obj) {
    demux_t *demux = (demux_t *)obj;
    uint64_t size;

    if (vlc_stream_GetSize(demux->s, &size))
        return VLC_EGENERIC;
    
    const uint8_t *peek;
    if (vlc_stream_Peek(demux->s, &peek, 4) < 4)
        return VLC_EGENERIC;
    
    if (strncmp((const char *)peek, "NESM", 4) && strncmp((const char *)peek, "NSFE", 4))
        return VLC_EGENERIC;
    
    block_t *data = NULL;
    if (size <= 0) {
        // data = vlc_stream_Block(demux->s, 1 << 24);
        // if (data == NULL)
            return VLC_EGENERIC;
    }

    demux_sys_t *sys = (demux_sys_t *)malloc(sizeof(*sys));
    if (unlikely(sys == NULL))
        return VLC_ENOMEM;
    
    VlcStream stream(demux->s);

    CNSFFile file;
    file.LoadStream(&stream, 1, 0);

    sys->nsf = new CNSFCore();
    sys->nsf->Initialize();
    sys->nsf->SetPlaybackOptions(48000,2);
    sys->nsf->LoadNSF(&file);
    sys->nsf->SetPlaybackSpeed(0);

    sys->nsf->SetChannelOptions(0, 1, 255, -45, 1);
    sys->nsf->SetChannelOptions(1, 1, 255, 45, 1);
    sys->nsf->SetChannelOptions(2, 1, 255, 0, 0);
    sys->nsf->SetChannelOptions(3, 1, 255, 0, 0);
    sys->nsf->SetChannelOptions(4, 1, 255, 0, 0);

    sys->nsf->SetTrack(0);

    es_format_t fmt;
    es_format_Init(&fmt, AUDIO_ES, VLC_CODEC_S16N);
    fmt.audio.i_rate = 48000;
    fmt.audio.i_bytes_per_frame = 4;
    fmt.audio.i_frame_length = 4;
    fmt.audio.i_channels = 2;
    fmt.audio.i_blockalign = 4;
    fmt.audio.i_bitspersample = 16;
    fmt.i_bitrate = 48000 * 4;

    sys->es = es_out_Add(demux->out, &fmt);
    date_Init(&sys->pts, 48000, 1);
    date_Set(&sys->pts, 0);

    demux->pf_demux = Demux;
    demux->pf_control = Control;
    demux->p_sys = sys;

    return VLC_SUCCESS;
}

static void Close(vlc_object_t *obj) {
    demux_t *demux = (demux_t *)obj;
    demux_sys_t *sys = demux->p_sys;

    delete sys->nsf;
    free(sys);
}

static int Demux(demux_t *demux) {
    demux_sys_t *sys = demux->p_sys;

    block_t *block = block_Alloc(2 * 2 * 48000);
    if (unlikely(block == NULL))
        return 0;

    sys->nsf->GetSamples(block->p_buffer, 2 * 2 * 48000);
    
    block->i_pts = block->i_dts = VLC_TICK_0 + date_Get(&sys->pts);
    es_out_SetPCR(demux->out, block->i_pts);
    es_out_Send(demux->out, sys->es, block);
    date_Increment(&sys->pts, 48000);
    return 1;
}

static int Control(demux_t *demux, int query, va_list args) {
    return VLC_EGENERIC;
}

vlc_module_begin()
    set_shortname("NotSo Fatso")
    set_description("NotSo Fatso")
    set_category(CAT_INPUT)
    set_subcategory(SUBCAT_INPUT_DEMUX)
    set_capability("demux", 60)
    set_callbacks(Open, Close)
vlc_module_end()
