#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <glib-2.0/glib/glist.h>
#include <glib-2.0/glib/gstring.h>


#include "rules_parser.h"
#include "local_structures.h"

#include "system.h"


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
    // Adding the default variant "" 
    Variant *v = g_slice_new0(Variant);
    v->id = "";
    v->description = _("Default");
    l->variant = g_slist_append(l->variant, v);
    
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "variant"))) {
            parse_variant(doc, cur, l);
        }
        cur = cur->next;
    }
}

void
parse_layout(xmlDocPtr doc, xmlNodePtr cur, KB_Rules *rules) {
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
parse_layout_list(xmlDocPtr doc, xmlNodePtr cur, KB_Rules *rules) {

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "layout"))) {
            parse_layout(doc, cur, rules);
        }
        cur = cur->next;
    }
}

KB_Rules*
load_KB_rules() {

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

    KB_Rules *rules = malloc(sizeof (KB_Rules));
    memset(rules, 0, sizeof (KB_Rules));


    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "layoutList"))) {
            parse_layout_list(doc, cur, rules);
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return rules;
}

