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
    bindtextdomain("lxkb_config", "/usr/share/locale");
    textdomain("lxkb_config");

    rules = xkb_rules_load();
    user_prefs = (XKB_Preferences*) xkb_preferences_load_from_env();

    showMainWindow(argc, argv);
    return (EXIT_SUCCESS);

}
