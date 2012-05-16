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
    gchar* id;
    gchar* description;
    gboolean multiple_selection;
    GSList* options;
} OptionGroup;


/**
 * Stores a set of preferences.
 * Inner lists contains must contains
 * Pair structures.
 **/
typedef struct {
    Model *model;
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
    GSList *models;
    GSList *layouts;
    GSList *options;
} XKB_Rules;


#endif	/* LOCAL_STRUCTURES_H */

