/*
Windows-specific: Provide clock_gettime64 to avoid depending on libwinpthread-1.dll.
VLC's libtorrent may need this function for Boost.Asio timing.
This implementation uses Windows API directly.
*/

#if defined(_WIN32)

#include <windows.h>
#include <time.h>

extern "C" {

int
clock_gettime64(clockid_t clock_id, struct timespec* tp)
{
    if (!tp)
        return -1;

    FILETIME ft;
    ULONGLONG since1601 = 0;

    switch (clock_id) {
    case CLOCK_REALTIME:
    case 0:
        GetSystemTimeAsFileTime(&ft);
        since1601 = (((ULONGLONG)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
        break;
    case CLOCK_MONOTONIC: {
        LARGE_INTEGER count;
        if (!QueryPerformanceCounter(&count))
            return -1;
        LARGE_INTEGER freq;
        if (!QueryPerformanceFrequency(&freq) || freq.QuadPart == 0)
            return -1;
        tp->tv_sec = (long)(count.QuadPart / freq.QuadPart);
        tp->tv_nsec = (long)((count.QuadPart % freq.QuadPart) * 1000000000 / freq.QuadPart);
        return 0;
    }
    default:
        return -1;
    }

    ULONGLONG since1970 = since1601 - 116444736000000000ULL;
    tp->tv_sec = (long)(since1970 / 10000000ULL);
    tp->tv_nsec = (long)((since1970 % 10000000ULL) * 100);

    return 0;
}

}  // extern "C"

#endif  // _WIN32