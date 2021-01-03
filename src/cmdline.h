#pragma once

#include <inttypes.h>

struct cmdline
{
    char *interface;
    char *path;
    unsigned int help : 1;
    char *conversion;

    uint64_t divide;
};

void parsecmdline(int argc, char *argv[], struct cmdline *cmd);