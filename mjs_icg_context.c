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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "multiple_ir.h"
#include "mjs_icg_fcb.h"
#include "mjs_icg_context.h"

int mjs_icg_context_init(struct mjs_icg_context *context)
{
    context->icg_fcb_block_list = NULL;
    context->icode = NULL;
    context->res_id = NULL;
    context->customizable_built_in_procedure_list = NULL;
    context->offset_item_pack_stack = NULL;
    context->stdlibs = NULL;
    return 0;
}

int mjs_icg_context_uninit(struct mjs_icg_context *context)
{
    (void)context;
    return 0;
}

