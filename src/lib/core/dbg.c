/**
 * @file dbg.c
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief This files contains implemention for dbg.h
 *
 */
#include <core/core.h>

static enum debug_level __global_debug_level = debug_level_trace;
static const char* __global_debug_level_to_s[] = {"INFO", "ERROR", "WARN",
                                                  "TRACE"};

void debug_level_set(enum debug_level level) { __global_debug_level = level; }
enum debug_level debug_level_get() { return __global_debug_level; }
bool debug_level_is_visible(enum debug_level level)
{
    return (__global_debug_level >= level);
}
const char* debug_level_str(enum debug_level level)
{
    return __global_debug_level_to_s[level];
}

const char* debug_time()
{
    static time_t t;
    static char buffer[26];
    static struct tm* tm;

    time(&t);
    tm = localtime(&t);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm);

    return buffer;
}
