/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Expression : Assignment
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
#include "mjs_icg_expr.h"
#include "mjs_icg_expr_assign.h"


static int mjs_icodegen_expression_assignment_expression_primary(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_expression *expression, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;
    struct mjs_ast_expression_primary *expression_primary = expression->u.expression_primary;
    uint32_t id;

    (void)map_offset_label_list;
    (void)offset_pack_break;

    switch (expression_primary->type)
    {
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_IDENTIFIER:

            if ((ret = multiply_resource_get_id(err, context->icode, context->res_id, \
                            &id, \
                            expression_primary->u.token_identifier->str, \
                            expression_primary->u.token_identifier->len)) != 0)
            { goto fail; }
            if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, \
                            OP_DUP, 0)) != 0) { goto fail; }
            if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, \
                            OP_POPG, id)) != 0) { goto fail; }
            break;

        case MJS_AST_EXPRESSION_PRIMARY_TYPE_LITERAL:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_ARRAY_LITERAL:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_OBJECT_LITERAL:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_SUB:

            multiple_error_update(err, -MULTIPLE_ERR_ICODEGEN, \
                    "%d:%d: error: invalid left-hand side value \'%s\'", \
                    expression_primary->u.token_literal->pos_ln,
                    expression_primary->u.token_literal->pos_col,
                    expression_primary->u.token_literal->str);
            ret = -MULTIPLE_ERR_ICODEGEN;
            goto fail;

        case MJS_AST_EXPRESSION_PRIMARY_TYPE_THIS:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_UNKNOWN:
            MULTIPLE_ERROR_INTERNAL();
            ret = -MULTIPLE_ERR_INTERNAL;
            goto fail;
    }

    goto done;
fail:
done:
    return ret;
};


static int mjs_icodegen_expression_assignment_member_expression(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_member_expression *member_expression, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    switch (member_expression->type)
    {
        case MJS_AST_MEMBER_EXPRESSION_TYPE_PRIMARY:
            if ((ret = mjs_icodegen_expression_assignment_expression_primary(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            member_expression->u.expression_primary, \
                            offset_pack_break)) != 0)
            { goto fail; }

            break;

        case MJS_AST_MEMBER_EXPRESSION_TYPE_FUNCTION:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_BRACKET:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_DOT:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_NEW:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_UNKNOWN:
            MULTIPLE_ERROR_INTERNAL();
            ret = -MULTIPLE_ERR_INTERNAL;
            goto fail;
    }

    goto done;
fail:
done:
    return ret;
}


static int mjs_icodegen_expression_assignment_new_expression(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_expression_lhs_new_expression *expr_lhs_new_expr, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    switch (expr_lhs_new_expr->type)
    {

        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_MEMBER:
            if ((ret = mjs_icodegen_expression_assignment_member_expression(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr_lhs_new_expr->u.member_expression, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_NEW:
        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_UNKNOWN:
            MULTIPLE_ERROR_INTERNAL();
            ret = -MULTIPLE_ERR_INTERNAL;
            goto fail;
    }


    goto done;
fail:
done:
    return ret;
}


int mjs_icodegen_expression_assignment(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_expression_assignment *expr_assign, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    if (expr_assign->op->value != '=')
    {
        MULTIPLE_ERROR_INTERNAL();
        ret = -MULTIPLE_ERR_INTERNAL;
        goto fail;
    }

    /* Right Value */
    if ((ret = mjs_icodegen_expression(err, \
                    context, \
                    icg_fcb_block, \
                    map_offset_label_list, \
                    expr_assign->expression_rhs, \
                    offset_pack_break)) != 0)
    { goto fail; }

    /* Left Value */
    if (expr_assign->expression_lhs->type != MJS_AST_EXPRESSION_TYPE_LHS)
    {
        multiple_error_update(err, -MULTIPLE_ERR_ICODEGEN, \
                "%d:%d: error: left-hand side expected before \'%s\'", \
                expr_assign->op->pos_ln, expr_assign->op->pos_col, \
                expr_assign->op->str);
        ret = -MULTIPLE_ERR_ICODEGEN;
        goto fail;
    }

    switch (expr_assign->expression_lhs->u.expression_lhs->type)
    {
        case MJS_AST_EXPRESSION_LHS_TYPE_NEW:
            if ((ret = mjs_icodegen_expression_assignment_new_expression(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr_assign->expression_lhs->u.expression_lhs->u.expression_lhs_new_expression, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

        case MJS_AST_EXPRESSION_LHS_TYPE_CALL:
        case MJS_AST_EXPRESSION_LHS_TYPE_UNKNOWN:
            MULTIPLE_ERROR_INTERNAL();
            ret = -MULTIPLE_ERR_INTERNAL;
            goto fail;
    }

    goto done;
fail:
done:
    return ret;
}

