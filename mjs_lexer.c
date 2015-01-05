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

#include "selfcheck.h"
#include <stdlib.h>
#include <string.h>

#include "multiple_err.h"
#include "multiply_lexer.h"
#include "mjs_lexer.h"

/* Status definitions for lexical analysis */
enum {
    LEX_STATUS_INIT = 0,

    LEX_STATUS_COMMENT, /* //.*?<EOL> */
    LEX_STATUS_COMMENT_ESCAPE, 
    LEX_STATUS_COMMENT_ML, /* <slash>\*.*?\*<slash> */
    LEX_STATUS_COMMENT_ML_ESCAPE, 
    LEX_STATUS_EOL, /* EOL of Windows? Mac? */
    LEX_STATUS_IDENTIFIER_P_1, /* Identifier ? */

    LEX_STATUS_INTEGER_BOH, /* 0[b|[0-9]|h] */
    LEX_STATUS_INTEGER_BOH_B, /* 0b */
    LEX_STATUS_INTEGER_BOH_B1, /* 0b[01] */
    LEX_STATUS_INTEGER_BOH_O, /* 0[0-9] */
    LEX_STATUS_INTEGER_BOH_H, /* 0x */
    LEX_STATUS_INTEGER_BOH_H1, /* 0x[01] */
    LEX_STATUS_INTEGER_D, /* 0x */

    LEX_STATUS_FLOAT_DOT_B, /* 0b???. */
    LEX_STATUS_FLOAT_DOT_O, /* 0???. */
    LEX_STATUS_FLOAT_DOT_D, /* ???. */
    LEX_STATUS_FLOAT_DOT_H, /* 0x???. */

    LEX_STATUS_STRING, /* "|' */
    LEX_STATUS_STRING_ESCAPE, /* \ */

    LEX_STATUS_BACK_FINISH, /* Finished, and break */
    LEX_STATUS_FINISH, /* Finished */
    LEX_STATUS_ERROR, /* Error */
};

#define JMP(status,dst) do{(status)=(dst);}while(0);
#define FIN(x) do{(x)=LEX_STATUS_FINISH;}while(0);
#define BFIN(x) do{(x)=LEX_STATUS_BACK_FINISH;}while(0);
#define UND(x) do{(x)=LEX_STATUS_ERROR;}while(0);
#define KEEP() do{}while(0);

#define APPLY_KEYWORD_BEGIN()\
    do{
#define APPLY_KEYWORD(token,keyword,keyword_value) \
    if ((token->len==strlen(keyword))&&(strncmp(token->str,keyword,token->len)==0)){\
        token->value=keyword_value;break;}
#define APPLY_KEYWORD_END()\
    }while(0)

#define ENOUGH_SPACE(p, endp, delta) (((endp)-(p)>=(delta))?(1):(0))

/* Get one token from the char stream */
static int eat_token(struct multiple_error *err, \
        struct token *new_token, \
        const char *p, const char *endp, \
        uint32_t *pos_col, uint32_t *pos_ln, \
        const int eol_type, size_t *move_on)
{
    const char *p_init = p;
    int status = LEX_STATUS_INIT;
    int ch = 0, ch2 = 0, ch3 = 0;
    size_t bytes_number;
    size_t prefix_strip = 0, postfix_strip = 0;
    int ch_string_close = '\"';

    int is_eol = 0; /* For updating EOL and Ln */

    /* Clean template */
    new_token->value = TOKEN_UNDEFINED;
    new_token->str = (char *)p_init;
    new_token->len = 0;
    new_token->pos_col = *pos_col;
    new_token->pos_ln = *pos_ln;

    while (p != endp)
    {
        ch = *p;
        if (endp - p >= 2)
        {
            /* 2-bytes operator */
            ch2 = *(p + 1);
            if (endp - p >= 3)
            {
                /* 3-bytes operator */
                ch3 = *(p + 2);
            }
        }
        switch (status)
        {
            case LEX_STATUS_EOL:
                if (ch == CHAR_LF) { FIN(status); } else { BFIN(status); }
                break;
            case LEX_STATUS_COMMENT:
                if (IS_EOL(ch)) 
                {
                    /* Reset location */
                    *pos_col = 1;
                    *pos_ln += 1;
                    is_eol = 1;
                    /* "" (Null String) */
                    new_token->value = TOKEN_WHITESPACE;
                    FIN(status);
                }
                else if (ch == '\\')
                {
                    JMP(status, LEX_STATUS_COMMENT_ESCAPE);
                }
                else if (IS_ID_HYPER(ch))
                {
                    bytes_number = id_hyper_length((char)ch); 
                    if ((bytes_number == 0) || ((size_t)(endp - p) < bytes_number)) 
                    { 
                        MULTIPLE_ERROR_INTERNAL(); 
                        return -MULTIPLE_ERR_LEXICAL; 
                    } 
                    bytes_number--; 
                    while (bytes_number-- != 0) { p += 1; } 
                }
                else
                {
                    KEEP();
                }
                break;
            case LEX_STATUS_COMMENT_ESCAPE:
                KEEP();
                JMP(status, LEX_STATUS_COMMENT);
                break;
            case LEX_STATUS_COMMENT_ML:
                if ((ENOUGH_SPACE(p,endp,2))&&(ch == '*')&&(ch2=='/'))
                {
                    p += 1;
                    JMP(status, LEX_STATUS_INIT);
                }
                else if (ch == '\\')
                {
                    JMP(status, LEX_STATUS_COMMENT_ML_ESCAPE);
                }
                else if (IS_ID_HYPER(ch))
                {
                    bytes_number = id_hyper_length((char)ch); 
                    if ((bytes_number == 0) || ((size_t)(endp - p) < bytes_number)) 
                    { 
                        MULTIPLE_ERROR_INTERNAL(); 
                        return -MULTIPLE_ERR_LEXICAL; 
                    } 
                    bytes_number--; 
                    while (bytes_number-- != 0) { p += 1; } 
                }
                else
                {
                    KEEP();
                }
                break;
            case LEX_STATUS_COMMENT_ML_ESCAPE:
                KEEP();
                JMP(status, LEX_STATUS_COMMENT_ML);
                break;
            case LEX_STATUS_INIT:
                if (IS_EOL(ch)) 
                {
                    /* Reset location */
                    *pos_col = 1;
                    *pos_ln += 1;
                    is_eol = 1;

                    new_token->value = TOKEN_WHITESPACE; 
                    switch (eol_type)
                    {
                        case EOL_UNIX:
                        case EOL_MAC:
                            FIN(status);
                            break;
                        case EOL_DOS:
                            JMP(status, LEX_STATUS_EOL);
                            break;
                    }
                }

                /* Comments */
                if (((*pos_col == 1) && (*pos_ln == 1) && \
                        ((ENOUGH_SPACE(p,endp,2))&&(ch == '#')&&(ch2=='!'))) || \
                        ((ENOUGH_SPACE(p,endp,2))&&(ch == '/')&&(ch2=='/')))
                {
                    p += 1;
                    JMP(status, LEX_STATUS_COMMENT);
                }
                else if ((ENOUGH_SPACE(p,endp,2))&&(ch == '/')&&(ch2=='*'))
                {
                    p += 1;
                    JMP(status, LEX_STATUS_COMMENT_ML);
                }
                else if (IS_WHITESPACE(ch)) 
                {
                    new_token->value = TOKEN_WHITESPACE; FIN(status);
                }
                else if ((ENOUGH_SPACE(p,endp,4)) && (ch == '>') && (ch2 == '>') && (ch2 == '>') && (*(p + 3) == '='))
                {new_token->value = TOKEN_OP_RRR_ASSIGN; FIN(status);p+=3;} /* >>>= */
                else if ((ENOUGH_SPACE(p,endp,3)) && (ch == '=') && (ch2 == '=') && (ch3 == '='))
                {new_token->value = TOKEN_OP_EQ3; FIN(status);p+=2;} /* === */
                else if ((ENOUGH_SPACE(p,endp,3)) && (ch == '!') && (ch2 == '=') && (ch3 == '='))
                {new_token->value = TOKEN_OP_NE3; FIN(status);p+=2;} /* !== */
                else if ((ENOUGH_SPACE(p,endp,3)) && (ch == '>') && (ch2 == '>') && (ch3 == '>'))
                {new_token->value = TOKEN_OP_RRR; FIN(status);p+=2;} /* >>> */
                else if ((ENOUGH_SPACE(p,endp,3)) && (ch == '<') && (ch2 == '<') && (ch3 == '='))
                {new_token->value = TOKEN_OP_LSHIFT_ASSIGN; FIN(status);p+=2;} /* <<= */
                else if ((ENOUGH_SPACE(p,endp,3)) && (ch == '>') && (ch2 == '>') && (ch3 == '='))
                {new_token->value = TOKEN_OP_RSHIFT_ASSIGN; FIN(status);p+=2;} /* >>= */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '<') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_LE; FIN(status);p++;} /* <= */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '>') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_GE; FIN(status);p++;} /* >= */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '=') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_EQ; FIN(status);p++;} /* == */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '!') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_NE; FIN(status);p++;} /* != */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '+') && (ch2 == '+')) 
                {new_token->value = TOKEN_OP_INC; FIN(status);p++;} /* ++ */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '-') && (ch2 == '-')) 
                {new_token->value = TOKEN_OP_DEC; FIN(status);p++;} /* -- */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '<') && (ch2 == '<')) 
                {new_token->value = TOKEN_OP_LL; FIN(status);p++;} /* << */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '>') && (ch2 == '>')) 
                {new_token->value = TOKEN_OP_RR; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '&') && (ch2 == '&')) 
                {new_token->value = TOKEN_OP_AND; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '|') && (ch2 == '|')) 
                {new_token->value = TOKEN_OP_OR; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '+') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_ADD_ASSIGN; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '-') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_SUB_ASSIGN; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '*') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_MUL_ASSIGN; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '/') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_DIV_ASSIGN; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '%') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_MOD_ASSIGN; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '&') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_AND_ASSIGN; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '|') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_OR_ASSIGN; FIN(status);p++;} /* >> */
                else if ((ENOUGH_SPACE(p,endp,2)) && (ch == '^') && (ch2 == '=')) 
                {new_token->value = TOKEN_OP_XOR_ASSIGN; FIN(status);p++;} /* >> */
                else if (ch == '{') {new_token->value = '{'; FIN(status);}
                else if (ch == '}') {new_token->value = '}'; FIN(status);}
                else if (ch == '(') {new_token->value = '('; FIN(status);}
                else if (ch == ')') {new_token->value = ')'; FIN(status);}
                else if (ch == '[') {new_token->value = '['; FIN(status);}
                else if (ch == ']') {new_token->value = ']'; FIN(status);}
                else if (ch == '.') {new_token->value = '.'; FIN(status);}
                else if (ch == ';') {new_token->value = ';'; FIN(status);}
                else if (ch == ',') {new_token->value = ','; FIN(status);}
                else if (ch == '<') {new_token->value = '<'; FIN(status);}
                else if (ch == '>') {new_token->value = '>'; FIN(status);}
                else if (ch == '+') {new_token->value = '+'; FIN(status);}
                else if (ch == '-') {new_token->value = '-'; FIN(status);}
                else if (ch == '*') {new_token->value = '*'; FIN(status);}
                else if (ch == '/') {new_token->value = '/'; FIN(status);}
                else if (ch == '%') {new_token->value = '%'; FIN(status);}
                else if (ch == '&') {new_token->value = '&'; FIN(status);}
                else if (ch == '|') {new_token->value = '|'; FIN(status);}
                else if (ch == '^') {new_token->value = '^'; FIN(status);}
                else if (ch == '!') {new_token->value = '!'; FIN(status);}
                else if (ch == '~') {new_token->value = '~'; FIN(status);}
                else if (ch == '?') {new_token->value = '?'; FIN(status);}
                else if (ch == ':') {new_token->value = ':'; FIN(status);}
                else if (ch == '=') {new_token->value = '='; FIN(status);}
                else if (IS_IDENTIFIER_START(ch))
                {
                    /* Identifier ? */
                    new_token->value = TOKEN_IDENTIFIER;
                    JMP(status, LEX_STATUS_IDENTIFIER_P_1);
                }
                else if (IS_ID_HYPER(ch)) 
                {
                    bytes_number = id_hyper_length((char)ch);
                    if ((bytes_number == 0) || ((size_t)(endp - p) < bytes_number))
                    {
                        MULTIPLE_ERROR_INTERNAL();
                        return -MULTIPLE_ERR_LEXICAL;
                    }
                    bytes_number--;
                    while (bytes_number-- != 0) { p += 1; }
                    /* Identifier ? */
                    new_token->value = TOKEN_IDENTIFIER;
                    JMP(status, LEX_STATUS_IDENTIFIER_P_1);
                }
                else if (ch == '0')
                {
                    /* 0x???? -> Hex */
                    /* 0b???? -> Bin */
                    /* 0???? -> Oct */
                    new_token->value = TOKEN_CONSTANT_INTEGER_DECIMAL;
                    JMP(status, LEX_STATUS_INTEGER_BOH);
                }
                else if (('1' <= ch) && (ch <= '9'))
                {
                    new_token->value = TOKEN_CONSTANT_INTEGER_DECIMAL;
                    JMP(status, LEX_STATUS_INTEGER_D);
                }
                else if ((ch == '\"') || (ch == '\''))
                {
                    ch_string_close = ch;
                    JMP(status, LEX_STATUS_STRING);
                }
                else {new_token->value = TOKEN_UNDEFINED; UND(status);} /* Undefined! */
                break;
            case LEX_STATUS_IDENTIFIER_P_1:
                if (IS_IDENTIFIER_PART(ch)) {KEEP();}
                else if (IS_ID_HYPER(ch)) 
                {
                    bytes_number = id_hyper_length((char)ch);
                    if ((bytes_number == 0) || ((size_t)(endp - p) < bytes_number))
                    {
                        MULTIPLE_ERROR_INTERNAL();
                        return -MULTIPLE_ERR_LEXICAL;
                    }
                    bytes_number--;
                    while (bytes_number-- != 0) { p += 1; }
                }
                else {new_token->value = TOKEN_IDENTIFIER; BFIN(status);} /* Identifier! */
                break;
            case LEX_STATUS_INTEGER_BOH: 
                /* 0<- */
                if ((ch == 'b')||(ch == 'B')) {JMP(status, LEX_STATUS_INTEGER_BOH_B);}
                else if (IS_INTEGER_DECIMAL(ch)) 
                {
                    new_token->value = TOKEN_CONSTANT_INTEGER_OCTAL;
                    JMP(status, LEX_STATUS_INTEGER_BOH_O);
                }
                else if ((ch == 'x')||(ch == 'X')) {JMP(status, LEX_STATUS_INTEGER_BOH_H);}
                else if (ch == '.') 
                {
                    new_token->value = TOKEN_CONSTANT_FLOAT_DECIMAL;
                    JMP(status, LEX_STATUS_FLOAT_DOT_D);
                }
                else {BFIN(status);} /* Decimal 0 */
                break;
            case LEX_STATUS_INTEGER_BOH_B:
                if (IS_INTEGER_BINARY(ch)) 
                {
                    new_token->value = TOKEN_CONSTANT_INTEGER_BINARY;
                    JMP(status, LEX_STATUS_INTEGER_BOH_B1);
                }
                else 
                {
                    /* 0b2 */
                    p -= 1;
                    BFIN(status);
                }
                break;
            case LEX_STATUS_INTEGER_BOH_B1:
                if (IS_INTEGER_BINARY(ch)) {KEEP();}
                else if (ch == '.')
                {
                    new_token->value = TOKEN_CONSTANT_FLOAT_BINARY;
                    JMP(status, LEX_STATUS_FLOAT_DOT_B);
                }
                else {BFIN(status);} /* Binary Integer! */
                break;
            case LEX_STATUS_INTEGER_BOH_O:
                if (IS_INTEGER_OCTAL(ch)) {KEEP();}
                else if (ch == '.')
                {
                    new_token->value = TOKEN_CONSTANT_FLOAT_OCTAL;
                    JMP(status, LEX_STATUS_FLOAT_DOT_O);
                }
                else {BFIN(status);} /* Octal Integer! */
                break;
            case LEX_STATUS_INTEGER_BOH_H:
                if (IS_INTEGER_HEXADECIMAL(ch)) 
                {
                    new_token->value = TOKEN_CONSTANT_INTEGER_HEXADECIMAL;
                    JMP(status, LEX_STATUS_INTEGER_BOH_H1);
                }
                else 
                {
                    /* 0xq */
                    p -= 1;
                    BFIN(status);
                }
                break;
            case LEX_STATUS_INTEGER_BOH_H1:
                if (IS_INTEGER_HEXADECIMAL(ch)){KEEP();}
                else if (ch == '.')
                {
                    new_token->value = TOKEN_CONSTANT_FLOAT_HEXADECIMAL;
                    JMP(status, LEX_STATUS_FLOAT_DOT_H);
                }
                else {BFIN(status);} /* Hexadecimal Integer! */
                break;
            case LEX_STATUS_INTEGER_D:
                if (IS_INTEGER_DECIMAL(ch)){KEEP();}
                else if (ch == '.')
                {
                    new_token->value = TOKEN_CONSTANT_FLOAT_DECIMAL;
                    JMP(status, LEX_STATUS_FLOAT_DOT_D);
                }
                else {BFIN(status);} /* Decimal Integer! */
                break;
            case LEX_STATUS_FLOAT_DOT_B:
                if (IS_INTEGER_BINARY(ch)){KEEP();}
                else {BFIN(status);} /* Binary Float! */
                break;
            case LEX_STATUS_FLOAT_DOT_O:
                if (IS_INTEGER_OCTAL(ch)){KEEP();}
                else {BFIN(status);} /* Octal Float! */
                break;
            case LEX_STATUS_FLOAT_DOT_D:
                if (IS_INTEGER_DECIMAL(ch)){KEEP();}
                else {BFIN(status);} /* Decimal Float! */
                break;
            case LEX_STATUS_FLOAT_DOT_H:
                if (IS_INTEGER_HEXADECIMAL(ch)){KEEP();}
                else {BFIN(status);} /* Hexadecimal Float! */
                break;
            case LEX_STATUS_STRING:
                if (ch == ch_string_close)
                {
                    /* "" (Null String) */
                    prefix_strip = 1;
                    postfix_strip = 1;
                    new_token->value = TOKEN_CONSTANT_STRING;
                    FIN(status);
                }
                else if (ch == '\\')
                {
                    /* "" (Null String) */
                    prefix_strip = 1;
                    postfix_strip = 1;
                    new_token->value = TOKEN_CONSTANT_STRING;
                    JMP(status, LEX_STATUS_STRING_ESCAPE);
                }
                else
                {
                    KEEP();
                }
                break;
            case LEX_STATUS_STRING_ESCAPE:
                KEEP();
                JMP(status, LEX_STATUS_STRING);
                break;
            case LEX_STATUS_ERROR:
                new_token->str = NULL;
                new_token->len = 0;
                multiple_error_update(err, -MULTIPLE_ERR_LEXICAL, \
                        "%d:%d: undefined token", \
                        *pos_ln, *pos_col);
                return -MULTIPLE_ERR_LEXICAL;
                break;
            case LEX_STATUS_BACK_FINISH:
                p--;
            case LEX_STATUS_FINISH:
                goto done;
                break;
            default:
                new_token->str = NULL;
                new_token->len = 0;
                multiple_error_update(err, -MULTIPLE_ERR_LEXICAL, \
                        "%d:%d: undefined lexical analysis state, "
                        "something impossible happened", \
                        *pos_ln, *pos_col);
                return -MULTIPLE_ERR_LEXICAL;
                break;
        }
        if (status == LEX_STATUS_BACK_FINISH) break;
        p += 1;
    }
    if (status == LEX_STATUS_INTEGER_BOH_B || status == LEX_STATUS_INTEGER_BOH_H)
    {
        /* 0b$ and 0x$ */
        p -= 1;
    }
done:
    if (!is_eol)
    {
        *pos_col += (uint32_t)(p - p_init);
    }
    if (new_token->value == TOKEN_UNDEFINED)
    {
        new_token->len = 0;
        *move_on = new_token->len;
    }
    else
    {
        new_token->len = (size_t)(p - p_init);
        *move_on = new_token->len;
        new_token->str += prefix_strip;
        new_token->len -= (size_t)(prefix_strip + postfix_strip);
    }
    return 0;
}

static int token_patch(struct multiple_error *err, struct token_list *list)
{
    struct token *token_cur;

	if (list == NULL) 
    {
        MULTIPLE_ERROR_NULL_PTR();
        return -MULTIPLE_ERR_NULL_PTR;
    }

	token_cur = list->begin;
    while (token_cur != NULL)
    {
        if (token_cur->value == TOKEN_IDENTIFIER)
        {
            APPLY_KEYWORD_BEGIN();
            /* ReservedWord */
            APPLY_KEYWORD(token_cur, "break", TOKEN_KEYWORD_BREAK);
            APPLY_KEYWORD(token_cur, "CASE", TOKEN_KEYWORD_CASE);
            APPLY_KEYWORD(token_cur, "CATCH", TOKEN_KEYWORD_CATCH);
            APPLY_KEYWORD(token_cur, "CONTINUE", TOKEN_KEYWORD_CONTINUE);
            APPLY_KEYWORD(token_cur, "DEBUGGER", TOKEN_KEYWORD_DEBUGGER);
            APPLY_KEYWORD(token_cur, "DEFAULT", TOKEN_KEYWORD_DEFAULT);
            APPLY_KEYWORD(token_cur, "DELETE", TOKEN_KEYWORD_DELETE);
            APPLY_KEYWORD(token_cur, "DO", TOKEN_KEYWORD_DO);
            APPLY_KEYWORD(token_cur, "ELSE", TOKEN_KEYWORD_ELSE);
            APPLY_KEYWORD(token_cur, "FINALLY", TOKEN_KEYWORD_FINALLY);
            APPLY_KEYWORD(token_cur, "FOR", TOKEN_KEYWORD_FOR);
            APPLY_KEYWORD(token_cur, "FUNCTION", TOKEN_KEYWORD_FUNCTION);
            APPLY_KEYWORD(token_cur, "IF", TOKEN_KEYWORD_IF);
            APPLY_KEYWORD(token_cur, "IN", TOKEN_KEYWORD_IN);
            APPLY_KEYWORD(token_cur, "INSTANCEOF", TOKEN_KEYWORD_INSTANCEOF);
            APPLY_KEYWORD(token_cur, "NEW", TOKEN_KEYWORD_NEW);
            APPLY_KEYWORD(token_cur, "RETURN", TOKEN_KEYWORD_RETURN);
            APPLY_KEYWORD(token_cur, "SWITCH", TOKEN_KEYWORD_SWITCH);
            APPLY_KEYWORD(token_cur, "THIS", TOKEN_KEYWORD_THIS);
            APPLY_KEYWORD(token_cur, "THROW", TOKEN_KEYWORD_THROW);
            APPLY_KEYWORD(token_cur, "TRY", TOKEN_KEYWORD_TRY);
            APPLY_KEYWORD(token_cur, "TYPEOF", TOKEN_KEYWORD_TYPEOF);
            APPLY_KEYWORD(token_cur, "VAR", TOKEN_KEYWORD_VAR);
            APPLY_KEYWORD(token_cur, "VOID", TOKEN_KEYWORD_VOID);
            APPLY_KEYWORD(token_cur, "WHILE", TOKEN_KEYWORD_WHILE);
            APPLY_KEYWORD(token_cur, "WITH", TOKEN_KEYWORD_WITH);

            /* FutureReservedWord */
            APPLY_KEYWORD(token_cur, "CLASS", TOKEN_KEYWORD_CLASS);
            APPLY_KEYWORD(token_cur, "ENUM", TOKEN_KEYWORD_ENUM);
            APPLY_KEYWORD(token_cur, "EXTENDS", TOKEN_KEYWORD_EXTENDS);
            APPLY_KEYWORD(token_cur, "SUPER", TOKEN_KEYWORD_SUPER);
            APPLY_KEYWORD(token_cur, "CONST", TOKEN_KEYWORD_CONST);
            APPLY_KEYWORD(token_cur, "EXPORT", TOKEN_KEYWORD_EXPORT);
            APPLY_KEYWORD(token_cur, "IMPORT", TOKEN_KEYWORD_IMPORT);
            APPLY_KEYWORD(token_cur, "IMPLEMENTS", TOKEN_KEYWORD_IMPLEMENTS);
            APPLY_KEYWORD(token_cur, "LET", TOKEN_KEYWORD_LET);
            APPLY_KEYWORD(token_cur, "PRIVATE", TOKEN_KEYWORD_PRIVATE);
            APPLY_KEYWORD(token_cur, "PUBLIC", TOKEN_KEYWORD_PUBLIC);
            APPLY_KEYWORD(token_cur, "INTERFACE", TOKEN_KEYWORD_INTERFACE);
            APPLY_KEYWORD(token_cur, "PACKAGE", TOKEN_KEYWORD_PACKAGE);
            APPLY_KEYWORD(token_cur, "PROTECTED", TOKEN_KEYWORD_PROTECTED);
            APPLY_KEYWORD(token_cur, "STATIC", TOKEN_KEYWORD_STATIC);
            APPLY_KEYWORD(token_cur, "YIELD", TOKEN_KEYWORD_YIELD);

            /* FutureReservedWord (Strict Mode Code) */
            APPLY_KEYWORD(token_cur, "IMPLEMENTS", TOKEN_KEYWORD_IMPLEMENTS);
            APPLY_KEYWORD(token_cur, "LET", TOKEN_KEYWORD_LET);
            APPLY_KEYWORD(token_cur, "PRIVATE", TOKEN_KEYWORD_PRIVATE);
            APPLY_KEYWORD(token_cur, "PUBLIC", TOKEN_KEYWORD_PUBLIC);
            APPLY_KEYWORD(token_cur, "INTERFACE", TOKEN_KEYWORD_INTERFACE);
            APPLY_KEYWORD(token_cur, "PACKAGE", TOKEN_KEYWORD_PACKAGE);
            APPLY_KEYWORD(token_cur, "PROTECTED", TOKEN_KEYWORD_PROTECTED);
            APPLY_KEYWORD(token_cur, "STATIC", TOKEN_KEYWORD_STATIC);
            APPLY_KEYWORD(token_cur, "YIELD", TOKEN_KEYWORD_YIELD);

            /* Literal */
            APPLY_KEYWORD(token_cur, "true", TOKEN_LITERAL_TRUE);
            APPLY_KEYWORD(token_cur, "false", TOKEN_LITERAL_FALSE);
            APPLY_KEYWORD(token_cur, "null", TOKEN_LITERAL_NULL);

            APPLY_KEYWORD_END();
        }
        token_cur = token_cur->next;
    }
    return 0;
}

int mjs_tokenize(struct multiple_error *err, struct token_list **list_out, const char *data, const size_t data_len)
{
    int ret = 0;
    uint32_t pos_col = 1, pos_ln = 1;
    struct token_list *new_list = NULL;
    struct token *token_template = NULL;
    const char *data_p = data, *data_endp = data_p + data_len;

    int eol_type = eol_detect(err, data, data_len);
    size_t move_on;

    if (eol_type < 0)
    {
        goto fail;
    }

    *list_out = NULL;

    if ((new_list = token_list_new()) == NULL)
    {
        MULTIPLE_ERROR_MALLOC();
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }

    if ((token_template = token_new()) == NULL)
    {
        MULTIPLE_ERROR_MALLOC();
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }

    while (data_p != data_endp)
    {
        if ((ret = eat_token(err, token_template, data_p, data_endp, &pos_col, &pos_ln, eol_type, &move_on)) != 0)
        {
            goto fail;
        }
        if (token_template->value != TOKEN_WHITESPACE)
        {
            if ((ret = token_list_append_token_with_template(new_list, token_template)) != 0)
            {
                goto fail;
            }
        }
        /* Move on */
        data_p += move_on;
    }
    ret = token_list_append_token_with_configure(new_list, TOKEN_FINISH, NULL, 0, pos_col, pos_ln);
    if (ret != 0) goto fail;

    if ((ret = token_patch(err, new_list)) != 0)
    {
        goto fail;
    }

    *list_out = new_list;
    ret = 0;
fail:
    if (token_template != NULL)
    {
        token_template->str = NULL;
        free(token_template);
    }
    if (ret != 0)
    {
        if (new_list != NULL) token_list_destroy(new_list);
    }
    return ret;
}

struct token_value_name_tbl_item
{
    const int value;
    const char *name;
};

static struct token_value_name_tbl_item token_value_name_tbl_items[] = 
{
};
#define TOKEN_VALUE_NAME_TBL_ITEMS_COUNT (sizeof(token_value_name_tbl_items)/sizeof(struct token_value_name_tbl_item))

/* Get token name */
int mjs_token_name(char **token_name, size_t *token_name_len, const int value)
{
    size_t i;

    if (generic_token_name(token_name, token_name_len, value) == 0)
    { return 0; }

    for (i = 0; i != TOKEN_VALUE_NAME_TBL_ITEMS_COUNT; i++)
    {
        if (value == token_value_name_tbl_items[i].value)
        {
            *token_name = (char *)token_value_name_tbl_items[i].name;
            *token_name_len = strlen(token_value_name_tbl_items[i].name);
            return 0;
        }
    }
    return -1;
}

