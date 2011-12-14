/* 
 * File:   manage_user_preferences.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 */

#include "local_structures.h"

#ifndef MANAGE_USER_PREFERENCES_H
#define	MANAGE_USER_PREFERENCES_H

/* 
 * IN: Nothing.
 * OUT: Structure XKB_Preferences containing
 *      the stored preferences.
 * 
 * DESCRIPTION: Loads usign gconf 
 *              the user preferences.
 */
XKB_Preferences * 
load_user_preferences() ;


/* 
 * IN: GLIST of PairLayoutVariant structure
 * OUT: VOID
 * 
 * DESCRIPTION: Stores the user preferences
 *              in using gconf.
 */
void save_user_preferences(XKB_Preferences *prefs);

/* IN: List of preferences, structure of rules
 *      part of the new configuration: layout
 *      description and variant description.
 * 
 * OUT: The new preferences list.
 * 
 * DESCRIPTION: Checks that the new configuration
 *      is not already in the list and adds it.
 *     
 *      You have to be aware of pass a valid 
 *      configuration !!!
 */
GSList *config_add_element(GSList *preferences, PairLayoutVariant *newconf);

/* IN: List of preferences, structure of rules
 *      part of the new configuration: layout
 *      description and variant description.
 * 
 * OUT: The new preferences list.
 * 
 * DESCRIPTION: Checks that the new configuration
 *      is not already in the list and adds it in 
 *      the top of the list.
 *     
 *      You have to be aware to pass a valid 
 *      configuration !!!
 */
GSList *
config_add_element_in_top(GSList *preferences, PairLayoutVariant *newconf);

/* IN: List of preferences and the
 *     configuration that will be
 *     deleted.
 * 
 * OUT: The new preferences list.
 * 
 * DESCRIPTION: Erase a configuration
 *      from the list.
 */
GSList *
config_remove_element(GSList *preferences, PairLayoutVariant *element);

/* IN: List of preferences and the
 *     id of the configuration that 
 *     will be deleted.
 * 
 * OUT: The new preferences list.
 * 
 * DESCRIPTION: Erase a configuration
 *      from the list.
 */
GSList *
config_remove_element_by_position(GSList *preferences, guint pos);
/* IN: List of preferences and the
 *     configuration that will be
 *     deleted.
 * 
 * OUT: TRUE -- if was found other
 *      way returns FALSE.
 * 
 * DESCRIPTION: Search for a given
 *      config.
 */
gboolean 
config_contain_element(GSList *preferences, PairLayoutVariant *element);

/* IN: - Struct of rules,
 *     - Description of the layout
 *     - Description of the variant
 * 
 * OUT: PairLayoutVariant representing
 *      the configuration.
 * 
 * DESCRIPTION: Search for a given
 *      configuration using the data
 *      suplied.
 */
PairLayoutVariant *
config_get_by_descriptions(XKB_Rules *rules, gchar *layoutDesc, gchar *variantDesc);

/* IN: - Struct of rules,
 *     - ID of the layout
 *     - ID of the variant
 * 
 * OUT: PairLayoutVariant representing
 *      the configuration.
 * 
 * DESCRIPTION: Search for a given
 *      configuration using the data
 *      suplied.
 */
PairLayoutVariant *
config_get_by_ids(XKB_Rules *rules, gchar *layoutId, gchar *variantId);
#endif	/* MANAGE_USER_PREFERENCES_H */

/* 
 * Methods over layouts and variants 
 */

gboolean
prefs_layout_variant_remove(XKB_Preferences *user_prefs, gchar *lay, gchar *var);

gboolean
prefs_layout_variant_set_main(XKB_Preferences *user_prefs, gchar *lay, gchar *var);

gboolean
prefs_layout_variant_add(XKB_Preferences *user_prefs, gchar *lay, gchar *var);

gboolean
prefs_layout_variant_contains(XKB_Preferences *user_prefs, gchar *lay, gchar *var);