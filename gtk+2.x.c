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

/*
gint
signal_handler_event(GtkWidget *widget, GdkEventButton *event, gpointer func_data) {
    if (GTK_IS_LIST_ITEM(widget) &&
            (event->type == GDK_2BUTTON_PRESS ||
            event->type == GDK_3BUTTON_PRESS)) {
        printf("I feel clicked on button");
    }

    return FALSE;
}
 */

void
button_global_callback(GtkWidget *widget, gpointer data) {
    gboolean result;
    GtkDialog * dialog;
    result = xkb_preferences_write_xorg_config(user_prefs);

    if (result) {
        dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_CLOSE,
                "The configuration was applied successfully.");
    } else {
        dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Error while applying the configuration.");
    }

    gtk_dialog_run(GTK_DIALOG(dialog));    
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
button_aplic_callback(GtkWidget *widget, gpointer data) {
    GtkDialog * dialog;
    gboolean result;
    result = xkb_preferences_set_to_system(user_prefs);
    if (result)
        result = xkb_preferences_save_to_gconf(user_prefs);

    if (result) {
        dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_CLOSE,
                "The configuration was applied successfully.");
    } else {
        dialog = gtk_message_dialog_new(NULL,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Error while applying the configuration.");
    }

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void
button_acept_callback(GtkWidget *widget, gpointer data) {
    xkb_preferences_set_to_system(user_prefs);
    xkb_preferences_save_to_gconf(user_prefs);
    gtk_main_quit();
}

void
button_cancel_callback(GtkWidget *widget, gpointer data) {
    user_prefs = xkb_preferences_load_from_gconf();
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

    g_signal_connect(window, "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 10);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *tabs = gtk_notebook_new();

    gtk_container_add(GTK_CONTAINER(vbox), tabs);

    Distribution_Tab *distab = (Distribution_Tab *) build_distribution_tab();

    gtk_notebook_append_page(GTK_NOTEBOOK(tabs), distab->tab_content, distab->tab_name);

    /*
     * Adding Control Buttons
     */

    GtkWidget *control_box = gtk_hbox_new(FALSE, 4);
    gtk_container_add(GTK_CONTAINER(vbox), control_box);

    GtkWidget *span, *button_cancel, *button_accept, * button_aplic, *button_global;

    span = gtk_label_new("");
    gtk_widget_set_size_request(span, 80, 20);

    button_cancel = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
    button_accept = gtk_button_new_from_stock(GTK_STOCK_OK);
    button_aplic = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    button_global = gtk_button_new_with_label(_("Apply to System"));

    gtk_container_add(GTK_CONTAINER(control_box), button_global);
    gtk_container_add(GTK_CONTAINER(control_box), span);

    gtk_container_add(GTK_CONTAINER(control_box), button_accept);
    gtk_container_add(GTK_CONTAINER(control_box), button_aplic);
    gtk_container_add(GTK_CONTAINER(control_box), button_cancel);


    g_signal_connect(button_global, "clicked",
            G_CALLBACK(button_global_callback), NULL);


    g_signal_connect(button_accept, "clicked",
            G_CALLBACK(button_acept_callback), NULL);

    g_signal_connect(button_aplic, "clicked",
            G_CALLBACK(button_aplic_callback), NULL);

    g_signal_connect(button_cancel, "clicked",
            G_CALLBACK(button_cancel_callback), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}