#include <string.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libxml2/libxml/parser.h>

#include "system.h"
#include "data_structures.h"
#include "manage_user_preferences.h"
#include "manage_rules.h"
#include "setxkbmap_interface.h"
#include "gtk+2.x.h"

#define INNER_SPACE 4

extern XKB_Preferences *user_prefs;
extern XKB_Rules *rules;

/* Sets data to the layouts combo box */
void
fill_cbox_models(GtkWidget *combo) {
    if (rules == NULL) {
        rules = xkb_rules_load();
    }
    GSList *it = rules->models;
    Model *m;
    gint item_count = 0, item_active = -1;
    while (it != NULL) {

        m = it->data;
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), m->description);
        if (strcmp(m->id, user_prefs->model->id) == 0)
            item_active = item_count;

        it = it->next;
        item_count++;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), item_active);
}

/* Change the model in user prefs */
void
combo_model_selected(GtkWidget *widget, void *user_data) {
    gchar *text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

    Model * model = (Model *) xkb_rules_get_model(rules, NULL, text);
    if (model != NULL)
        user_prefs->model = model;
}

void
button_global_callback(GtkWidget *widget, gpointer data) {
    gboolean result;
    GtkDialog * dialog;
    result = xkb_preferences_write_xorg_config(user_prefs);
    /*
        if (result) {
            dialog = gtk_message_dialog_new(NULL,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_INFO,
                    GTK_BUTTONS_CLOSE,
                    _("The configuration was applied successfully."));
        } else {
            dialog = gtk_message_dialog_new(NULL,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_CLOSE,
                    _("Error while applying the configuration."));
        }

        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(GTK_WIDGET(dialog));*/
}

/*
 * Tab_Page Other Options
 */
Others_Tab *
build_tab_others() {
    GtkWidget *hbox, *vbox;
    // Initialization
    Others_Tab *tab = g_slice_new0(Others_Tab);
    tab->tab_name = gtk_label_new(_("Other Options"));
    tab->tab_content = gtk_vbox_new(FALSE, INNER_SPACE);
    gtk_container_set_border_width(GTK_CONTAINER(tab->tab_content), 8);

    GtkFrame *model_frame = GTK_FRAME(gtk_frame_new(_("Keyboard Model:")));
    
    gtk_widget_set_size_request(GTK_WIDGET(model_frame), 100, 40);

    gtk_container_add(GTK_CONTAINER(tab->tab_content), GTK_WIDGET(model_frame));

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 4);
    
    gtk_container_add(GTK_CONTAINER(model_frame), GTK_WIDGET(vbox));

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    GtkWidget *warning_image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_DIALOG);


    GtkWidget *warning_label = gtk_label_new(_("WARNING: \n"
            "Be aware that changing these settings may result in \n"
            "system malfunction."));

    gtk_container_add(GTK_CONTAINER(hbox), warning_image);
    gtk_container_add(GTK_CONTAINER(hbox), warning_label);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);


    GtkWidget *model_label = gtk_label_new(_("Model: "));
    tab->model_cbox = gtk_combo_box_entry_new_text();
    gtk_widget_set_size_request(GTK_WIDGET(tab->model_cbox), 330, 10);

    fill_cbox_models(tab->model_cbox);
    g_signal_connect(G_OBJECT(tab->model_cbox), "changed",
            G_CALLBACK(combo_model_selected), NULL);

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    gtk_container_add(GTK_CONTAINER(hbox), model_label);
    gtk_container_add(GTK_CONTAINER(hbox), tab->model_cbox);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    GtkWidget *span = gtk_label_new(" ");
    gtk_container_add(GTK_CONTAINER(tab->tab_content), span);

    // Option Keyboard Switch 

    // Global config
    GtkWidget *button_global;
    button_global = gtk_button_new_with_label(_("Apply to System"));
    gtk_widget_set_size_request(button_global, 90, BUTTON_HIGH);

    gtk_container_add(GTK_CONTAINER(tab->tab_content), button_global);
    g_signal_connect(button_global, "clicked",
            G_CALLBACK(button_global_callback), NULL);

    return tab;
}

