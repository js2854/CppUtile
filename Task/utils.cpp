#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
#include <assert.h>
#include "utils.h"

void SleepUs(UInt64 us)
{
    struct timeval tval;
    tval.tv_sec = us/1000000;
    tval.tv_usec = us % 1000000;
    select(0, NULL, NULL, NULL, &tval);
}

UInt64 MicroSeconds()
{
    struct timespec tp = {0};
    SInt32 s32Ret = ::clock_gettime(CLOCK_MONOTONIC, &tp); // Need link with -lrt.
    assert(s32Ret == 0);
    return (UInt64)tp.tv_sec * 1000000 + tp.tv_nsec/1000;
}

UInt64 MilliSeconds()
{
    return MicroSeconds() / 1000;
}
