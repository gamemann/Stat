#pragma once

#include <inttypes.h>

char *lowerstr(char *str);
uint64_t getstat(const char *path);
char *trim(char* str);
char *execcmd(const char *cmd);