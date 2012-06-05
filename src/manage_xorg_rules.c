#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>
#include <glib-2.0/glib.h>


#include "manage_rules.h"
#include "data_structures.h"

#include "system.h"

#include "config.h"


extern XKB_Rules *rules;

XKB_Rules * xkb_xorg_get_rules() {
    if (rules == NULL) {
        XKB_Rules * r = xkb_rules_load();
        if (r == NULL) {
            printf("ERROR: Unable to load rules from: %s\n", XKB_RULES_PATH);
            exit(EXIT_FAILURE);
        } else
            return r;
    } else
        return rules;

}

/* Description: Layout comparing function *
 * Usage: Use it as complement to g_slist_sort.
 * Input: Tow layouts
 * Output: 0 if they are equals, -1  if the first is 
 *         lesser than the second, else 1.
 */
gint compare_layouts(const gconstpointer a, const gconstpointer b) {
    return strcmp(((Layout *) a)->description, ((Layout *) b)->description);
}

/* Description: Model comparing function *
 * Usage: Use it as complement to g_slist_sort.
 * Input: Tow Models
 * Output: 0 if they are equals, -1  if the first is 
 *         lesser than the second, else 1.
 */
gint compare_models(const gconstpointer a, const gconstpointer b) {
    return strcmp(((Model *) a)->description, ((Model *) b)->description);
}

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
            v->description = _((char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
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
            l->description = _((char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
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
    /* Sort layouts */
    rules->layouts = g_slist_sort(rules->layouts, compare_layouts);
}

void
parse_model(xmlDocPtr doc, xmlNodePtr cur, XKB_Rules *rules) {
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

    Model *m = malloc(sizeof (Model));
    memset(m, 0, sizeof (Model));

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "name"))) {
            m->id = (gchar *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //xmlFree(id);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "description"))) {
            m->description = _((char *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
            //xmlFree(id);
        }
        cur = cur->next;
    }

    if (m->id != NULL && m->description != NULL)
        rules->models = g_slist_append(rules->models, (gpointer) m);
    else
        free(m);
}

void
parse_model_list(xmlDocPtr doc, xmlNodePtr cur, XKB_Rules *rules) {

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if (!xmlStrcmp(cur->name, (const xmlChar *) "model")) {
            parse_model(doc, cur, rules);
        }
        cur = cur->next;
    }
    /* Sort models */
    rules->models = g_slist_sort(rules->models, compare_models);
}

Option *
parse_group_option(xmlDocPtr doc, xmlNodePtr cur) {
    cur = cur->xmlChildrenNode;
    // go into configItem node
    cur = cur->next;
    cur = cur->xmlChildrenNode;


    Option *opt = malloc(sizeof (Option));
    memset(opt, 0, sizeof (Option));

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "name"))) {
            opt->id = (gchar *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //printf("id: %s\n", opt->id);
            //xmlFree(id);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "description"))) {
            opt->description = _((gchar *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
            //printf("Description: %s\n", opt->description);
            //xmlFree(id);
        }
        cur = cur->next;
    }
    return opt;
}

void
parse_group_info(xmlDocPtr doc, xmlNodePtr cur, OptionGroup * opt_group) {
    cur = cur->xmlChildrenNode;

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "name"))) {
            opt_group->id = (gchar *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            //printf("id: %s\n", opt_group->id);
            //xmlFree(id);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "description"))) {
            opt_group->description = _((gchar *) xmlNodeListGetString(doc, cur->xmlChildrenNode, 1));
            //printf("Description: %s\n", opt_group->description);
            //xmlFree(id);
        }
        cur = cur->next;
    }
}

void
parse_group(xmlDocPtr doc, xmlNodePtr cur, OptionGroup * opt_group) {
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "configItem"))) {
            parse_group_info(doc, cur, opt_group);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "option"))) {
            Option * opt = parse_group_option(doc, cur);
            if (opt != NULL) {
                opt_group->options = g_slist_append(opt_group->options, opt);
            }
        }
        cur = cur->next;
    }
}

void
parse_options_groups(xmlDocPtr doc, xmlNodePtr cur, XKB_Rules *rules) {
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {

        if (!xmlStrcmp(cur->name, (const xmlChar *) "group")) {
            OptionGroup * opt_group = malloc(sizeof (OptionGroup));
            memset(opt_group, 0, sizeof (OptionGroup));
            // read allowMultipleSelection attribute
            xmlAttr* attribute = cur->properties;
            while (attribute && attribute->name && attribute->children) {
                if (xmlStrcmp(attribute->name, "allowMultipleSelection") == 0) {
                    xmlChar* value = xmlNodeListGetString(cur->doc, attribute->children, 1);
                    if (xmlStrcmp(value, "true") == 0)
                        opt_group->multiple_selection = TRUE;
                    else
                        opt_group->multiple_selection = FALSE;

                    xmlFree(value);
                }
                attribute = attribute->next;
            }

            parse_group(doc, cur, opt_group);

            if (opt_group != NULL) {
                rules->options = g_slist_append(rules->options, opt_group);
            }
        }

        cur = cur->next;
    }

}

XKB_Rules*
xkb_rules_load() {
    fprintf(stderr, "DEBUG: Loading xkb rules from \"%s\"\n", XKB_RULES_PATH);

    textdomain("xkeyboard-config");

    xmlDocPtr doc;
    xmlNodePtr cur;

    // Path for rules are imported from config.h 
    doc = xmlParseFile(XKB_RULES_PATH);

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
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "modelList"))) {
            parse_model_list(doc, cur, rules);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "layoutList"))) {
            parse_layout_list(doc, cur, rules);
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *) "optionList"))) {
            parse_options_groups(doc, cur, rules);
        }
        cur = cur->next;
    }

    xmlFreeDoc(doc);
    textdomain(PACKAGE);
    return rules;
}

Model *
xkb_rules_get_model(XKB_Rules *rules, gchar *mod_id, gchar *mod_desc) {
    if ((mod_id == NULL) && (mod_desc == NULL)) {
        return NULL;
    }

    GSList *mod_it = rules->models;
    Model * model;

    while (mod_it != NULL) {
        model = mod_it->data;

        if ((mod_id != NULL) && (strcmp(model->id, mod_id)) == 0)
            return model;

        if ((mod_desc != NULL) && (strcmp(model->description, mod_desc)) == 0)
            return model;

        mod_it = mod_it->next;
    }
    return NULL;
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
    
    if (layout == NULL)
        return NULL;

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

OptionGroup *
xkb_rules_get_option_group(XKB_Rules *rules, gchar *opt_grp_id, gchar *opt_grp_desc) {
    if ((opt_grp_id == NULL) && (opt_grp_desc == NULL)) {
        return NULL;
    }

    GSList *opt_grp_it = rules->options;
    OptionGroup *opt_grp;

    while (opt_grp_it != NULL) {
        opt_grp = opt_grp_it->data;
        //printf("id: %s desc: %s \n", opt_grp->id, opt_grp->description);

        if ((opt_grp_id != NULL) && (strcmp(opt_grp->id, opt_grp_id)) == 0)
            return opt_grp;

        if ((opt_grp_desc != NULL) && (strcmp(opt_grp->description, opt_grp_desc)) == 0)
            return opt_grp;

        opt_grp_it = opt_grp_it->next;
    }
    return NULL;
}

Option *
xkb_rules_get_option(OptionGroup *opt_grp, gchar *opt_id, gchar *opt_desc) {
    if ((opt_id == NULL) && (opt_desc == NULL)) {
        return NULL;
    }

    GSList *opt_grp_it = opt_grp->options;
    Option *opt;

    while (opt_grp_it != NULL) {
        opt = opt_grp_it->data;

        if ((opt_id != NULL) && (strcmp(opt->id, opt_id)) == 0)
            return opt;

        if ((opt_desc != NULL) && (strcmp(opt->description, opt_desc)) == 0)
            return opt;

        opt_grp_it = opt_grp_it->next;
    }
    return NULL;
}
