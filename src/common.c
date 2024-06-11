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

/**
 * Trims a string from whitespaces.
 * 
 * @param str A pointer to the string you'd like to trim.
 * 
 * @return void
*/
char *trim(char* str)
{
    char* end;

    while (isspace((unsigned char)*str))
    {
        str++;
    }

    if (*str == 0)
    {
        return str;
    }

    end = str + strlen(str) - 1;

    while (end > str && isspace((unsigned char)*end)) 
    {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

/**
 * Executes a command and returns the result.
 * 
 * @param cmd The command to execute.
 * 
 * @return A string.
*/
char *execcmd(const char *cmd)
{
    FILE* fp;
    char buffer[128];
    size_t size = 0;
    size_t buffer_size = 128;
    char* result = malloc(buffer_size);

    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");

        exit(1);
    }

    result[0] = '\0';

    fp = popen(cmd, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to run command\n");

        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        size_t buffer_len = strlen(buffer);

        if (size + buffer_len + 1 > buffer_size)
        {
            buffer_size *= 2;
            result = realloc(result, buffer_size);

            if (result == NULL)
            {
                fprintf(stderr, "Memory reallocation failed\n");

                exit(1);
            }
        }

        strcat(result, buffer);
        size += buffer_len;
    }

    pclose(fp);

    return result;
}