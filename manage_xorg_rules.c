#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <glib-2.0/glib/glist.h>
#include <glib-2.0/glib/gstring.h>


#include "manage_rules.h"
#include "data_structures.h"

#include "system.h"

/* Default path for rules*/
#define rules_path_lst "/usr/share/X11/xkb/rules/xfree86.lst"
#define rules_path_xml "/usr/share/X11/xkb/rules/xfree86.xml"

void
parse_variant(xmlDocPtr doc, xmlNodePtr cur, Layout *l) {
    cur = cur->xmlChildrenNode;
    // Go into config item section
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "configItem"))) {
            cur = cur->children;
            break;
        }
        cur = cur->next;
    }

    Variant *v = malloc(sizeof (Variant));
    memset(v, 0, sizeof (Variant));

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "name"))) {
            v->id = (gchar *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //printf("id: %s\n", l->id);
            //xmlFree(id);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "description"))) {
            v->description = (char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //printf("Description: %s\n", l->description);
            //xmlFree(id);
        }
        cur = cur->next;
    }


    if (v->id != NULL && v->description != NULL)
        l->variant = g_slist_append(l->variant, (gpointer) v);
    else
        free(v);
}

void
parse_variants_list(xmlDocPtr doc, xmlNodePtr cur, Layout *l) {

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "variant"))) {
            parse_variant(doc, cur, l);
        }
        cur = cur->next;
    }
}

void
parse_layout(xmlDocPtr doc, xmlNodePtr cur, XKB_Rules *rules) {
    xmlNodePtr parent = cur->xmlChildrenNode;
    cur = parent;
    // Go into config item section
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "configItem"))) {
            cur = cur->children;
            break;
        }
        cur = cur->next;
    }

    Layout *l = malloc(sizeof (Layout));
    memset(l, 0, sizeof (Layout));

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "name"))) {
            l->id = (gchar *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //printf("id: %s\n", l->id);
            //xmlFree(id);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "description"))) {
            l->description = (char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //printf("Description: %s\n", l->description);
            //xmlFree(id);
        }
        cur = cur->next;
    }

    // Adding the default variant "" 
    Variant *v = g_slice_new0(Variant);
    v->id = "";
    v->description = _("Default");
    l->variant = g_slist_append(l->variant, v);

    cur = parent;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "variantList"))) {
            parse_variants_list(doc, cur, l);
            break;
        }
        cur = cur->next;
    }

    if (l->id != NULL && l->description != NULL)
        rules->layouts = g_slist_append(rules->layouts, (gpointer) l);
    else
        free(l);
}

void
parse_layout_list(xmlDocPtr doc, xmlNodePtr cur, XKB_Rules *rules) {

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "layout"))) {
            parse_layout(doc, cur, rules);
        }
        cur = cur->next;
    }
}

XKB_Rules*
xkb_rules_load() {

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(rules_path_xml);

    if (doc == NULL) {
        fprintf(stderr, "Document not parsed successfully. \n");
        return NULL;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "xkbConfigRegistry")) {
        fprintf(stderr, "document of the wrong type, root node != xkbConfigRegistry");
        xmlFreeDoc(doc);
        return NULL;
    }

    cur = cur->xmlChildrenNode;

    XKB_Rules *rules = malloc(sizeof (XKB_Rules));
    memset(rules, 0, sizeof (XKB_Rules));


    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "layoutList"))) {
            parse_layout_list(doc, cur, rules);
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return rules;
}

Layout *
xkb_rules_get_layout(XKB_Rules *rules, gchar *lay_id, gchar *lay_desc) {
    if ((lay_id == NULL) && (lay_desc == NULL)) {
        return NULL;
    }

    GSList *lay_it = rules->layouts;
    Layout *layout;

    while (lay_it != NULL) {
        layout = lay_it->data;

        if ((lay_id != NULL) && (strcmp(layout->id, lay_id)) == 0)
            return layout;

        if ((lay_desc != NULL) && (strcmp(layout->description, lay_desc)) == 0)
            return layout;

        lay_it = lay_it->next;
    }
    return NULL;
}

Variant *
xkb_rules_layout_get_variant(Layout *layout, gchar *var_id, gchar *var_desc) {
    if ((var_id == NULL) && (var_desc == NULL)) {
        return layout->variant->data;
    }

    GSList *var_it = layout->variant;
    Variant *variant;

    while (var_it != NULL) {
        variant = var_it->data;

        if ((var_id != NULL) && (strcmp(variant->id, var_id)) == 0)
            return variant;

        if ((var_desc != NULL) && (strcmp(variant->description, var_desc)) == 0)
            return variant;


        var_it = var_it->next;
    }
    return NULL;
}