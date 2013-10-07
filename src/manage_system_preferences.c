#include <stdio.h>
#include <stdlib.h>
#include <glib/gi18n.h>
#include <glib-2.0/glib.h>

#include "data_structures.h"

#define SYSTEM_CONFIG_PATH "/etc/default/keyboard"
#define SYSTEM_TMP_PATH "/tmp/keyboard.conf"

/* Description: This method creates a configuration file
 *              acording to SYSTEM specifications and moves
 *              it to the location specified in SYSTEM_CONFIG_PATH.
 * 
 * Input: Structure containin the keyboard preferences.
 * Output: True if succes or false if fails.
 * 
 */

gboolean
xkb_preferences_write_system_config (XKB_Preferences * prefs ) {

    FILE *tmp = fopen(SYSTEM_TMP_PATH, "w");

    if (tmp == NULL) 
        return FALSE;

    if (prefs->model != NULL) {
        fprintf(tmp, "XKBMODEL=\"%s\"\n", prefs->model);
    }

    if (prefs->layouts != NULL) {
        fprintf(tmp, "XKBLAYOUT=\"");

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
        fprintf(tmp, "XKBVARIANT=\"");

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
        fprintf(tmp, "XKBOPTIONS=\"");

        GSList *opt_ptr = prefs->options;

        fprintf(tmp,"%s", (gchar *) opt_ptr->data);
        opt_ptr = opt_ptr->next;

        while (opt_ptr != NULL) {
            fprintf(tmp,", %s", (gchar *) opt_ptr->data);

            opt_ptr = opt_ptr->next;
        }


        fprintf(tmp, "\"\n");
    }

    
    fclose(tmp);

    gchar *msg = _("Enter your password to set this configuration to the whole system.");
    GString *cmd = g_string_new("gksu -m '");
    g_string_append( cmd, msg );
    g_string_append(cmd, "' 'mv " SYSTEM_TMP_PATH " " SYSTEM_CONFIG_PATH" \'");
    
    g_debug("generated command %s", cmd->str);
    
    int ret_value;
    ret_value = system(cmd->str);
    
    if (ret_value == -1) {
        remove(SYSTEM_TMP_PATH);
        return FALSE;
    }
    
    return TRUE;
}
