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


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "multiple_ir.h"

#include "multiply.h"
#include "multiply_assembler.h"

#include "mjs_icg.h"
#include "mjs_icg_fcb.h"
#include "mjs_icg_aux.h"

int mjs_icg_fcb_block_append_from_precompiled_pic_text( \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct multiply_text_precompiled *text_precompiled)
{
    int ret = 0;
    struct multiply_text_precompiled_line *text_precompiled_line_cur = NULL;
    size_t idx;

    for (idx = 0; idx != text_precompiled->size; idx++)
    {
        text_precompiled_line_cur = &text_precompiled->lines[idx];
        if ((ret = mjs_icg_fcb_block_append_with_configure(icg_fcb_block, \
                        text_precompiled_line_cur->opcode, \
                        text_precompiled_line_cur->operand)) != 0)
        { goto fail; }
    }

fail:
    return ret;
}


struct mjs_map_offset_label *mjs_map_offset_label_new( \
        uint32_t offset, \
        char *str, size_t len, \
        uint32_t pos_ln, uint32_t pos_col)
{
    struct mjs_map_offset_label *new_mjs_map_offset_label = NULL;

    new_mjs_map_offset_label = (struct mjs_map_offset_label *)malloc(sizeof(struct mjs_map_offset_label));
    if (new_mjs_map_offset_label == NULL) { goto fail; }
    new_mjs_map_offset_label->prev = new_mjs_map_offset_label->next = NULL;
    new_mjs_map_offset_label->offset = offset;
    new_mjs_map_offset_label->len = len;
    new_mjs_map_offset_label->str = (char *)malloc(sizeof(char) * (len + 1));
    if (new_mjs_map_offset_label->str == NULL) { goto fail; }
    memcpy(new_mjs_map_offset_label->str, str, len);
    new_mjs_map_offset_label->str[len] = '\0';
    new_mjs_map_offset_label->pos_ln = pos_ln;
    new_mjs_map_offset_label->pos_col = pos_col;

    goto done;
fail:
    if (new_mjs_map_offset_label != NULL)
    {
        mjs_map_offset_label_destroy(new_mjs_map_offset_label);
        new_mjs_map_offset_label = NULL;
    }
done:
    return new_mjs_map_offset_label;
}

int mjs_map_offset_label_destroy(struct mjs_map_offset_label *mjs_map_offset_label)
{
    if (mjs_map_offset_label->str != NULL) free(mjs_map_offset_label->str);
    free(mjs_map_offset_label);

    return 0;
}

struct mjs_map_offset_label_list *mjs_map_offset_label_list_new(void)
{
    struct mjs_map_offset_label_list *new_list = NULL;

    new_list = (struct mjs_map_offset_label_list *)malloc(sizeof(struct mjs_map_offset_label_list));
    if (new_list == NULL) { goto fail; }
    new_list->begin = new_list->end = NULL;

    goto done;
fail:
    if (new_list != NULL)
    {
         mjs_map_offset_label_list_destroy(new_list);
         new_list = NULL;
    }
done:
    return new_list;
}

int mjs_map_offset_label_list_destroy(struct mjs_map_offset_label_list *list)
{
    struct mjs_map_offset_label *mjs_map_offset_label_cur, *mjs_map_offset_label_next;

    mjs_map_offset_label_cur = list->begin;
    while (mjs_map_offset_label_cur != NULL)
    {
        mjs_map_offset_label_next = mjs_map_offset_label_cur->next; 
        mjs_map_offset_label_destroy(mjs_map_offset_label_cur);
        mjs_map_offset_label_cur = mjs_map_offset_label_next; 
    }
    free(list);

    return 0;
}

int mjs_map_offset_label_list_append(struct mjs_map_offset_label_list *list, \
        struct mjs_map_offset_label *new_mjs_map_offset_label)
{
    if (list->begin == NULL)
    {
        list->begin = list->end = new_mjs_map_offset_label;
    }
    else
    {
        new_mjs_map_offset_label->prev = list->end;
        list->end->next = new_mjs_map_offset_label;
        list->end = new_mjs_map_offset_label;
    }

    return 0;
}

int mjs_map_offset_label_list_append_with_configure( \
        struct mjs_map_offset_label_list *list, \
        uint32_t offset, \
        char *str, size_t len, \
        uint32_t pos_ln, uint32_t pos_col)
{
    struct mjs_map_offset_label *new_mjs_map_offset_label = NULL;

    new_mjs_map_offset_label = mjs_map_offset_label_new( \
            offset, \
            str, len, \
            pos_ln, pos_col);
    if (new_mjs_map_offset_label == NULL) { return -1; }

    mjs_map_offset_label_list_append(list, new_mjs_map_offset_label);

    return 0;
}


int mjs_icodegen_statement_list_apply_goto(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list)
{
    int ret = 0;
    struct mjs_map_offset_label *map_offset_label_cur;
    struct multiply_offset_item *offset_item_target;
    uint32_t offset_label;
    uint32_t offset_goto;

    /* Apply goto to labels */
    map_offset_label_cur = map_offset_label_list->begin;
    while (map_offset_label_cur != NULL)
    {
        /* Locate to the label definition */
        offset_item_target = multiply_offset_item_pack_stack_lookup_by_label( \
                context->offset_item_pack_stack, \
                map_offset_label_cur->str, \
                map_offset_label_cur->len, \
                icg_fcb_block);
        if (offset_item_target == NULL)
        {
            multiple_error_update(err, -MULTIPLE_ERR_ICODEGEN, \
                    "%d:%d: error: label '%s' not found", \
                    map_offset_label_cur->pos_ln, map_offset_label_cur->pos_col, \
                    map_offset_label_cur->str);
            ret = -MULTIPLE_ERR_ICODEGEN;
            goto fail;
        }

        offset_label = offset_item_target->offset;
        offset_goto = map_offset_label_cur->offset;
        if ((ret = mjs_icg_fcb_block_link_relative(icg_fcb_block, offset_goto, offset_label)) != 0) { goto fail; }

        map_offset_label_cur = map_offset_label_cur->next;
    }

    goto done;
fail:
done:
    return ret;
}

