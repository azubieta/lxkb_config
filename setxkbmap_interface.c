/* 
 * File:   setxkbmap_interface.c
 * Author: alexis
 *
 * Created on 9 de octubre de 2011, 18:52
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib-2.0/glib/gslist.h>
#include <glib-2.0/glib/gslice.h>
#include <string.h>

#include "local_structures.h"

void set_layout_and_variant(GSList *preferences) {

    char *command = malloc(sizeof (char) * 500);
    memset(command, 0, sizeof (sizeof (char) * 50));
    strcat(command, "setxkbmap \"");

    GSList *it = preferences;
    if (it == NULL)
        return;

    PairLayoutVariant *lv = it->data;
    strcat(command, lv->layoutId);
    if (strcmp(lv->variantId, "") != 0) {
        strcat(command, "(");
        strcat(command, lv->variantId);
        strcat(command, ")");
    }

    it = it->next;

    while (it != NULL) {
        lv = it->data;
        strcat(command, ",");
        strcat(command, lv->layoutId);
        if (strcmp(lv->variantId, "") != 0) {
            strcat(command, "(");
            strcat(command, lv->variantId);
            strcat(command, ")");
        }
        it = it->next;
    }
    strcat(command, "\"");
    system(command);
}

/* Description: This method calls setxkbmap whit the 
 *              specific arguments to set the keyboard
 *              preferences.
 * Input: Struct with the user preferences.
 * Output: Nothing.
 */
void
set_user_kb_preferences(XKB_Preferences *prefs) {

    char *command = malloc(sizeof (char) * 2048);
    memset(command, 0, sizeof (sizeof (char) * 50));
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
        strcat(command, prefs-> model);
    }

    

    printf("%s\n", command);
    system(command);
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
get_user_kb_preferences() {
    FILE *setxbkmap_output = popen("setxkbmap -v 10", "r");
    XKB_Preferences *prefs = g_slice_alloc0(sizeof (XKB_Preferences));

    gchar *buff = g_slice_alloc0(sizeof (char) * 1024);

    fscanf(setxbkmap_output, "%s", buff);

    while (!feof(setxbkmap_output)) {
        //printf("\n-->%s", buff);
        /*
         * read model
         */
        if (strcmp(buff, "model:") == 0) {
            prefs->model = g_slice_alloc0(sizeof (char) * 128);
            fscanf(setxbkmap_output, "%s", prefs->model);

            //printf("\nmodel: %s\n", prefs->model);
        }
        
        /*
         * read layouts and variants
         */
        if (strcmp(buff, "layout:") == 0) {

            fscanf(setxbkmap_output, "%s", buff);
            gchar *it = strtok(buff, ",");
            gchar *lay_buff = g_slice_alloc0(sizeof (char) * 128);
            gchar *var_buff = g_slice_alloc0(sizeof (char) * 128);

            remove_parentheses(it, lay_buff, var_buff);

            //printf("\nLayout: %s Variant: %s", lay_buff, var_buff);

            prefs->layouts = g_slist_append(prefs->layouts, lay_buff);
            prefs->variants = g_slist_append(prefs->variants, var_buff);

            while ((it = strtok(NULL, ",")) != NULL) {
                lay_buff = g_slice_alloc0(sizeof (char) * 128);
                var_buff = g_slice_alloc0(sizeof (char) * 128);

                remove_parentheses(it, lay_buff, var_buff);

                //printf("\nLayout: %s Variant: %s", lay_buff, var_buff);

                prefs->layouts = g_slist_append(prefs->layouts, lay_buff);
                prefs->variants = g_slist_append(prefs->variants, var_buff);
            }

        }

        /*
         *  read options
         */
        
        if (strcmp(buff, "options:") == 0) {
            fscanf(setxbkmap_output, "%s", buff);
            gchar *it = strtok(buff, ",");

            //printf("\n Option: %s \n", it);
            prefs->options = g_slist_append(prefs->options, strdup(it));


            while ((it = strtok(NULL, ",")) != NULL) {
                prefs->options = g_slist_append(prefs->options, strdup(it));
                //printf("\nOption: %s", it);
            }
        }

        fscanf(setxbkmap_output, "%s", buff);
    }
    
    pclose(setxbkmap_output);
    return prefs;
}