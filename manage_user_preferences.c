#include <stdio.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <stdlib.h>
#include <string.h>

#include "manage_user_preferences.h"
#include "local_structures.h"
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
        return NULL;
    }

    prefs->layouts = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/layouts", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/layouts\" :%s", error->message);
        return NULL;
    }

    prefs->variants = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/variants", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/variants\" :%s", error->message);
        return NULL;
    }

    prefs->options = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/options", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/options\" :%s", error->message);
        return NULL;
    }

    return prefs;
}

void
xkb_preferences_save_to_gconf(XKB_Preferences * prefs) {
    GConfEngine *engine;
    g_type_init();
    engine = gconf_engine_get_default();
    GError *error = NULL;

    if (prefs->model != NULL) {
        gconf_engine_set_string(engine, "/desktop/lxde/xkeyboard/model", prefs->model, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/model\": %s", error->message);
            return;
        }
    }
    if ((prefs->variants != NULL) && (prefs->layouts != NULL)) {
        gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/layouts", GCONF_VALUE_STRING, prefs->layouts, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/layouts\": %s", error->message);
            return;
        }

        gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/variants", GCONF_VALUE_STRING, prefs->variants, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/variants\": %s", error->message);
            return;
        }
    }
    if (prefs->options != NULL) {
        gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/options", GCONF_VALUE_STRING, prefs->options, &error);
        if (error != NULL) {
            fprintf(stderr, "ERROR while writing \"xkeyboard/options\": %s", error->message);
            return;
        }
    }
}

PairLayoutVariant *
config_get_by_ids(XKB_Rules *rules, gchar *layoutId, gchar *variantId) {
    GSList *it = rules->layouts;
    Layout *l = it != NULL ? it->data : NULL;

    while (it != NULL &&
            (strcmp(l->id, layoutId) != 0)) {
        it = it ->next;
        l = it != NULL ? it->data : NULL;
    }

    if (l == NULL)
        return NULL;

    it = l->variant;
    Variant *v = (it != NULL) ? it->data : NULL;
    while (it != NULL &&
            (strcmp(v->id, variantId) != 0)) {
        it = it ->next;
        v = (it != NULL) ? it->data : NULL;
    }

    if (v == NULL)
        return NULL;

    PairLayoutVariant *out = g_slice_new0(PairLayoutVariant);

    out->layoutId = l->id;
    out->layoutDesc = l->description;
    out->variantId = v->id;
    out->variantDesc = v->description;

    return out;
}

PairLayoutVariant *
config_get_by_descriptions(XKB_Rules *rules, gchar *layoutDesc, gchar *variantDesc) {
    GSList *it = rules->layouts;
    Layout *l = it != NULL ? it->data : NULL;

    while (it != NULL &&
            (strcmp(l->description, layoutDesc) != 0)) {
        it = it ->next;
        l = it != NULL ? it->data : NULL;
    }

    if (l == NULL)
        return NULL;

    it = l->variant;
    Variant *v = (it != NULL) ? it->data : NULL;
    while (it != NULL &&
            (strcmp(v->description, variantDesc) != 0)) {
        it = it ->next;
        v = (it != NULL) ? it->data : NULL;
    }

    if (v == NULL)
        return NULL;

    PairLayoutVariant *out = g_slice_new0(PairLayoutVariant);

    out->layoutId = l->id;
    out->layoutDesc = l->description;
    out->variantId = v->id;
    out->variantDesc = v->description;

    return out;
}

GSList *
config_add_element(GSList *preferences, PairLayoutVariant *newconf) {
    if (config_contain_element(preferences, newconf) == FALSE)
        return g_slist_append(preferences, newconf);
    else
        return preferences;
}

GSList *
config_add_element_in_top(GSList *preferences, PairLayoutVariant *newconf) {
    if (config_contain_element(preferences, newconf) == FALSE)
        return g_slist_insert(preferences, newconf, 0);
    else
        return preferences;
}

GSList *
config_remove_element(GSList *preferences, PairLayoutVariant *element) {
    GSList *it = preferences;
    PairLayoutVariant *data = it->data;

    while (it != NULL &&
            ((strcmp(data->layoutId, element->layoutId) != 0) ||
            (strcmp(data->variantId, element->variantId) != 0))) {
        it = it->next;
        data = it != NULL ? it->data : NULL;
    }
    if (it != NULL)
        return g_slist_delete_link(preferences, it);
    else
        return preferences;
}

GSList *
config_remove_element_by_position(GSList *preferences, guint pos) {
    GSList *it = preferences;
    while (it != NULL && pos > 0) {
        it = it->next;
    }
    if (it != NULL)
        return g_slist_delete_link(preferences, it);
    else
        return preferences;
}

gboolean
config_contain_element(GSList *preferences, PairLayoutVariant *element) {
    GSList *it = preferences;
    if (it == NULL)
        return FALSE;
    PairLayoutVariant *data = it->data;

    while (it != NULL &&
            ((strcmp(data->layoutId, element->layoutId) != 0) ||
            (strcmp(data->variantId, element->variantId) != 0))) {
        it = it->next;
        data = it != NULL ? it->data : NULL;
    }
    if (it == NULL)
        return FALSE;
    else
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