/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Standard Library : Handler
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

#include <stdio.h>
#include <string.h>

#include "mjs_icg_stdlib_hdl.h"

/* Field Handler */

struct mjs_icg_add_built_in_field_handler *mjs_icg_add_built_in_field_handler_lookup( \
        struct mjs_icg_add_built_in_field_handler *field_handler_start, \
        char *name, size_t len)
{
    struct mjs_icg_add_built_in_field_handler *field_handler_cur;

    field_handler_cur = field_handler_start;
    while (field_handler_cur != NULL)
    {
        if (field_handler_cur->name == NULL) break;

        if ((field_handler_cur->name_len == len) && \
                (strncmp(field_handler_cur->name, name, len) == 0))
        {
            return field_handler_cur;
        }
        field_handler_cur++;
    }

    return NULL;
}

struct mjs_icg_add_built_in_table_handler *mjs_icg_add_built_in_table_handler_lookup( \
        struct mjs_icg_add_built_in_table_handler *table_handler_start, \
        char *name, size_t len)
{
    struct mjs_icg_add_built_in_table_handler *table_handler_cur;

    table_handler_cur = table_handler_start;
    while (table_handler_cur != NULL)
    {
        if (table_handler_cur->name == NULL) break;

        if ((table_handler_cur->name_len == len) && \
                (strncmp(table_handler_cur->name, name, len) == 0))
        {
            return table_handler_cur;
        }
        table_handler_cur++;
    }

    return NULL;
}

