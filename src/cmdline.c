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
        if ((c = getopt_long(argc, argv, "i:p:c:h", opts, NULL)) != -1)
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