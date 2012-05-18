/* system.h: system-dependent declarations; include this first.

   Copyright 1996, 2005, 2006, 2007, 2008 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef LXKB_CONFIG_SYSTEM_H
#define LXKB_CONFIG_SYSTEM_H

#include <config.h>

/* Assume ANSI C89 headers are available.  */
#include <locale.h>
#include <libintl.h>

#include "gettext.h"

/* Use POSIX headers.  If they are not available, we use the substitute
   provided by gnulib.  */
#include <getopt.h>
#include <unistd.h>

#define _(STRING)    gettext(STRING)

#endif /* LXKB_CONFIG_SYSTEM_H */
