#include <stdio.h>
#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>

#include "data_structures.h"
#include "manage_rules.h"
#include "manage_user_preferences.h"
#include "setxkbmap_interface.h"
#include "system.h"

XKB_Rules *rules = NULL;
XKB_Preferences *user_prefs;

const char* program_name;
void print_usage(FILE* stream, int exit_code);

int main(int argc, char** argv) {

    /* Set locale via LC_ALL.  */
    setlocale(LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);

    
    rules = xkb_xorg_get_rules();
    
    if (rules == NULL) {
        printf( _("X-Keyboard rules can't be loaded.\n"
                    "Plase check that you have xkb-data installed.") );
        return(EXIT_FAILURE);
    }
    
    
    user_prefs =  xkb_preferences_load_from_env();
    if (user_prefs == NULL) {
        printf( _("Keyboar user preferences can't be loaded.\n"
                    "Plase check that you have setxkbmap installed.") );
        return (EXIT_FAILURE);
    }
    
    
    showMainWindow(argc, argv);
    return (EXIT_SUCCESS);

}
