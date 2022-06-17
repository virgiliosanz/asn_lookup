#pragma once
/**
 * @file memory.h
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief Useful macros for memory alloc and free
 *
 *
 * @see http://www.learncthehardway.com
 */

#include <stdlib.h>
void* memdup(const void* src, size_t sz);

/**
 * Sample of usage:
 *
 * @code
 * struct foobar *foobar = ALLOC_INIT(struct foobar, {
 *       .field = value,
 *       .other_field = other_value,
 *       .yet_another_field = yet_another_value
 * });
 * @endcode
 *
 * https://tia.mat.br/posts/2015/05/01/initializing_a_heap_allocated_structure_in_c.html
 */
#define ALLOC_INIT(type, ...) (type*)memdup((type[]){__VA_ARGS__}, sizeof(type))

/**
 * Safe free resources
 */
#define ALLOC_FREE(var)                                                        \
    if (var) {                                                                 \
        free(var);                                                             \
        var = NULL;                                                            \
    }
