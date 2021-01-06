#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include "cmdline.h"
#include "common.h"

const struct option opts[] = 
{
    {"dev", required_argument, NULL, 'i'},
    {"path", required_argument, NULL, 'p'},
    {"convert", required_argument, NULL, 'c'},
    {"custom", required_argument, NULL, 1},
    {"interval", required_argument, NULL, 2},
    {"count", required_argument, NULL, 'n'},
    {"time", required_argument, NULL, 't'},
    {"pps", no_argument, NULL, 3},
    {"bps", no_argument, NULL, 4},
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
void parsecmdline(int argc, char *argv[], struct cmdline *cmd)
{
    int c = -1;

    // Parse command line.
    while (optind < argc)
    {
        if ((c = getopt_long(argc, argv, "i:p:c:n:t:h", opts, NULL)) != -1)
        {
            switch (c)
            {
                case 'i':
                    cmd->interface = optarg;

                    break;

                case 'p':
                    cmd->path = optarg;

                    break;

                case 'c':
                    cmd->conversion = lowerstr(optarg);

                    break;

                case 1:
                    cmd->divide = (uint64_t) strtoull((const char *)optarg, (char **)optarg, 0);

                    break;

                case 2:
                    cmd->interval = (uint64_t) strtoull((const char *)optarg, (char **)optarg, 0);

                    break;

                case 3:
                    cmd->pps = 1;

                    break;

                case 4:
                    cmd->bps = 1;

                    break;
                
                case 'n':
                    cmd->countmax = (uint64_t) strtoull((const char *)optarg, (char **)optarg, 0);

                    break;

                case 't':
                    cmd->timelimit = (uint32_t)strtoul((const char *)optarg, (char **)optarg, 0);

                    break;

                case 'h':
                    cmd->help = 1;

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