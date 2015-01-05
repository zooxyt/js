/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Expression
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
#include "mjs_icg_expr.h"
#include "mjs_icg_expr_assign.h"


/* Declaration */
static int mjs_icodegen_member_expression(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_member_expression *member_expr, \
        struct multiply_offset_item_pack *offset_pack_break);


static int mjs_icodegen_argument_list(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_argument_list *argument_list, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;
    struct mjs_ast_argument *argument_cur;

    argument_cur = argument_list->begin;
    while (argument_cur != NULL)
    {
        if ((ret = mjs_icodegen_expression(err, \
                        context, \
                        icg_fcb_block, \
                        map_offset_label_list, \
                        argument_cur->expr, \
                        offset_pack_break)) != 0)
        { goto fail; }

        argument_cur = argument_cur->next; 
    }
    
    goto done;
fail:
done:
    return ret;
}

static int mjs_icodegen_expression_primary(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_expression *expression, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;
    struct mjs_ast_expression_primary *expression_primary = expression->u.expression_primary;
    uint32_t id;
    char *buffer_str = NULL;
    size_t buffer_str_len;

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
                            OP_PUSH, id)) != 0) { goto fail; }
            break;

        case MJS_AST_EXPRESSION_PRIMARY_TYPE_LITERAL:

            switch (expression_primary->u.token_literal->value)
            {
                case TOKEN_CONSTANT_STRING:

                    buffer_str_len = expression_primary->u.token_literal->len;
                    buffer_str = (char *)malloc(sizeof(char) * (buffer_str_len + 1));
                    if (buffer_str == NULL)
                    { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
                    memcpy(buffer_str, \
                            expression_primary->u.token_literal->str, \
                            expression_primary->u.token_literal->len);
                    buffer_str[buffer_str_len] = '\0';
                    multiply_replace_escape_chars(buffer_str, &buffer_str_len);

                    if ((ret = multiply_resource_get_str(err, context->icode, context->res_id, \
                                    &id, \
                                    buffer_str, \
                                    buffer_str_len)) != 0)
                    { goto fail; }
                    if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, OP_PUSH, id)) != 0) 
                    { goto fail; }

                    free(buffer_str);
                    buffer_str = NULL;

                    break;

                case TOKEN_CONSTANT_INTEGER_BINARY:
                case TOKEN_CONSTANT_INTEGER_OCTAL:
                case TOKEN_CONSTANT_INTEGER_DECIMAL:
                case TOKEN_CONSTANT_INTEGER_HEXADECIMAL:
                default:
                    MULTIPLE_ERROR_INTERNAL();
                    ret = -MULTIPLE_ERR_INTERNAL;
                    goto fail;
            }

            break;

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


static int mjs_icodegen_member_expression_dot(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_member_expression_dot *member_expr_dot, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;
    struct token *token_prefix;
    struct token *token_suffix;
    struct multiply_text_precompiled *new_text_precompiled = NULL;
    uint32_t id;
    const int LBL_HASKEY = 0, LBL_TAIL = 1;

    token_suffix = member_expr_dot->identifier_name;
    if (member_expr_dot->member_expression->type == MJS_AST_MEMBER_EXPRESSION_TYPE_PRIMARY)
    {
        if ((member_expr_dot->member_expression->u.expression_primary->type == MJS_AST_EXPRESSION_TYPE_PRIMARY) && \
                (member_expr_dot->member_expression->u.expression_primary->u.expression_primary->type == MJS_AST_EXPRESSION_PRIMARY_TYPE_IDENTIFIER))
        {
            token_prefix = member_expr_dot->member_expression->u.expression_primary->u.expression_primary->u.token_identifier;
            if ((mjs_icg_stdlib_table_list_register(context->stdlibs, \
                            token_prefix->str, \
                            token_prefix->len, \
                            token_suffix->str, \
                            token_suffix->len)) != 0)
            { MULTIPLE_ERROR_INTERNAL(); ret = -MULTIPLE_ERR_INTERNAL; goto fail; }
        }
    }

    /* Suffix Part */
    if ((ret = multiply_resource_get_str(err, context->icode, context->res_id, \
                    &id, \
                    token_suffix->str, \
                    token_suffix->len)) != 0)
    { goto fail; }
    if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, \
                    OP_PUSH, id)) != 0) { goto fail; }

    /* Prefix Part */
    if ((ret = mjs_icodegen_member_expression(err, \
                    context, \
                    icg_fcb_block, \
                    map_offset_label_list, \
                    member_expr_dot->member_expression, \
                    offset_pack_break)) != 0)
    { goto fail; }

    /* Get member */
    if ((ret = multiply_asm_precompile(err, \
                    context->icode, \
                    context->res_id, \
                    &new_text_precompiled, \

                    /* State : <bottom> index, hash <top> */
                    MULTIPLY_ASM_OP_INT , OP_PUSH    , 2          , 
                    MULTIPLY_ASM_OP     , OP_PICKCP  , 
                    MULTIPLY_ASM_OP_INT , OP_PUSH    , 2          , 
                    MULTIPLY_ASM_OP     , OP_PICKCP  , 

                    /* State : <bottom> index, hash, index, hash <top> */
                    MULTIPLY_ASM_OP     , OP_HASHHASKEY, 
                    MULTIPLY_ASM_OP_LBLR, OP_JMPCR   , LBL_HASKEY ,

                    /* Has no key, push a none */
                    MULTIPLY_ASM_OP     , OP_DROP    , 
                    MULTIPLY_ASM_OP     , OP_DROP    , 
                    MULTIPLY_ASM_OP_NONE, OP_PUSH    , 
                    MULTIPLY_ASM_OP_LBLR, OP_JMPR    , LBL_TAIL ,

                    /* Has key */
                    MULTIPLY_ASM_LABEL  , LBL_HASKEY , 
                    MULTIPLY_ASM_OP     , OP_REFGET    , 

                    MULTIPLY_ASM_LABEL  , LBL_TAIL   , 

                    MULTIPLY_ASM_FINISH)) != 0)
    { goto fail; }

    if ((ret = mjs_icg_fcb_block_append_from_precompiled_pic_text( \
                    icg_fcb_block, \
                    new_text_precompiled)) != 0)
    { goto fail; }
    goto done;
fail:
done:
    if (new_text_precompiled != NULL)
    { multiply_text_precompiled_destroy(new_text_precompiled); }
    return ret;
}


static int mjs_icodegen_member_expression(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_member_expression *member_expr, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    switch (member_expr->type)
    {
        case MJS_AST_MEMBER_EXPRESSION_TYPE_PRIMARY:
            if ((ret = mjs_icodegen_expression(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            member_expr->u.expression_primary, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

        case MJS_AST_MEMBER_EXPRESSION_TYPE_FUNCTION:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_BRACKET:
            MULTIPLE_ERROR_INTERNAL();
            ret = -MULTIPLE_ERR_INTERNAL;
            goto fail;
        case MJS_AST_MEMBER_EXPRESSION_TYPE_DOT:
            if ((ret = mjs_icodegen_member_expression_dot(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            member_expr->u.expression_dot, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;
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


static int mjs_icodegen_expression_lhs_new_expression(struct multiple_error *err, \
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
            if ((ret = mjs_icodegen_member_expression(err, \
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

static int mjs_icodegen_expression_lhs_call_expression(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_expression_lhs_call_expression *expr_lhs_call_expr, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;
    uint32_t id;

    switch (expr_lhs_call_expr->type)
    {
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_MEMBER_ARGS:

            /* Arguments */
            if ((ret = mjs_icodegen_argument_list(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr_lhs_call_expr->u.member_args->args, \
                            offset_pack_break)) != 0)
            { goto fail; }

            /* Reverse */
            if ((ret = multiply_resource_get_int(err, context->icode, context->res_id, \
                            &id, \
                            (int)(expr_lhs_call_expr->u.member_args->args->size))) != 0)
            { goto fail; }
            if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, OP_PUSH, id)) != 0)
            { goto fail; }
            if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, OP_REVERSEP, 0)) != 0) 
            { goto fail; }

            /* Function */
            if ((ret = mjs_icodegen_member_expression(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr_lhs_call_expr->u.member_args->member_expr, \
                            offset_pack_break)) != 0)
            { goto fail; }

            /* Call it */
            if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, \
                            OP_FUNCMK, 0)) != 0) { goto fail; }
            if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, \
                            OP_CALLC, 0)) != 0) { goto fail; }
            break;

        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_CALL_ARGS:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_BRACKET:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_DOT:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_UNKNOWN:
            MULTIPLE_ERROR_INTERNAL();
            ret = -MULTIPLE_ERR_INTERNAL;
            goto fail;
    }

    goto done;
fail:
done:
    return ret;
}

static int mjs_icodegen_expression_lhs(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_expression_lhs *expr_lhs, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    switch (expr_lhs->type)
    {
        case MJS_AST_EXPRESSION_LHS_TYPE_NEW: 
            if ((ret = mjs_icodegen_expression_lhs_new_expression(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr_lhs->u.expression_lhs_new_expression, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

        case MJS_AST_EXPRESSION_LHS_TYPE_CALL: 
            if ((ret = mjs_icodegen_expression_lhs_call_expression(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr_lhs->u.expression_lhs_call_expression, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

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


int mjs_icodegen_expression(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_expression *expr, \
        struct multiply_offset_item_pack *offset_pack_break)
{
    int ret = 0;

    switch (expr->type)
    {
        case MJS_AST_EXPRESSION_TYPE_ASSIGNMENT:
            if ((ret = mjs_icodegen_expression_assignment(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr->u.expression_assignment, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

        case MJS_AST_EXPRESSION_TYPE_PRIMARY:
            if ((ret = mjs_icodegen_expression_primary(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;
        case MJS_AST_EXPRESSION_TYPE_CONDITIONAL:
        case MJS_AST_EXPRESSION_TYPE_BINARY:
        case MJS_AST_EXPRESSION_TYPE_UNARY:
        case MJS_AST_EXPRESSION_TYPE_POSTFIX:
        case MJS_AST_EXPRESSION_TYPE_LHS:
            if ((ret = mjs_icodegen_expression_lhs(err, \
                            context, \
                            icg_fcb_block, \
                            map_offset_label_list, \
                            expr->u.expression_lhs, \
                            offset_pack_break)) != 0)
            { goto fail; }
            break;

        case MJS_AST_EXPRESSION_TYPE_UNKNOWN:
            MULTIPLE_ERROR_NOT_IMPLEMENTED();
            ret = -MULTIPLE_ERR_NOT_IMPLEMENTED;
            goto fail;
    }

    goto done;
fail:
done:
    return ret;
}

