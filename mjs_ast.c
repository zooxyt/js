/* Multiple JavaScript Programming Language : Abstract Syntax Tree
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

#include <stdlib.h>
#include <stdio.h>

#include "multiple_err.h"
#include "mjs_lexer.h"
#include "mjs_ast.h"


/* Expression : Primary */

struct mjs_ast_expression_primary *mjs_ast_expression_primary_new(enum mjs_ast_expression_primary_type type)
{
    struct mjs_ast_expression_primary *new_expression_primary = NULL;

    new_expression_primary = (struct mjs_ast_expression_primary *)malloc(sizeof(struct mjs_ast_expression_primary));
    if (new_expression_primary == NULL) { goto fail; }
    new_expression_primary->type = MJS_AST_EXPRESSION_PRIMARY_TYPE_UNKNOWN;
    switch (type)
    {
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_THIS:
            new_expression_primary->u.token_this = NULL;
            break;
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_IDENTIFIER:
            new_expression_primary->u.token_identifier = NULL;
            break;
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_LITERAL:
            new_expression_primary->u.token_literal = NULL;
            break;
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_ARRAY_LITERAL:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_OBJECT_LITERAL:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_SUB:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_UNKNOWN:
            goto fail;
    }
    new_expression_primary->type = type;
    goto done;
fail:
    if (new_expression_primary != NULL)
    {
        mjs_ast_expression_primary_destroy(new_expression_primary);
        new_expression_primary = NULL;
    }
done:
    return new_expression_primary;
}

int mjs_ast_expression_primary_destroy(struct mjs_ast_expression_primary *expression_primary)
{
    switch (expression_primary->type)
    {
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_THIS:
            if (expression_primary->u.token_this != NULL)
            { token_destroy(expression_primary->u.token_this); }
            break;
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_IDENTIFIER:
            if (expression_primary->u.token_identifier != NULL)
            { token_destroy(expression_primary->u.token_identifier); }
            break;
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_LITERAL:
            if (expression_primary->u.token_literal != NULL)
            { token_destroy(expression_primary->u.token_literal); }
            break;
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_ARRAY_LITERAL:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_OBJECT_LITERAL:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_SUB:
        case MJS_AST_EXPRESSION_PRIMARY_TYPE_UNKNOWN:
            break;
    }
    free(expression_primary);

    return 0;
}


/* Expression : Assignment */

struct mjs_ast_expression_assignment *mjs_ast_expression_assignment_new(void)
{
    struct mjs_ast_expression_assignment *new_expression_assignment = NULL;

    new_expression_assignment = (struct mjs_ast_expression_assignment *)malloc(sizeof(struct mjs_ast_expression_assignment));
    if (new_expression_assignment == NULL) { goto fail; }
    new_expression_assignment->expression_lhs = NULL;
    new_expression_assignment->expression_rhs = NULL;
    new_expression_assignment->op = NULL;

    goto done;
fail:
    if (new_expression_assignment != NULL)
    {
        mjs_ast_expression_assignment_destroy(new_expression_assignment);
        new_expression_assignment = NULL;
    }
done:
    return new_expression_assignment;
}

int mjs_ast_expression_assignment_destroy(struct mjs_ast_expression_assignment *expression_assignment)
{
    if (expression_assignment->expression_lhs != NULL) 
    { mjs_ast_expression_destroy(expression_assignment->expression_lhs); }
    if (expression_assignment->expression_rhs != NULL)
    { mjs_ast_expression_destroy(expression_assignment->expression_rhs); }
    if (expression_assignment->op != NULL) 
    { token_destroy(expression_assignment->op); }
    free(expression_assignment);

    return 0;
}


/* Expression : Conditional */

struct mjs_ast_expression_conditional *mjs_ast_expression_conditional_new(void)
{
    struct mjs_ast_expression_conditional *new_expression_conditional = NULL;

    new_expression_conditional = (struct mjs_ast_expression_conditional *)malloc(sizeof(struct mjs_ast_expression_conditional));
    if (new_expression_conditional == NULL) { goto fail; }
    new_expression_conditional->expression_condition = NULL;
    new_expression_conditional->expression_true = NULL;
    new_expression_conditional->expression_false = NULL;

    goto done;
fail:
    if (new_expression_conditional != NULL)
    {
        mjs_ast_expression_conditional_destroy(new_expression_conditional);
        new_expression_conditional = NULL;
    }
done:
    return new_expression_conditional;
}

int mjs_ast_expression_conditional_destroy(struct mjs_ast_expression_conditional *expression_conditional)
{
    if (expression_conditional->expression_condition != NULL) 
    { mjs_ast_expression_destroy(expression_conditional->expression_condition); }
    if (expression_conditional->expression_true != NULL)
    { mjs_ast_expression_destroy(expression_conditional->expression_true); }
    if (expression_conditional->expression_false != NULL)
    { mjs_ast_expression_destroy(expression_conditional->expression_false); }
    free(expression_conditional);

    return 0;
}

/* Expression : Binary */

struct mjs_ast_expression_binary *mjs_ast_expression_binary_new(void)
{
    struct mjs_ast_expression_binary *new_expression_binary = NULL;

    new_expression_binary = (struct mjs_ast_expression_binary *)malloc(sizeof(struct mjs_ast_expression_binary));
    if (new_expression_binary == NULL) { goto fail; }
    new_expression_binary->expression_lhs = NULL;
    new_expression_binary->expression_rhs = NULL;
    new_expression_binary->op = NULL;

    goto done;
fail:
    if (new_expression_binary != NULL)
    {
        mjs_ast_expression_binary_destroy(new_expression_binary);
        new_expression_binary = NULL;
    }
done:
    return new_expression_binary;
}

int mjs_ast_expression_binary_destroy(struct mjs_ast_expression_binary *expression_binary)
{
    if (expression_binary->expression_lhs != NULL) 
    { mjs_ast_expression_destroy(expression_binary->expression_lhs); }
    if (expression_binary->expression_rhs != NULL)
    { mjs_ast_expression_destroy(expression_binary->expression_rhs); }
    if (expression_binary->op != NULL) 
    { token_destroy(expression_binary->op); }
    free(expression_binary);

    return 0;
}


/* Expression : Unary */

struct mjs_ast_expression_unary *mjs_ast_expression_unary_new(void)
{
    struct mjs_ast_expression_unary *new_expression_unary = NULL;

    new_expression_unary = (struct mjs_ast_expression_unary *)malloc(sizeof(struct mjs_ast_expression_unary));
    if (new_expression_unary == NULL) { goto fail; }
    new_expression_unary->op = NULL;
    new_expression_unary->expression_sub = NULL;

    goto done;
fail:
    if (new_expression_unary != NULL)
    {
        mjs_ast_expression_unary_destroy(new_expression_unary);
        new_expression_unary = NULL;
    }
done:
    return new_expression_unary;
}

int mjs_ast_expression_unary_destroy(struct mjs_ast_expression_unary *expression_unary)
{
    if (expression_unary->op != NULL) 
    { token_destroy(expression_unary->op); }
    if (expression_unary->expression_sub != NULL)
    { mjs_ast_expression_destroy(expression_unary->expression_sub); }
    free(expression_unary);

    return 0;
}


/* Expression : Postfix */

struct mjs_ast_expression_postfix *mjs_ast_expression_postfix_new(void)
{
    struct mjs_ast_expression_postfix *new_expression_postfix = NULL;

    new_expression_postfix = (struct mjs_ast_expression_postfix *)malloc(sizeof(struct mjs_ast_expression_postfix));
    if (new_expression_postfix == NULL) { goto fail; }
    new_expression_postfix->op = NULL;
    new_expression_postfix->expression_sub = NULL;

    goto done;
fail:
    if (new_expression_postfix != NULL)
    {
        mjs_ast_expression_postfix_destroy(new_expression_postfix);
        new_expression_postfix = NULL;
    }
done:
    return new_expression_postfix;
}

int mjs_ast_expression_postfix_destroy(struct mjs_ast_expression_postfix *expression_postfix)
{
    if (expression_postfix->op != NULL) 
    { token_destroy(expression_postfix->op); }
    if (expression_postfix->expression_sub != NULL)
    { mjs_ast_expression_destroy(expression_postfix->expression_sub); }
    free(expression_postfix);

    return 0;
}


/* Expression : Member Expression : dot */

struct mjs_ast_member_expression_dot *mjs_ast_member_expression_dot_new(void)
{
    struct mjs_ast_member_expression_dot *new_member_expression_dot = NULL;

    new_member_expression_dot = (struct mjs_ast_member_expression_dot *)malloc(sizeof(struct mjs_ast_member_expression_dot));
    if (new_member_expression_dot == NULL) { goto fail; }
    new_member_expression_dot->member_expression = NULL;
    new_member_expression_dot->identifier_name = NULL;

    goto done;
fail:
    if (new_member_expression_dot != NULL)
    {
        mjs_ast_member_expression_dot_destroy(new_member_expression_dot);
        new_member_expression_dot = NULL;
    }
done:
    return new_member_expression_dot;
}

int mjs_ast_member_expression_dot_destroy(struct mjs_ast_member_expression_dot *member_expression_dot)
{
    if (member_expression_dot->identifier_name != NULL)
    { token_destroy(member_expression_dot->identifier_name); }
    if (member_expression_dot->member_expression != NULL)
    { mjs_ast_member_expression_destroy(member_expression_dot->member_expression); }
    free(member_expression_dot);
    return 0;
}


/* Member Expression */

struct mjs_ast_member_expression *mjs_ast_member_expression_new(enum mjs_ast_member_expression_type type)
{
    struct mjs_ast_member_expression *new_member_expression = NULL;

    new_member_expression = (struct mjs_ast_member_expression *)malloc( \
            sizeof(struct mjs_ast_member_expression));
    if (new_member_expression == NULL) { goto fail; }
    new_member_expression->type = MJS_AST_MEMBER_EXPRESSION_TYPE_UNKNOWN;
    switch (type)
    {
        case MJS_AST_MEMBER_EXPRESSION_TYPE_PRIMARY:
            new_member_expression->u.expression_primary = NULL;
            break;
        case MJS_AST_MEMBER_EXPRESSION_TYPE_FUNCTION:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_BRACKET:
            goto fail;
        case MJS_AST_MEMBER_EXPRESSION_TYPE_DOT:
            new_member_expression->u.expression_dot = NULL;
            break;
        case MJS_AST_MEMBER_EXPRESSION_TYPE_NEW:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_UNKNOWN:
            goto fail;
    }
    new_member_expression->type = type;

    goto done;
fail:
    if (new_member_expression != NULL)
    {
        mjs_ast_member_expression_destroy(new_member_expression);
        new_member_expression = NULL;
    }
done:
    return new_member_expression;
}

int mjs_ast_member_expression_destroy(struct mjs_ast_member_expression *member_expression)
{
    switch (member_expression->type)
    {
        case MJS_AST_MEMBER_EXPRESSION_TYPE_PRIMARY:
            if (member_expression->u.expression_primary != NULL)
            { mjs_ast_expression_destroy(member_expression->u.expression_primary); }
            break;
        case MJS_AST_MEMBER_EXPRESSION_TYPE_FUNCTION:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_BRACKET:
            break;
        case MJS_AST_MEMBER_EXPRESSION_TYPE_DOT:
            if (member_expression->u.expression_dot != NULL)
            { mjs_ast_member_expression_dot_destroy(member_expression->u.expression_dot); }
            break;
        case MJS_AST_MEMBER_EXPRESSION_TYPE_NEW:
        case MJS_AST_MEMBER_EXPRESSION_TYPE_UNKNOWN:
            break;
    }
    free(member_expression);

    return 0;
}


/* Expression : Left Hand Side : New */

struct mjs_ast_expression_lhs_new_expression *mjs_ast_expression_lhs_new_expression_new( \
        enum mjs_ast_expression_lhs_new_expression_type type)
{
    struct mjs_ast_expression_lhs_new_expression *new_expression_lhs_new_expression = NULL;

    new_expression_lhs_new_expression = (struct mjs_ast_expression_lhs_new_expression *)malloc( \
            sizeof(struct mjs_ast_expression_lhs_new_expression));
    if (new_expression_lhs_new_expression == NULL) { goto fail; }
    new_expression_lhs_new_expression->type = MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_UNKNOWN;
    switch (type)
    {
        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_MEMBER:
            new_expression_lhs_new_expression->u.member_expression = NULL;
            break;
        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_NEW:
            new_expression_lhs_new_expression->u.sub_new_expression = NULL;
            break;

        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_UNKNOWN:
            goto fail;
    }
    new_expression_lhs_new_expression->type = type;

    goto done;
fail:
    if (new_expression_lhs_new_expression != NULL)
    {
        mjs_ast_expression_lhs_new_expression_destroy(new_expression_lhs_new_expression);
        new_expression_lhs_new_expression = NULL;
    }
done:
    return new_expression_lhs_new_expression;
}

int mjs_ast_expression_lhs_new_expression_destroy(struct mjs_ast_expression_lhs_new_expression *expression_lhs_new_expression)
{
    switch (expression_lhs_new_expression->type)
    {
        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_MEMBER:
            if (expression_lhs_new_expression->u.member_expression != NULL)
            { mjs_ast_member_expression_destroy(expression_lhs_new_expression->u.member_expression); }
            break;
        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_NEW:
            if (expression_lhs_new_expression->u.sub_new_expression != NULL)
            { mjs_ast_expression_lhs_new_expression_destroy(expression_lhs_new_expression->u.sub_new_expression); }
            break;

        case MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_UNKNOWN:
            break;
    }

    free(expression_lhs_new_expression);

    return 0;
}


/* Argument */

struct mjs_ast_argument *mjs_ast_argument_new(void)
{
    struct mjs_ast_argument *new_argument = NULL;

    new_argument = (struct mjs_ast_argument *)malloc(sizeof(struct mjs_ast_argument));
    if (new_argument == NULL) { goto fail; }
    new_argument->expr = NULL;
    new_argument->prev = NULL;
    new_argument->next = NULL;

    goto done;
fail:
    if (new_argument != NULL)
    {
        mjs_ast_argument_destroy(new_argument);
        new_argument = NULL;
    }
done:
    return new_argument;
}

int mjs_ast_argument_destroy(struct mjs_ast_argument *argument)
{
    if (argument->expr != NULL) { mjs_ast_expression_destroy(argument->expr); }
    free(argument);
    return 0;
}

struct mjs_ast_argument_list *mjs_ast_argument_list_new(void)
{
    struct mjs_ast_argument_list *new_list = NULL;

    new_list = (struct mjs_ast_argument_list *)malloc(sizeof(struct mjs_ast_argument_list));
    if (new_list == NULL) { goto fail; }
    new_list->begin = NULL;
    new_list->end = NULL;
    new_list->size = 0;

    goto done;
fail:
    if (new_list != NULL)
    {
        mjs_ast_argument_list_destroy(new_list);
        new_list = NULL;
    }
done:
    return new_list;
}

int mjs_ast_argument_list_destroy(struct mjs_ast_argument_list *argument_list)
{
    struct mjs_ast_argument *argument_cur, *argument_next;

    argument_cur = argument_list->begin;
    while (argument_cur != NULL)
    {
        argument_next = argument_cur->next;
        mjs_ast_argument_destroy(argument_cur);
        argument_cur = argument_next;
    }
    free(argument_list);

    return 0;
}

int mjs_ast_argument_list_append(struct mjs_ast_argument_list *argument_list, \
        struct mjs_ast_argument *new_argument)
{
    if (argument_list->begin == NULL)
    {
        argument_list->begin = argument_list->end = new_argument;
    }
    else
    {
        new_argument->prev = argument_list->end;
        argument_list->end->next = new_argument;
        argument_list->end = new_argument;
    }
    argument_list->size = 0;

    return 0;
}


/* Expression : Left Hand Side : Call : Member Args */

struct mjs_ast_expression_lhs_call_expression_member_args *mjs_ast_expression_lhs_call_expression_member_args_new(void)
{
    struct mjs_ast_expression_lhs_call_expression_member_args *new_expression_member_args = NULL;

    new_expression_member_args = (struct mjs_ast_expression_lhs_call_expression_member_args *)malloc( \
            sizeof(struct mjs_ast_expression_lhs_call_expression_member_args));
    if (new_expression_member_args == NULL) { goto fail; }
    new_expression_member_args->member_expr = NULL;
    new_expression_member_args->args = NULL;

    goto done;
fail:
    if (new_expression_member_args != NULL)
    {
        mjs_ast_expression_lhs_call_expression_member_args_destroy(new_expression_member_args);
        new_expression_member_args = NULL;
    }
done:
    return new_expression_member_args;
}

int mjs_ast_expression_lhs_call_expression_member_args_destroy( \
        struct mjs_ast_expression_lhs_call_expression_member_args *expression_lhs_call_expression_member_args)
{
    if (expression_lhs_call_expression_member_args->member_expr != NULL)
    { mjs_ast_member_expression_destroy(expression_lhs_call_expression_member_args->member_expr); }
    if (expression_lhs_call_expression_member_args->args != NULL)
    { mjs_ast_argument_list_destroy(expression_lhs_call_expression_member_args->args); }
    free(expression_lhs_call_expression_member_args);

    return 0;
}


/* Expression : Left Hand Side : Call */

struct mjs_ast_expression_lhs_call_expression *mjs_ast_expression_lhs_call_expression_new( \
        enum mjs_ast_expression_lhs_call_expression_type type)
{
    struct mjs_ast_expression_lhs_call_expression *call_expression_lhs_call_expression = NULL;

    call_expression_lhs_call_expression = (struct mjs_ast_expression_lhs_call_expression *)malloc( \
            sizeof(struct mjs_ast_expression_lhs_call_expression));
    if (call_expression_lhs_call_expression == NULL) { goto fail; }
    call_expression_lhs_call_expression->type = MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_UNKNOWN;
    switch (type)
    {
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_MEMBER_ARGS:
            call_expression_lhs_call_expression->u.member_args = NULL;
            break;
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_CALL_ARGS:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_BRACKET:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_DOT:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_UNKNOWN:
            goto fail;
    }
    call_expression_lhs_call_expression->type = type;

    goto done;
fail:
    if (call_expression_lhs_call_expression != NULL)
    {
        mjs_ast_expression_lhs_call_expression_destroy(call_expression_lhs_call_expression);
        call_expression_lhs_call_expression = NULL;
    }
done:
    return call_expression_lhs_call_expression;
}

int mjs_ast_expression_lhs_call_expression_destroy(struct mjs_ast_expression_lhs_call_expression *expression_lhs_call_expression)
{
    switch (expression_lhs_call_expression->type)
    {
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_MEMBER_ARGS:
            if (expression_lhs_call_expression->u.member_args != NULL)
            { mjs_ast_expression_lhs_call_expression_member_args_destroy(expression_lhs_call_expression->u.member_args); }
            break;
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_CALL_ARGS:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_BRACKET:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_DOT:
        case MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_UNKNOWN:
            break;
    }
    free(expression_lhs_call_expression);

    return 0;
}


/* Expression : Left Hand Side */

struct mjs_ast_expression_lhs *mjs_ast_expression_lhs_new(enum mjs_ast_expression_lhs_type type)
{
    struct mjs_ast_expression_lhs *new_expression_lhs = NULL;

    new_expression_lhs = (struct mjs_ast_expression_lhs *)malloc(sizeof(struct mjs_ast_expression_lhs));
    if (new_expression_lhs == NULL) { goto fail; }
    new_expression_lhs->type = MJS_AST_EXPRESSION_LHS_TYPE_UNKNOWN;
    switch (type)
    {
        case MJS_AST_EXPRESSION_LHS_TYPE_NEW:
            new_expression_lhs->u.expression_lhs_new_expression = NULL;
            break;
        case MJS_AST_EXPRESSION_LHS_TYPE_CALL:
            new_expression_lhs->u.expression_lhs_call_expression = NULL;
            break;

        case MJS_AST_EXPRESSION_LHS_TYPE_UNKNOWN:
            goto fail;
    }

    new_expression_lhs->type = type;

    goto done;
fail:
    if (new_expression_lhs != NULL)
    {
        mjs_ast_expression_lhs_destroy(new_expression_lhs);
        new_expression_lhs = NULL;
    }
done:
    return new_expression_lhs;
}

int mjs_ast_expression_lhs_destroy(struct mjs_ast_expression_lhs *expression_lhs)
{
    switch (expression_lhs->type)
    {
        case MJS_AST_EXPRESSION_LHS_TYPE_NEW:
            if (expression_lhs->u.expression_lhs_new_expression != NULL)
            { mjs_ast_expression_lhs_new_expression_destroy(expression_lhs->u.expression_lhs_new_expression); }
            break;
        case MJS_AST_EXPRESSION_LHS_TYPE_CALL:
            if (expression_lhs->u.expression_lhs_call_expression != NULL)
            { mjs_ast_expression_lhs_call_expression_destroy(expression_lhs->u.expression_lhs_call_expression); }
            break;

        case MJS_AST_EXPRESSION_LHS_TYPE_UNKNOWN:
            break;
    }
    free(expression_lhs);

    return 0;
}


/* Expression */

struct mjs_ast_expression *mjs_ast_expression_new(enum mjs_ast_expression_type type)
{
    struct mjs_ast_expression *new_expression = NULL;

    new_expression = (struct mjs_ast_expression *)malloc(sizeof(struct mjs_ast_expression));
    if (new_expression == NULL) { goto fail; }
    new_expression->type = MJS_AST_EXPRESSION_TYPE_UNKNOWN;
    switch (type)
    {
        case MJS_AST_EXPRESSION_TYPE_ASSIGNMENT:
            new_expression->u.expression_assignment = NULL;
            break;
        case MJS_AST_EXPRESSION_TYPE_CONDITIONAL:
            new_expression->u.expression_conditional = NULL;
            break;
        case MJS_AST_EXPRESSION_TYPE_BINARY:
            new_expression->u.expression_binary = NULL;
            break;
        case MJS_AST_EXPRESSION_TYPE_UNARY:
            new_expression->u.expression_unary = NULL;
            break;
        case MJS_AST_EXPRESSION_TYPE_POSTFIX:
            new_expression->u.expression_postfix = NULL;
            break;
        case MJS_AST_EXPRESSION_TYPE_LHS:
            new_expression->u.expression_lhs = NULL;
            break;
        case MJS_AST_EXPRESSION_TYPE_PRIMARY:
            new_expression->u.expression_primary = NULL;
            break;

        case MJS_AST_EXPRESSION_TYPE_UNKNOWN:
            goto fail;
    }
    new_expression->type = type;

    goto done;
fail:
    if (new_expression != NULL)
    {
        mjs_ast_expression_destroy(new_expression);
        new_expression = NULL;
    }
done:
    return new_expression;
}

int mjs_ast_expression_destroy(struct mjs_ast_expression *ast_expression)
{
    switch (ast_expression->type)
    {
        case MJS_AST_EXPRESSION_TYPE_ASSIGNMENT:
            if (ast_expression->u.expression_assignment != NULL)
            { mjs_ast_expression_assignment_destroy(ast_expression->u.expression_assignment); }
            break;
        case MJS_AST_EXPRESSION_TYPE_CONDITIONAL:
            if (ast_expression->u.expression_conditional != NULL)
            { mjs_ast_expression_conditional_destroy(ast_expression->u.expression_conditional); }
            break;
        case MJS_AST_EXPRESSION_TYPE_BINARY:
            if (ast_expression->u.expression_binary != NULL)
            { mjs_ast_expression_binary_destroy(ast_expression->u.expression_binary); }
            break;
        case MJS_AST_EXPRESSION_TYPE_UNARY:
            if (ast_expression->u.expression_unary != NULL)
            { mjs_ast_expression_unary_destroy(ast_expression->u.expression_unary); }
            break;
        case MJS_AST_EXPRESSION_TYPE_POSTFIX:
            if (ast_expression->u.expression_postfix != NULL)
            { mjs_ast_expression_postfix_destroy(ast_expression->u.expression_postfix); }
            break;
        case MJS_AST_EXPRESSION_TYPE_LHS:
            if (ast_expression->u.expression_lhs != NULL)
            { mjs_ast_expression_lhs_destroy(ast_expression->u.expression_lhs); }
            break;
        case MJS_AST_EXPRESSION_TYPE_PRIMARY:
            if (ast_expression->u.expression_primary != NULL)
            { mjs_ast_expression_primary_destroy(ast_expression->u.expression_primary); }
            break;

        case MJS_AST_EXPRESSION_TYPE_UNKNOWN:
            break;
    }
    free(ast_expression);

    return 0;
}


/* Statement : Expression */

struct mjs_ast_statement_expression *mjs_ast_statement_expression_new(void)
{
    struct mjs_ast_statement_expression *new_statement_expression = NULL;

    new_statement_expression = (struct mjs_ast_statement_expression *)malloc( \
            sizeof(struct mjs_ast_statement_expression));
    if (new_statement_expression == NULL) { goto fail; }
    new_statement_expression->expression = NULL;

    goto done;
fail:
    if (new_statement_expression != NULL)
    {
        mjs_ast_statement_expression_destroy(new_statement_expression);
        new_statement_expression = NULL;
    }
done:
    return new_statement_expression;
}

int mjs_ast_statement_expression_destroy( \
        struct mjs_ast_statement_expression *statement_expression)
{
    if (statement_expression->expression != NULL)
    { mjs_ast_expression_destroy(statement_expression->expression); }
    free(statement_expression);

    return 0;
}


/* Statement */

struct mjs_ast_statement *mjs_ast_statement_new(enum mjs_ast_statement_type type)
{
    struct mjs_ast_statement *new_statement = NULL;

    if ((new_statement = (struct mjs_ast_statement *)malloc(sizeof(struct mjs_ast_statement))) == NULL)
    { goto fail; }
    new_statement->type = MJS_AST_STATEMENT_TYPE_UNKNOWN;
    switch (type)
    {
        case MJS_AST_STATEMENT_TYPE_EXPRESSION:
            new_statement->u.statement_expression = NULL;
            break;

        case MJS_AST_STATEMENT_TYPE_UNKNOWN:
        case MJS_AST_STATEMENT_TYPE_BLOCK:
        case MJS_AST_STATEMENT_TYPE_VARIABLE:
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
            goto fail;
    }
    new_statement->type = type;

    goto done;
fail:
    if (new_statement != NULL)
    {
        mjs_ast_statement_destroy(new_statement);
        new_statement = NULL;
    }
done:
    return new_statement;
}

int mjs_ast_statement_destroy(struct mjs_ast_statement *statement)
{
    switch (statement->type)
    {
        case MJS_AST_STATEMENT_TYPE_EXPRESSION:
            if (statement->u.statement_expression != NULL)
            { mjs_ast_statement_expression_destroy(statement->u.statement_expression); }
            break;

        case MJS_AST_STATEMENT_TYPE_UNKNOWN:
        case MJS_AST_STATEMENT_TYPE_BLOCK:
        case MJS_AST_STATEMENT_TYPE_VARIABLE:
        case MJS_AST_STATEMENT_TYPE_EMPTY:
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
            break;
    }
    free(statement);

    return 0;
}


/* Source Elements */

struct mjs_ast_source_element *mjs_ast_source_element_new(enum mjs_ast_source_element_type type)
{
    struct mjs_ast_source_element *new_source_element = NULL;

    new_source_element = (struct mjs_ast_source_element *)malloc(sizeof(struct mjs_ast_source_element));
    if (new_source_element == NULL) { goto fail; }
    new_source_element->next = NULL;
    new_source_element->type = MJS_AST_SOURCE_ELEMENT_TYPE_UNKNOWN;

    switch (type)
    {
        case MJS_AST_SOURCE_ELEMENT_TYPE_STATEMENT:
            new_source_element->u.statement = NULL;
            break;

        case MJS_AST_SOURCE_ELEMENT_TYPE_FUNDECL:
        case MJS_AST_SOURCE_ELEMENT_TYPE_UNKNOWN:
            goto fail;
    }
    new_source_element->type = type;

    goto done;
fail:
    if (new_source_element != NULL)
    {
        mjs_ast_source_element_destroy(new_source_element);
        new_source_element = NULL;
    }
done:
    return new_source_element;
}

int mjs_ast_source_element_destroy(struct mjs_ast_source_element *source_element)
{
    switch (source_element->type)
    {
        case MJS_AST_SOURCE_ELEMENT_TYPE_STATEMENT:
            if (source_element->u.statement != NULL)
            { mjs_ast_statement_destroy(source_element->u.statement); }
            break;

        case MJS_AST_SOURCE_ELEMENT_TYPE_FUNDECL:
        case MJS_AST_SOURCE_ELEMENT_TYPE_UNKNOWN:
            break;
    }
    free(source_element);

    return 0;
}


/* Source Element List */

struct mjs_ast_source_element_list *mjs_ast_source_element_list_new(void)
{
    struct mjs_ast_source_element_list *new_source_element_list = NULL;

    new_source_element_list = (struct mjs_ast_source_element_list *)malloc(sizeof(struct mjs_ast_source_element_list));
    if (new_source_element_list == NULL) { goto fail; }
    new_source_element_list->begin = NULL;
    new_source_element_list->end = NULL;

    goto done;
fail:
    if (new_source_element_list != NULL)
    {
        mjs_ast_source_element_list_destroy(new_source_element_list);
        new_source_element_list = NULL;
    }
done:
    return new_source_element_list;
}

int mjs_ast_source_element_list_destroy(struct mjs_ast_source_element_list *list)
{
    struct mjs_ast_source_element *source_element_cur, *source_element_next;

    source_element_cur = list->begin;
    while (source_element_cur != NULL)
    {
        source_element_next = source_element_cur->next; 
        mjs_ast_source_element_destroy(source_element_cur);
        source_element_cur = source_element_next; 
    }
    free(list);

    return 0;
}

int mjs_ast_source_element_list_append(struct mjs_ast_source_element_list *list, \
        struct mjs_ast_source_element *new_source_element)
{
    if (list->begin == NULL)
    {
        list->begin = list->end = new_source_element;
    }
    else
    {
        list->end->next = new_source_element;
        list->end = new_source_element;
    }

    return 0;
}


/* Program */

struct mjs_ast_program *mjs_ast_program_new(void)
{
    struct mjs_ast_program *new_program = NULL;

    new_program = (struct mjs_ast_program *)malloc(sizeof(struct mjs_ast_program));
    if (new_program == NULL) { goto fail; }
    new_program->source_elements = NULL;

    goto done;
fail:
    if (new_program != NULL)
    {
        mjs_ast_program_destroy(new_program);
        new_program = NULL;
    }
done:
    return new_program;
}

int mjs_ast_program_destroy(struct mjs_ast_program *program)
{
    if (program->source_elements != NULL)
    { mjs_ast_source_element_list_destroy(program->source_elements); }
    free(program);

    return 0;
}

