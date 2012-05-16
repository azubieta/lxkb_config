#include <string.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "system.h"
#include "data_structures.h"
#include "manage_user_preferences.h"
#include "manage_rules.h"
#include "setxkbmap_interface.h"
#include "gtk+2.x.h"

#define INNER_SPACE 4

/*
 * Tab_Page Credits
 */
About_Tab *
build_tab_credits() {
    // Initialization
    About_Tab *tab = g_slice_new0(About_Tab);
    tab->tab_name = gtk_label_new(_("About"));
    tab->tab_content = gtk_vbox_new(FALSE, INNER_SPACE);
    gtk_container_set_border_width(GTK_CONTAINER(tab->tab_content), 8);
    GtkWidget *credits = gtk_label_new(
            "Developers:\n"
            "Alexis López Zubieta --> azubieta@estudiantes.uci.cu\n\n"
            "Repository:\n"
            "https://github.com/azubieta/lxkb_config\n\n"
            "Last version:\n"
            "https://github.com/azubieta/lxkb_config/downloads"
            );
    gtk_container_add(GTK_CONTAINER(tab->tab_content), credits);  
    return tab;
}

