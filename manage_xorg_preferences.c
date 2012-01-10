
#include <stdio.h>
#include <stdlib.h>
#include <glib-2.0/glib/gslist.h>

#include "data_structures.h"

#define XORG_CONFIG_PATH "/usr/lib/X11/xorg.conf.d/keyboard.conf"
#define XORG_TMP_PATH "/tmp/keyboard.conf"

/* Description: This method creates a configuration file
 *              acording to XORG specifications and moves
 *              it to the location specified in XORG_CONFIG_PATH.
 * 
 * Input: Structure containin the keyboard preferences.
 * Output: True if succes or false if fails.
 * 
 */

gboolean
xkb_preferences_write_xorg_config (XKB_Preferences * prefs ) {

    FILE *tmp = fopen(XORG_TMP_PATH, "w");

    if (tmp == NULL) 
        return FALSE;
    
    
    fprintf(tmp, "Section \"InputClass\"\n");
    fprintf(tmp, "\tIdentifier \"keyboard-layout\"\n");
    fprintf(tmp, "\tDriver \"evdev\"\n");
    fprintf(tmp, "\tMatchIsKeyboard \"yes\"\n");

    if (prefs->model != NULL) {
        fprintf(tmp, "\tOption \"XkbModel\" \"%s\"\n", prefs->model);
    }

    if (prefs->layouts != NULL) {
        fprintf(tmp, "\tOption \"XkbLayout\" \"");

        GSList *lay_ptr = prefs->layouts;

        fprintf(tmp, "%s" ,(gchar *) lay_ptr->data);
        lay_ptr = lay_ptr->next;

        while (lay_ptr != NULL) {
            fprintf(tmp,", %s", (gchar *) lay_ptr->data);

            lay_ptr = lay_ptr->next;
        }


        fprintf(tmp, "\"\n");
    }

    if (prefs->variants != NULL) {
        fprintf(tmp, "\tOption \"XkbVariant\" \"");

        GSList *var_ptr = prefs->variants;

        fprintf(tmp,"%s", (gchar *) var_ptr->data);
        var_ptr = var_ptr->next;

        while (var_ptr != NULL) {
            fprintf(tmp,", %s", (gchar *) var_ptr->data);

            var_ptr = var_ptr->next;
        }


        fprintf(tmp, "\"\n");
    }

    if (prefs->options != NULL) {
        fprintf(tmp, "\tOption \"XkbOptions\" \"");

        GSList *opt_ptr = prefs->options;

        fprintf(tmp,"%s", (gchar *) opt_ptr->data);
        opt_ptr = opt_ptr->next;

        while (opt_ptr != NULL) {
            fprintf(tmp,", %s", (gchar *) opt_ptr->data);

            opt_ptr = opt_ptr->next;
        }


        fprintf(tmp, "\"\n");
    }

    fprintf(tmp, "EndSection");
    fclose(tmp);

    int ret_value;
    ret_value = system("gksu -m 'Introduce your password to set this configuration to the whole system.' "
            "mv /tmp/keyboard.conf /usr/lib/X11/xorg.conf.d/01-keyboard-layout.conf");
    
    if (ret_value == -1) {
        remove(XORG_TMP_PATH);
        return FALSE;
    }
    
    return TRUE;
}