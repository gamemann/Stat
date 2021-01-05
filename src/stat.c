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
        fprintf(stdout, "Usage: ./stat [-i <interface> --pps --bps --path <path> -c <\"kbps\" or \"mbps\" or \"gbps\"> --custom <integer>]\n" \
            "--pps => Set path to RX packet path.\n" \
            "--bps => Set path to RX byte path.\n" \
            "-p --path => Use count (integer) from a given path instead." \
            "-i --dev => The name of the interface to use when setting --pps or --bps.\n" \
            "-c --convert => Convert count to either \"kbps\", \"mbps\", or \"gbps\".\n" \
            "--custom => Divides the count value by this much before outputting to stdout.\n"
            "--interval => Use this interval (in microseconds) instead of one second.\n" \
        );

        return 0;
    }

    char *unit = "PPS";

    // Get path to RX bytes on Linux file system.
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

        uint64_t output = bps / ((divide > 0) ? divide : 1);

        // Get date in human format and print BPS with it (conversion supported).
        char date[255];
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        strftime(date, sizeof(date), "%c", tm);

        fprintf(stdout, "%" PRIu64 " %s - %s\n", output, unit, date);
    }

    return 0;
}