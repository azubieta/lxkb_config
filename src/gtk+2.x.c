#include <string.h>
#include <gtk/gtk.h>
#include <glib-2.0/glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "system.h"
#include "data_structures.h"

#include "gtk+2.x.h"

#include "setxkbmap_interface.h"
#include "manage_rules.h"
#include "manage_user_preferences.h"

extern XKB_Preferences *user_prefs;
extern XKB_Rules *rules;

void
button_aplic_callback(GtkWidget *widget, gpointer data) {
    GtkDialog * dialog;
    gboolean result;
    result = xkb_preferences_set_to_system(user_prefs);
    if (result)
        result = save_prefs_to_autostart(user_prefs);

    if (result) {
        dialog = GTK_DIALOG(gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_CLOSE,
                _("The configuration was applied successfully.")));
    } else {
        dialog = GTK_DIALOG(gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                _("Error while applying the configuration.")));
    }

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
button_acept_callback(GtkWidget *widget, gpointer data) {
    xkb_preferences_set_to_system(user_prefs);
    save_prefs_to_autostart(user_prefs);
    gtk_main_quit();
}

void
button_cancel_callback(GtkWidget *widget, gpointer data) {
    user_prefs = (XKB_Preferences *) xkb_preferences_load_from_env();
    xkb_preferences_set_to_system(user_prefs);
    gtk_main_quit();
}

void showMainWindow(int argc, char** argv) {
    gtk_init(&argc, &argv);

    GtkWindow *window;


    // Main window 
    window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 460, 450);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);
    gtk_window_set_title(GTK_WINDOW(window), _("Keyboard configuration tool"));

    /* TODO Set application icon
     GtkImage *app_icon = GTK_IMAGE(gtk_image_new_from_icon_name("keyboard", GTK_ICON_SIZE_SMALL_TOOLBAR));
     gtk_window_set_icon( window, gtk_image_get_pixbuf(app_icon));
     */
    g_signal_connect(window, "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 10);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *tabs = gtk_notebook_new();

    gtk_container_add(GTK_CONTAINER(vbox), tabs);

    /*
     * Adding tabs to the notebook
     */

    Distribution_Tab *tab_distributions = (Distribution_Tab *) build_tab_distribution();
    Others_Tab *tab_others = (Others_Tab *) build_tab_others();
    About_Tab *tab_credits = (About_Tab *) build_tab_credits();

    gtk_notebook_append_page(GTK_NOTEBOOK(tabs), tab_distributions->tab_content, tab_distributions->tab_name);
    gtk_notebook_append_page(GTK_NOTEBOOK(tabs), tab_others->tab_content, tab_others->tab_name);
    gtk_notebook_append_page(GTK_NOTEBOOK(tabs), tab_credits->tab_content, tab_credits->tab_name);
    /*
     * Adding Control Buttons
     */

    GtkWidget *control_box = gtk_hbox_new(FALSE, 4);
    gtk_container_add(GTK_CONTAINER(vbox), control_box);

    GtkWidget *span, *button_cancel, *button_accept, * button_aplic;

    span = gtk_label_new("");



    gtk_widget_set_size_request(span, 45, 0);

    button_cancel = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
    gtk_widget_set_size_request(button_cancel, BUTTON_WIDTH, BUTTON_HIGH);

    button_accept = gtk_button_new_from_stock(GTK_STOCK_OK);
    gtk_widget_set_size_request(button_accept, BUTTON_WIDTH, BUTTON_HIGH);

    button_aplic = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_widget_set_size_request(button_aplic, BUTTON_WIDTH, BUTTON_HIGH);

    gtk_container_add(GTK_CONTAINER(control_box), span);

    gtk_container_add(GTK_CONTAINER(control_box), button_accept);
    gtk_container_add(GTK_CONTAINER(control_box), button_aplic);
    gtk_container_add(GTK_CONTAINER(control_box), button_cancel);


    g_signal_connect(button_accept, "clicked",
            G_CALLBACK(button_acept_callback), NULL);

    g_signal_connect(button_aplic, "clicked",
            G_CALLBACK(button_aplic_callback), NULL);

    g_signal_connect(button_cancel, "clicked",
            G_CALLBACK(button_cancel_callback), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}