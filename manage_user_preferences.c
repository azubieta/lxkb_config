#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manage_user_preferences.h"
#include "data_structures.h"
#include "manage_rules.h"

gboolean
xkb_preferences_layout_variant_remove(XKB_Preferences *user_prefs, gchar *lay, gchar *var) {
    GSList *lay_ptr = user_prefs->layouts;
    GSList *var_ptr = user_prefs->variants;

    while (lay_ptr != NULL) {
        if ((strcmp(lay_ptr->data, lay) == 0) &&
                (strcmp(var_ptr->data, var) == 0)) {

            user_prefs->layouts = g_slist_remove(user_prefs->layouts, lay_ptr->data);
            user_prefs->variants = g_slist_remove(user_prefs->variants, var_ptr->data);

            return TRUE;
        }

        lay_ptr = lay_ptr->next;
        var_ptr = var_ptr->next;
    }
    return FALSE;
}

gboolean
xkb_preferences_layout_variant_set_main(XKB_Preferences *user_prefs, gchar *lay, gchar *var) {
    GSList *lay_ptr = user_prefs->layouts;
    GSList *var_ptr = user_prefs->variants;

    while (lay_ptr != NULL) {
        if ((strcmp(lay_ptr->data, lay) == 0) &&
                (strcmp(var_ptr->data, var) == 0)) {

            user_prefs->layouts = g_slist_remove(user_prefs->layouts, lay_ptr->data);
            user_prefs->variants = g_slist_remove(user_prefs->variants, var_ptr->data);

            user_prefs->layouts = g_slist_insert(user_prefs->layouts, lay, 0);
            user_prefs->variants = g_slist_insert(user_prefs->variants, var, 0);

            return TRUE;
        }

        lay_ptr = lay_ptr->next;
        var_ptr = var_ptr->next;
    }

    return FALSE;
}

gboolean
xkb_preferences_layout_variant_append(XKB_Preferences *user_prefs, gchar *lay, gchar *var) {
    if (!xkb_preferences_layout_variant_contains(user_prefs, lay, var)) {

        user_prefs->layouts = g_slist_append(user_prefs->layouts, lay);
        user_prefs->variants = g_slist_append(user_prefs->variants, var);
        return TRUE;
    }
    return FALSE;
}

gboolean
xkb_preferences_layout_variant_contains(XKB_Preferences *user_prefs, gchar *lay, gchar *var) {
    GSList *lay_ptr = user_prefs->layouts;
    GSList *var_ptr = user_prefs->variants;

    while (lay_ptr != NULL) {
        if ((strcmp(lay_ptr->data, lay) == 0) &&
                (strcmp(var_ptr->data, var) == 0)) {


            return TRUE;
        }

        lay_ptr = lay_ptr->next;
        var_ptr = var_ptr->next;
    }
    return FALSE;
}