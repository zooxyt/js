/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Auxiliary
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

#ifndef _MJS_ICG_AUX_H_
#define _MJS_ICG_AUX_H_

#include <stdio.h>
#include <stdint.h>

#include "multiple_ir.h"

#include "multiply.h"

#include "mjs_icg_context.h"
#include "mjs_icg_fcb.h"

int mjs_icg_fcb_block_append_from_precompiled_pic_text( \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct multiply_text_precompiled *text_precompiled);


/* A data structure for connecting goto's offset and label */

struct mjs_map_offset_label
{
    uint32_t offset;

    char *str;
    size_t len;

    uint32_t pos_ln;
    uint32_t pos_col;

    struct mjs_map_offset_label *next;
    struct mjs_map_offset_label *prev;
};
struct mjs_map_offset_label *mjs_map_offset_label_new( \
        uint32_t offset, \
        char *str, size_t len, \
        uint32_t pos_ln, uint32_t pos_col);
int mjs_map_offset_label_destroy(struct mjs_map_offset_label *mjs_map_offset_label);

struct mjs_map_offset_label_list
{
    struct mjs_map_offset_label *begin;
    struct mjs_map_offset_label *end;
};
struct mjs_map_offset_label_list *mjs_map_offset_label_list_new(void);
int mjs_map_offset_label_list_destroy(struct mjs_map_offset_label_list *list);
int mjs_map_offset_label_list_append(struct mjs_map_offset_label_list *list, \
        struct mjs_map_offset_label *new_mjs_map_offset_label);
int mjs_map_offset_label_list_append_with_configure( \
        struct mjs_map_offset_label_list *list, \
        uint32_t offset, \
        char *str, size_t len, \
        uint32_t pos_ln, uint32_t pos_col);

int mjs_icodegen_statement_list_apply_goto(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list);

#endif

