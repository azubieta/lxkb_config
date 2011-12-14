#include <stdio.h>
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <stdlib.h>
#include <string.h>

#include "manage_user_preferences.h"
#include "local_structures.h"
#include "rules_parser.h"

#include "gui_gtk.h"

GSList *
load_user_preferences(KB_Rules *rules) {
    GConfEngine *engine;
    GError *error = NULL;

    g_type_init();

    engine = gconf_engine_get_default();
    GSList *layouts = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/layouts", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/layouts\" :%s", error->message);
        return NULL;
    }
    GSList *variants = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/variants", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/variants\" :%s", error->message);
        return NULL;
    }

    GSList *out = NULL;
    PairLayoutVariant *lv = NULL;
    while (layouts != NULL) {

        lv = config_get_by_ids(rules, layouts->data, variants->data);

        if (lv != NULL)
            out = g_slist_append(out, lv);

        //printf("%s:%s\n", lv->layoutId, lv->variantId);

        layouts = layouts->next;

        if (variants != NULL)
            variants = variants->next;
    }
    return out;
}

GSList *
quick_load_user_preferences() {
    GConfEngine *engine;
    GError *error = NULL;

    g_type_init();

    engine = gconf_engine_get_default();
    GSList *layouts = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/layouts", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/layouts\" :%s", error->message);
        return NULL;
    }
    GSList *variants = gconf_engine_get_list(engine, "/desktop/lxde/xkeyboard/variants", GCONF_VALUE_STRING, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while reading \"xkeyboard/variants\" :%s", error->message);
        return NULL;
    }

    GSList *out = NULL;
    PairLayoutVariant *lv = NULL;
    while (layouts != NULL) {
        lv = g_slice_new0(PairLayoutVariant);
        lv->layoutId = layouts->data;
        lv->variantId = variants->data;

        out = g_slist_append(out, lv);

        layouts = layouts->next;

        if (variants != NULL)
            variants = variants->next;
    }
    return out;

}

void
save_user_preferences(GSList *preferences) {
    // Preparing data for store
    GSList *layoutsId = NULL;
    GSList *variantsId = NULL;

    GSList *it = preferences;
    PairLayoutVariant *lv = NULL;

    while (it != NULL) {
        lv = it->data;
        layoutsId = g_slist_append(layoutsId, lv->layoutId);
        variantsId = g_slist_append(variantsId, lv->variantId);

        it = it->next;
    }
    GConfEngine *engine;
    g_type_init();
    engine = gconf_engine_get_default();
    GError *error = NULL;

    gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/layouts", GCONF_VALUE_STRING, layoutsId, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while writing \"xkeyboard/layouts\": %s", error->message);
        return;
    }
    gconf_engine_set_list(engine, "/desktop/lxde/xkeyboard/variants", GCONF_VALUE_STRING, variantsId, &error);
    if (error != NULL) {
        fprintf(stderr, "ERROR while writing \"xkeyboard/variants\": %s", error->message);
        return;
    }
}

PairLayoutVariant *
config_get_by_ids(KB_Rules *rules, gchar *layoutId, gchar *variantId) {
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
config_get_by_descriptions(KB_Rules *rules, gchar *layoutDesc, gchar *variantDesc) {
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