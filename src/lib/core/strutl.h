#pragma once
/**
 * @file strutl.h
 * @author Virgilio Sanz <virgilio.sanz@gmail.com>
 * @date Apr 2017
 * @brief String manipulation functions and macros
 *
 * @see http://www.learncthehardway.com/
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * remove \n or \r or both, the first it's found from an string. Very useful
 * when reading files.
 */
#define trim_eol(str)                                                          \
    do {                                                                       \
        (str)[strcspn((str), "\r\n")] = 0;                                     \
    } while (0);

/*
 * Looks for field number "pos" in a "sep" delimiter string.
 * Stores the result at dest it should have at least max_size bytes...
 */
bool get_field(char* line, char* dest, size_t max_size, char sep, uint32_t pos);

/* if room_width < 0 align right */
void align_str(char* dest, const char* src, int room_width);
