/**
 * @file memory.c
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief This files contains implemention for memory.h
 *
 */
#include <core/core.h>

void* memdup(const void* src, size_t sz)
{
    void* mem = malloc(sz);
    return mem ? memcpy(mem, src, sz) : NULL;
}
