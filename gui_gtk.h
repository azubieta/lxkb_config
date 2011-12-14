/* 
 * File:   gui_gtk.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 */
#include <gtk/gtk.h>

#ifndef GUI_GTK_H
#define	GUI_GTK_H

typedef struct {
    GtkWidget *window, *suppor_table, *hseparator1, *hseparator2;
    GtkWidget *prefered_dist_label;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkWidget *button_default, * button_delete;
    GtkWidget *new_distribution_label;
    GtkWidget *layout_label, *variant_label;
    GtkWidget *layout_cbox, *variant_cbox;
    
    GtkWidget *button_cancel, *button_acept, * button_aplic;
    GtkWidget *button_add_config;
} MainWindow;

enum {
    LAYOUT = 0,
    VARIANT,
    COLUMNS
};

void showMainWindow(int argc, char** argv);

void
button_delete_callback(GtkWidget *widget, gpointer data);

void
button_default_callback(GtkWidget *widget, gpointer data);

void
button_aplic_callback(GtkWidget *widget, gpointer data);

void
button_acept_callback(GtkWidget *widget, gpointer data);

void
button_add_config_action(GtkWidget *button, gpointer *data);

void 
button_add_config_callback(GtkWidget *widget, gpointer data);

/* Add two columns to the GtkTreeView.*/
void setup_tree_view(GtkWidget *treeview);
/* Updates the data contained by list store */
void update_list_store(GtkListStore *store);

/* Sets data to the layouts combo box */
void fill_cbox_layouts(GtkWidget *combo);
/* Sets data to the variants combo box */
void fill_cbox_variants(GtkWidget *combo, gchar *layout);

void combo_selected(GtkWidget *widget, GtkWidget *cbox);
#endif	/* GUI_GTK_H */

