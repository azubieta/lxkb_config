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

/* Sets data to the keys options combo box */
void
fill_cbox_keys(GtkWidget *combo) {

    rules = xkb_xorg_get_rules();


    OptionGroup* opt_grp = xkb_rules_get_option_group(rules, "grp", NULL);

    // apppend None option
    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), _("None"));

    GSList *it = opt_grp->options;

    Option *o;
    gint item_count = 1, item_active = 0;
    while (it != NULL) {
        o = it->data;
        if (xkb_preferences_option_is_set(user_prefs, o->id)) {
            item_active = item_count;
        }
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), o->description);
        it = it->next;
        item_count++;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), item_active);
}

/* Sets data to the layouts combo box */
void
fill_cbox_models(GtkWidget *combo) {

    rules = xkb_xorg_get_rules();

    GSList *it = rules->models;
    Model *m;
    gint item_count = 0, item_active = -1;
    while (it != NULL) {

        m = it->data;
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), m->description);
        if (strcmp(m->id, user_prefs->model) == 0)
            item_active = item_count;

        it = it->next;
        item_count++;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), item_active);
}

/* Change the keys option in user prefs */
void
combo_keys_selected(GtkWidget *widget, void *user_data) {
    gchar *text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

    OptionGroup * opt_grp = xkb_rules_get_option_group(rules, "grp", NULL);
    opt_grp->multiple_selection = FALSE;
    Option * option = xkb_rules_get_option(opt_grp, NULL, text);

    if (strcmp(text, _("None")) == 0)
        xkb_preferences_option_unset_group(user_prefs, opt_grp);


    if (option != NULL) {
        // remove it if setted
        xkb_preferences_option_unset(user_prefs, opt_grp, option);
        xkb_preferences_option_set(user_prefs, opt_grp, option);
    }
}

/* Change the model in user prefs */
void
combo_model_selected(GtkWidget *widget, void *user_data) {
    gchar *text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

    Model * model = xkb_rules_get_model(rules, NULL, text);
    if (model != NULL)
        user_prefs->model = model->id;
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

    // Option Keyboard Switch 
    GtkFrame *keys_frame = GTK_FRAME(gtk_frame_new(_("Key(s) to change layout:")));
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 4);

    gtk_container_add(GTK_CONTAINER(keys_frame), GTK_WIDGET(vbox));

    gtk_widget_set_size_request(GTK_WIDGET(keys_frame), 330, 50);

    gtk_container_add(GTK_CONTAINER(tab->tab_content), GTK_WIDGET(keys_frame));

    GtkWidget *keys_image = gtk_image_new_from_icon_name("keyboard", GTK_ICON_SIZE_DIALOG);

    GtkWidget *keys_label = gtk_label_new(
            _("Select a key combination to switch between layouts\n"
            "or just select 'None'."));

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    gtk_container_add(GTK_CONTAINER(hbox), keys_image);
    gtk_container_add(GTK_CONTAINER(hbox), keys_label);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    tab->keys_cbox = gtk_combo_box_new_text();
    gtk_widget_set_size_request(GTK_WIDGET(tab->keys_cbox), 330, 30);

    fill_cbox_keys(tab->keys_cbox);
    g_signal_connect(G_OBJECT(tab->keys_cbox), "changed",
            G_CALLBACK(combo_keys_selected), NULL);


    gtk_container_add(GTK_CONTAINER(vbox), tab->keys_cbox);


    // keyboard model frame
    GtkFrame *model_frame = GTK_FRAME(gtk_frame_new(_("Keyboard Model:")));

    gtk_widget_set_size_request(GTK_WIDGET(model_frame), 330, 50);

    gtk_container_add(GTK_CONTAINER(tab->tab_content), GTK_WIDGET(model_frame));

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 4);

    gtk_container_add(GTK_CONTAINER(model_frame), GTK_WIDGET(vbox));

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    GtkWidget *warning_image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_DIALOG);


    GtkWidget *warning_label = gtk_label_new(_("WARNING: "
            "Be aware that changing these settings may\n"
            "result in system malfunction."));

    gtk_container_add(GTK_CONTAINER(hbox), warning_image);
    gtk_container_add(GTK_CONTAINER(hbox), warning_label);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);


    tab->model_cbox = gtk_combo_box_new_text();
    gtk_widget_set_size_request(GTK_WIDGET(tab->model_cbox), 330, 30);

    fill_cbox_models(tab->model_cbox);
    g_signal_connect(G_OBJECT(tab->model_cbox), "changed",
            G_CALLBACK(combo_model_selected), NULL);

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    gtk_container_add(GTK_CONTAINER(hbox), tab->model_cbox);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);



    // Global configuration
    GtkFrame * global_config_frame = GTK_FRAME(gtk_frame_new(_("Global Configuration:")));
    gtk_widget_set_size_request(GTK_WIDGET(global_config_frame), 330, 50);
    gtk_container_add(GTK_CONTAINER(tab->tab_content), GTK_WIDGET(global_config_frame));



    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 4);
    gtk_container_add(GTK_CONTAINER(global_config_frame), hbox);

    GtkWidget *global_cfg_image = gtk_image_new_from_stock(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_DIALOG);
    gtk_container_add(GTK_CONTAINER(hbox), global_cfg_image);

    vbox = gtk_vbox_new(FALSE, INNER_SPACE);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 4);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);

    GtkWidget *msg_label = gtk_label_new(_(
            "Apply this keyboard configuration not only to your\n"
            "session, but also to the whole system."));
    gtk_container_add(GTK_CONTAINER(vbox), msg_label);

    GtkWidget *button_global;
    button_global = gtk_button_new_with_label(_("Apply Globaly"));

    g_signal_connect(button_global, "clicked",
            G_CALLBACK(button_global_callback), NULL);

    gtk_container_add(GTK_CONTAINER(vbox), button_global);


    return tab;
}

