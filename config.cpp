#include "config.h"

constexpr const float &min(const int &a, const float &b) {
    return std::min((float)a, b);
}

#ifndef _WIN32

int lstrlen(LPCSTR lpString) {
    return strlen(lpString);
}

void Sleep(DWORD dwMilliseconds) {
    if (dwMilliseconds <= 0) {
        return;
    }

    struct timespec ts = {
        dwMilliseconds / 1000L,
        (dwMilliseconds % 1000L) * 1000000L
    };
    
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {}
}

#endif
