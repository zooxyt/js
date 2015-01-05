/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Built-in Procedures
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

#include "mjs_icg_built_in_proc.h"

struct mjs_icg_add_built_in_handler
{
    const char *name;
    const size_t name_len;
    int (*func)(struct multiple_error *err, \
            struct multiple_ir *multiple_ir, \
            struct multiply_resource_id_pool *res_id);
    int (*dependence)(struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list);
};

/* print() */
/*
 * def lualst(obj)
 *     if type(obj) == type([]) then
 *         i = 0
 *         len = size(obj)
 *         first = true
 *         while i != len do
 *             if first then
 *                 first = false
 *             else
 *                 print("\t")
 *             end
 *             print(obj[i])
 *             i = i + 1
 *         end
 *     else
 *         if obj == none then
 *             print("nil")
 *         else
 *             print(obj)
 *         end
 *     end
 *     print("\n")
 * end
 */
static int mjs_icg_add_built_in_procs_print( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;
    const int LBL_39 = 0, LBL_38 = 1, LBL_27 = 2;
    const int LBL_29 = 3, LBL_16 = 4, LBL_41 = 5; 
    const int LBL_isn_none_1 = 6, LBL_isn_none_2 = 7;

    if ((ret = multiply_asm(err, icode, res_id,
                    MULTIPLY_ASM_OP_ID    , OP_LSTARGC , "obj",
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    , "obj",

                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP_RAW   , OP_LSTMK   ,     0,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP       , OP_NE      ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_39,

                    MULTIPLY_ASM_OP_INT   , OP_PUSH    ,     0,
                    MULTIPLY_ASM_OP_ID    , OP_POP     ,    "i",
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,  "obj",
                    MULTIPLY_ASM_OP       , OP_SIZE    ,
                    MULTIPLY_ASM_OP_ID    , OP_POP     ,  "len",
                    MULTIPLY_ASM_OP_TRUE  , OP_PUSH    ,
                    MULTIPLY_ASM_OP_ID    , OP_POP     ,"first",

                    MULTIPLY_ASM_LABEL    , LBL_16     ,
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,    "i",
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,  "len",

                    MULTIPLY_ASM_OP       , OP_EQ      ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_38,

                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,"first",
                    MULTIPLY_ASM_OP       , OP_NOTL    ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_27,

                    MULTIPLY_ASM_OP_FALSE , OP_PUSH    ,
                    MULTIPLY_ASM_OP_ID    , OP_POP     ,"first",
                    MULTIPLY_ASM_OP_LBL   , OP_JMP     , LBL_29,

                    MULTIPLY_ASM_LABEL    , LBL_27     ,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,   "\t",
                    MULTIPLY_ASM_OP       , OP_PRINT   ,
                    MULTIPLY_ASM_LABEL    , LBL_29     ,
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,    "i",
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,  "obj",
                    MULTIPLY_ASM_OP       , OP_REFGET  ,

                    MULTIPLY_ASM_OP       , OP_DUP     , 
                    MULTIPLY_ASM_OP_NONE  , OP_PUSH    , 
                    MULTIPLY_ASM_OP       , OP_NE      , 
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_isn_none_1,
                    MULTIPLY_ASM_OP       , OP_DROP    , 
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    , "nil",
                    MULTIPLY_ASM_LABEL    , LBL_isn_none_1,
                    MULTIPLY_ASM_OP       , OP_PRINT   ,

                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,    "i",
                    MULTIPLY_ASM_OP_INT   , OP_PUSH    ,      1,
                    MULTIPLY_ASM_OP       , OP_ADD     ,
                    MULTIPLY_ASM_OP_ID    , OP_POP     ,    "i",
                    MULTIPLY_ASM_OP_LBL   , OP_JMP     , LBL_16,
                    MULTIPLY_ASM_LABEL    , LBL_38     ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMP     , LBL_41,

                    MULTIPLY_ASM_LABEL    , LBL_39     ,
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    ,  "obj",

                    MULTIPLY_ASM_OP       , OP_DUP     , 
                    MULTIPLY_ASM_OP_NONE  , OP_PUSH    , 
                    MULTIPLY_ASM_OP       , OP_NE      , 
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_isn_none_2,
                    MULTIPLY_ASM_OP       , OP_DROP    , 
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    , "nil",
                    MULTIPLY_ASM_LABEL    , LBL_isn_none_2,
                    MULTIPLY_ASM_OP       , OP_PRINT   ,

                    MULTIPLY_ASM_LABEL    , LBL_41     ,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,   "\n",
                    MULTIPLY_ASM_OP       , OP_PRINT   ,

                    MULTIPLY_ASM_OP_RAW   , OP_LSTMK   , 0,
                    MULTIPLY_ASM_OP       , OP_RETURN  ,

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_type( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;
    const int LBL_0 = 0, LBL_1 = 1, LBL_2 = 2;
    const int LBL_3 = 3, LBL_4 = 4;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP_ID    , OP_ARGC    , "obj",
                    MULTIPLY_ASM_OP_ID    , OP_PUSH    , "obj",

                    /* String */
                    MULTIPLY_ASM_OP       , OP_DUP     ,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,    "",
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP       , OP_NE      ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_0,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,"string",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,
                    MULTIPLY_ASM_LABEL    , LBL_0      ,

                    /* Number : Integer */
                    MULTIPLY_ASM_OP       , OP_DUP     ,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP_INT   , OP_PUSH    ,       0,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP       , OP_NE      ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_1,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,"number",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,
                    MULTIPLY_ASM_LABEL    , LBL_1      ,

                    /* Number : Boolean */
                    MULTIPLY_ASM_OP       , OP_DUP     ,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP_TRUE  , OP_PUSH    ,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP       , OP_NE      ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_2,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,"boolean",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,
                    MULTIPLY_ASM_LABEL    , LBL_2      ,

                    /* Number : None */
                    MULTIPLY_ASM_OP       , OP_DUP     ,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP_NONE  , OP_PUSH    ,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP       , OP_NE      ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_3,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,"nil",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,
                    MULTIPLY_ASM_LABEL    , LBL_3      ,

                    /* Number : Function */
                    MULTIPLY_ASM_OP       , OP_DUP     ,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP_RAW   , OP_LAMBDAMK, 0,
                    MULTIPLY_ASM_OP       , OP_TYPE    ,
                    MULTIPLY_ASM_OP       , OP_NE      ,
                    MULTIPLY_ASM_OP_LBL   , OP_JMPC    , LBL_4,
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,"function",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,
                    MULTIPLY_ASM_LABEL    , LBL_4      ,

                    /* Unknown */
                    MULTIPLY_ASM_OP_STR   , OP_PUSH    ,"unknown",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,


                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_tonumber( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP       , OP_ARGCS   ,

                    /* TODO: To change to 'number'? */
                    MULTIPLY_ASM_OP_TYPE  , OP_CONVERT , "int",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

static int mjs_icg_add_built_in_procs_tostring( \
        struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id)
{
    int ret = 0;

    if ((ret = multiply_asm(err, icode, res_id, 
                    MULTIPLY_ASM_OP       , OP_ARGCS   ,

                    MULTIPLY_ASM_OP_TYPE  , OP_CONVERT , "str",
                    MULTIPLY_ASM_OP       , OP_RETURN  ,

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; } 
    goto done;
fail:
done:
    return ret;
}

/* Also check mjs_icg_fcb_built_in_proc.h */

static struct mjs_icg_add_built_in_handler mjs_icg_add_built_in_handlers[] = 
{
    {"print", 5, mjs_icg_add_built_in_procs_print, NULL},
    {"type", 4, mjs_icg_add_built_in_procs_type, NULL},
    {"tonumber", 8, mjs_icg_add_built_in_procs_tonumber, NULL},
    {"tostring", 8, mjs_icg_add_built_in_procs_tostring, NULL},
};
#define MJS_ICG_ADD_BUILT_IN_HANDLERS_COUNT (sizeof(mjs_icg_add_built_in_handlers)/sizeof(struct mjs_icg_add_built_in_handler))

int mjs_icg_add_built_in_procs(struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id, \
        struct mjs_icg_fcb_block *icg_fcb_block_init, \
        struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list,
        uint32_t insert_point, uint32_t *instrument_count)
{
    int ret = 0;
    struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure_cur;
    struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure_target = NULL;
    int i;
    uint32_t id;
    uint32_t instrument_number;
    *instrument_count = 0;

    /* Dependence */
    customizable_built_in_procedure_cur = customizable_built_in_procedure_list->begin;
    while (customizable_built_in_procedure_cur != NULL)
    {
        if (customizable_built_in_procedure_cur->called != 0)
        {
            for (i = 0; i != MJS_ICG_ADD_BUILT_IN_HANDLERS_COUNT; i++)
            {
                if ((mjs_icg_add_built_in_handlers[i].name_len == customizable_built_in_procedure_cur->name_len) && \
                        (strncmp(customizable_built_in_procedure_cur->name, \
                                 mjs_icg_add_built_in_handlers[i].name, \
                                 customizable_built_in_procedure_cur->name_len) == 0))
                {
                    customizable_built_in_procedure_target = mjs_icg_customizable_built_in_procedure_list_lookup( \
                            customizable_built_in_procedure_list, \
                            mjs_icg_add_built_in_handlers[i].name, \
                            mjs_icg_add_built_in_handlers[i].name_len);
                    if (customizable_built_in_procedure_target == NULL)
                    {
                        if ((ret = mjs_icg_add_built_in_handlers[i].dependence(customizable_built_in_procedure_list)) != 0)
                        { goto fail; }
                    }
                }
            }
        }
        customizable_built_in_procedure_cur = customizable_built_in_procedure_cur->next; 
    }

    customizable_built_in_procedure_cur = customizable_built_in_procedure_list->begin;
    while (customizable_built_in_procedure_cur != NULL)
    {
        if (customizable_built_in_procedure_cur->called != 0)
        {
            for (i = 0; i != MJS_ICG_ADD_BUILT_IN_HANDLERS_COUNT; i++)
            {
                if ((mjs_icg_add_built_in_handlers[i].name_len == customizable_built_in_procedure_cur->name_len) && \
                        (strncmp(customizable_built_in_procedure_cur->name, \
                                 mjs_icg_add_built_in_handlers[i].name, \
                                 customizable_built_in_procedure_cur->name_len) == 0))
                {
                    instrument_number = (uint32_t)(icode->text_section->size);
                    customizable_built_in_procedure_cur->instrument_number_icode = instrument_number;

                    if ((ret = mjs_icg_add_built_in_handlers[i].func(err, \
                                    icode, res_id)) != 0)
                    { goto fail; }

                    /* Make Lambda */
                    if ((ret = mjs_icg_fcb_block_insert_with_configure_type( \
                                    icg_fcb_block_init, \
                                    insert_point++, \
                                    OP_LAMBDAMK, instrument_number, \
                                    MJS_ICG_FCB_LINE_TYPE_BLTIN_PROC_MK)) != 0) { goto fail; }
                    /* Save to variable */
                    if ((ret = multiply_resource_get_id(err, icode, res_id, \
                                    &id,  \
                                    customizable_built_in_procedure_cur->name, \
                                    customizable_built_in_procedure_cur->name_len)) != 0)
                    { goto fail; }
                    if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                                    icg_fcb_block_init, insert_point++, \
                                    OP_POPC, id)) != 0) { goto fail; }

                    (*instrument_count) += 2;
                    break;
                }
            }
        }
        customizable_built_in_procedure_cur = customizable_built_in_procedure_cur->next; 
    }

    goto done;
fail:
done:
    return ret;
}

