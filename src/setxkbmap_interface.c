/* 
 * File:   setxkbmap_interface.c
 * Author: Alexis López Zubieta
 *
 * Created on 9 de octubre de 2011, 18:52
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include <glib-2.0/glib.h>

#include "manage_rules.h"
#include "data_structures.h"

/* Description: This method generates a call tosetxkbmap 
 *              whit the specific arguments to set the 
 *              keyboard preferences.
 * Input: Struct with the user preferences.
 * Output: Command line instructuion.
 */
gchar* generate_setxkbmap_command(XKB_Preferences *prefs) {
    gchar *command = g_slice_alloc0(sizeof (char) * 2048);
    strcat(command, "setxkbmap ");

    // Adding options
    if (prefs->options != NULL) {
        strcat(command, " -option ");

        GSList *opt_it = prefs->options;

        strcat(command, opt_it->data);

        opt_it = opt_it->next;

        while (opt_it != NULL) {
            strcat(command, " -option ");
            strcat(command, opt_it->data);

            opt_it = opt_it->next;
        }
    }


    // Adding layouts and variants
    if (prefs->layouts != NULL) {
        strcat(command, " \"");
        GSList *lay_it = prefs->layouts;
        GSList *var_it = prefs->variants;

        // First element
        strcat(command, lay_it->data);
        if (strcmp(var_it->data, "") != 0) {
            strcat(command, "(");
            strcat(command, var_it->data);
            strcat(command, ")");
        }

        lay_it = lay_it->next;
        var_it = var_it->next;

        while (lay_it != NULL) {
            strcat(command, ",");
            strcat(command, lay_it->data);
            if (strcmp(var_it->data, "") != 0) {
                strcat(command, "(");
                strcat(command, var_it->data);
                strcat(command, ")");
            }

            lay_it = lay_it->next;
            var_it = var_it->next;
        }
        strcat(command, "\"");
    }

    // This argument must be the last
    if (prefs->model != NULL) {
        strcat(command, " -model ");
        strcat(command, prefs->model);
    }
    return command;

}

/* Description: This method calls setxkbmap whit the 
 *              specific arguments to set the keyboard
 *              preferences.
 * Input: Struct with the user preferences.
 * Output: Nothing.
 */
gboolean
xkb_preferences_set_to_env(XKB_Preferences *prefs) {
    fprintf(stderr, "DEBUG: Setting user preferences with \"setxkbmap\"\n");

    char * command = generate_setxkbmap_command(prefs);
    fprintf(stderr, "DEBUG: %s\n", command);

    int result;

    // remove all options
    result = system("setxkbmap -option");
    if (result == -1) {
        printf("ERROR: setxkmap can't be executed.\n");
        exit(EXIT_FAILURE);
    }
    result = system(command);

    if (result == -1) {
        printf("ERROR: \"%s\" can't be executed.\n", command);
        exit(EXIT_FAILURE);
    }

    g_slice_free1(sizeof (char) * 2048, command);
    if (result == -1)
        return FALSE;
    else
        return TRUE;
}

void
remove_parentheses(gchar *str, gchar *layout, gchar *variant) {
    gchar *ptr = str;
    gchar *var_ptr;
    while ((*ptr != '(') && (*ptr != 0)) ptr++;
    if (*ptr != 0) {
        *ptr = 0;
        var_ptr = ++ptr;
        while ((*ptr != ')') && (*ptr != 0)) ptr++;
        *ptr = 0;

        strcpy(variant, var_ptr);
    }
    strcpy(layout, str);

}

/* Desription: This method obtains the user preferences
 *             by parsing the setxkbmap -v 10 output.
 * Input: None.
 * Output: Structure containing the user preferences.
 */

XKB_Preferences *
xkb_preferences_load_from_env() {

    fprintf(stderr, "DEBUG: Loading user preferences with \"setxkbmap\"\n");

    XKB_Rules * rules = xkb_xorg_get_rules();
    setlocale(LC_ALL, "C");

    FILE *setxbkmap_output = popen("setxkbmap -v 10", "r");
    XKB_Preferences *prefs = g_slice_alloc0(sizeof (XKB_Preferences));

    gchar *buff = g_slice_alloc0(sizeof (char) * 1024);
    gboolean nothing_loaded = TRUE;

    fscanf(setxbkmap_output, "%s", buff);

    while (!feof(setxbkmap_output)) {
        /*
         * read model
         */
        if (strcmp(buff, "model:") == 0) {
            nothing_loaded = FALSE;
            gchar * model_id = g_slice_alloc0(sizeof (char) * 128);
            fscanf(setxbkmap_output, "%s", model_id);
            prefs->model = model_id;

            printf("DEBUG: model: %s\n", prefs->model);

        }

        /*
         * read layouts and variants
         */
        if (strcmp(buff, "layout:") == 0) {
            nothing_loaded = FALSE;

            fscanf(setxbkmap_output, "%s", buff);
            gchar *it = strtok(buff, ",");
            gchar *lay_buff = g_slice_alloc0(sizeof (char) * 128);
            gchar *var_buff = g_slice_alloc0(sizeof (char) * 128);

            remove_parentheses(it, lay_buff, var_buff);

            printf("DEBUG: Layout: %s Variant: %s \n", lay_buff, var_buff);

            prefs->layouts = g_slist_append(prefs->layouts, lay_buff);
            prefs->variants = g_slist_append(prefs->variants, var_buff);

            while ((it = strtok(NULL, ",")) != NULL) {
                lay_buff = g_slice_alloc0(sizeof (char) * 128);
                var_buff = g_slice_alloc0(sizeof (char) * 128);

                remove_parentheses(it, lay_buff, var_buff);

                printf("DEBUG: Layout: %s Variant: %s", lay_buff, var_buff);

                prefs->layouts = g_slist_append(prefs->layouts, lay_buff);
                prefs->variants = g_slist_append(prefs->variants, var_buff);
            }

        }

        /*
         *  read options
         */

        if (strcmp(buff, "options:") == 0) {
            nothing_loaded = FALSE;
            fscanf(setxbkmap_output, "%s", buff);
            gchar *it = strtok(buff, ",");

            printf("DEBUG: Option: %s \n", it);
            prefs->options = g_slist_append(prefs->options, strdup(it));


            while ((it = strtok(NULL, ",")) != NULL) {
                prefs->options = g_slist_append(prefs->options, strdup(it));
                printf("DEBUG: Option: %s", it);
            }
        }

        fscanf(setxbkmap_output, "%s", buff);
    }

    setlocale(LC_ALL, "");

    if (nothing_loaded) {
        g_slice_free(XKB_Preferences, prefs);
        return NULL;
    }
    pclose(setxbkmap_output);
    return prefs;
}
