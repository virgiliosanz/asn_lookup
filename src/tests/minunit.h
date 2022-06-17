#pragma once
/**
 * @file memory.h
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief Useful macros for unit testing
 *
 *
 * @see http://www.learncthehardway.com
 */
#include <core/core.h>

#undef NDEBUG

#define mu_suite_start() char* message = NULL

#define mu_assert(test, message, ...)                                          \
    if (!(test)) {                                                             \
        error(message, ##__VA_ARGS__);                                         \
        return message;                                                        \
    }
#define mu_run_test(test)                                                      \
    trace("-----%s", " " #test);                                               \
    message = test();                                                          \
    tests_run++;                                                               \
    if (message)                                                               \
        return message;

#define RUN_TESTS(name)                                                        \
    int main(int argc, char* argv[])                                           \
    {                                                                          \
        UNUSED(argc);                                                          \
        setlocale(LC_ALL, "spanish");                                          \
        trace("----- RUNNING: %s", argv[0]);                                   \
        printf("----\nRUNNING: %s\n", argv[0]);                                \
        char* result = name();                                                 \
        if (result != NULL) {                                                  \
            printf("FAILED: %s\n", result);                                    \
        }                                                                      \
        else {                                                                 \
            printf("ALL TESTS PASSED\n");                                      \
        }                                                                      \
        printf("Tests run: %d\n", tests_run);                                  \
        exit(result != NULL);                                                  \
    }

int tests_run;
