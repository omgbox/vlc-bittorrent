#include <windows.h>

struct timespec64 {
    long long tv_sec;
    long long tv_nsec;
};

typedef int clockid_t;
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

static long long qpc_freq = 0;
static long long qpc_base_sec = 0;
static long long qpc_base_nsec = 0;

static void init_qpc(void) {
    if (qpc_freq) return;
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    qpc_freq = freq.QuadPart;

    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    __int64 filetime = (((__int64)ft.dwHighDateTime << 32) | ft.dwLowDateTime);
    __int64 unixtime = (filetime - 116444736000000000LL) / 10000000LL;
    __int64 unixnsec = ((filetime - 116444736000000000LL) % 10000000LL) * 100LL;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    __int64 qpc_sec = now.QuadPart / qpc_freq;
    __int64 qpc_nsec = (now.QuadPart % qpc_freq) * 1000000000LL / qpc_freq;

    qpc_base_sec = unixtime - qpc_sec;
    qpc_base_nsec = unixnsec - qpc_nsec;
    if (qpc_base_nsec < 0) {
        qpc_base_sec--;
        qpc_base_nsec += 1000000000LL;
    }
}

extern "C" int clock_gettime64(clockid_t clock_id, struct timespec64 *tp) {
    if (!tp) return -1;

    if (clock_id == CLOCK_REALTIME) {
        init_qpc();
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        long long total_nsec = (now.QuadPart * 1000000000LL) / qpc_freq;
        tp->tv_sec = qpc_base_sec + total_nsec / 1000000000LL;
        tp->tv_nsec = (total_nsec % 1000000000LL) + qpc_base_nsec;
        if (tp->tv_nsec >= 1000000000LL) {
            tp->tv_sec++;
            tp->tv_nsec -= 1000000000LL;
        }
        return 0;
    }

    if (clock_id == CLOCK_MONOTONIC) {
        init_qpc();
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        long long total_nsec = (now.QuadPart * 1000000000LL) / qpc_freq;
        tp->tv_sec = total_nsec / 1000000000LL;
        tp->tv_nsec = total_nsec % 1000000000LL;
        return 0;
    }

    return -1;
}
