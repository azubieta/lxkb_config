/* File:   local_structures.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 */
#include <glib-2.0/glib.h>

#ifndef LOCAL_STRUCTURES_H
#define	LOCAL_STRUCTURES_H


typedef struct {
    gchar* id;
    gchar* description;
} Model;

typedef struct {
    gchar* id;
    gchar* description;
    GSList *variant;
} Layout;

typedef struct {
    gchar* id;
    gchar* description;
} Variant;

typedef struct {
    gchar* id;
    gchar* description;
} Option;


typedef struct opt {
    GString* id;
    GString* description;
    gboolean multiple_selection;
    GSList* options;
} OptionGroup;


/**
 * Stores a set of preferences.
 * Inner lists contains must contains
 * Pair structures.
 **/
typedef struct {
    gchar *model;
    GSList *layouts;
    GSList *variants;
    GSList *options;
} XKB_Preferences;

/** 
 * Stores all the information from rules_path.
 * The option names havent arguments.
 * Exp: grp
 **/
typedef struct {
    GSList *modules;
    GSList *layouts;
    GSList *options;
} XKB_Rules;


/* Holds the data that is shown 
 * in the menu and saved as user
 * preferences.
 */
typedef struct {
    gchar *layoutId;
    gchar *layoutDesc;
    gchar *variantId;
    gchar *variantDesc;
} PairLayoutVariant;

/* INPUT: KB_Rules structure, a layout description 
 * OUTPUT: void
 * DESCRIPTION: Adds a layout to the list */
void add_layout(XKB_Rules *rules, Layout *l);


#endif	/* LOCAL_STRUCTURES_H */

