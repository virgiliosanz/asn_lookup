#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Looks for field number "pos" in a "sep" delimiter string.
 * Stores the result at dest it should have at least max_size bytes...
 */
bool get_field(char* line, char* dest, size_t max_size, char sep, uint32_t pos)
{
    char* start = line;
    char* end = {0};
    uint32_t found = 0;

    size_t size = 0;
    while ((end = strchr(start, sep)) != NULL) {
        found++;
        if (found == pos) {
            size = 0;
            while (start != end) {
                *dest = *start;

                start++;
                dest++;
                size++;

                if (size == max_size)
                    break; // protect memory
            }
            *dest = '\0';
            return true;
        }
        start = end + 1;
    }
    return false;
}

void align_str(char* dest, const char* src, int room_width)
{
    int len = strlen(src);
    int width = abs(room_width);
    int i;

    // align right
    if (0 > room_width) {
        for (i = 0; i < (width - len); i++) {
            *dest = ' ';
            dest++;
        }
        while (i < width) {
            *dest = *src;
            dest++;
            src++;
            i++;
        }
    }
    else {
        for (i = 0; i < len; i++) {
            *dest = *src;
            dest++;
            src++;
        }
        while (i < width) {
            *dest = ' ';
            dest++;
            i++;
        }
    }
}
