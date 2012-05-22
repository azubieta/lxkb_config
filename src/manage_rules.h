/* 
 * File:   rules_parser.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 *
 */
#include "data_structures.h"

#ifndef RULES_PARSER_H
#define	RULES_PARSER_H


/* 
 * Loads the data stored in rules_path:
 * Returs an structure with the keyboard system rules.
 */

XKB_Rules * xkb_xorg_get_rules();

/* 
 * Loads the data stored in rules_path: models, layouts, 
 * variants and options.
 * 
 * You shuld use xkb_xorg_get_rules instead of this
 */
XKB_Rules* xkb_rules_load();


/* 
 * Description: Finds a variant in a layout.
 * Input: Layout structure, variant identificator.
 * Output: If success a variant pointer
 *         else NULL;
 */
Variant *
xkb_rules_layout_get_variant(Layout *layout, gchar *var_id, gchar *var_desc);

/* 
 * Description: Finds a layout in rules.
 * Input: Rules.
 *        layout identificator, layout description,
 *        one of thes could be NULL but not both.
 * Output: If success a layout pointer
 *         else NULL;
 */
Layout *
xkb_rules_get_layout(XKB_Rules *rules, gchar *lay_id, gchar *lay_desc);

/* 
 * Description: Finds an option group in rules.
 * Input: Rules.
 *        option group identificator, option group description,
 *        one of thes could be NULL but not both.
 * Output: If success a option group pointer
 *         else NULL;
 */
OptionGroup *
xkb_rules_get_option_group(XKB_Rules *rules, gchar *opt_grp_id, gchar *opt_grp_desc);

/* 
 * Description: Finds a group in an option group.
 * Input: option group.
 *        option identificator, option description,
 *        one of thes could be NULL but not both.
 * Output: If success a group pointer
 *         else NULL;
 */
Option *
xkb_rules_get_option(OptionGroup *opt_grp, gchar *opt_id, gchar *opt_desc);

/* 
 * Description: Finds an model in rules.
 * Input: Rules.
 *        model identificator, model description,
 *        one of thes could be NULL but not both.
 * Output: If success a option group pointer
 *         else NULL;
 */

Model *
xkb_rules_get_model(XKB_Rules *rules, gchar *mod_id, gchar *mod_desc);
#endif	/* RULES_PARSER_H */

