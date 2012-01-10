/* 
 * File:   manage_user_preferences.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 */

#include "data_structures.h"

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
xkb_preferences_load_from_gconf();


/* 
 * IN: GLIST of PairLayoutVariant structure
 * OUT: VOID
 * 
 * DESCRIPTION: Stores the user preferences
 *              in using gconf.
 */
gboolean xkb_preferences_save_to_gconf(XKB_Preferences *prefs);

/* 
 * Methods over layouts and variants 
 */

gboolean
xkb_preferences_layout_variant_remove(XKB_Preferences *user_prefs, gchar *lay, gchar *var);

gboolean
xkb_preferences_layout_variant_set_main(XKB_Preferences *user_prefs, gchar *lay, gchar *var);

gboolean
xkb_preferences_layout_variant_append(XKB_Preferences *user_prefs, gchar *lay, gchar *var);

gboolean
xkb_preferences_layout_variant_contains(XKB_Preferences *user_prefs, gchar *lay, gchar *var);

#endif	/* MANAGE_USER_PREFERENCES_H */

