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

gboolean
xkb_preferences_option_is_set(XKB_Preferences *user_prefs, gchar *opt_id) {
    GSList *opt_it = user_prefs->options;
    gboolean founded = FALSE;
    gchar *id;
    while (opt_it != NULL && !founded) {
        id = opt_it->data;
        if (strcmp(id, opt_id) == 0) {
            founded = TRUE;
        }
        opt_it = opt_it->next;
    }
    return founded;
}

gboolean
belongs_to_group(gchar *grp_id, gchar *opt_id) {
    gint i;
    for (i = 0; *(grp_id + i) != 0; i++) {
        if (*(grp_id + i) != *(opt_id + i))
            return FALSE;
    }
    return TRUE;
}

void
xkb_preferences_option_unset_group(XKB_Preferences *user_prefs, OptionGroup *opt_grp) {
    GSList *opt_it = user_prefs->options, *old_it;
    gchar *opt_id;
    // remove other options of the group if not multiple_selection
    while (opt_it != NULL) {
        opt_id = opt_it->data;
        //printf("%s\n", opt_id);
        if (belongs_to_group(opt_grp->id, opt_id)) {
            old_it = opt_it;
            opt_it = opt_it->next;
            user_prefs->options = g_slist_delete_link(user_prefs->options, old_it);
        } else
            opt_it = opt_it->next;
    }
}

gboolean
xkb_preferences_option_set(XKB_Preferences *user_prefs, OptionGroup *opt_grp, Option *opt) {
    // remove other options of the group if not multiple_selection
    if (!opt_grp->multiple_selection) {
        xkb_preferences_option_unset_group(user_prefs, opt_grp);    
    }
    user_prefs->options = g_slist_append(user_prefs->options, opt->id);
}

gboolean
xkb_preferences_option_unset(XKB_Preferences *user_prefs, OptionGroup *opt_grp, Option *opt) {
    GSList *opt_it = user_prefs->options;
    gboolean founded = FALSE;
    gchar *opt_id;
    while (opt_it != NULL && !founded) {
        opt_id = opt_it->data;
        if (strcmp(opt->id, opt_id) == 0) {
            founded = TRUE;
        }
        opt_it = opt_it->next;
    }
    if (founded) {
        user_prefs->options = g_slist_remove(user_prefs->options, opt_id);
        return TRUE;
    } else
        return FALSE;


}
