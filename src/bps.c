#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "common.h"
#include "cmdline.h"

int main(int argc, char *argv[])
{
    // Start off by parsing our command line.
    struct cmdline cmd = {0};

    parsecmdline(argc, argv, &cmd);

    // Check for help flag.
    if (cmd.help)
    {
        fprintf(stdout, "Usage: ./bps -i <interface> [-c <\"kbps\" or \"mbps\" or \"gbps\"> --custom <integer>]\n" \
            "-p --path => Use packet count (integer) from a given path instead." \
            "-i --dev => The name of the interface to get PPS from.\n" \
            "-c --convert => Convert BPS to either \"kbps\", \"mbps\", or \"gbps\"\n" \
            "--custom => Divides the BPS value by this much before outputting to stdin.\n"
            "--interval => Use this interval (in microseconds) instead of one second." \
        );

        return 0;
    }

    // Check for interface.
    if (cmd.interface == NULL)
    {
        fprintf(stderr, "No interface set. Please specify an interface with the `-i` or `--dev` argument.\n");

        return 1;
    }

    // Get path to RX bytes on Linux file system.
    char path[256];

    if (cmd.path != NULL)
    {
        sprintf(path, "%s", cmd.path);
    }
    else
    {
        sprintf(path, "/sys/class/net/%s/statistics/rx_bytes", cmd.interface);
    }

    uint64_t divide = 1;

    if (cmd.divide > 0)
    {
        divide = cmd.divide;
    }

    // Get total byte count and create a loop for each second.
    uint64_t totbps = getstat(path);

    while (1)
    {
        // Check for custom interval. Otherwise, use one second.
        if (cmd.interval > 0)
        {
            usleep(cmd.interval);
        }
        else
        {
            sleep(1);
        }

        // Get current total byte count and subtract totbps from it to get current BPS.
        uint64_t curbps = getstat(path);
        uint64_t bps = curbps - totbps;

        // Update totbps.
        totbps = curbps;

        // Do preset conversions.
        if (cmd.conversion != NULL)
        {
            if (strcmp(cmd.conversion, "kbps") == 0)
            {
                divide = 125;
            }
            else if (strcmp(cmd.conversion, "mbps") == 0)
            {
                divide = 125000;
            }
            else if (strcmp(cmd.conversion, "gbps") == 0)
            {
                divide = 134217728;
            }
            else
            {
                // Wrong conversion. Set it back to NULL.
                cmd.conversion = NULL;
            }
        }

        uint64_t output = bps / ((divide > 0) ? divide : 1);

        // Get date in human format and print BPS with it (conversion supported).
        char date[255];
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        strftime(date, sizeof(date), "%c", tm);

        fprintf(stdout, "%" PRIu64 " %s - %s\n", output, ((cmd.conversion != NULL) ? cmd.conversion : (divide == 1) ? "BPS" : "Custom"), date);
    }

    return 0;
}