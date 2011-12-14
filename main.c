#include <stdio.h>
#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>
#include "local_structures.h"
#include "rules_parser.h"
#include "manage_user_preferences.h"
#include "setxkbmap_interface.h"
#include "gui_gtk.h"

#include "system.h"

GSList *preferences = NULL;
KB_Rules *rules = NULL;

const char* program_name;
void print_usage(FILE* stream, int exit_code);

int main(int argc, char** argv) {
    
  /* Set locale via LC_ALL.  */
  setlocale (LC_ALL, "");

#if ENABLE_NLS

  /* Set the text message domain.  */
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif


    /** Parsing arguments **/

    /* A string listing valid short options letters.  */
    const char* const short_options = "hs";
    /* An array describing valid long options.  */
    const struct option long_options[] = {
        { "help", 0, NULL, 'h'},
        { "setup", 0, NULL, 's'},
        { NULL, 0, NULL, 0} /* Required at end of array.  */
    };

    program_name = argv[0];

    int next_option;
    int setup = 0;

    do {
        next_option = getopt_long(argc, argv, short_options,
                long_options, NULL);
        switch (next_option) {
            case 'h': /* -h or --help */
                /* User has requested usage information.  
                 * Print it to standard output, and exit
                 * with exit code zero (normal termination). */
                print_usage(stdout, 0);

            case 's': /* -v or --setup */
                setup = 1;
                break;

            case '?': /* The user specified an invalid option.  */
                /* Print usage information to standard error, 
                 * and exit with exit code one (indicating 
                 * abonormal termination).  */
                print_usage(stderr, 1);

            case -1: /* Done with options.  */
                break;

            default: /* Something else: unexpected.  */
                abort();
        }
    } while (next_option != -1);

    /** End arguments parsing **/

    if (setup) {
        preferences = quick_load_user_preferences();

        set_layout_and_variant(preferences);
        exit(EXIT_SUCCESS);
    } else {
        rules = load_KB_rules();
        preferences = load_user_preferences(rules);
        showMainWindow(argc, argv);
        return (EXIT_SUCCESS);
    }
}

void print_usage(FILE* stream, int exit_code) {
    fprintf(stream, "Usage:  %s options [ inputfile ... ]\n", program_name);
    fprintf(stream,
            "  -h  --help             Display this usage information.\n"
            "  -s  --setup            Set the keyboard configurartion "
            "                         according to the saved preferences\n"
            );
    exit(exit_code);
}
