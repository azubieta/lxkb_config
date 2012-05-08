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
 * models, layouts, variants and options.
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

#endif	/* RULES_PARSER_H */

