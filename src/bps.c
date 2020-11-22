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

char *conversion = NULL;
uint64_t divide = 1;

static struct option longoptions[] =
{
    {"dev", required_argument, NULL, 'i'},
    {"convert", required_argument, NULL, 'c'},
    {"custom", required_argument, NULL, 1},
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
        if ((c = getopt_long(argc, argv, "i:c:h", longoptions, NULL)) != -1)
        {
            switch (c)
            {
                case 'i':
                    interface = optarg;

                    break;

                case 'c':
                    conversion = lowerstr(optarg);

                    break;

                case 1:
                    divide = (uint64_t) strtoull((const char *)optarg, (char **)optarg, 0);

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
        fprintf(stdout, "Usage: ./bps -i <interface> [-c <\"kbps\" or \"mbps\" or \"gbps\"> --custom <integer>]\n" \
            "-i --dev => The name of the interface to get PPS from.\n" \
            "-c --convert => Convert BPS to either \"kbps\", \"mbps\", or \"gbps\"\n" \
            "--custom => Divides the BPS value by this much before outputting to stdin.\n"
        );

        return 0;
    }

    // Check for interface.
    if (interface == NULL)
    {
        fprintf(stderr, "No interface set. Please specify an interface with the `-i` or `--dev` argument.\n");

        return 1;
    }

    // Get path to RX bytes on Linux file system.
    char path[256];
    sprintf(path, "/sys/class/net/%s/statistics/rx_bytes", interface);

    // Get total byte count and create a loop for each second.
    uint64_t totbps = getstat(path);

    while (1)
    {
        // Sleep for a second.
        sleep(1);

        // Get current total byte count and subtract totbps from it to get current BPS.
        uint64_t curbps = getstat(path);
        uint64_t bps = curbps - totbps;

        // Update totbps.
        totbps = curbps;

        // Do preset conversions.
        if (conversion != NULL)
        {
            if (strcmp(conversion, "kbps") == 0)
            {
                divide = 125;
            }
            else if (strcmp(conversion, "mbps") == 0)
            {
                divide = 125000;
            }
            else if (strcmp(conversion, "gbps") == 0)
            {
                divide = 134217728;
            }
            else
            {
                // Wrong conversion. Set it back to NULL.
                conversion = NULL;
            }
        }

        uint64_t output = bps / ((divide > 0) ? divide : 1);

        // Get date in human format and print BPS with it (conversion supported).
        char date[255];
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        strftime(date, sizeof(date), "%c", tm);

        fprintf(stdout, "%" PRIu64 " %s - %s\n", output, ((conversion != NULL) ? conversion : (divide == 1) ? "BPS" : "Custom"), date);
    }

    return 0;
}