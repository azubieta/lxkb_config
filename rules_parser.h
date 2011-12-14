/* 
 * File:   rules_parser.h
 * Author: Alexis L'opez Zubieta
 * e-mail: azubieta@estudiantes.uci.cu
 *
 */
#include "local_structures.h"

#ifndef RULES_PARSER_H
#define	RULES_PARSER_H
/* Default path for rules*/
#define rules_path_lst "/usr/share/X11/xkb/rules/xfree86.lst"
#define rules_path_xml "/usr/share/X11/xkb/rules/xfree86.xml"

/* 
 * Loads the data stored in rules_path:
 * models, layouts, variants and options.
 */
KB_Rules* load_KB_rules();

// TODO Implement this
/* Release the struct KB_Rules
 * */
void free_KB_rules(KB_Rules *kb_rules);

// TODO Implement this
/* Gets the description of an specific Option
 * or NULL when it's not found.
 */
char * get_option_description(char *global, char* especific, KB_Rules *kb_rules);

// TODO Implement this
/* Gets the description of an specific Layout
 * or NULL when it's not found.
 * */
char * get_layout_description(char *id, KB_Rules *kb_rules);

// TODO Implement this
/* Gets the description of an specific Variant
 * or NULL when it's not found.
 * */
char * get_variant_description(char *id, KB_Rules *kb_rules);

#endif	/* RULES_PARSER_H */

