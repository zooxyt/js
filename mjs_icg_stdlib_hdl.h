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

#ifndef _MJS_ICG_STDLIB_HDL_H_
#define _MJS_ICG_STDLIB_HDL_H_

#include <stdio.h>

#include "multiple_ir.h"
#include "multiple_err.h"

#include "multiply.h"

/* Field Handler */

enum mjs_icg_add_built_in_field_handler_type
{
    MJS_BUILT_IN_METHOD = 0,
    MJS_BUILT_IN_PROPERTY,
    MJS_BUILT_IN_FINISH,
};

struct mjs_icg_add_built_in_field_handler
{
    enum mjs_icg_add_built_in_field_handler_type type;
    const char *name;
    const size_t name_len;

    int (*func)(struct multiple_error *err, \
            struct multiple_ir *multiple_ir, \
            struct multiply_resource_id_pool *res_id);
};

struct mjs_icg_add_built_in_field_handler *mjs_icg_add_built_in_field_handler_lookup( \
        struct mjs_icg_add_built_in_field_handler *field_handler_start, \
        char *name, size_t len);


/* Table Handler */

struct mjs_icg_add_built_in_table_handler
{
    const char *name;
    const size_t name_len;
    struct mjs_icg_add_built_in_field_handler *field_handler;
};

struct mjs_icg_add_built_in_table_handler *mjs_icg_add_built_in_table_handler_lookup( \
        struct mjs_icg_add_built_in_table_handler *table_handler_start, \
        char *name, size_t len);


#endif

