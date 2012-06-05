#include <string.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "system.h"
#include "data_structures.h"
#include "manage_user_preferences.h"
#include "manage_rules.h"
#include "setxkbmap_interface.h"
#include "gtk+2.x.h"

extern XKB_Preferences *user_prefs;
extern XKB_Rules *rules;

#define INNER_SPACE 4

/*
 * GTK_DIALOG Layout add
 */

/* Sets data to the variants combo box */
void
fill_cbox_variants(GtkWidget *combo, gchar *layout) {

    rules = xkb_xorg_get_rules();


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

void
update_tmp_prefs(New_Layout_Dialog *dist_dialog) {
    g_slist_free(dist_dialog->tmp_prefs->layouts);
    g_slist_free(dist_dialog->tmp_prefs->variants);
    dist_dialog->tmp_prefs->layouts = NULL;
    dist_dialog->tmp_prefs->variants = NULL;

    if ((dist_dialog->active_layout == NULL) || (dist_dialog->active_variant == NULL))
        return;

    Layout *layout = xkb_rules_get_layout(rules, NULL, dist_dialog->active_layout);
    Variant *variant = xkb_rules_layout_get_variant(layout, NULL, dist_dialog->active_variant);

    if ((layout == NULL) || (variant == NULL) || (layout->id == NULL) || (variant->id == NULL)) {
        return;
    }


    dist_dialog->tmp_prefs->layouts = g_slist_append(dist_dialog->tmp_prefs->layouts, layout->id);
    dist_dialog->tmp_prefs->variants = g_slist_append(dist_dialog->tmp_prefs->variants, variant->id);

    gtk_entry_set_text(GTK_ENTRY(dist_dialog->test_field), (gchar *) "");

    xkb_preferences_set_to_env(dist_dialog->tmp_prefs);
}

/* Notify to the variants combo box 
 * to switch the data*/
void
combo_layout_selected(GtkWidget *widget, New_Layout_Dialog *dist_dialog) {
    gchar *text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
    dist_dialog->active_layout = text;

    fill_cbox_variants(dist_dialog->variant_cbox, text);
    gtk_combo_box_set_active(GTK_COMBO_BOX(dist_dialog->variant_cbox), 0);

    //update_tmp_prefs(dist_dialog);
}

void
combo_variant_selected(GtkWidget *widget, New_Layout_Dialog *dist_dialog) {
    gchar *text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

    dist_dialog->active_variant = text;

    update_tmp_prefs(dist_dialog);
}

/* Sets data to the layouts combo box */
void
fill_cbox_layouts(GtkWidget *combo) {

    rules = xkb_xorg_get_rules();

    GSList *it = rules->layouts;
    Layout *l;

    while (it != NULL) {
        l = it->data;
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), l->description);

        it = it->next;
    }
}

/* Function to open a dialog box displaying the message provided. */
GtkWidget *
build_dialog_new_layout(GtkWidget *parent, New_Layout_Dialog *dist_dialog) {


    /* Create the widgets */
    GtkWidget *hbox, *vbox, *dialog;
    dialog = gtk_dialog_new_with_buttons(_("Pick a keyboard layout"),
            GTK_WINDOW(parent),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_STOCK_OK,
            GTK_RESPONSE_ACCEPT,
            GTK_STOCK_CANCEL,
            GTK_RESPONSE_REJECT,
            NULL);

    gtk_widget_set_size_request(dialog, 450, 180);
    dist_dialog->content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 4);

    GtkFrame * frame = GTK_FRAME(gtk_frame_new(_("Layouts:")));
    gtk_widget_set_size_request(GTK_WIDGET(frame), 300, 50);
    gtk_container_add(GTK_CONTAINER(dist_dialog->content), GTK_WIDGET(frame));

    vbox = gtk_vbox_new(FALSE, INNER_SPACE);
    gtk_container_add(GTK_CONTAINER(frame), vbox);



    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    GtkWidget *layout_label = gtk_label_new(_("Layout:"));
    GtkWidget *variant_label = gtk_label_new(_("Variant: "));

    gtk_container_add(GTK_CONTAINER(hbox), layout_label);
    gtk_container_add(GTK_CONTAINER(hbox), variant_label);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);


    dist_dialog->layout_cbox = gtk_combo_box_entry_new_text();
    dist_dialog->variant_cbox = gtk_combo_box_new_text();

    gtk_widget_set_size_request(dist_dialog->layout_cbox, 100, 40);
    gtk_widget_set_size_request(dist_dialog->variant_cbox, 200, 40);
    fill_cbox_layouts(dist_dialog->layout_cbox);

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);
    gtk_container_add(GTK_CONTAINER(hbox), dist_dialog->layout_cbox);
    gtk_container_add(GTK_CONTAINER(hbox), dist_dialog->variant_cbox);

    gtk_widget_set_size_request(dist_dialog->layout_cbox, 70, 30);
    gtk_widget_set_size_request(dist_dialog->variant_cbox, 70, 30);

    gtk_container_add(GTK_CONTAINER(vbox), hbox);

    g_signal_connect(G_OBJECT(dist_dialog->layout_cbox), "changed",
            G_CALLBACK(combo_layout_selected), dist_dialog);

    g_signal_connect(G_OBJECT(dist_dialog->variant_cbox), "changed",
            G_CALLBACK(combo_variant_selected), dist_dialog);

    hbox = gtk_hbox_new(FALSE, INNER_SPACE);

    // Testa area

    GtkWidget *label = gtk_label_new(_("Test here:"));
    dist_dialog->test_field = gtk_entry_new_with_max_length(50);

    gtk_container_add(GTK_CONTAINER(hbox), GTK_WIDGET(label));
    gtk_container_add(GTK_CONTAINER(hbox), GTK_WIDGET(dist_dialog->test_field));

    gtk_widget_set_size_request(label, 70, 30);
    gtk_widget_set_size_request(dist_dialog->test_field, 340, 30);

    gtk_container_add(GTK_CONTAINER(dist_dialog->content), GTK_WIDGET(hbox));
    gtk_widget_show_all(dist_dialog->content);

    return dialog;
}

/* 
 * GTK_TREE_VIEW Layouts
 */

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

        if (layout != NULL) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                    LAYOUT, layout->description,
                    VARIANT, variant->description, -1);
        }

        lay_it = lay_it->next;
        var_it = var_it->next;
    }
}

void
button_new_config_callback(GtkWidget *widget, gpointer data) {
    Layouts_Tab *tab = data;

    New_Layout_Dialog *dist_dialog = g_slice_alloc0(sizeof (New_Layout_Dialog));
    dist_dialog->tmp_prefs = g_slice_alloc0(sizeof (XKB_Preferences));
    GtkWidget *dialog = build_dialog_new_layout(NULL, dist_dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    Layout *layout;
    Variant *variant;
    if ((result == GTK_RESPONSE_ACCEPT) &&
            (dist_dialog->active_layout != NULL && dist_dialog->active_variant != NULL)) {

        layout = xkb_rules_get_layout(rules, NULL, dist_dialog->active_layout);
        variant = xkb_rules_layout_get_variant(layout, NULL, dist_dialog->active_variant);

        
        if (layout == NULL || variant == NULL)
            return;
        xkb_preferences_layout_variant_append(user_prefs, layout->id, variant->id);
        list_store_update(tab->store);
    } else {
    }

    /* TODO: Memory clean must be performed */

    xkb_preferences_set_to_env(user_prefs);

    gtk_widget_destroy(dialog);
}

void
button_delete_callback(GtkWidget *widget, gpointer data) {
    Layouts_Tab *w = data;

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
    Layouts_Tab *w = data;

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

        g_free(layoutDesc);
        g_free(variantDesc);
    }
}

/*
 * Tab_Page Layouts
 */
Layouts_Tab *
build_tab_layouts() {
    // Initialization
    Layouts_Tab *tab = g_slice_new0(Layouts_Tab);
    tab->tab_name = gtk_label_new(_("Layouts"));
    tab->tab_content = gtk_vbox_new(FALSE, INNER_SPACE);
    gtk_container_set_border_width(GTK_CONTAINER(tab->tab_content), 8);

    // Utils
    GtkWidget *span;

    GtkWidget *hbox, *vbox;

    tab->treeview = gtk_tree_view_new();
    setup_tree_view(tab->treeview);
    gtk_widget_set_size_request(tab->treeview, 250, 300);

    tab->store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    list_store_update(tab->store);

    gtk_tree_view_set_model(GTK_TREE_VIEW(tab->treeview), GTK_TREE_MODEL(tab->store));
    g_object_unref(tab->store);

    gtk_container_add(GTK_CONTAINER(tab->tab_content), tab->treeview);

    hbox = gtk_hbox_new(FALSE, 4);

    tab->button_default = gtk_button_new_from_stock(GTK_STOCK_GOTO_TOP);
    gtk_widget_set_size_request(tab->button_default, BUTTON_WIDTH, BUTTON_HIGH);

    tab->button_delete = gtk_button_new_from_stock(GTK_STOCK_REMOVE);
    gtk_widget_set_size_request(tab->button_delete, BUTTON_WIDTH, BUTTON_HIGH);

    tab->button_new = gtk_button_new_from_stock(GTK_STOCK_ADD);
    gtk_widget_set_size_request(tab->button_new, BUTTON_WIDTH, BUTTON_HIGH);

    g_signal_connect(tab->button_default, "clicked",
            G_CALLBACK(button_default_callback), tab);
    g_signal_connect(tab->button_delete, "clicked",
            G_CALLBACK(button_delete_callback), tab);
    g_signal_connect(tab->button_new, "clicked",
            G_CALLBACK(button_new_config_callback), tab);


    span = gtk_label_new("");
    gtk_widget_set_size_request(span, 10, 0);

    gtk_container_add(GTK_CONTAINER(hbox), tab->button_default);
    gtk_container_add(GTK_CONTAINER(hbox), span);

    gtk_container_add(GTK_CONTAINER(hbox), tab->button_new);

    gtk_container_add(GTK_CONTAINER(hbox), tab->button_delete);

    gtk_container_add(GTK_CONTAINER(tab->tab_content), hbox);


    return tab;
}

