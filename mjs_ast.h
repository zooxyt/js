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


#ifndef _MJS_AST_H_
#define _MJS_AST_H_

#include <stdio.h>
#include <stdint.h>
#include "mjs_lexer.h"


/* Expression : Primary */
/*
 * PrimaryExpression :
 *  'this'
 *  Identifier
 *  Literal
 *  ArrayLiteral
 *  ObjectLiteral
 *  '(' Expression ')'
 */
enum mjs_ast_expression_primary_type
{
    MJS_AST_EXPRESSION_PRIMARY_TYPE_UNKNOWN = 0,
    MJS_AST_EXPRESSION_PRIMARY_TYPE_THIS,
    MJS_AST_EXPRESSION_PRIMARY_TYPE_IDENTIFIER,
    MJS_AST_EXPRESSION_PRIMARY_TYPE_LITERAL,
    MJS_AST_EXPRESSION_PRIMARY_TYPE_ARRAY_LITERAL,
    MJS_AST_EXPRESSION_PRIMARY_TYPE_OBJECT_LITERAL,
    MJS_AST_EXPRESSION_PRIMARY_TYPE_SUB,
};
struct mjs_ast_expression_primary
{
    enum mjs_ast_expression_primary_type type;
    union
    {
        struct token *token_this;
        struct token *token_identifier;
        struct token *token_literal;
    } u;
};
struct mjs_ast_expression_primary *mjs_ast_expression_primary_new(enum mjs_ast_expression_primary_type type);
int mjs_ast_expression_primary_destroy(struct mjs_ast_expression_primary *expression_primary);

/* Expression : Assignment */
struct mjs_ast_expression_assignment
{
    struct mjs_ast_expression *expression_lhs;
    struct token *op;
    struct mjs_ast_expression *expression_rhs;
};
struct mjs_ast_expression_assignment *mjs_ast_expression_assignment_new(void);
int mjs_ast_expression_assignment_destroy(struct mjs_ast_expression_assignment *expression_assignment);

/* Expression : Conditional */
struct mjs_ast_expression_conditional
{
    struct mjs_ast_expression *expression_condition;
    struct mjs_ast_expression *expression_true;
    struct mjs_ast_expression *expression_false;
};
struct mjs_ast_expression_conditional *mjs_ast_expression_conditional_new(void);
int mjs_ast_expression_conditional_destroy(struct mjs_ast_expression_conditional *expression_conditional);

/* Expression : Binary */
struct mjs_ast_expression_binary
{
    struct mjs_ast_expression *expression_lhs;
    struct token *op;
    struct mjs_ast_expression *expression_rhs;
};
struct mjs_ast_expression_binary *mjs_ast_expression_binary_new(void);
int mjs_ast_expression_binary_destroy(struct mjs_ast_expression_binary *expression_binary);

/* Expression : Unary */
struct mjs_ast_expression_unary
{
    struct token *op;
    struct mjs_ast_expression *expression_sub;
};
struct mjs_ast_expression_unary *mjs_ast_expression_unary_new(void);
int mjs_ast_expression_unary_destroy(struct mjs_ast_expression_unary *expression_unary);

/* Expression : Postfix */
struct mjs_ast_expression_postfix
{
    struct token *op;
    struct mjs_ast_expression *expression_sub;
};
struct mjs_ast_expression_postfix *mjs_ast_expression_postfix_new(void);
int mjs_ast_expression_postfix_destroy(struct mjs_ast_expression_postfix *expression_postfix);


/* Expression : Member Expression : dot */
struct mjs_ast_member_expression_dot
{
    struct mjs_ast_member_expression *member_expression;
    struct token *identifier_name;
};
struct mjs_ast_member_expression_dot *mjs_ast_member_expression_dot_new(void);
int mjs_ast_member_expression_dot_destroy(struct mjs_ast_member_expression_dot *member_expression_dot);


/* Expression : Member Expression */
/*
 * MemberExpression :
 *   PrimaryExpression
 *   FunctionExpression
 *   MemberExpression [ Expression ]
 *   MemberExpression . IdentifierName
 *   'new' MemberExpression Arguments
 */
enum mjs_ast_member_expression_type
{
    MJS_AST_MEMBER_EXPRESSION_TYPE_UNKNOWN = 0,
    MJS_AST_MEMBER_EXPRESSION_TYPE_PRIMARY,
    MJS_AST_MEMBER_EXPRESSION_TYPE_FUNCTION,
    MJS_AST_MEMBER_EXPRESSION_TYPE_BRACKET,
    MJS_AST_MEMBER_EXPRESSION_TYPE_DOT,
    MJS_AST_MEMBER_EXPRESSION_TYPE_NEW,
};
struct mjs_ast_member_expression
{
    enum mjs_ast_member_expression_type type;
    union
    {
        struct mjs_ast_expression *expression_primary;
        struct mjs_ast_member_expression_dot *expression_dot;
    } u;
};
struct mjs_ast_member_expression *mjs_ast_member_expression_new(enum mjs_ast_member_expression_type type);
int mjs_ast_member_expression_destroy(struct mjs_ast_member_expression *member_expression);

/* Expression : Left Hand Side : New */
/*
 * NewExpression :
 *   MemberExpression
 *   'new' NewExpression
 */
enum mjs_ast_expression_lhs_new_expression_type
{
    MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_UNKNOWN = 0,
    MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_MEMBER,
    MJS_AST_EXPRESSION_LHS_NEW_EXPRESSION_TYPE_NEW,
};
struct mjs_ast_expression_lhs_new_expression
{
    enum mjs_ast_expression_lhs_new_expression_type type;
    union
    {
        struct mjs_ast_member_expression *member_expression;
        struct mjs_ast_expression_lhs_new_expression *sub_new_expression;
    } u;
};
struct mjs_ast_expression_lhs_new_expression *mjs_ast_expression_lhs_new_expression_new( \
        enum mjs_ast_expression_lhs_new_expression_type type);
int mjs_ast_expression_lhs_new_expression_destroy( \
        struct mjs_ast_expression_lhs_new_expression *expression_lhs_new_expression);


/* Argument */

struct mjs_ast_argument
{
    struct mjs_ast_expression *expr;

    struct mjs_ast_argument *prev;
    struct mjs_ast_argument *next;
};
struct mjs_ast_argument *mjs_ast_argument_new(void);
int mjs_ast_argument_destroy(struct mjs_ast_argument *argument);

struct mjs_ast_argument_list
{
    struct mjs_ast_argument *begin;
    struct mjs_ast_argument *end;
    size_t size;
};
struct mjs_ast_argument_list *mjs_ast_argument_list_new(void);
int mjs_ast_argument_list_destroy(struct mjs_ast_argument_list *argument_list);
int mjs_ast_argument_list_append(struct mjs_ast_argument_list *argument_list, \
        struct mjs_ast_argument *new_argument);


/* Expression : Left Hand Side : Call : Member Args */
struct mjs_ast_expression_lhs_call_expression_member_args
{
    struct mjs_ast_member_expression *member_expr;
    struct mjs_ast_argument_list *args;
};
struct mjs_ast_expression_lhs_call_expression_member_args *mjs_ast_expression_lhs_call_expression_member_args_new(void);
int mjs_ast_expression_lhs_call_expression_member_args_destroy(struct mjs_ast_expression_lhs_call_expression_member_args *expression_lhs_call_expression_member_args);

/* Expression : Left Hand Side : Call */
/*
 * CallExpression :
 *   MemberExpression Arguments
 *   CallExpression Arguments
 *   CallExpression [ Expression ]
 *   CallExpression . IdentifierName
 */
enum mjs_ast_expression_lhs_call_expression_type
{
    MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_UNKNOWN = 0,
    MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_MEMBER_ARGS,
    MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_CALL_ARGS,
    MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_BRACKET,
    MJS_AST_EXPRESSION_LHS_CALL_EXPRESSION_TYPE_DOT,
};
struct mjs_ast_expression_lhs_call_expression
{
    enum mjs_ast_expression_lhs_call_expression_type type;
    union
    {
        struct mjs_ast_expression_lhs_call_expression_member_args *member_args;
    } u;
};
struct mjs_ast_expression_lhs_call_expression *mjs_ast_expression_lhs_call_expression_new( \
        enum mjs_ast_expression_lhs_call_expression_type type);
int mjs_ast_expression_lhs_call_expression_destroy( \
        struct mjs_ast_expression_lhs_call_expression *expression_lhs_call_expression);

/* Expression : Left Hand Side */
enum mjs_ast_expression_lhs_type
{
    MJS_AST_EXPRESSION_LHS_TYPE_UNKNOWN = 0,
    MJS_AST_EXPRESSION_LHS_TYPE_NEW, 
    MJS_AST_EXPRESSION_LHS_TYPE_CALL, 
};
struct mjs_ast_expression_lhs
{
    enum mjs_ast_expression_lhs_type type;
    union
    {
        struct mjs_ast_expression_lhs_new_expression *expression_lhs_new_expression;
        struct mjs_ast_expression_lhs_call_expression *expression_lhs_call_expression;
    } u;
};
struct mjs_ast_expression_lhs *mjs_ast_expression_lhs_new(enum mjs_ast_expression_lhs_type type);
int mjs_ast_expression_lhs_destroy(struct mjs_ast_expression_lhs *expression_lhs);



/* Expression */
enum mjs_ast_expression_type
{
    MJS_AST_EXPRESSION_TYPE_UNKNOWN = 0,
    MJS_AST_EXPRESSION_TYPE_ASSIGNMENT,
    MJS_AST_EXPRESSION_TYPE_CONDITIONAL,
    MJS_AST_EXPRESSION_TYPE_BINARY,
    MJS_AST_EXPRESSION_TYPE_UNARY,
    MJS_AST_EXPRESSION_TYPE_POSTFIX,
    MJS_AST_EXPRESSION_TYPE_LHS,
    MJS_AST_EXPRESSION_TYPE_PRIMARY,
};
struct mjs_ast_expression
{
    enum mjs_ast_expression_type type;
    union
    {
        struct mjs_ast_expression_assignment *expression_assignment;
        struct mjs_ast_expression_conditional *expression_conditional;
        struct mjs_ast_expression_binary *expression_binary;
        struct mjs_ast_expression_unary *expression_unary;
        struct mjs_ast_expression_postfix *expression_postfix;
        struct mjs_ast_expression_lhs *expression_lhs;
        struct mjs_ast_expression_primary *expression_primary;
    } u;
};
struct mjs_ast_expression *mjs_ast_expression_new(enum mjs_ast_expression_type type);
int mjs_ast_expression_destroy(struct mjs_ast_expression *expression);

/* Statement : Block */
struct mjs_ast_statement_block
{
    int member;
};
struct mjs_ast_statement_block *mjs_ast_statement_block_new(void);
int mjs_ast_statement_block_destroy( \
        struct mjs_ast_statement_block *statement_block);

/* Statement : Expression */
struct mjs_ast_statement_expression
{
    struct mjs_ast_expression *expression;
};
struct mjs_ast_statement_expression *mjs_ast_statement_expression_new(void);
int mjs_ast_statement_expression_destroy( \
        struct mjs_ast_statement_expression *statement_expression);

/* Statement */
enum mjs_ast_statement_type
{
    MJS_AST_STATEMENT_TYPE_UNKNOWN = 0,
    MJS_AST_STATEMENT_TYPE_BLOCK,
    MJS_AST_STATEMENT_TYPE_VARIABLE,
    MJS_AST_STATEMENT_TYPE_EMPTY,
    MJS_AST_STATEMENT_TYPE_EXPRESSION,
    MJS_AST_STATEMENT_TYPE_IF,
    MJS_AST_STATEMENT_TYPE_ITERATION,
    MJS_AST_STATEMENT_TYPE_CONTINUE,
    MJS_AST_STATEMENT_TYPE_BREAK,
    MJS_AST_STATEMENT_TYPE_RETURN,
    MJS_AST_STATEMENT_TYPE_WITH,
    MJS_AST_STATEMENT_TYPE_LABELLED,
    MJS_AST_STATEMENT_TYPE_SWITCH,
    MJS_AST_STATEMENT_TYPE_THROW,
    MJS_AST_STATEMENT_TYPE_TRY,
    MJS_AST_STATEMENT_TYPE_DEBUGGER,
};
struct mjs_ast_statement
{
    enum mjs_ast_statement_type type;
    union
    {
        struct mjs_ast_statement_expression *statement_expression;
    } u;
};
struct mjs_ast_statement *mjs_ast_statement_new(enum mjs_ast_statement_type type);
int mjs_ast_statement_destroy(struct mjs_ast_statement *source_statement);

/* Source Element */
enum mjs_ast_source_element_type
{
    MJS_AST_SOURCE_ELEMENT_TYPE_UNKNOWN = 0,
    MJS_AST_SOURCE_ELEMENT_TYPE_STATEMENT,
    MJS_AST_SOURCE_ELEMENT_TYPE_FUNDECL,
};
struct mjs_ast_source_element
{
    enum mjs_ast_source_element_type type;
    union
    {
        struct mjs_ast_statement *statement;
    } u;
    struct mjs_ast_source_element *next;
};
struct mjs_ast_source_element *mjs_ast_source_element_new(enum mjs_ast_source_element_type type);
int mjs_ast_source_element_destroy(struct mjs_ast_source_element *list);

/* Source Element List */
struct mjs_ast_source_element_list
{
    struct mjs_ast_source_element *begin;
    struct mjs_ast_source_element *end;
};
struct mjs_ast_source_element_list *mjs_ast_source_element_list_new(void);
int mjs_ast_source_element_list_destroy(struct mjs_ast_source_element_list *list);
int mjs_ast_source_element_list_append(struct mjs_ast_source_element_list *list, \
        struct mjs_ast_source_element *new_source_element);

/* Program */
struct mjs_ast_program
{
    struct mjs_ast_source_element_list *source_elements;
};
struct mjs_ast_program *mjs_ast_program_new(void);
int mjs_ast_program_destroy(struct mjs_ast_program *program);


#endif

