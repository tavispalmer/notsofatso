#include <stdlib.h>

#define MODULE_NAME 3_0_0f
#define MODULE_STRING "notsofatso"

#ifdef _WIN32
// poll is not defined for some reason
struct pollfd;
int poll (struct pollfd *, unsigned, int);
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_demux.h>

static int Open(vlc_object_t *);
static void Close(vlc_object_t *);
static int demux(demux_t *);
static int control(demux_t *, int i_query, va_list args);

vlc_module_begin()
    set_description("notsofatso")
    set_category(CAT_INPUT)
    set_subcategory(SUBCAT_INPUT_DEMUX)
    set_capability("demux", 1)
    set_callbacks(Open, Close)
vlc_module_end()

static int Open(vlc_object_t *obj) {
    return VLC_EGENERIC;
    // demux_t *p_demux = (demux_t *)obj;
    // p_demux->pf_demux = demux;
    // p_demux->pf_control = control;

    // return VLC_SUCCESS;
}

static void Close(vlc_object_t *obj) {
}

static int demux(demux_t *demux) {
    return VLC_SUCCESS;
}

static int control(demux_t *demux, int i_query, va_list args) {
    return VLC_SUCCESS;
}
