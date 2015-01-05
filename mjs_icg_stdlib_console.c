/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Standard Library : Console
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multiple_err.h"
#include "multiple_ir.h"

#include "multiply.h"
#include "multiply_assembler.h"

#include "vm_opcode.h"
#include "vm_types.h"
#include "vm_predef.h"

#include "mjs_lexer.h"
#include "mjs_ast.h"
#include "mjs_icg.h"
#include "mjs_icg_fcb.h"

static int mjs_icg_add_built_in_procs_console_log( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP     , OP_ARGCS, 
                    MULTIPLY_ASM_OP     , OP_PRINT, 
                    MULTIPLY_ASM_OP_STR , OP_PUSH, "\n",
                    MULTIPLY_ASM_OP     , OP_PRINT, 
                    MULTIPLY_ASM_OP     , OP_RETNONE, 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

extern struct mjs_icg_add_built_in_field_handler mjs_icg_add_built_in_field_handlers_console[];

struct mjs_icg_add_built_in_field_handler mjs_icg_add_built_in_field_handlers_console[] =
{
    { MJS_BUILT_IN_METHOD, "log", 3, mjs_icg_add_built_in_procs_console_log },
    { MJS_BUILT_IN_FINISH, NULL, 0, NULL},
};

