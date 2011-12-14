#include <string.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "system.h"
#include "local_structures.h"
#include "manage_user_preferences.h"
#include "manage_rules.h"
#include "setxkbmap_interface.h"
#include "gtk+2.x.h"


extern GSList *preferences;
extern XKB_Preferences *user_prefs;
extern XKB_Rules *rules;

/* Sets data to the variants combo box */
void
fill_cbox_variants(GtkWidget *combo, gchar *layout) {
    if (rules == NULL) {
        rules = xkb_rules_load();
    }

    int nelemnts = gtk_tree_model_iter_n_children(
            gtk_combo_box_get_model(GTK_COMBO_BOX(combo)),
            NULL);
    int i = 0;
    for (i = 0; i < nelemnts; i++) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(combo), 0);
    }


    GSList *it = rules->layouts;
    Layout *l;

    while (it != NULL) {
        l = it->data;
        if (strcmp(l->description, layout) == 0)
            break;
        it = it ->next;
    }
    if (it == NULL) {
        fprintf(stderr, "ERROR: %s is not a valid layout.\n", layout);
        return;
    }
    it = l->variant;
    Variant *v;
    while (it != NULL) {
        v = it->data;
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), v->description);

        it = it->next;
    }
}

/* Notify to the variants combo box 
 * to switch the data*/
void
combo_selected(GtkWidget *widget, GtkWidget *cbox) {
    gchar *text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
    fill_cbox_variants(cbox, text);
    g_free(text);
}

/* Sets data to the layouts combo box */
void
fill_cbox_layouts(GtkWidget *combo) {
    if (rules == NULL) {
        rules = xkb_rules_load();
    }
    GSList *it = rules->layouts;
    Layout *l;

    while (it != NULL) {
        l = it->data;
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), l->description);

        it = it->next;
    }
}

enum {
    LAYOUT = 0,
    VARIANT,
    COLUMNS
};

/* Add two columns to the GtkTreeView.*/
void
setup_tree_view(GtkWidget *treeview) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    /* Create a new GtkCellRendererText, add it to the tree view column and
     * append the column to the tree view. */
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes
            (_("Layout"), renderer, "text", LAYOUT, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes
            (_("Variant"), renderer, "text", VARIANT, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

}

gint
signal_handler_event(GtkWidget *widget, GdkEventButton *event, gpointer func_data) {
    if (GTK_IS_LIST_ITEM(widget) &&
            (event->type == GDK_2BUTTON_PRESS ||
            event->type == GDK_3BUTTON_PRESS)) {
        printf("I feel clicked on button");
    }

    return FALSE;
}

/* Updates the data contained by list store */

void
list_store_update(GtkListStore *store) {

    gtk_list_store_clear(store);

    GtkTreeIter iter;

    GSList *lay_it = user_prefs->layouts;
    GSList *var_it = user_prefs->variants;

    Layout *layout;
    Variant *variant;

    while (lay_it != NULL) {
        layout = xkb_rules_get_layout(rules, lay_it->data, NULL);
        variant = xkb_rules_layout_get_variant(layout, var_it->data, NULL);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                LAYOUT, layout->description,
                VARIANT, variant->description, -1);

        lay_it = lay_it->next;
        var_it = var_it->next;
    }
}

void
button_add_config_callback(GtkWidget *widget, gpointer data) {
    Distribution_Tab *tab = data;
    gchar *layoutDesc, *variantDesc;
    layoutDesc = gtk_combo_box_get_active_text(GTK_COMBO_BOX(tab->layout_cbox));
    variantDesc = gtk_combo_box_get_active_text(GTK_COMBO_BOX(tab->variant_cbox));

    if (layoutDesc == NULL || variantDesc == NULL)
        return;

    Layout *layout = xkb_rules_get_layout(rules, NULL, layoutDesc);
    Variant *variant = xkb_rules_layout_get_variant(layout, NULL, variantDesc);

    xkb_preferences_layout_variant_append(user_prefs, layout->id, variant->id);
    list_store_update(tab->store);
}

void
button_delete_callback(GtkWidget *widget, gpointer data) {
    Distribution_Tab *w = data;

    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(w->treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *layoutDesc;
        gchar *variantDesc;

        gtk_tree_model_get(model, &iter, 0, &layoutDesc, -1);
        gtk_tree_model_get(model, &iter, 1, &variantDesc, -1);

        Layout *layout = xkb_rules_get_layout(rules, NULL, layoutDesc);
        Variant *variant = xkb_rules_layout_get_variant(layout, NULL, variantDesc);

        xkb_preferences_layout_variant_remove(user_prefs, layout->id, variant->id);
        list_store_update(w->store);

        //g_print("selected row is: %s %s\n", layoutDesc, variantDesc);

        g_free(layoutDesc);
        g_free(variantDesc);
    }
}

void
button_default_callback(GtkWidget *widget, gpointer data) {
    Distribution_Tab *w = data;

    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;



    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(w->treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *layoutDesc;
        gchar *variantDesc;

        gtk_tree_model_get(model, &iter, 0, &layoutDesc, -1);
        gtk_tree_model_get(model, &iter, 1, &variantDesc, -1);

        Layout *layout = xkb_rules_get_layout(rules, NULL, layoutDesc);
        Variant *variant = xkb_rules_layout_get_variant(layout, NULL, variantDesc);

        xkb_preferences_layout_variant_set_main(user_prefs, layout->id, variant->id);
        
        list_store_update(w->store);

        //g_print("selected row is: %s %s\n", layoutDesc, variantDesc);

        g_free(layoutDesc);
        g_free(variantDesc);
    }
}

void
button_aplic_callback(GtkWidget *widget, gpointer data) {
    xkb_preferences_set_to_system(user_prefs);
    xkb_preferences_save_to_gconf(user_prefs);
}

void
button_acept_callback(GtkWidget *widget, gpointer data) {
    xkb_preferences_set_to_system(user_prefs);
    xkb_preferences_save_to_gconf(user_prefs);
    gtk_main_quit();
}

#define INNER_SPACE 4

Distribution_Tab *
build_distribution_tab() {
    // Initialization
    Distribution_Tab *tab = g_slice_new0(Distribution_Tab);
    tab->tab_name = gtk_label_new(_("Distribution"));
    tab->tab_content = gtk_vbox_new(FALSE, INNER_SPACE);

    // Utils
    GtkWidget *span;

    GtkWidget *hbox, *vbox;

    vbox = gtk_vbox_new(FALSE, INNER_SPACE);

    GtkWidget * frame = gtk_frame_new(_("Prefered distributions:"));
    gtk_container_add(GTK_CONTAINER(tab->tab_content), frame);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    tab->treeview = gtk_tree_view_new();
    setup_tree_view(tab->treeview);
    gtk_widget_set_size_request(tab->treeview, 300, 200);

    tab->store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    list_store_update(tab->store);

    gtk_tree_view_set_model(GTK_TREE_VIEW(tab->treeview), GTK_TREE_MODEL(tab->store));
    g_object_unref(tab->store);

    gtk_container_add(GTK_CONTAINER(vbox), tab->treeview);

    hbox = gtk_hbox_new(FALSE, 4);

    tab->button_default = gtk_button_new_with_label(_("Default"));
    tab->button_delete = gtk_button_new_with_label(_("Delete"));

    g_signal_connect(tab->button_default, "clicked",
            G_CALLBACK(button_default_callback), tab);
    g_signal_connect(tab->button_delete, "clicked",
            G_CALLBACK(button_delete_callback), tab);

    span = gtk_label_new("");
    gtk_widget_set_size_request(span, 200, 25);
    gtk_container_add(GTK_CONTAINER(hbox), span);
    gtk_container_add(GTK_CONTAINER(hbox), tab->button_default);
    gtk_container_add(GTK_CONTAINER(hbox), tab->button_delete);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);


    frame = gtk_frame_new(_("Add a new distribution:"));
    gtk_container_add(GTK_CONTAINER(tab->tab_content), frame);
    vbox = gtk_vbox_new(FALSE, INNER_SPACE);
    gtk_container_add(GTK_CONTAINER(frame), vbox);

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    GtkWidget *layout_label = gtk_label_new(_("Layout:"));
    GtkWidget *variant_label = gtk_label_new(_("Variant: "));

    gtk_container_add(GTK_CONTAINER(hbox), layout_label);
    gtk_container_add(GTK_CONTAINER(hbox), variant_label);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);


    tab->layout_cbox = gtk_combo_box_new_text();
    tab->variant_cbox = gtk_combo_box_new_text();

    gtk_widget_set_size_request(tab->layout_cbox, 100, 40);
    gtk_widget_set_size_request(tab->variant_cbox, 200, 40);
    fill_cbox_layouts(tab->layout_cbox);

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    gtk_container_add(GTK_CONTAINER(hbox), tab->layout_cbox);
    gtk_container_add(GTK_CONTAINER(hbox), tab->variant_cbox);

    gtk_widget_set_size_request(tab->layout_cbox, 70, 30);
    gtk_widget_set_size_request(tab->variant_cbox, 70, 30);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    g_signal_connect(G_OBJECT(tab->layout_cbox), "changed",
            G_CALLBACK(combo_selected), (gpointer) tab->variant_cbox);


    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    tab->button_add_config = gtk_button_new_with_label(_("Add"));
    g_signal_connect(tab->button_add_config, "clicked",
            G_CALLBACK(button_add_config_callback), tab);


    span = gtk_label_new("");
    gtk_widget_set_size_request(span, 250, 25);
    gtk_container_add(GTK_CONTAINER(hbox), span);
    gtk_container_add(GTK_CONTAINER(hbox), tab->button_add_config);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    return tab;
}

void showMainWindow(int argc, char** argv) {
    gtk_init(&argc, &argv);

    GtkWindow *window;

    // Main window 
    window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 450, 450);
    gtk_window_set_title(GTK_WINDOW(window), _("Keyboard configuration tool"));

    g_signal_connect(window, "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 4);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *tabs = gtk_notebook_new();

    gtk_container_add(GTK_CONTAINER(vbox), tabs);

    Distribution_Tab *distab = build_distribution_tab();

    gtk_notebook_append_page(GTK_NOTEBOOK(tabs), distab->tab_content, distab->tab_name);

    /*
     * Adding Control Buttons
     */

    GtkWidget *control_box = gtk_hbox_new(FALSE, 2);
    gtk_container_add(GTK_CONTAINER(vbox), control_box);

    GtkWidget *span, *button_cancel, *button_accept, * button_aplic;

    span = gtk_label_new("");
    gtk_widget_set_size_request(span, 100, 25);
    gtk_container_add(GTK_CONTAINER(control_box), span);


    button_cancel = gtk_button_new_with_label(_("Cancel"));
    button_accept = gtk_button_new_with_label(_("Accept"));
    button_aplic = gtk_button_new_with_label(_("Apply"));

    gtk_container_add(GTK_CONTAINER(control_box), button_cancel);
    gtk_container_add(GTK_CONTAINER(control_box), button_accept);
    gtk_container_add(GTK_CONTAINER(control_box), button_aplic);


    gtk_widget_set_size_request(button_accept, 70, 25);
    gtk_widget_set_size_request(button_aplic, 70, 25);
    gtk_widget_set_size_request(button_cancel, 70, 25);


    g_signal_connect(button_accept, "clicked",
            G_CALLBACK(button_acept_callback), NULL);

    g_signal_connect(button_aplic, "clicked",
            G_CALLBACK(button_aplic_callback), NULL);

    g_signal_connect(button_cancel, "clicked",
            G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();
}