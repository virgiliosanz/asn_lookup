#pragma once

/** Macro used for not used params (avoid compiler warnings.) */
#define UNUSED(x) (void)(x)

/**
 * Convert numbers to string for macro processing.
 * Sample:
 *
 * #define MAJOR_VER 2
 * #define MINOR_VER 6
 * #define MY_FILE "/home/user/.myapp" STR(MAJOR_VER) STR(MINOR_VER)
 */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
