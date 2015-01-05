/* Multiple JavaScript Programming Language : Lexical Scanner
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

/* An implementation of JavaScript Programming Language, 
 * also known as ECMAScript, specification can be found in
 * <<Standard ECMA-262 ECMAScript® Language Specification Edition 5.1 (June 2011) 
 * http://www.ecma-international.org/publications/standards/Ecma-262.htm
 */

#ifndef _MJS_LEXER_H_
#define _MJS_LEXER_H_

#include <stdio.h>

#include "multiply_lexer.h"

/* Token Types */
enum
{
    /* ReservedWord */
    TOKEN_KEYWORD_BREAK = CUSTOM_TOKEN_STARTPOINT,
    TOKEN_KEYWORD_CASE,
    TOKEN_KEYWORD_CATCH,
    TOKEN_KEYWORD_CONTINUE,
    TOKEN_KEYWORD_DEBUGGER,
    TOKEN_KEYWORD_DEFAULT,
    TOKEN_KEYWORD_DELETE,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_FINALLY,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_FUNCTION,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_IN,
    TOKEN_KEYWORD_INSTANCEOF,
    TOKEN_KEYWORD_NEW,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_SWITCH,
    TOKEN_KEYWORD_THIS,
    TOKEN_KEYWORD_THROW,
    TOKEN_KEYWORD_TRY,
    TOKEN_KEYWORD_TYPEOF,
    TOKEN_KEYWORD_VAR,
    TOKEN_KEYWORD_VOID,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_WITH,

    /* FutureReservedWord */
    TOKEN_KEYWORD_CLASS,
    TOKEN_KEYWORD_ENUM,
    TOKEN_KEYWORD_EXTENDS,
    TOKEN_KEYWORD_SUPER,
    TOKEN_KEYWORD_CONST,
    TOKEN_KEYWORD_EXPORT,
    TOKEN_KEYWORD_IMPORT,

    /* FutureReservedWord (Strict Mode Code) */
    TOKEN_KEYWORD_IMPLEMENTS,
    TOKEN_KEYWORD_LET,
    TOKEN_KEYWORD_PRIVATE,
    TOKEN_KEYWORD_PUBLIC,
    TOKEN_KEYWORD_INTERFACE,
    TOKEN_KEYWORD_PACKAGE,
    TOKEN_KEYWORD_PROTECTED,
    TOKEN_KEYWORD_STATIC,
    TOKEN_KEYWORD_YIELD,

    TOKEN_OP_LE, /* <= */
    TOKEN_OP_GE, /* >= */
    TOKEN_OP_EQ, /* == */
    TOKEN_OP_NE, /* != */
    TOKEN_OP_EQ3, /* === */
    TOKEN_OP_NE3, /* !== */
    TOKEN_OP_INC, /* ++ */
    TOKEN_OP_DEC, /* -- */
    TOKEN_OP_LL, /* << */
    TOKEN_OP_RR, /* >> */
    TOKEN_OP_RRR, /* >>> */
    TOKEN_OP_AND, /* && */
    TOKEN_OP_OR, /* || */
    TOKEN_OP_ADD_ASSIGN, /* += */
    TOKEN_OP_SUB_ASSIGN, /* -= */
    TOKEN_OP_MUL_ASSIGN, /* *= */
    TOKEN_OP_DIV_ASSIGN, /* /= */
    TOKEN_OP_MOD_ASSIGN, /* %= */
    TOKEN_OP_LSHIFT_ASSIGN, /* <<= */
    TOKEN_OP_RSHIFT_ASSIGN, /* >>= */
    TOKEN_OP_AND_ASSIGN, /* &= */
    TOKEN_OP_OR_ASSIGN, /* |= */
    TOKEN_OP_XOR_ASSIGN, /* ^= */
    TOKEN_OP_RRR_ASSIGN, /* >>>= */

    TOKEN_LITERAL_TRUE,
    TOKEN_LITERAL_FALSE,
    TOKEN_LITERAL_NULL,
};

#define IS_DIGIT(ch) \
    (('0'<=(ch))&&((ch)<='9'))

#define IS_IDENTIFIER_START(ch) \
    ((('A'<=(ch))&&((ch)<='Z'))|| \
     (('a'<=(ch))&&((ch)<='z'))|| \
     (ch=='$')|| \
     (ch=='_'))

#define IS_IDENTIFIER_PART(ch) \
    ((IS_IDENTIFIER_START(ch))||(IS_DIGIT(ch)))

#define IS_LITERAL(x) \
    (((x)==(TOKEN_CONSTANT_INTEGER_BINARY))||\
     ((x)==(TOKEN_CONSTANT_INTEGER_OCTAL))||\
     ((x)==(TOKEN_CONSTANT_INTEGER_DECIMAL))||\
     ((x)==(TOKEN_CONSTANT_INTEGER_HEXADECIMAL))||\
     ((x)==(TOKEN_CONSTANT_FLOAT_BINARY))||\
     ((x)==(TOKEN_CONSTANT_FLOAT_OCTAL))||\
     ((x)==(TOKEN_CONSTANT_FLOAT_DECIMAL))||\
     ((x)==(TOKEN_CONSTANT_FLOAT_HEXADECIMAL))||\
     ((x)==(TOKEN_CONSTANT_STRING)))

/* Get token name */
int mjs_token_name(char **token_name, size_t *token_name_len, const int value);

/* Lexical scan source code */
int mjs_tokenize(struct multiple_error *err, \
        struct token_list **list_out, \
        const char *data, const size_t data_len);

#endif

