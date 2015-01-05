/* Multiple JavaScript Programming Language : Parser
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
#include <string.h>
#include <stdlib.h>

#include "multiple_misc.h"
#include "multiple_err.h"

#include "mjs_lexer.h"
#include "mjs_ast.h"
#include "mjs_parser.h"

#include "vm_opcode.h"


/* Declaration */
static int mjs_parse_expression(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io);
static int mjs_parse_expression_assignment(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io);


/* Expression : Primary Experssion */
static int mjs_parse_primary_expression(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;

    new_expression = mjs_ast_expression_new(MJS_AST_EXPRESSION_TYPE_PRIMARY);
    if (new_expression == NULL)
    { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
    switch (token_cur->value)
    {
        case TOKEN_KEYWORD_THIS:
            new_expression->u.expression_primary = mjs_ast_expression_primary_new( \
                    MJS_AST_EXPRESSION_PRIMARY_TYPE_THIS);
            if (new_expression->u.expression_primary == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_expression->u.expression_primary->u.token_this = token_clone(token_cur);
            if (new_expression->u.expression_primary->u.token_this == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            token_cur = token_cur->next;
            break;
        case TOKEN_IDENTIFIER:
            new_expression->u.expression_primary = mjs_ast_expression_primary_new( \
                    MJS_AST_EXPRESSION_PRIMARY_TYPE_IDENTIFIER);
            if (new_expression->u.expression_primary == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_expression->u.expression_primary->u.token_identifier = token_clone(token_cur);
            if (new_expression->u.expression_primary->u.token_identifier == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            token_cur = token_cur->next;
            break;
        case TOKEN_CONSTANT_INTEGER_BINARY:
        case TOKEN_CONSTANT_INTEGER_OCTAL:
        case TOKEN_CONSTANT_INTEGER_DECIMAL:
        case TOKEN_CONSTANT_INTEGER_HEXADECIMAL:
        case TOKEN_CONSTANT_FLOAT_BINARY:
        case TOKEN_CONSTANT_FLOAT_OCTAL:
        case TOKEN_CONSTANT_FLOAT_DECIMAL:
        case TOKEN_CONSTANT_FLOAT_HEXADECIMAL:
        case TOKEN_CONSTANT_STRING:
        case TOKEN_LITERAL_TRUE:
        case TOKEN_LITERAL_FALSE:
        case TOKEN_LITERAL_NULL:
            new_expression->u.expression_primary = mjs_ast_expression_primary_new( \
                    MJS_AST_EXPRESSION_PRIMARY_TYPE_LITERAL);
            if (new_expression->u.expression_primary == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_expression->u.expression_primary->u.token_literal = token_clone(token_cur);
            if (new_expression->u.expression_primary->u.token_literal == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            token_cur = token_cur->next;
            break;
        default:
            multiple_error_update(err, -MULTIPLE_ERR_PARSING, \
                    "%d:%d: error: unexpected content", \
                    token_cur->pos_ln, token_cur->pos_col);
            ret = -MULTIPLE_ERR_PARSING;
            goto fail;
    }

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
done:
    *token_cur_io = token_cur;
    return ret;
}

/* Expression : Member Experssion */
static int mjs_parse_member_expression(struct multiple_error *err, \
        struct mjs_ast_member_expression **member_expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_member_expression *new_member_expression = NULL;
    struct mjs_ast_member_expression *new_member_expression_2 = NULL;

    if (token_cur->value == TOKEN_KEYWORD_NEW)
    {
        /* 'new' MemberExpression Arguments */
        MULTIPLE_ERROR_INTERNAL();
        ret = -MULTIPLE_ERR_INTERNAL;
        goto fail;
    }
    else
    {
        if ((new_member_expression = mjs_ast_member_expression_new( \
                        MJS_AST_MEMBER_EXPRESSION_TYPE_PRIMARY)) == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        if ((ret = mjs_parse_primary_expression(err, \
                        &new_member_expression->u.expression_primary, \
                        &token_cur)) != 0)
        { goto fail; }

        if (token_cur->value == '.')
        {
            /* Skip '.' */
            token_cur = token_cur->next;
            if (token_cur->value != TOKEN_IDENTIFIER)
            {
                multiple_error_update(err, -MULTIPLE_ERR_PARSING, \
                        "%d:%d: error: expected identifier", \
                        token_cur->pos_ln, token_cur->pos_col);
                ret = -MULTIPLE_ERR_PARSING;
                goto fail;
            }
            if ((new_member_expression_2 = mjs_ast_member_expression_new( \
                            MJS_AST_MEMBER_EXPRESSION_TYPE_DOT)) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            if ((new_member_expression_2->u.expression_dot = mjs_ast_member_expression_dot_new()) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_member_expression_2->u.expression_dot->member_expression = new_member_expression;
            new_member_expression = NULL;
            if ((new_member_expression_2->u.expression_dot->identifier_name = token_clone(token_cur)) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            /* Skip identifier */
            token_cur = token_cur->next;
            new_member_expression = new_member_expression_2; new_member_expression_2 = NULL;
        }
    }

    *member_expression_out = new_member_expression;

    goto done;
fail:
    if (new_member_expression != NULL)
    { mjs_ast_member_expression_destroy(new_member_expression); }
    if (new_member_expression_2 != NULL)
    { mjs_ast_member_expression_destroy(new_member_expression_2); }
done:
    *token_cur_io = token_cur;
    return ret;
}

/* Expression : Left Hand Side : New Expression */
static int mjs_parse_expression_lhs_new_expression(struct multiple_error *err, \
        struct mjs_ast_expression_lhs_new_expression **expression_lhs_new_expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression_lhs_new_expression *new_expression_lhs_new_expression = NULL;

    if (token_cur->value == TOKEN_KEYWORD_NEW)
    {
        /* Skip 'new' */
        token_cur = token_cur->next;

        if ((new_expression_lhs_new_expression = mjs_ast_expression_lhs_new_expression_new( \
                        MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_NEW)) == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        if ((ret = mjs_parse_expression_lhs_new_expression(err, \
                        &new_expression_lhs_new_expression->u.sub_new_expression, \
                        &token_cur)) != 0)
        { goto fail; }
    }
    else
    {
        if ((new_expression_lhs_new_expression = mjs_ast_expression_lhs_new_expression_new( \
                        MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_MEMBER)) == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        if ((ret = mjs_parse_member_expression(err, \
                        &new_expression_lhs_new_expression->u.member_expression, \
                        &token_cur)) != 0)
        { goto fail; }
    }

    *expression_lhs_new_expression_out = new_expression_lhs_new_expression;

    goto done;
fail:
    if (new_expression_lhs_new_expression != NULL)
    { mjs_ast_expression_lhs_new_expression_destroy(new_expression_lhs_new_expression); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Argument List */

/*static int mjs_parse_argument(struct multiple_error *err, \*/
/*struct mjs_ast_argument **argument_out, \*/
/*struct token **token_cur_io)*/
/*{*/
/*int ret = 0;*/
/*struct token *token_cur = *token_cur_io;*/
/*struct mjs_ast_argument *new_argument = NULL;*/

/*if ((new_argument = mjs_ast_argument_new()) == NULL)*/
/*{ MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }*/
/*if ((ret = mjs_parse_expression(err, \*/
/*&new_argument->expr, \*/
/*&token_cur)) != 0)*/
/*{ goto fail; }*/

/**argument_out = new_argument;*/

/*goto done;*/
/*fail:*/
/*if (new_argument != NULL)*/
/*{ mjs_ast_argument_destroy(new_argument); }*/
/*done:*/
/**token_cur_io = token_cur;*/
/*return ret;*/
/*}*/

static int mjs_parse_arguments(struct multiple_error *err, \
        struct mjs_ast_argument_list **argument_list_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_argument_list *new_argument_list = NULL;
    struct mjs_ast_argument *new_argument = NULL;

    if (token_cur->value != '(')
    {
        multiple_error_update(err, -MULTIPLE_ERR_PARSING, \
                "%d:%d: error: expected \'(\'", \
                token_cur->pos_ln, token_cur->pos_col);
        ret = -MULTIPLE_ERR_PARSING;
        goto fail;
    }
    /* Skip '(' */
    token_cur = token_cur->next;

    if ((new_argument_list = mjs_ast_argument_list_new()) == NULL)
    { goto fail; }

    while ((token_cur != NULL) && (token_cur->value != TOKEN_FINISH))
    {
        if ((new_argument = mjs_ast_argument_new()) == NULL)
        { goto fail; }
        if ((ret = mjs_parse_expression_assignment(err, \
                        &new_argument->expr, \
                        &token_cur)) != 0)
        { goto fail; }
        mjs_ast_argument_list_append(new_argument_list, new_argument);
        new_argument = NULL;

        if (token_cur->value == ')') break;
        else if (token_cur->value == ',') 
        {
            token_cur = token_cur->next;
        }
        else
        {
            multiple_error_update(err, -MULTIPLE_ERR_PARSING, \
                    "%d:%d: error: expected \')\' or \',\'", \
                    token_cur->pos_ln, token_cur->pos_col);
            ret = -MULTIPLE_ERR_PARSING;
            goto fail;
        }

    }

    /* Skip ')' */
    if (token_cur == NULL)
    { MULTIPLE_ERROR_INTERNAL(); ret = -MULTIPLE_ERR_INTERNAL; goto fail; }
    token_cur = token_cur->next;

    *argument_list_out = new_argument_list;

    goto done;
fail:
    if (new_argument_list != NULL)
    { mjs_ast_argument_list_destroy(new_argument_list); }
    if (new_argument != NULL)
    { mjs_ast_argument_destroy(new_argument); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Expression : Left Hand Side : Call Expression */

/* Expression : Left Hand Side */

static int mjs_parse_expression_lhs(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;
    struct mjs_ast_member_expression *new_member_expression = NULL;

    new_expression = mjs_ast_expression_new(MJS_AST_EXPRESSION_TYPE_LHS);
    if (new_expression == NULL)
    { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }

    /*
     * NewExpression :
     *   MemberExpression
     *   'new' NewExpression
     * CallExpression :
     *   MemberExpression Arguments
     *   CallExpression Arguments
     *   CallExpression [ Expression ]
     *   CallExpression . IdentifierName
     */


    if (token_cur->value == TOKEN_KEYWORD_NEW)
    {
        /* -> New Expression */
        if ((new_expression->u.expression_lhs = mjs_ast_expression_lhs_new(MJS_AST_EXPRESSION_LHS_TYPE_NEW)) == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        if ((ret = mjs_parse_expression_lhs_new_expression(err, \
                        &new_expression->u.expression_lhs->u.expression_lhs_new_expression, \
                        &token_cur)) != 0)
        { goto fail; }
    }
    else
    {
        /* Try to parse a member expression first */
        if ((ret = mjs_parse_member_expression(err, \
                        &new_member_expression, \
                        &token_cur)) != 0)
        { goto fail; }

        if (token_cur->value == '(')
        {
            /* -> Call Expression */
            if ((new_expression->u.expression_lhs = mjs_ast_expression_lhs_new(MJS_AST_EXPRESSION_LHS_TYPE_CALL)) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            if ((new_expression->u.expression_lhs->u.expression_lhs_call_expression = mjs_ast_expression_lhs_call_expression_new( \
                            MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_MEMBER_ARGS)) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            if ((new_expression->u.expression_lhs->u.expression_lhs_call_expression->u.member_args = mjs_ast_expression_lhs_call_expression_member_args_new()) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_expression->u.expression_lhs->u.expression_lhs_call_expression->u.member_args->member_expr = new_member_expression;
            new_member_expression = NULL;
            if ((mjs_parse_arguments(err, \
                            &new_expression->u.expression_lhs->u.expression_lhs_call_expression->u.member_args->args, \
                            &token_cur)) != 0)
            { goto fail; }
        }
        else
        {
            /* -> New Expression */
            if ((new_expression->u.expression_lhs = mjs_ast_expression_lhs_new(MJS_AST_EXPRESSION_LHS_TYPE_NEW)) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            if ((new_expression->u.expression_lhs->u.expression_lhs_new_expression = mjs_ast_expression_lhs_new_expression_new( \
                    MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_MEMBER)) == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_expression->u.expression_lhs->u.expression_lhs_new_expression->u.member_expression = new_member_expression;
            new_member_expression = NULL;
        }
    }

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
    if (new_member_expression != NULL)
    { mjs_ast_member_expression_destroy(new_member_expression); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Expression : Postfix */
static int mjs_parse_expression_postfix(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;
    struct mjs_ast_expression *new_expression_2 = NULL;

    if ((ret = mjs_parse_expression_lhs(err, \
                    &new_expression, \
                    &token_cur)) != 0)
    { goto fail; }

    if ((token_cur->value == TOKEN_OP_INC) || \
            (token_cur->value == TOKEN_OP_DEC))
    {
        new_expression_2 = mjs_ast_expression_new(MJS_AST_EXPRESSION_TYPE_POSTFIX);
        if (new_expression_2 == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        new_expression_2->u.expression_postfix = mjs_ast_expression_postfix_new();
        if (new_expression_2->u.expression_postfix == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        new_expression_2->u.expression_postfix->op = token_clone(token_cur);
        token_cur = token_cur->next;
        new_expression_2->u.expression_postfix->expression_sub = new_expression;
        new_expression = new_expression_2; new_expression_2 = NULL;
    }

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Expression : Unary */

static int mjs_parse_expression_unary(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;

    switch (token_cur->value)
    {
        case TOKEN_KEYWORD_DELETE:
        case TOKEN_KEYWORD_VOID:
        case TOKEN_KEYWORD_TYPEOF:
        case TOKEN_OP_INC:
        case TOKEN_OP_DEC:
        case '+':
        case '-':
        case '~':
        case '!':

            new_expression = mjs_ast_expression_new(MJS_AST_EXPRESSION_TYPE_UNARY);
            if (new_expression == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_expression->u.expression_unary = mjs_ast_expression_unary_new();
            if (new_expression->u.expression_unary == NULL)
            { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
            new_expression->u.expression_unary->op = token_clone(token_cur);
            token_cur = token_cur->next;

            if ((ret = mjs_parse_expression_unary(err, \
                            &new_expression->u.expression_unary->expression_sub, \
                            &token_cur)) != 0)
            { goto fail; }

            break;

        default:
            if ((ret = mjs_parse_expression_postfix(err, \
                            &new_expression, \
                            &token_cur)) != 0)
            { goto fail; }
            break;
    }

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Expression : Binary */

enum mjs_parse_expression_binary_or
{
    MJS_PARSE_EXPRESSION_BINARY_BEGIN = 0,
    MJS_PARSE_EXPRESSION_BINARY_LOGICAL_OR = MJS_PARSE_EXPRESSION_BINARY_BEGIN, 
    MJS_PARSE_EXPRESSION_BINARY_LOGICAL_AND, 
    MJS_PARSE_EXPRESSION_BINARY_BITWISE_OR, 
    MJS_PARSE_EXPRESSION_BINARY_BITWISE_XOR, 
    MJS_PARSE_EXPRESSION_BINARY_BITWISE_AND, 
    MJS_PARSE_EXPRESSION_BINARY_EQ,
    MJS_PARSE_EXPRESSION_BINARY_REL,
    MJS_PARSE_EXPRESSION_BINARY_SHIFT,
    MJS_PARSE_EXPRESSION_BINARY_ADD,
    MJS_PARSE_EXPRESSION_BINARY_MUL,
    MJS_PARSE_EXPRESSION_BINARY_END = MJS_PARSE_EXPRESSION_BINARY_MUL,
};

static int mjs_parse_expression_binary_level_operator_fit(int level, struct token *token_cur)
{
    switch (level)
    {
        case MJS_PARSE_EXPRESSION_BINARY_LOGICAL_OR:
            return (token_cur->value == TOKEN_OP_OR) ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_LOGICAL_AND:
            return (token_cur->value == TOKEN_OP_AND) ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_BITWISE_OR:
            return (token_cur->value == '|') ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_BITWISE_XOR:
            return (token_cur->value == '^') ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_BITWISE_AND:
            return (token_cur->value == '&') ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_EQ:
            return ((token_cur->value == TOKEN_OP_EQ) || \
                    (token_cur->value == TOKEN_OP_EQ3) || \
                    (token_cur->value == TOKEN_OP_NE) || \
                    (token_cur->value == TOKEN_OP_NE3)) ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_REL:
            return ((token_cur->value == '<') || \
                    (token_cur->value == '>') || \
                    (token_cur->value == TOKEN_OP_LE) || \
                    (token_cur->value == TOKEN_OP_GE) || \
                    (token_cur->value == TOKEN_KEYWORD_INSTANCEOF) || \
                    (token_cur->value == TOKEN_KEYWORD_IN)) ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_SHIFT:
            return ((token_cur->value == TOKEN_OP_LL) || \
                    (token_cur->value == TOKEN_OP_RR) || \
                    (token_cur->value == TOKEN_OP_RRR)) ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_ADD:
            return ((token_cur->value == '+') || \
                    (token_cur->value == '-')) ? 1 : 0;
        case MJS_PARSE_EXPRESSION_BINARY_MUL:
            return ((token_cur->value == '*') || \
                    (token_cur->value == '/') || \
                    (token_cur->value == '%')) ? 1 : 0;
        default:
            return 0;
    }
}

static int mjs_parse_expression_binary_level(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        int level, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;
    struct mjs_ast_expression *new_expression_2 = NULL;

    /* Left Hand Side */
    if (level == MJS_PARSE_EXPRESSION_BINARY_END)
    {
        if ((ret = mjs_parse_expression_unary(err, \
                        &new_expression, \
                        &token_cur)) != 0)
        { goto fail; }
    }
    else
    {
        if ((ret = mjs_parse_expression_binary_level(err, \
                        &new_expression, \
                        level + 1, \
                        &token_cur)) != 0)
        { goto fail; }
    }

    /* Operator */
    if (mjs_parse_expression_binary_level_operator_fit(level, token_cur) != 0)
    {
        new_expression_2 = mjs_ast_expression_new(MJS_AST_EXPRESSION_TYPE_BINARY);
        if (new_expression_2 == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        new_expression_2->u.expression_binary = mjs_ast_expression_binary_new();
        if (new_expression_2->u.expression_binary == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        new_expression_2->u.expression_binary->op = token_clone(token_cur);
        token_cur = token_cur->next;
        new_expression_2->u.expression_binary->expression_lhs = new_expression;
        new_expression = new_expression_2;

        /* Right Hand Side */
        if (level == MJS_PARSE_EXPRESSION_BINARY_MUL)
        {
            if ((ret = mjs_parse_expression_unary(err, \
                            &new_expression_2->u.expression_binary->expression_rhs, \
                            &token_cur)) != 0)
            { goto fail; }
        }
        else
        {
            if ((ret = mjs_parse_expression_binary_level(err, \
                            &new_expression_2->u.expression_binary->expression_rhs, \
                            level - 1, \
                            &token_cur)) != 0)
            { goto fail; }
        }
    }

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
    if (new_expression_2 != NULL)
    { mjs_ast_expression_destroy(new_expression_2); }
done:
    *token_cur_io = token_cur;
    return ret;
}

static int mjs_parse_expression_binary(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    return mjs_parse_expression_binary_level(err, \
            expression_out, \
            MJS_PARSE_EXPRESSION_BINARY_BEGIN, \
            token_cur_io);
}


/* Expression : Conditional */

static int mjs_parse_expression_conditional(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;
    struct mjs_ast_expression *new_expression_2 = NULL;

    if ((ret = mjs_parse_expression_binary(err, \
                    &new_expression, \
                    &token_cur)) != 0)
    { goto fail; }

    while ((token_cur != NULL) && (token_cur->value == '?'))
    {
        /* Skip '?' */
        token_cur = token_cur->next;

        /* Condition */
        new_expression_2 = mjs_ast_expression_new(MJS_AST_EXPRESSION_TYPE_CONDITIONAL);
        if (new_expression_2 == NULL)
        {
            MULTIPLE_ERROR_MALLOC();
            ret = -MULTIPLE_ERR_MALLOC;
            goto fail;
        }
        new_expression_2->u.expression_conditional->expression_condition = new_expression;
        new_expression = new_expression_2; new_expression_2 = NULL;

        /* True part */
        if ((ret = mjs_parse_expression_binary(err, \
                        &new_expression->u.expression_conditional->expression_true, \
                        &token_cur)) != 0)
        { goto fail; }

        if (token_cur == NULL)
        {
            MULTIPLE_ERROR_INTERNAL();
            ret = -MULTIPLE_ERR_INTERNAL;
            goto fail;
        }
        else if (token_cur->value != ':')
        {
            multiple_error_update(err, -MULTIPLE_ERR_PARSING, \
                    "%d:%d: error: expected \':\'", \
                    token_cur->pos_ln, token_cur->pos_col);
            ret = -MULTIPLE_ERR_PARSING;
            goto fail;
        }

        /* Skip ':' */
        token_cur = token_cur->next;

        /* False part */
        if ((ret = mjs_parse_expression_binary(err, \
                        &new_expression->u.expression_conditional->expression_false, \
                        &token_cur)) != 0)
        { goto fail; }
    }

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
    if (new_expression_2 != NULL)
    { mjs_ast_expression_destroy(new_expression_2); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Expression : Assignment */

static int mjs_parse_expression_assignment(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;
    struct mjs_ast_expression *new_expression_2 = NULL;

    if ((ret = mjs_parse_expression_conditional(err, \
                    &new_expression, \
                    &token_cur)) != 0)
    { goto fail; }

    while ((token_cur != NULL)&&(token_cur->value != TOKEN_FINISH))
    {
        switch (token_cur->value)
        {
            case '=':
            case TOKEN_OP_ADD_ASSIGN:
            case TOKEN_OP_SUB_ASSIGN:
            case TOKEN_OP_MUL_ASSIGN:
            case TOKEN_OP_DIV_ASSIGN:
            case TOKEN_OP_MOD_ASSIGN:
            case TOKEN_OP_LSHIFT_ASSIGN:
            case TOKEN_OP_RSHIFT_ASSIGN:
            case TOKEN_OP_AND_ASSIGN:
            case TOKEN_OP_OR_ASSIGN:
            case TOKEN_OP_XOR_ASSIGN:
            case TOKEN_OP_RRR_ASSIGN:

                new_expression_2 = mjs_ast_expression_new(MJS_AST_EXPRESSION_TYPE_ASSIGNMENT);
                if (new_expression_2 == NULL)
                { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
                new_expression_2->u.expression_assignment = mjs_ast_expression_assignment_new();
                if (new_expression_2->u.expression_assignment == NULL)
                { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
                /* Operator */
                if ((new_expression_2->u.expression_assignment->op = token_clone(token_cur)) == NULL)
                {
                    MULTIPLE_ERROR_MALLOC();
                    ret = -MULTIPLE_ERR_MALLOC;
                    goto fail;
                }
                token_cur = token_cur->next;
                /* Left Hand Side */
                new_expression_2->u.expression_assignment->expression_lhs = new_expression;
                new_expression = new_expression_2; new_expression_2 = NULL;
                /* Right Hand Side */
                if ((ret = mjs_parse_expression(err, \
                                &new_expression->u.expression_assignment->expression_rhs, \
                                &token_cur)) != 0)
                { goto fail; }
                break;

            default:
                goto finish;
        }
    }
finish:

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
    if (new_expression_2 != NULL)
    { mjs_ast_expression_destroy(new_expression_2); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Expression */

static int mjs_parse_expression(struct multiple_error *err, \
        struct mjs_ast_expression **expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_expression *new_expression = NULL;

    if ((ret = mjs_parse_expression_assignment(err, \
                    &new_expression, \
                    &token_cur)) != 0)
    { goto fail; }

    *expression_out = new_expression;

    goto done;
fail:
    if (new_expression != NULL)
    { mjs_ast_expression_destroy(new_expression); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Statement : Expression */

static int mjs_parse_statement_expression(struct multiple_error *err, \
        struct mjs_ast_statement_expression **statement_expression_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_statement_expression *new_statement_expression = NULL;

    if ((new_statement_expression = mjs_ast_statement_expression_new()) == NULL)
    { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }

    if ((ret = mjs_parse_expression(err, \
                    &new_statement_expression->expression, \
                    &token_cur)) != 0)
    { goto fail; }

    *statement_expression_out = new_statement_expression;

    goto done;
fail:
    if (new_statement_expression != NULL)
    { mjs_ast_statement_expression_destroy(new_statement_expression); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Statement */

static int mjs_parse_statement(struct multiple_error *err, \
        struct mjs_ast_statement **statement_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_statement *new_statement = NULL;

    if (token_cur->value == TOKEN_KEYWORD_FUNCTION)
    {
        /* Function Declaration */

        MULTIPLE_ERROR_INTERNAL();
        ret = -MULTIPLE_ERR_INTERNAL;
        goto fail;
    }
    else if (token_cur->value == '{')
    {
        /* Block */

        MULTIPLE_ERROR_INTERNAL();
        ret = -MULTIPLE_ERR_INTERNAL;
        goto fail;
    }
    else if (token_cur->value == ';')
    {
        if ((new_statement = mjs_ast_statement_new(MJS_AST_STATEMENT_TYPE_EMPTY)) == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        token_cur = token_cur->next;
    }
    else
    {
        if ((new_statement = mjs_ast_statement_new(MJS_AST_STATEMENT_TYPE_EXPRESSION)) == NULL)
        { MULTIPLE_ERROR_MALLOC(); ret = -MULTIPLE_ERR_MALLOC; goto fail; }
        if ((ret = mjs_parse_statement_expression(err, \
                        &new_statement->u.statement_expression, \
                        &token_cur)) != 0)
        { goto fail; }
    }

    *statement_out = new_statement;

    goto done;
fail:
    if (new_statement != NULL)
    { mjs_ast_statement_destroy(new_statement); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Source Element */

static int mjs_parse_source_element(struct multiple_error *err, \
        struct mjs_ast_source_element **source_element_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_source_element *new_source_element = NULL;

    if (token_cur->value == TOKEN_KEYWORD_FUNCTION)
    {
        /* SourceElement: FunctionDeclaration */

        /* Skip 'function' */
        token_cur = token_cur->next;

        MULTIPLE_ERROR_INTERNAL();
        ret = -MULTIPLE_ERR_INTERNAL;
        goto fail;
    }
    else
    {
        /* SourceElement: Statement */

        if ((new_source_element = mjs_ast_source_element_new( \
                        MJS_AST_SOURCE_ELEMENT_TYPE_STATEMENT)) == NULL)
        {
            MULTIPLE_ERROR_MALLOC();
            ret = -MULTIPLE_ERR_MALLOC;
            goto fail;
        }
        if ((ret = mjs_parse_statement(err, \
                        &new_source_element->u.statement, \
                        &token_cur)) != 0)
        { goto fail; }
    }

    *source_element_out = new_source_element;

    goto done;
fail:
    if (new_source_element != NULL)
    { mjs_ast_source_element_destroy(new_source_element); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Source Elements */

static int mjs_parse_source_elements(struct multiple_error *err, \
        struct mjs_ast_source_element_list **source_element_list_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_source_element *new_source_element = NULL;
    struct mjs_ast_source_element_list *new_source_element_list = NULL;

    if ((new_source_element_list = mjs_ast_source_element_list_new()) == NULL)
    {
        MULTIPLE_ERROR_MALLOC();
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }

    while ((token_cur != NULL) && (token_cur->value != TOKEN_FINISH))
    {

        if ((ret = mjs_parse_source_element(err, \
                        &new_source_element, \
                        &token_cur)) != 0)
        { goto fail; }

        mjs_ast_source_element_list_append(new_source_element_list, \
                new_source_element);
        new_source_element = NULL;
    }

    *source_element_list_out = new_source_element_list;

    goto done;
fail:
    if (new_source_element_list != NULL)
    { mjs_ast_source_element_list_destroy(new_source_element_list); }
done:
    *token_cur_io = token_cur;
    return ret;
}


/* Program */

static int mjs_parse_program(struct multiple_error *err, \
        struct mjs_ast_program **program_out, \
        struct token **token_cur_io)
{
    int ret = 0;
    struct token *token_cur = *token_cur_io;
    struct mjs_ast_program *new_program = NULL;

    if ((new_program = mjs_ast_program_new()) == NULL)
    {
        MULTIPLE_ERROR_MALLOC();
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }

    if ((ret = mjs_parse_source_elements(err, \
                    &new_program->source_elements, \
                    &token_cur)) != 0)
    { goto fail; }

    *program_out = new_program;

    goto done;
fail:
    if (new_program != NULL)
    { mjs_ast_program_destroy(new_program); }
done:
    *token_cur_io = token_cur;
    return ret;
}


int mjs_parse(struct multiple_error *err, \
        struct mjs_ast_program **program_out, \
        struct token_list *list)
{
    int ret = 0;
    struct mjs_ast_program *new_program = NULL;
    struct token *token_cur = list->begin;

    *program_out = NULL;

    if ((ret = mjs_parse_program(err, \
                    &new_program, \
                    &token_cur)) != 0)
    { goto fail; }

    *program_out = new_program;

    goto done;
fail:
    if (new_program != NULL) { mjs_ast_program_destroy(new_program); }
done:
    return ret;
}

