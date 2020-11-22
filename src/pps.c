#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <getopt.h>
#include <time.h>

#include "common.h"

uint8_t help = 0;
char *interface = NULL;

static struct option longoptions[] =
{
    {"dev", required_argument, NULL, 'i'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
};

/**
 * Parses our command line.
 * 
 * @param argc Argument count.
 * @param argv A pointer to our argument character array.
 * 
 * @return void
 */
void parsecmdline(int argc, char *argv[])
{
    int c = -1;

    // Parse command line.
    while (optind < argc)
    {
        if ((c = getopt_long(argc, argv, "i:h", longoptions, NULL)) != -1)
        {
            switch (c)
            {
                case 'i':
                    interface = optarg;

                    break;

                case 'h':
                    help = 1;

                    break;

                case '?':
                    fprintf(stderr, "Missing argument.\n");

                    break;
            }
        }
        else
        {
            optind++;
        }
    }
}

int main(int argc, char *argv[])
{
    // Start off by parsing our command line.
    parsecmdline(argc, argv);

    // Check for help flag.
    if (help)
    {
        fprintf(stdout, "Usage: ./pps -i <interface>\n" \
            "-i --dev => The name of the interface to get PPS from.\n"
        );

        return 0;
    }

    // Check for interface.
    if (interface == NULL)
    {
        fprintf(stderr, "No interface set. Please specify an interface with the `-i` or `--dev` argument.\n");

        return 1;
    }

    // Get path to RX packets on Linux file system.
    char path[256];
    sprintf(path, "/sys/class/net/%s/statistics/rx_packets", interface);

    // Get total packet count and create a loop for each second.
    uint64_t totpckts = getstat(path);

    while (1)
    {
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

        // Sleep for a second.
        sleep(1);
    }

    return 0;
}