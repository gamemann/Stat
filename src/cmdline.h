#pragma once

#include <inttypes.h>

typedef struct cmdline
{
    char *interface;
    char *path;
    unsigned int help : 1;
    char *conversion;

    uint64_t divide;

    uint64_t interval;
    
    uint64_t countmax;
    uint32_t timelimit;

    unsigned int pps : 1;
    unsigned int bps : 1;

    char *cmd;
    char *sep;
    char *key;
} cmdline_t;

void parsecmdline(int argc, char *argv[], struct cmdline *cmd);