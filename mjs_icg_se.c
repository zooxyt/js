/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Source Elements
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

#include "selfcheck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multiple_ir.h"
#include "multiple_misc.h" 
#include "multiple_err.h"

#include "multiply_assembler.h"
#include "multiply_str_aux.h"

#include "vm_predef.h"
#include "vm_opcode.h"
#include "vm_types.h"

#include "mjs_lexer.h"
#include "mjs_ast.h"

#include "mjs_icg_aux.h"
#include "mjs_icg_fcb.h"
#include "mjs_icg_context.h"

#include "mjs_icg_built_in_proc.h"
#include "mjs_icg_built_in_table.h"

#include "mjs_icg.h"
#include "mjs_icg_se.h"
#include "mjs_icg_stmt.h"


static int mjs_icodegen_source_element(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_source_element *element, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    switch (element->type)
    {
        case MJS_AST_SOURCE_ELEMENT_TYPE_STATEMENT:
            if ((ret = mjs_icodegen_statement(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            element->u.statement, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

        case MJS_AST_SOURCE_ELEMENT_TYPE_FUNDECL:
        case MJS_AST_SOURCE_ELEMENT_TYPE_UNKNOWN:
            MULTIPLE_ERROR_NOT_IMPLEMENTED();
            ret = -MULTIPLE_ERR_NOT_IMPLEMENTED;
            goto fail;
    }

    goto done;
fail:
done:
    return ret;
}

int mjs_icodegen_source_element_list(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_source_element_list *list, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;
    struct mjs_ast_source_element *source_element_cur;
    struct multiply_offset_item_pack *new_offset_item_pack = NULL;

    /* For recording labels */
    new_offset_item_pack = multiply_offset_item_pack_new();
    if (new_offset_item_pack == NULL)
    {
        MULTIPLE_ERROR_MALLOC();
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }
    multiply_offset_item_pack_stack_push(context->offset_item_pack_stack, \
            new_offset_item_pack);
    new_offset_item_pack = NULL;

    /* Source Elements */
    source_element_cur = list->begin;
    while (source_element_cur != NULL)
    {
        if ((ret = mjs_icodegen_source_element(err, \
                        context, \
                        icg_fcb_block, \
                        map_offset_label_list, \
                        source_element_cur, \
                        offset_pack_break)) != 0)
        { goto fail; }
        source_element_cur = source_element_cur->next;
    }

    goto done;
fail:
done:
    return ret;
}

