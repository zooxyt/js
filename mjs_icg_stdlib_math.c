/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Standard Library : Mathematical
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

#include "multiple_ir.h"
#include "multiple_err.h"

#include "multiply.h"
#include "multiply_assembler.h"

#include "vm_opcode.h"
#include "vm_types.h"
#include "vm_predef.h"

#include "mjs_lexer.h"
#include "mjs_ast.h"
#include "mjs_icg.h"
#include "mjs_icg_fcb.h"


static int mjs_icg_add_built_in_procs_math_abs( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP     , OP_ARGCS   ,
                    MULTIPLY_ASM_OP_RAW , OP_FASTLIB , OP_FASTLIB_ABS,
                    MULTIPLY_ASM_OP     , OP_RETURN  , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_math_cos( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP     , OP_ARGCS   ,
                    MULTIPLY_ASM_OP_RAW , OP_FASTLIB , OP_FASTLIB_COS,
                    MULTIPLY_ASM_OP     , OP_RETURN  , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_math_exp( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP     , OP_ARGCS   ,
                    MULTIPLY_ASM_OP_RAW , OP_FASTLIB , OP_FASTLIB_EXP,
                    MULTIPLY_ASM_OP     , OP_RETURN  , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_math_pi( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP_FLOAT, OP_PUSH , (double)3.1415926535898,
                    MULTIPLY_ASM_OP     , OP_RETURN  , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_math_sin( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP     , OP_ARGCS   ,
                    MULTIPLY_ASM_OP_RAW , OP_FASTLIB , OP_FASTLIB_SIN,
                    MULTIPLY_ASM_OP     , OP_RETURN  , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_math_sqrt( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP     , OP_ARGCS   ,
                    MULTIPLY_ASM_OP_RAW , OP_FASTLIB , OP_FASTLIB_SQRT,
                    MULTIPLY_ASM_OP     , OP_RETURN  , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_math_tan( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP     , OP_ARGCS   ,
                    MULTIPLY_ASM_OP_RAW , OP_FASTLIB , OP_FASTLIB_TAN,
                    MULTIPLY_ASM_OP     , OP_RETURN  , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

extern struct mjs_icg_add_built_in_field_handler mjs_icg_add_built_in_field_handlers_math[];

struct mjs_icg_add_built_in_field_handler mjs_icg_add_built_in_field_handlers_math[] =
{
    { MJS_BUILT_IN_METHOD, "abs", 3, mjs_icg_add_built_in_procs_math_abs },
    { MJS_BUILT_IN_METHOD, "cos", 3, mjs_icg_add_built_in_procs_math_cos },
    { MJS_BUILT_IN_METHOD, "exp", 3, mjs_icg_add_built_in_procs_math_exp },
    { MJS_BUILT_IN_PROPERTY, "pi", 2, mjs_icg_add_built_in_procs_math_pi },
    { MJS_BUILT_IN_METHOD, "sin", 3, mjs_icg_add_built_in_procs_math_sin },
    { MJS_BUILT_IN_METHOD, "sqrt", 4, mjs_icg_add_built_in_procs_math_sqrt },
    { MJS_BUILT_IN_METHOD, "tan", 3, mjs_icg_add_built_in_procs_math_tan },
    { MJS_BUILT_IN_FINISH, NULL, 0, NULL},
};

