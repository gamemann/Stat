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
        fprintf(stdout, "Usage: ./pps -i <interface>\n" \
            "-i --dev => The name of the interface to get PPS from.\n" \
            "-p --path => Use packet count (integer) from a given path instead."
        );

        return 0;
    }

    // Check for interface.
    if (cmd.interface == NULL)
    {
        fprintf(stderr, "No interface set. Please specify an interface with the `-i` or `--dev` argument.\n");

        return 1;
    }

    // Get path to RX packets on Linux file system.
    char path[256];

    if (cmd.path != NULL)
    {
        sprintf(path, "%s", cmd.path);
    }
    else
    {
        sprintf(path, "/sys/class/net/%s/statistics/rx_packets", cmd.interface);
    }

    // Get total packet count and create a loop for each second.
    uint64_t totpckts = getstat(path);

    while (1)
    {
        // Sleep for a second.
        sleep(1);
        
        // Get current total packet count and subtract totpckts from it to get current PPS.
        uint64_t curpckts = getstat(path);
        uint64_t pps = curpckts - totpckts;

        // Update totpckts.
        totpckts = curpckts;

        // Get date in human format and print PPS with it.
        char date[255];
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        strftime(date, sizeof(date), "%c", tm);

        fprintf(stdout, "%" PRIu64 " PPS - %s\n", pps, date);
    }

    return 0;
}