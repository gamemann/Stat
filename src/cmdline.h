#pragma once

#include <inttypes.h>

struct cmdline
{
    char *interface;
    char *path;
    unsigned int help : 1;
    char *conversion;

    uint64_t divide;

    uint64_t interval;

    unsigned int pps : 1;
    unsigned int bps : 1;
};

void parsecmdline(int argc, char *argv[], struct cmdline *cmd);