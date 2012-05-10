#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "manage_user_preferences.h"
#include "data_structures.h"
#include "manage_rules.h"

/* Desription: This method generates an autostart file
 *             that calls setxkbmap with the arguments
 *             needed to set up the passed preferences.
 * Input: User preferences.
 * Output: True on success, else False.
 */
gboolean save_prefs_to_autostart(XKB_Preferences *user_prefs) {
    
    gchar * cmd = (gchar *) generate_setxkbmap_command(user_prefs);

    char* dirname;
    const char grp[] = "Desktop Entry";
    GKeyFile* kf;

    char* file, *data;
    gsize len;


    /* create user autostart dir */
    dirname = g_build_filename(g_get_user_config_dir(), "autostart", NULL);
    g_mkdir_with_parents(dirname, 0700);
    g_free(dirname);

    kf = g_key_file_new();

    g_key_file_set_string(kf, grp, "Type", "Application");
    g_key_file_set_string(kf, grp, "Icon", "preferences-desktop-keyboard");
    g_key_file_set_string(kf, grp, "Name", _("Keyboard Settings setup"));
    g_key_file_set_string(kf, grp, "Comment", _("Sets up your preferred keyboard settings"));
    g_key_file_set_string(kf, grp, "Exec", cmd);
    g_key_file_set_string(kf, grp, "OnlyShowIn", "LXDE");

    data = g_key_file_to_data(kf, &len, NULL);
    file = g_build_filename(g_get_user_config_dir(),
            "autostart",
            "keyboard-settings-autostart.desktop",
            NULL);
    
    /* save it to user-specific autostart dir */
    g_debug("saved to: %s", file);

    g_file_set_contents(file, data, len, NULL);
    g_key_file_free(kf);
    g_free(file);
    g_free(data);

    return TRUE;
}