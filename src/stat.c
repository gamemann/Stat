#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include "common.h"
#include "cmdline.h"

uint8_t cont = 1;

void sighdl(int tmp)
{
    cont = 0;
}

__uint64_t get_current(cmdline_t *cmd, const char *path)
{
    uint64_t cur = 0;

    if (cmd->cmd != NULL)
    {
        // Make sure key is available.
        if (cmd->key == NULL)
        {
            fprintf(stderr, "Command specified, but no key for separator included. Please use --key.\n");

            return cur;
        }

        char *sep = " ";

        if (cmd->sep != NULL)
        {
            sep = cmd->sep;
        }

        char *output = execcmd(cmd->cmd);

        // Make sure we have output.
        if (output == NULL)
        {
            fprintf(stderr, "Command '%s' output is NULL.\n", cmd->cmd);

            return 0;
        }

        // We need to copy output memory address since strsep() modifies output below.
        char *output_copy = output;

        // Loop through each line (\n separator).
        char *line = NULL;

        while ((line = strsep(&output, "\n")))
        {
            // Make sure we have at least one byte.
            if (strlen(line) < 1)
            {
                continue;
            }

            // Trim the string so it doesn't mess anything up.
            char *trimmed = trim(line);

            // We want to now split by our input separator (key).
            char *token = NULL;

            // Indicate the position we're at.
            int i = 1;

            while ((token = strsep(&trimmed, sep)))
            {
                // Key check.
                if (i == 1 && strcmp(cmd->key, token) == 0)
                {
                    i++;

                    continue;
                }

                // If the key matches, i will be 2. Therefore, use current token as value and convert to integer.
                if (i == 2)
                {
                    cur = (__uint64_t) strtoull((const char *)token, (char **)token, 0);
                }

                break;
            }
        }

        // Free output string.
        if (output_copy != NULL)
        {
            free(output_copy);
        }
    }
    else
    {
        cur = getstat(path);
    }

    return cur;
}

int main(int argc, char *argv[])
{
    // Start off by parsing our command line.
    struct cmdline cmd = {0};

    parsecmdline(argc, argv, &cmd);

    // Check for help flag.
    if (cmd.help)
    {
        fprintf(stdout, "Usage: ./stat [-i <interface> --pps --bps --path <path> -c <\"kbps\" or \"mbps\" or \"gbps\"> --custom <integer>]\n" \
            "--pps => Set path to RX packet path.\n" \
            "--bps => Set path to RX byte path.\n" \
            "-p --path => Use count (integer) from a given path instead." \
            "-i --dev => The name of the interface to use when setting --pps or --bps.\n" \
            "-c --convert => Convert count to either \"kbps\", \"mbps\", or \"gbps\".\n" \
            "--custom => Divides the count value by this much before outputting to stdout.\n"
            "--interval => Use this interval (in microseconds) instead of one second.\n" \
            "--count -n => Maximum amount of times to request the counter before stopping program (0 = no limit).\n" \
            "--time -t => Time limit (in seconds) before stopping program (0 = no limit).\n" \
            "--cmd => The command to execute and retrieve output from.\n" \
            "--sep => The separator to apply to the command's output.\n" \
            "--key => The key to search for when separating the command output.\n" \
        );

        return 0;
    }

    char *unit = "PPS";

    // Get path on Linux file system.
    char path[256];

    if (cmd.path != NULL)
    {
        sprintf(path, "%s", cmd.path);
    }
    else if (cmd.bps)
    {
        unit = "BPS";
        sprintf(path, "/sys/class/net/%s/statistics/rx_bytes", (cmd.interface != NULL) ? cmd.interface : "eth0");
    }
    else
    {
        sprintf(path, "/sys/class/net/%s/statistics/rx_packets", (cmd.interface != NULL) ? cmd.interface : "etho");
    }

    uint64_t divide = 1;

    if (cmd.divide > 0)
    {
        unit = "Custom";
        divide = cmd.divide;
    }

    signal(SIGINT, sighdl);

    // Get current counter and create a loop.
    uint64_t old = get_current(&cmd, path);
    uint64_t totcount = 0;

    uint64_t count = 0;
    time_t stotime = time(NULL) + cmd.timelimit;
    time_t statime = time(NULL);
    time_t curtime = 0;

    while (cont)
    {
        // Check stops.
        curtime = time(NULL);

        if (cmd.timelimit > 0 && curtime >= stotime)
        {
            break;
        }

        if (cmd.countmax > 0 && count >= cmd.countmax)
        {
            break;
        }

        // Check for custom interval. Otherwise, use one second.
        if (cmd.interval > 0)
        {
            usleep(cmd.interval);
        }
        else
        {
            sleep(1);
        }

        uint64_t cur = get_current(&cmd, path);
        uint64_t new = cur - old;

        // Save old counter.
        old = cur;

        // Do preset conversions.
        if (cmd.conversion != NULL)
        {
            if (strcmp(cmd.conversion, "kbps") == 0)
            {
                unit = "kbps";
                divide = 125;
            }
            else if (strcmp(cmd.conversion, "mbps") == 0)
            {
                unit = "mbps";
                divide = 125000;
            }
            else if (strcmp(cmd.conversion, "gbps") == 0)
            {
                unit = "gbps";
                divide = 134217728;
            }
            else
            {
                // Wrong conversion. Set it back to NULL.
                cmd.conversion = NULL;
            }
        }

        uint64_t output = new / ((divide > 0) ? divide : 1);
        totcount += output;

        // Get date in human format and print current counter with it.
        char date[255];
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        strftime(date, sizeof(date), "%c", tm);

        fprintf(stdout, "%" PRIu64 " %s - %s\n", output, unit, date);

        count++;
    }

    time_t tottime = curtime - statime;
    uint64_t avgcount = totcount / count;

    fprintf(stdout, "Received an average of %" PRIu64 " %s with a total of %" PRIu64 " %s over %lu seconds.\n", avgcount, unit, totcount, unit, tottime);

    return 0;
}