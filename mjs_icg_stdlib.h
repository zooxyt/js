/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Standard Library
 * Copyright(C) 2014 Cheryl Natsu

 * This file is part of multiple - Multiple Paradigm Language Emulator

 * multiple is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * multiple is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#ifndef _MJS_ICG_STDLIB_H_
#define _MJS_ICG_STDLIB_H_

#include <stdio.h>

#include "mjs_icg_stdlib_hdl.h"
#include "mjs_icg_stdlib_math.h"

struct mjs_icg_stdlib_field
{
    char *name;
    size_t len;

    int used;

    struct mjs_icg_stdlib_field *next;
};

struct mjs_icg_stdlib_field_list
{
    struct mjs_icg_stdlib_field *begin;
    struct mjs_icg_stdlib_field *end;

    size_t size;
};


struct mjs_icg_stdlib_table
{
    char *name;
    size_t len;

    struct mjs_icg_stdlib_field_list *fields;

    struct mjs_icg_stdlib_table *next;
};


struct mjs_icg_stdlib_table_list
{
    struct mjs_icg_stdlib_table *begin;
    struct mjs_icg_stdlib_table *end;

    size_t size;
};
struct mjs_icg_stdlib_table_list *mjs_icg_stdlib_table_list_new(void);
int mjs_icg_stdlib_table_list_destroy(struct mjs_icg_stdlib_table_list *table_list);
int mjs_icg_stdlib_table_list_register(struct mjs_icg_stdlib_table_list *table_list, \
        char *table_name, size_t table_name_len, \
        char *field_name, size_t field_name_len);


extern struct mjs_icg_add_built_in_table_handler mjs_icg_add_built_in_table_handlers[];

#endif

