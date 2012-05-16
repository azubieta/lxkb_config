/* 
 * File:   gtk+2.x.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 */

#ifndef GTK_2_X
#define	GTK_2_X

// Sizes of the widget buttons
#define BUTTON_HIGH 30
#define BUTTON_WIDTH 70

#include <gtk/gtk.h>
#include "data_structures.h"

typedef struct {
    GtkWidget *tab_name;
    GtkWidget *tab_content;

    GtkWidget *prefered_dist_label;
    GtkWidget *treeview;
    GtkListStore *store;

    GtkWidget *button_default, * button_delete, *button_new, *button_edit;


    GtkWidget *button_add_config;
} Distribution_Tab;

typedef struct {
    GtkWidget *tab_name;
    GtkWidget *tab_content;
    
} About_Tab;

typedef struct {
    GtkWidget *tab_name;
    GtkWidget *tab_content;
    
    GtkWidget * model_cbox;
    GtkWidget * keys_cbox;
} Others_Tab;

typedef struct {
    GtkWidget *name;
    GtkWidget *content;
    gchar *active_layout, *active_variant;
    GtkWidget *test_field;
    GtkWidget *layout_cbox, *variant_cbox;

    XKB_Preferences * tmp_prefs;
} Distribution_Dialog;


extern void
showMainWindow(int argc, char** argv);

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* GTK_2_X */

