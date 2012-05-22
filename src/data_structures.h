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
 * Inner lists contains shuld have only
 * the ids of the elements.
 */
typedef struct {
    gchar *model;       // id of the model
    GSList *layouts;    // ids of layous    \_ The layout id and the variant id
    GSList *variants;   // ids of variants  /  must be in the same position.
    GSList *options;    // ids of options
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

