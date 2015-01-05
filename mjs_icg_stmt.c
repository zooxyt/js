/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Statement
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

#include "multiply.h"
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
#include "mjs_icg_stmt.h"
#include "mjs_icg_expr.h"


static int mjs_icodegen_statement_expression(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_statement_expression *stmt_expr, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    return mjs_icodegen_expression(err, \
            context, \
            icg_fcb_block, \
            map_offset_label_list, \
            stmt_expr->expression, \
            offset_pack_break);
}


int mjs_icodegen_statement(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_statement *stmt, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    switch (stmt->type)
    {
        case MJS_AST_STATEMENT_TYPE_EXPRESSION:

            if ((ret = mjs_icodegen_statement_expression(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            stmt->u.statement_expression, \
                            offset_pack_break)) != 0)
            { goto fail; }

            /* Drop the return value */
            if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, \
                            OP_DROP, 0)) != 0) { goto fail; }

            break;
        case MJS_AST_STATEMENT_TYPE_UNKNOWN:
        case MJS_AST_STATEMENT_TYPE_BLOCK:
        case MJS_AST_STATEMENT_TYPE_VARIABLE:
            MULTIPLE_ERROR_NOT_IMPLEMENTED();
            ret = -MULTIPLE_ERR_NOT_IMPLEMENTED;
            goto fail;

        case MJS_AST_STATEMENT_TYPE_EMPTY:
            break;

        case MJS_AST_STATEMENT_TYPE_IF:
        case MJS_AST_STATEMENT_TYPE_ITERATION:
        case MJS_AST_STATEMENT_TYPE_CONTINUE:
        case MJS_AST_STATEMENT_TYPE_BREAK:
        case MJS_AST_STATEMENT_TYPE_RETURN:
        case MJS_AST_STATEMENT_TYPE_WITH:
        case MJS_AST_STATEMENT_TYPE_LABELLED:
        case MJS_AST_STATEMENT_TYPE_SWITCH:
        case MJS_AST_STATEMENT_TYPE_THROW:
        case MJS_AST_STATEMENT_TYPE_TRY:
        case MJS_AST_STATEMENT_TYPE_DEBUGGER:
            MULTIPLE_ERROR_NOT_IMPLEMENTED();
            ret = -MULTIPLE_ERR_NOT_IMPLEMENTED;
            goto fail;
    }

    goto done;
fail:
done:
    return ret;
}

