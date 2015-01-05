/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Floating Code Block
   Copyright(C) 2013-2014 Cheryl Natsu

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

#ifndef _MJS_ICG_FCB_H_
#define _MJS_ICG_FCB_H_

#include <stdio.h>
#include <stdint.h>

/* Attributes for each line */

struct mjs_icg_fcb_line_attr
{
    uint32_t attr_id;
    uint32_t res_id;
    struct mjs_icg_fcb_line_attr *next;
};

struct mjs_icg_fcb_line_attr *mjs_icg_fcb_line_attr_new(uint32_t attr_id, \
        uint32_t res_id); 
int mjs_icg_fcb_line_attr_destroy(struct mjs_icg_fcb_line_attr *attr);

struct mjs_icg_fcb_line_attr_list
{
    struct mjs_icg_fcb_line_attr *begin;
    struct mjs_icg_fcb_line_attr *end;
    size_t size;
};

struct mjs_icg_fcb_line_attr_list *mjs_icg_fcb_line_attr_list_new(void);
int mjs_icg_fcb_line_attr_list_destroy(struct mjs_icg_fcb_line_attr_list *list);
int mjs_icg_fcb_line_attr_list_append(struct mjs_icg_fcb_line_attr_list *list, \
        struct mjs_icg_fcb_line_attr *new_attr);
int mjs_icg_fcb_line_attr_list_append_with_configure(struct mjs_icg_fcb_line_attr_list *list, \
        uint32_t attr_id, uint32_t res_id);


enum
{
    /* No needed to do anything to operand */
    MJS_ICG_FCB_LINE_TYPE_NORMAL = 0,        

    /* operand = global_start + operand */
    MJS_ICG_FCB_LINE_TYPE_PC = 1,            

    /* operand = global_offsets_of_lambda_procs[res_id] */
    MJS_ICG_FCB_LINE_TYPE_LAMBDA_MK = 2,     

    /* operand = global_offsets_of_built_in_proces[res_id] 
     * At the beginning of __init__ */
    MJS_ICG_FCB_LINE_TYPE_BLTIN_PROC_MK = 3, 
};

struct mjs_icg_fcb_line
{
    uint32_t opcode;
    uint32_t operand;
    int type;
    struct mjs_icg_fcb_line_attr_list *attrs;

    struct mjs_icg_fcb_line *prev;
    struct mjs_icg_fcb_line *next;
};
struct mjs_icg_fcb_line *mjs_icg_fcb_line_new(void);
int mjs_icg_fcb_line_destroy(struct mjs_icg_fcb_line *icg_fcb_line);
struct mjs_icg_fcb_line *mjs_icg_fcb_line_new_with_configure(uint32_t opcode, uint32_t operand);
struct mjs_icg_fcb_line *mjs_icg_fcb_line_new_with_configure_type(uint32_t opcode, uint32_t operand, int type);

struct mjs_icg_fcb_block
{
    struct mjs_icg_fcb_line *begin;
    struct mjs_icg_fcb_line *end;
    size_t size;

    struct mjs_icg_fcb_block *prev;
    struct mjs_icg_fcb_block *next;
};
struct mjs_icg_fcb_block *mjs_icg_fcb_block_new(void);
int mjs_icg_fcb_block_destroy(struct mjs_icg_fcb_block *icg_fcb_block);
int mjs_icg_fcb_block_append(struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_icg_fcb_line *new_icg_fcb_line);
int mjs_icg_fcb_block_insert(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number_insert, \
        struct mjs_icg_fcb_line *new_icg_fcb_line);

int mjs_icg_fcb_block_append_with_configure(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t opcode, uint32_t operand);
int mjs_icg_fcb_block_append_with_configure_type(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t opcode, uint32_t operand, int type);

uint32_t mjs_icg_fcb_block_get_instrument_number(struct mjs_icg_fcb_block *icg_fcb_block);

int mjs_icg_fcb_block_insert_with_configure(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument, uint32_t opcode, uint32_t operand);
int mjs_icg_fcb_block_insert_with_configure_type(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument, uint32_t opcode, uint32_t operand, int type);

int mjs_icg_fcb_block_link(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number_from, uint32_t instrument_number_to);
int mjs_icg_fcb_block_link_relative(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number_from, uint32_t instrument_number_to);

struct mjs_icg_fcb_block_list
{
    struct mjs_icg_fcb_block *begin;
    struct mjs_icg_fcb_block *end;
    size_t size;
};
struct mjs_icg_fcb_block_list *mjs_icg_fcb_block_list_new(void);
int mjs_icg_fcb_block_list_destroy(struct mjs_icg_fcb_block_list *icg_fcb_block_list);
int mjs_icg_fcb_block_list_append(struct mjs_icg_fcb_block_list *icg_fcb_block_list, \
        struct mjs_icg_fcb_block *new_icg_fcb_block);


struct mjs_icg_customizable_built_in_procedure_write_back
{
    uint32_t instrument_number;
    struct mjs_icg_customizable_built_in_procedure_write_back *next;
};
struct mjs_icg_customizable_built_in_procedure_write_back *mjs_icg_customizable_built_in_procedure_write_back_new(uint32_t instrument_number);
int mjs_icg_customizable_built_in_procedure_write_back_destroy(struct mjs_icg_customizable_built_in_procedure_write_back *write_back);

struct mjs_icg_customizable_built_in_procedure_write_back_list
{
    struct mjs_icg_customizable_built_in_procedure_write_back *begin;
    struct mjs_icg_customizable_built_in_procedure_write_back *end;
    size_t size;
};
struct mjs_icg_customizable_built_in_procedure_write_back_list *mjs_icg_customizable_built_in_procedure_write_back_list_new(void);
int mjs_icg_customizable_built_in_procedure_write_back_list_destroy(struct mjs_icg_customizable_built_in_procedure_write_back_list *write_back_list);
int mjs_icg_customizable_built_in_procedure_write_back_list_append_with_configure(struct mjs_icg_customizable_built_in_procedure_write_back_list *write_back_list, uint32_t instrument_number);

struct mjs_icg_customizable_built_in_procedure
{
    char *name;
    size_t name_len;
    int called;

    /* instrument number in icode */
    uint32_t instrument_number_icode;
    struct mjs_icg_customizable_built_in_procedure_write_back_list *write_backs;

    struct mjs_icg_customizable_built_in_procedure *next;
};
struct mjs_icg_customizable_built_in_procedure *mjs_icg_customizable_built_in_procedure_new(const char *name, const size_t name_len);
int mjs_icg_customizable_built_in_procedure_destroy(struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure);

struct mjs_icg_customizable_built_in_procedure_list
{
    struct mjs_icg_customizable_built_in_procedure *begin;
    struct mjs_icg_customizable_built_in_procedure *end;
};
struct mjs_icg_customizable_built_in_procedure_list *mjs_icg_customizable_built_in_procedure_list_new(void);
int mjs_icg_customizable_built_in_procedure_list_destroy(struct mjs_icg_customizable_built_in_procedure_list *list);

struct mjs_icg_customizable_built_in_procedure *mjs_icg_customizable_built_in_procedure_list_lookup(struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list, \
        const char *name, const size_t name_len);
int mjs_icg_customizable_built_in_procedure_list_called( \
        struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list, \
        const char *name, const size_t name_len);
int mjs_icg_customizable_built_in_procedure_list_add_writeback( \
        struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list, \
        const char *name, const size_t name_len, uint32_t instrument_number);

#endif


