/* 
 * File:   gtk+2.x.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 */

#ifndef GTK_2_X
#define	GTK_2_X

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *tab_name; 
    GtkWidget *tab_content;
    
    GtkWidget *prefered_dist_label;
    GtkWidget *treeview;
    GtkListStore *store;
    
    GtkWidget *button_default, * button_delete;
    GtkWidget *layout_cbox, *variant_cbox;
    
    GtkWidget *button_add_config;
} Distribution_Tab;




extern void
showMainWindow(int argc, char** argv);

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* GTK_2_X */

