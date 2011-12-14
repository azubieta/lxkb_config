#include <string.h>
#include <gtk/gtk.h>

#include "system.h"

#include "local_structures.h"
#include "manage_user_preferences.h"
#include "rules_parser.h"
#include "gui_gtk.h"
#include "setxkbmap_interface.h"


extern GSList *preferences;
extern KB_Rules *rules;

void showMainWindow(int argc, char** argv) {
    MainWindow *window;
    window = g_slice_new0(MainWindow);

    gtk_init(&argc, &argv);

    // Main window 
    window->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window->window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window->window), 400, 400);
    gtk_window_set_title(GTK_WINDOW(window->window), _("Keyboard layout configuration tool"));

    g_signal_connect(window->window, "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    // Main container 
    window->suppor_table = gtk_table_new(13, 8, FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window->suppor_table), 4);

    gtk_container_add(GTK_CONTAINER(window->window), window->suppor_table);

    window->prefered_dist_label = gtk_label_new(_("Prefered distributions:     "));

    gtk_table_attach_defaults(GTK_TABLE(window->suppor_table), window->prefered_dist_label, 0, 4, 0, 1);

    window->treeview = gtk_tree_view_new();
    setup_tree_view(window->treeview);
    gtk_widget_set_size_request(window->treeview, 300, 200);

    window->store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    update_list_store(window->store);

    gtk_tree_view_set_model(GTK_TREE_VIEW(window->treeview), GTK_TREE_MODEL(window->store));
    g_object_unref(window->store);

    gtk_table_attach_defaults(GTK_TABLE(window->suppor_table), window->treeview, 1, 8, 1, 5);

    // adding loyaut list buttons 
    window->button_default = gtk_button_new_with_label(_("Default"));
    window->button_delete = gtk_button_new_with_label(_("Delete"));

    g_signal_connect(window->button_default, "clicked",
            G_CALLBACK(button_default_callback), window);

    g_signal_connect(window->button_delete, "clicked",
            G_CALLBACK(button_delete_callback), window);

    gtk_table_attach(GTK_TABLE(window->suppor_table), window->button_default, 
                        6, 7, 5, 6, GTK_FILL, GTK_SHRINK, 2,2);
    gtk_table_attach(GTK_TABLE(window->suppor_table), window->button_delete, 
                7, 8, 5, 6, GTK_FILL, GTK_SHRINK, 2,2);
    // Separator 1
    window->hseparator1 = gtk_hseparator_new();
    gtk_table_attach_defaults(GTK_TABLE(window->suppor_table), window->hseparator1, 4, 8, 6, 7);


    // New distribution section 
    window->new_distribution_label = gtk_label_new(_("Add a new distribution: "));
    
    window->layout_label = gtk_label_new(_("Layout:"));
    window->variant_label = gtk_label_new(_("Variant: "));

    gtk_table_attach_defaults(GTK_TABLE(window->suppor_table), window->new_distribution_label, 0, 4, 6, 7);
    
    gtk_table_attach(GTK_TABLE(window->suppor_table), window->layout_label, 
                        1, 4, 7, 8, GTK_EXPAND, GTK_SHRINK, 0,0);
    gtk_table_attach_defaults(GTK_TABLE(window->suppor_table), window->variant_label, 6, 7, 7, 8);


    window->layout_cbox = gtk_combo_box_new_text();
    window->variant_cbox = gtk_combo_box_new_text();

    gtk_widget_set_size_request(window->layout_cbox, 100, 40);
    gtk_widget_set_size_request(window->variant_cbox, 200, 40);
    fill_cbox_layouts(window->layout_cbox);

    gtk_table_attach(GTK_TABLE(window->suppor_table), window->layout_cbox,
                                1, 4, 8, 9, GTK_FILL, GTK_SHRINK, 2,2);
    
    gtk_table_attach(GTK_TABLE(window->suppor_table), window->variant_cbox, 
                                5, 8, 8, 9, GTK_FILL, GTK_SHRINK, 2,2);

    g_signal_connect(G_OBJECT(window->layout_cbox), "changed",
            G_CALLBACK(combo_selected), (gpointer) window->variant_cbox);

    window->button_add_config = gtk_button_new_with_label(_("Add"));
    
    gtk_table_attach(GTK_TABLE(window->suppor_table), window->button_add_config, 
                6, 8, 9, 10, GTK_FILL, GTK_SHRINK, 2,2);

    g_signal_connect(window->button_add_config, "clicked",
            G_CALLBACK(button_add_config_callback), window);

    // separator 2 
    window->hseparator2 = gtk_hseparator_new();
    gtk_table_attach_defaults(GTK_TABLE(window->suppor_table), window->hseparator2, 0, 8, 11, 12);

    // Global buttons
    window->button_cancel = gtk_button_new_with_label(_("Cancel"));
    window->button_acept = gtk_button_new_with_label(_("Acept"));


    window->button_aplic = gtk_button_new_with_label(_("Apply"));
    g_signal_connect(window->button_acept, "clicked",
            G_CALLBACK(button_acept_callback), NULL);

    g_signal_connect(window->button_aplic, "clicked",
            G_CALLBACK(button_aplic_callback), NULL);

    g_signal_connect(window->button_cancel, "clicked",
            G_CALLBACK(gtk_main_quit), NULL);

    // packing buttons 
    gtk_table_attach(GTK_TABLE(window->suppor_table), window->button_cancel,
                        5, 6, 12, 13, GTK_FILL, GTK_SHRINK, 2,2);
    gtk_table_attach(GTK_TABLE(window->suppor_table), window->button_acept, 
                        6, 7, 12, 13, GTK_FILL, GTK_SHRINK, 2,2);
    gtk_table_attach(GTK_TABLE(window->suppor_table), window->button_aplic,
                        7, 8, 12, 13, GTK_FILL, GTK_SHRINK, 2,2);



    gtk_widget_show_all(window->window);

    gtk_main();
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
        rules = load_KB_rules();
    }
    GSList *it = rules->layouts;
    Layout *l;

    while (it != NULL) {
        l = it->data;
        gtk_combo_box_append_text(GTK_COMBO_BOX(combo), l->description);

        it = it->next;
    }
}

/* Sets data to the variants combo box */
void
fill_cbox_variants(GtkWidget *combo, gchar *layout) {
    if (rules == NULL) {
        rules = load_KB_rules();
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

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}

/* Updates the data contained by list store */
void
update_list_store(GtkListStore *store) {

    gtk_list_store_clear(store);

    GtkTreeIter iter;

    PairLayoutVariant *data;

    GSList *it = preferences;
    while (it != NULL) {
        data = it->data;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, LAYOUT, data->layoutDesc,
                VARIANT, data->variantDesc, -1);

        it = it->next;
    }
}

void
button_add_config_callback(GtkWidget *widget, gpointer data) {
    MainWindow *w = data;
    gchar *layoutDesc, *variantDesc;
    layoutDesc = gtk_combo_box_get_active_text(GTK_COMBO_BOX(w->layout_cbox));
    variantDesc = gtk_combo_box_get_active_text(GTK_COMBO_BOX(w->variant_cbox));

    if (layoutDesc == NULL || variantDesc == NULL)
        return;

    PairLayoutVariant *element;
    element = config_get_by_descriptions(rules, layoutDesc, variantDesc);

    preferences = config_add_element(preferences, element);
    update_list_store(w->store);
}

void
button_delete_callback(GtkWidget *widget, gpointer data) {
    MainWindow *w = data;

    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;

    /* This will only work in single or browse selection mode! */

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(w->treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *layoutDesc;
        gchar *variantDesc;

        gtk_tree_model_get(model, &iter, 0, &layoutDesc, -1);
        gtk_tree_model_get(model, &iter, 1, &variantDesc, -1);

        PairLayoutVariant *lv = config_get_by_descriptions(rules, layoutDesc, variantDesc);

        preferences = config_remove_element(preferences, lv);
        update_list_store(w->store);

        //g_print("selected row is: %s %s\n", layoutDesc, variantDesc);

        g_free(layoutDesc);
        g_free(variantDesc);
    }
}

void
button_default_callback(GtkWidget *widget, gpointer data) {
    MainWindow *w = data;

    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;

    /* This will only work in single or browse selection mode! */

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(w->treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gchar *layoutDesc;
        gchar *variantDesc;

        gtk_tree_model_get(model, &iter, 0, &layoutDesc, -1);
        gtk_tree_model_get(model, &iter, 1, &variantDesc, -1);

        PairLayoutVariant *lv = config_get_by_descriptions(rules, layoutDesc, variantDesc);

        preferences = config_remove_element(preferences, lv);
        preferences = config_add_element_in_top(preferences, lv);
        update_list_store(w->store);

        //g_print("selected row is: %s %s\n", layoutDesc, variantDesc);

        g_free(layoutDesc);
        g_free(variantDesc);
    }
}

void
button_aplic_callback(GtkWidget *widget, gpointer data) {
    set_layout_and_variant(preferences);
    save_user_preferences(preferences);
}

void
button_acept_callback(GtkWidget *widget, gpointer data) {
    set_layout_and_variant(preferences);
    save_user_preferences(preferences);
    gtk_main_quit();
}
