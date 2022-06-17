#pragma once

#include <stdlib.h>

struct command {
    const char** argv;
};
/**
 * Example:
 *      const char* ls[] = {"ls", "-lR", "..", 0};
 *      const char* awk[] = {"awk", "\"{print $1}\"", 0};
 *      const char* sort[] = {"sort", 0};
 *      const char* uniq[] = {"uniq", 0};
 *
 *      struct command cmd[] = {{ls}, {awk}, {sort}, {uniq}};
 *      int ret = fork_pipes(4, cmd);
 */
int fork_pipes(size_t n, struct command* cmd);
