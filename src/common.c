#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <ctype.h>

/**
 * Simply lower-cases a string.
 * 
 * @param str Pointer to the full string we want to lower-case.
 * 
 * @return A character pointer to the lower-cased string.
 */
char *lowerstr(char *str) 
{
    for (char *p = str; *p; p++) 
    {
        *p = tolower(*p);
    }

    return str;
}

/**
 * Retrieves data from a file within the Linux file system and returns its value as an unsigned 64-bit value (uint64_t). Useful for getting statistics such as networking in /sys/class/net/<interface>/statistics/.
 * 
 * @param path Path to the file on the Linux file system.
 * 
 * @return An unsigned 64-bit value of the data inside the file (uint64_t).
 */
uint64_t getstat(const char *path)
{
    FILE *fp = fopen(path, "r");
    char buff[255];

    if (fp == NULL)
    {
        fprintf(stderr, "Error parsing stat file (%s) :: %s\n", path, strerror(errno));

        return 0;
    }

    fscanf(fp, "%s", buff);

    fclose(fp);

    return (uint64_t) strtoull((const char *)buff, (char **)buff, 0);
}