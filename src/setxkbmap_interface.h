/* 
 * File:   setxkbmap_interface.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 */

#ifndef SETXKBMAP_INTERFACE_H
#define	SETXKBMAP_INTERFACE_H

/* Description: This method calls setxkbmap whit the 
 *              specific arguments to set the keyboard
 *              preferences.
 * Input: Struct with the user preferences.
 * Output: Nothing.
 */
gboolean
xkb_preferences_set_to_env(XKB_Preferences *prefs);

/* Desription: This method obtains the user preferences
 *             by parsing the setxkbmap -v 10 output.
 * Input: None.
 * Output: Structure containing the user preferences.
 */

XKB_Preferences *
xkb_preferences_load_from_env();

/* Description: This method generates a call tosetxkbmap 
 *              whit the specific arguments to set the 
 *              keyboard preferences.
 * Input: Struct with the user preferences.
 * Output: Command line instructuion.
 */
gchar* generate_setxkbmap_command(XKB_Preferences *prefs);

#endif	/* SETXKBMAP_INTERFACE_H */

