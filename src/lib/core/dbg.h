#pragma once
/**
 * @file dbg.h
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief Contains debug macros an rutines for your program.
 *
 * A part from debugging the module it's really usefull to manage error
 * checking. Maily using *_if functions. For example:
 *
 * @code
 * #include <stdlib.h>
 * #include <dbg.h>
 *
 * int main() {
 *
 * 	int* mem = calloc(10000, sizeof(int));
 * 	error_if(NULL == mem, error, "Alloc error: %s", strerror(errno));
 *
 * 	free(mem);
 * 	return EXIT_SUCCESS;
 *
 * error:
 *  if (mem) free(mem);
 * 	return EXIT_FAILED;
 * };
 * @endcode
 *
 *
 * @see http://www.learncodethehardway.com/
 */
#include <stdbool.h>

enum debug_level {
    debug_level_info,
    debug_level_error,
    debug_level_warn,
    debug_level_trace,
    debug_level_all
};
/**
 * Sets debug level.. You might chaning everywhere in your code. For example,
 * you want to debug just one method
 */
void debug_level_set(enum debug_level level);
enum debug_level debug_level_get();
const char* debug_level_str(enum debug_level level);
bool debug_level_is_visible(enum debug_level level);
const char* debug_time();

// __FILENAME__ is defined in CMakelist.txt and passed to compiler
#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif

/**
 * Generic Macro. Do not use it directly...
 */
#define debug(level, M, ...)                                                   \
    if (debug_level_is_visible(level)) {                                       \
        fprintf(stderr, "[%s] %s - %s (%d) %s: " M "\n",                       \
                debug_level_str(level), debug_time(), __FILENAME__, __LINE__,  \
                __FUNCTION__, ##__VA_ARGS__);                                  \
    }
/**
 * Info debug level. Use it for information on running, like normal activity
 * information
 */

#define error(M, ...) debug(debug_level_error, M, ##__VA_ARGS__)
#define error_if(A, label, M, ...)                                             \
    if (A) {                                                                   \
        error("assert failed: " #A " - " M, ##__VA_ARGS__);                    \
        goto label;                                                            \
    }

#define warn(M, ...) debug(debug_level_warn, M, ##__VA_ARGS__)
#define warn_if(A, M, ...)                                                     \
    if (A) {                                                                   \
        warn("assert failed: " #A " - " M, ##__VA_ARGS__);                     \
    }

#define trace(M, ...) debug(debug_level_trace, M, ##__VA_ARGS__)
#define trace_if(A, M, ...)                                                    \
    if (A) {                                                                   \
        trace("assert failed: " #A " - " M, ##__VA_ARGS__);                    \
    }

/** alias for info */
#define info(M, ...)                                                           \
    fprintf(stderr, "[INFO] %s: " M "\n", debug_time(), ##__VA_ARGS__);
