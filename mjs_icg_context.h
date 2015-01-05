/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Global Context
   Copyright(C) 2014 Cheryl Natsu

   This file is part of multiple - Multiple Paradigm Language Emulator

   multiple is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   multiple is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
   */

#ifndef _MJS_ICG_CONTEXT_H_
#define _MJS_ICG_CONTEXT_H_

#include "multiple_ir.h"

#include "multiply_offset.h"

#include "mjs_icg_fcb.h"
#include "mjs_icg_built_in_proc.h"
#include "mjs_icg_stdlib.h"

struct mjs_icg_context
{
    struct mjs_icg_fcb_block_list *icg_fcb_block_list;
    struct multiple_ir *icode;
    struct multiply_resource_id_pool *res_id;
    struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list;
    struct multiply_offset_item_pack_stack *offset_item_pack_stack;
    struct mjs_icg_stdlib_table_list *stdlibs;
};

int mjs_icg_context_init(struct mjs_icg_context *context);
int mjs_icg_context_uninit(struct mjs_icg_context *context);

#endif



