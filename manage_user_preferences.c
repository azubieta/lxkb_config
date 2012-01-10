#include <stdio.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <stdlib.h>
#include <string.h>

#include "manage_user_preferences.h"
#include "data_structures.h"
#include "manage_rules.h"

XKB_Preferences *
xkb_preferences_load_from_gconf() {
    GConfEngine *engine;
    GError *error = NULL;

    XKB_Preferences *prefs = g_slice_alloc0(sizeof (XKB_Preferences));

    g_type_init();

    engine = gconf_engine_get_default();

    prefs->model = gconf_engine_get_string(engine, "/desktop/lxde/xkeyboard/model", &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/model\" :%s", error->message);
        g_slice_free1(sizeof(prefs), prefs);
        return NULL;
    }

    prefs->layouts = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/layouts", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/layouts\" :%s", error->message);
        g_slice_free1(sizeof(prefs), prefs);
        return NULL;
    }

    prefs->variants = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/variants", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/variants\" :%s", error->message);
        g_slice_free1(sizeof(prefs), prefs);
        return NULL;
    }

    prefs->options = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/options", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/options\" :%s", error->message);
        g_slice_free1(sizeof(prefs), prefs);
        return NULL;
    }

    return prefs;
}

gboolean
xkb_preferences_save_to_gconf(XKB_Preferences * prefs) {
    GConfEngine *engine;
    g_type_init();
    engine = gconf_engine_get_default();
    GError *error = NULL;

    if (prefs->model != NULL) {
        gconf_engine_set_string(engine, "/desktop/lxde/xkeyboard/model", prefs->model, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/model\": %s", error->message);
            return FALSE;
        }
    }
    if ((prefs->variants != NULL) && (prefs->layouts != NULL)) {
        gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/layouts", GCONF_VALUE_STRING, prefs->layouts, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/layouts\": %s", error->message);
            return FALSE;
        }

        gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/variants", GCONF_VALUE_STRING, prefs->variants, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/variants\": %s", error->message);
            return FALSE;
        }
    }
    
    if (prefs->options != NULL) {
        gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/options", GCONF_VALUE_STRING, prefs->options, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/options\": %s", error->message);
            return FALSE;
        }
    }
    return TRUE;
}

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