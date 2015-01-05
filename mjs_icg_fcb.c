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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "multiple_err.h"
#include "multiple_ir.h"

#include "multiply_num.h"

#include "mjs_icg_fcb.h"
#include "mjs_icg_fcb_built_in_proc.h"


/* Attributes for each line */

struct mjs_icg_fcb_line_attr *mjs_icg_fcb_line_attr_new(uint32_t attr_id, uint32_t res_id)
{
    struct mjs_icg_fcb_line_attr *new_attr = NULL;

    new_attr = (struct mjs_icg_fcb_line_attr *)malloc(sizeof(struct mjs_icg_fcb_line_attr));
    if (new_attr == NULL) { goto fail; }
    new_attr->attr_id = attr_id;
    new_attr->res_id = res_id;
    new_attr->next = NULL;

fail:
    return new_attr;
}

int mjs_icg_fcb_line_attr_destroy(struct mjs_icg_fcb_line_attr *attr)
{
    free(attr);
    return 0;
}

struct mjs_icg_fcb_line_attr_list *mjs_icg_fcb_line_attr_list_new(void)
{
    struct mjs_icg_fcb_line_attr_list *new_attr_list = NULL;

    new_attr_list = (struct mjs_icg_fcb_line_attr_list *)malloc(sizeof(struct mjs_icg_fcb_line_attr_list));
    if (new_attr_list == NULL) { goto fail; }
    new_attr_list->begin = new_attr_list->end = NULL;
    new_attr_list->size = 0;

fail:
    return new_attr_list;
}

int mjs_icg_fcb_line_attr_list_destroy(struct mjs_icg_fcb_line_attr_list *list)
{
    struct mjs_icg_fcb_line_attr *attr_cur, *attr_next;

    attr_cur = list->begin;
    while (attr_cur != NULL)
    {
        attr_next = attr_cur->next; 
        mjs_icg_fcb_line_attr_destroy(attr_cur);
        attr_cur = attr_next;
    }
    free(list);
    return 0;
}

int mjs_icg_fcb_line_attr_list_append(struct mjs_icg_fcb_line_attr_list *list, \
        struct mjs_icg_fcb_line_attr *new_attr)
{
    if (list->begin == NULL)
    {
        list->begin = list->end = new_attr;
    }
    else
    {
        list->end->next = new_attr;
        list->end = new_attr;
    }
    list->size += 1;

    return 0;
}

int mjs_icg_fcb_line_attr_list_append_with_configure(struct mjs_icg_fcb_line_attr_list *list, \
        uint32_t attr_id, uint32_t res_id)
{
    struct mjs_icg_fcb_line_attr *new_attr = NULL;

    new_attr = mjs_icg_fcb_line_attr_new(attr_id, res_id);
    if (new_attr == NULL) return -MULTIPLE_ERR_MALLOC;

    mjs_icg_fcb_line_attr_list_append(list, new_attr);

    return 0;
}


struct mjs_icg_fcb_line *mjs_icg_fcb_line_new(void)
{
    struct mjs_icg_fcb_line *new_icg_fcb_line = NULL;

    new_icg_fcb_line = (struct mjs_icg_fcb_line *)malloc(sizeof(struct mjs_icg_fcb_line));
    if (new_icg_fcb_line == NULL) goto fail;
    new_icg_fcb_line->opcode = new_icg_fcb_line->operand = 0;
    new_icg_fcb_line->type = MJS_ICG_FCB_LINE_TYPE_NORMAL;
    new_icg_fcb_line->attrs = NULL;
    new_icg_fcb_line->prev = new_icg_fcb_line->next = NULL;
    goto done;
fail:
    if (new_icg_fcb_line != NULL) { free(new_icg_fcb_line); new_icg_fcb_line = NULL; }
done:
    return new_icg_fcb_line;
}

int mjs_icg_fcb_line_destroy(struct mjs_icg_fcb_line *icg_fcb_line)
{
    if (icg_fcb_line == NULL) return -MULTIPLE_ERR_NULL_PTR;

    if (icg_fcb_line->attrs != NULL)
    {
        mjs_icg_fcb_line_attr_list_destroy(icg_fcb_line->attrs);
    }
    free(icg_fcb_line); 
    return 0;
}

static struct mjs_icg_fcb_line *mjs_icg_fcb_line_new_with_configure_raw(uint32_t opcode, uint32_t operand, int type)
{
    struct mjs_icg_fcb_line *new_icg_fcb_line = NULL;

    new_icg_fcb_line = mjs_icg_fcb_line_new();
    if (new_icg_fcb_line == NULL) { goto fail; }
    new_icg_fcb_line->type = type;
    new_icg_fcb_line->opcode = opcode;
    new_icg_fcb_line->operand = operand;
    new_icg_fcb_line->attrs = NULL;

    goto done;
fail:
    if (new_icg_fcb_line != NULL)
    {
        mjs_icg_fcb_line_destroy(new_icg_fcb_line);
        new_icg_fcb_line = NULL;
    }
done:
    return new_icg_fcb_line;
}


struct mjs_icg_fcb_line *mjs_icg_fcb_line_new_with_configure(uint32_t opcode, uint32_t operand)
{
    return mjs_icg_fcb_line_new_with_configure_raw(opcode, operand, MJS_ICG_FCB_LINE_TYPE_NORMAL);
}

struct mjs_icg_fcb_line *mjs_icg_fcb_line_new_with_configure_type(uint32_t opcode, uint32_t operand, int type)
{
    return mjs_icg_fcb_line_new_with_configure_raw(opcode, operand, type);
}

struct mjs_icg_fcb_block *mjs_icg_fcb_block_new(void)
{
    struct mjs_icg_fcb_block *new_icg_fcb_block = NULL;

    new_icg_fcb_block = (struct mjs_icg_fcb_block *)malloc(sizeof(struct mjs_icg_fcb_block));
    if (new_icg_fcb_block == NULL) goto fail;
    new_icg_fcb_block->begin = new_icg_fcb_block->end = NULL;
    new_icg_fcb_block->prev = new_icg_fcb_block->next = NULL;
    new_icg_fcb_block->size = 0;
    goto done;
fail:
    if (new_icg_fcb_block != NULL) { free(new_icg_fcb_block); }
done:
    return new_icg_fcb_block;
}

int mjs_icg_fcb_block_destroy(struct mjs_icg_fcb_block *icg_fcb_block)
{
    struct mjs_icg_fcb_line *icg_fcb_line_cur, *icg_fcb_line_next;

    if (icg_fcb_block == NULL) return -MULTIPLE_ERR_NULL_PTR;
    icg_fcb_line_cur = icg_fcb_block->begin;
    while (icg_fcb_line_cur != NULL)
    {
        icg_fcb_line_next = icg_fcb_line_cur->next; 
        mjs_icg_fcb_line_destroy(icg_fcb_line_cur);
        icg_fcb_line_cur = icg_fcb_line_next;
    }
    free(icg_fcb_block); 
    return 0;
}

int mjs_icg_fcb_block_append(struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_icg_fcb_line *new_icg_fcb_line)
{
    if (icg_fcb_block == NULL) return -MULTIPLE_ERR_NULL_PTR;
    if (new_icg_fcb_line == NULL) return -MULTIPLE_ERR_NULL_PTR;

    if (icg_fcb_block->begin == NULL)
    {
        icg_fcb_block->begin = icg_fcb_block->end = new_icg_fcb_line;
    }
    else
    {
        icg_fcb_block->end->next = new_icg_fcb_line;
        new_icg_fcb_line->prev = icg_fcb_block->end;
        icg_fcb_block->end = new_icg_fcb_line;
    }
    icg_fcb_block->size += 1;

    return 0;
}

int mjs_icg_fcb_block_insert(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number_insert, \
        struct mjs_icg_fcb_line *new_icg_fcb_line)
{
    struct mjs_icg_fcb_line *icg_fcb_line_cur = NULL, *icg_fcb_line_prev = NULL;
    uint32_t instrument_number = instrument_number_insert;

    /* Locate to the position */
    icg_fcb_line_cur = icg_fcb_block->begin;
    while ((icg_fcb_line_cur != NULL) && (instrument_number-- != 0))
    {
        icg_fcb_line_prev = icg_fcb_line_cur;
        icg_fcb_line_cur = icg_fcb_line_cur->next; 
    }

    if (icg_fcb_line_cur == NULL) { return -MULTIPLE_ERR_INTERNAL; }

    /* Insert */
    if (icg_fcb_line_prev == NULL)
    {
        new_icg_fcb_line->next = icg_fcb_block->begin;
        icg_fcb_block->begin->prev = new_icg_fcb_line;
        icg_fcb_block->begin = new_icg_fcb_line;
    }
    else
    {
        icg_fcb_line_prev->next = new_icg_fcb_line;
        new_icg_fcb_line->prev = icg_fcb_line_prev;
        new_icg_fcb_line->next = icg_fcb_line_cur;
        icg_fcb_line_cur->prev = new_icg_fcb_line;
    }
    icg_fcb_block->size += 1;

    /* Fix */
    icg_fcb_line_cur = icg_fcb_block->begin;
    while (icg_fcb_line_cur != NULL)
    {
        if (icg_fcb_line_cur->type == MJS_ICG_FCB_LINE_TYPE_PC)
        {
            if (icg_fcb_line_cur->operand > instrument_number_insert)
            {
                icg_fcb_line_cur->operand += 1;
            }
        }
        icg_fcb_line_cur = icg_fcb_line_cur->next;
    }

    return 0;
}


int mjs_icg_fcb_block_append_with_configure(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t opcode, uint32_t operand)
{
    int ret;
    struct mjs_icg_fcb_line *new_icg_fcb_line = NULL;

    new_icg_fcb_line = mjs_icg_fcb_line_new_with_configure(opcode, operand);
    if (new_icg_fcb_line == NULL) 
    {
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }
    if ((ret = mjs_icg_fcb_block_append(icg_fcb_block, new_icg_fcb_line)) != 0)
    { goto fail; }

    goto done;
fail:
    if (new_icg_fcb_line != NULL)
    {
        mjs_icg_fcb_line_destroy(new_icg_fcb_line);
        new_icg_fcb_line = NULL;
    }
done:
    return ret;
}

int mjs_icg_fcb_block_append_with_configure_type(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t opcode, uint32_t operand, int type)
{
    int ret;
    struct mjs_icg_fcb_line *new_icg_fcb_line = NULL;

    new_icg_fcb_line = mjs_icg_fcb_line_new_with_configure_type(opcode, operand, type);
    if (new_icg_fcb_line == NULL) 
    {
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }
    if ((ret = mjs_icg_fcb_block_append(icg_fcb_block, new_icg_fcb_line)) != 0)
    { goto fail; }

    goto done;
fail:
    if (new_icg_fcb_line != NULL)
    {
        mjs_icg_fcb_line_destroy(new_icg_fcb_line);
        new_icg_fcb_line = NULL;
    }
done:
    return ret;
}


uint32_t mjs_icg_fcb_block_get_instrument_number(struct mjs_icg_fcb_block *icg_fcb_block)
{
    return (uint32_t)(icg_fcb_block->size);
}

static int mjs_icg_fcb_block_insert_with_configure_raw(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number, uint32_t opcode, uint32_t operand, int type)
{
    int ret;
    struct mjs_icg_fcb_line *new_icg_fcb_line = NULL;

    new_icg_fcb_line = mjs_icg_fcb_line_new_with_configure_raw(opcode, operand, type);
    if (new_icg_fcb_line == NULL) 
    {
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail;
    }

    if ((ret = mjs_icg_fcb_block_insert(icg_fcb_block, instrument_number, new_icg_fcb_line)) != 0)
    { goto fail; }

    goto done;
fail:
    if (new_icg_fcb_line != NULL)
    {
        mjs_icg_fcb_line_destroy(new_icg_fcb_line);
        new_icg_fcb_line = NULL;
    }
done:
    return ret;
}

int mjs_icg_fcb_block_insert_with_configure(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number, uint32_t opcode, uint32_t operand)
{
    return mjs_icg_fcb_block_insert_with_configure_raw(icg_fcb_block, instrument_number, \
            opcode, operand, MJS_ICG_FCB_LINE_TYPE_NORMAL);
}

int mjs_icg_fcb_block_insert_with_configure_type(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number, uint32_t opcode, uint32_t operand, int type)
{
    return mjs_icg_fcb_block_insert_with_configure_raw(icg_fcb_block, instrument_number, \
            opcode, operand, type);
}

int mjs_icg_fcb_block_link(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number_from, uint32_t instrument_number_to)
{
    struct mjs_icg_fcb_line *icg_fcb_line_cur;
    icg_fcb_line_cur = icg_fcb_block->begin;
    while ((instrument_number_from != 0) && (icg_fcb_line_cur != NULL))
    {
        icg_fcb_line_cur = icg_fcb_line_cur->next; 
        instrument_number_from -= 1;
    }
    if (icg_fcb_line_cur != NULL) 
    {
        icg_fcb_line_cur->operand = instrument_number_to;
        return 0;
    }
    else
    {
        return -1;
    }
}

int mjs_icg_fcb_block_link_relative(struct mjs_icg_fcb_block *icg_fcb_block, \
        uint32_t instrument_number_from, uint32_t instrument_number_to)
{
    struct mjs_icg_fcb_line *icg_fcb_line_cur;
    uint32_t instrument_number_from_bak = instrument_number_from;

    icg_fcb_line_cur = icg_fcb_block->begin;
    while ((instrument_number_from != 0) && (icg_fcb_line_cur != NULL))
    {
        icg_fcb_line_cur = icg_fcb_line_cur->next; 
        instrument_number_from -= 1;
    }
    if (icg_fcb_line_cur != NULL) 
    {
        icg_fcb_line_cur->operand = snr_sam_to_cmp((int32_t)instrument_number_to - (int32_t)instrument_number_from_bak);
        return 0;
    }
    else
    {
        return -1;
    }
}

struct mjs_icg_fcb_block_list *mjs_icg_fcb_block_list_new(void)
{
    struct mjs_icg_fcb_block_list *new_icg_fcb_block_list = NULL;

    new_icg_fcb_block_list = (struct mjs_icg_fcb_block_list *)malloc(sizeof(struct mjs_icg_fcb_block_list));
    if (new_icg_fcb_block_list == NULL) goto fail;
    new_icg_fcb_block_list->begin = new_icg_fcb_block_list->end = NULL;
    new_icg_fcb_block_list->size = 0;
    goto done;
fail:
    if (new_icg_fcb_block_list != NULL) { free(new_icg_fcb_block_list); }
done:
    return new_icg_fcb_block_list;
}

int mjs_icg_fcb_block_list_destroy(struct mjs_icg_fcb_block_list *icg_fcb_block_list)
{
    struct mjs_icg_fcb_block *icg_fcb_block_cur, *icg_fcb_block_next;

    if (icg_fcb_block_list == NULL) return -MULTIPLE_ERR_NULL_PTR;
    icg_fcb_block_cur = icg_fcb_block_list->begin;
    while (icg_fcb_block_cur != NULL)
    {
        icg_fcb_block_next = icg_fcb_block_cur->next; 
        mjs_icg_fcb_block_destroy(icg_fcb_block_cur);
        icg_fcb_block_cur = icg_fcb_block_next;
    }
    free(icg_fcb_block_list); 
    return 0;
}

int mjs_icg_fcb_block_list_append(struct mjs_icg_fcb_block_list *icg_fcb_block_list, \
        struct mjs_icg_fcb_block *new_icg_fcb_block)
{
    if (icg_fcb_block_list == NULL) return -MULTIPLE_ERR_NULL_PTR;
    if (new_icg_fcb_block == NULL) return -MULTIPLE_ERR_NULL_PTR;

    if (icg_fcb_block_list->begin == NULL)
    {
        icg_fcb_block_list->begin = icg_fcb_block_list->end = new_icg_fcb_block;
    }
    else
    {
        icg_fcb_block_list->end->next = new_icg_fcb_block;
        icg_fcb_block_list->end = new_icg_fcb_block;
    }
    icg_fcb_block_list->size += 1;
    return 0;
}


struct mjs_icg_customizable_built_in_procedure_write_back *mjs_icg_customizable_built_in_procedure_write_back_new(uint32_t instrument_number)
{
    struct mjs_icg_customizable_built_in_procedure_write_back *new_write_back = NULL;

    new_write_back = (struct mjs_icg_customizable_built_in_procedure_write_back *)malloc(sizeof(struct mjs_icg_customizable_built_in_procedure_write_back));
    if (new_write_back == NULL) { goto fail; }
    new_write_back->instrument_number = instrument_number;

fail:
    return new_write_back;
}

int mjs_icg_customizable_built_in_procedure_write_back_destroy(struct mjs_icg_customizable_built_in_procedure_write_back *write_back)
{
    free(write_back);
    return 0;
}

struct mjs_icg_customizable_built_in_procedure_write_back_list *mjs_icg_customizable_built_in_procedure_write_back_list_new(void)
{
    struct mjs_icg_customizable_built_in_procedure_write_back_list *new_write_back_list = NULL;

    new_write_back_list = (struct mjs_icg_customizable_built_in_procedure_write_back_list *)malloc(sizeof(struct mjs_icg_customizable_built_in_procedure_write_back_list));
    if (new_write_back_list == NULL) { goto fail; }
    new_write_back_list->begin = new_write_back_list->end = NULL;
    new_write_back_list->size = 0;

fail:
    return new_write_back_list;
}

int mjs_icg_customizable_built_in_procedure_write_back_list_destroy(struct mjs_icg_customizable_built_in_procedure_write_back_list *write_back_list)
{
    struct mjs_icg_customizable_built_in_procedure_write_back *write_back_cur, *write_back_next;

    write_back_cur = write_back_list->begin;
    while (write_back_cur != NULL)
    {
        write_back_next = write_back_cur->next;
        mjs_icg_customizable_built_in_procedure_write_back_destroy(write_back_cur);
        write_back_cur = write_back_next;
    }
    free(write_back_list);

    return 0;
}

int mjs_icg_customizable_built_in_procedure_write_back_list_append_with_configure(struct mjs_icg_customizable_built_in_procedure_write_back_list *write_back_list, uint32_t instrument_number)
{
    int ret = 0;
    struct mjs_icg_customizable_built_in_procedure_write_back *new_write_back = NULL;

    new_write_back = mjs_icg_customizable_built_in_procedure_write_back_new(instrument_number);
    if (new_write_back == NULL) 
    { 
        ret = -MULTIPLE_ERR_MALLOC;
        goto fail; 
    }

    if (write_back_list->begin == NULL)
    {
        write_back_list->begin = write_back_list->end = new_write_back;
    }
    else
    {
        write_back_list->end->next = new_write_back;
        write_back_list->end = new_write_back;
    }
    write_back_list->size += 1;

fail:
    return ret;
}

struct mjs_icg_customizable_built_in_procedure *mjs_icg_customizable_built_in_procedure_new(const char *name, const size_t name_len)
{
    struct mjs_icg_customizable_built_in_procedure *new_customizable_built_in_procedure = NULL;

    new_customizable_built_in_procedure = (struct mjs_icg_customizable_built_in_procedure *)malloc(sizeof(struct mjs_icg_customizable_built_in_procedure));
    if (new_customizable_built_in_procedure == NULL) { goto fail; }
    new_customizable_built_in_procedure->write_backs = NULL;
    new_customizable_built_in_procedure->name = NULL;
    new_customizable_built_in_procedure->name = (char *)malloc(sizeof(char) * (name_len + 1));
    if (new_customizable_built_in_procedure->name == NULL) { goto fail; };
    memcpy(new_customizable_built_in_procedure->name, name, name_len);
    new_customizable_built_in_procedure->name[name_len] = '\0';
    new_customizable_built_in_procedure->name_len = name_len;
    new_customizable_built_in_procedure->called = 0;
    new_customizable_built_in_procedure->instrument_number_icode = 0;
    if ((new_customizable_built_in_procedure->write_backs = mjs_icg_customizable_built_in_procedure_write_back_list_new()) == NULL)
    { goto fail; }
    new_customizable_built_in_procedure->next = NULL;

    goto done;
fail:
    if (new_customizable_built_in_procedure != NULL)
    {
        if (new_customizable_built_in_procedure->write_backs != NULL) mjs_icg_customizable_built_in_procedure_write_back_list_destroy(new_customizable_built_in_procedure->write_backs);
        if (new_customizable_built_in_procedure->name != NULL) free(new_customizable_built_in_procedure->name);
        free(new_customizable_built_in_procedure);
        new_customizable_built_in_procedure = NULL;
    }
done:
    return new_customizable_built_in_procedure; 
}

int mjs_icg_customizable_built_in_procedure_destroy(struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure)
{
    if (customizable_built_in_procedure == NULL) return -MULTIPLE_ERR_NULL_PTR;

	if (customizable_built_in_procedure->write_backs != NULL) mjs_icg_customizable_built_in_procedure_write_back_list_destroy(customizable_built_in_procedure->write_backs);
    if (customizable_built_in_procedure->name != NULL) free(customizable_built_in_procedure->name);
    free(customizable_built_in_procedure);

    return 0;
}

struct mjs_icg_customizable_built_in_procedure_list *mjs_icg_customizable_built_in_procedure_list_new(void)
{
    struct mjs_icg_customizable_built_in_procedure_list *new_customizable_built_in_procedure_list = NULL;

    new_customizable_built_in_procedure_list = (struct mjs_icg_customizable_built_in_procedure_list *)malloc(sizeof(struct mjs_icg_customizable_built_in_procedure_list));
    if (new_customizable_built_in_procedure_list == NULL) { goto fail; }
    new_customizable_built_in_procedure_list->begin = new_customizable_built_in_procedure_list->end = NULL;
    goto done;
fail:
    if (new_customizable_built_in_procedure_list != NULL)
    {
        free(new_customizable_built_in_procedure_list);
        new_customizable_built_in_procedure_list = NULL;
    }
done:
    return new_customizable_built_in_procedure_list;
}

int mjs_icg_customizable_built_in_procedure_list_destroy(struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list)
{
    struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure_cur, *customizable_built_in_procedure_next;

    if (customizable_built_in_procedure_list == NULL) return -MULTIPLE_ERR_NULL_PTR;

    customizable_built_in_procedure_cur = customizable_built_in_procedure_list->begin;
    while (customizable_built_in_procedure_cur != NULL)
    {
        customizable_built_in_procedure_next = customizable_built_in_procedure_cur->next;
        mjs_icg_customizable_built_in_procedure_destroy( customizable_built_in_procedure_cur);
        customizable_built_in_procedure_cur = customizable_built_in_procedure_next;
    }

    free(customizable_built_in_procedure_list);

    return 0;
}

static int mjs_icg_customizable_built_in_procedure_list_append(struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list, \
        struct mjs_icg_customizable_built_in_procedure *new_customizable_built_in_procedure)
{
    int ret = 0;

    if (customizable_built_in_procedure_list == NULL) return -MULTIPLE_ERR_NULL_PTR;
    if (new_customizable_built_in_procedure == NULL) return -MULTIPLE_ERR_NULL_PTR;

    if (customizable_built_in_procedure_list->begin == NULL)
    {
        customizable_built_in_procedure_list->begin = customizable_built_in_procedure_list->end = new_customizable_built_in_procedure;
    }
    else
    {
        customizable_built_in_procedure_list->end->next = new_customizable_built_in_procedure;
        customizable_built_in_procedure_list->end = new_customizable_built_in_procedure;
    }

    return ret;
}

struct mjs_icg_customizable_built_in_procedure *mjs_icg_customizable_built_in_procedure_list_lookup(struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list, \
        const char *name, const size_t name_len)
{
    struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure_cur;

    customizable_built_in_procedure_cur = customizable_built_in_procedure_list->begin;
    while (customizable_built_in_procedure_cur != NULL)
    {
        if ((customizable_built_in_procedure_cur->name_len == name_len) && \
                (strncmp(customizable_built_in_procedure_cur->name, name, name_len) == 0))
        {
            return customizable_built_in_procedure_cur;
        }
        customizable_built_in_procedure_cur = customizable_built_in_procedure_cur->next; 
    }

    return NULL;
}

static int is_customizable_built_in_procedure(const char *name, const size_t name_len)
{
    size_t i;
    for (i = 0; i != CUSTOMIZABLE_BUILT_IN_PROCEDURE_COUNT; i++)
    {
        if ((name_len == strlen(customizable_built_in_procedures[i])) && \
                (strncmp(name, customizable_built_in_procedures[i], name_len) == 0))
        { return 1; }
    }
    return 0;
}

int mjs_icg_customizable_built_in_procedure_list_called( \
        struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list, \
        const char *name, const size_t name_len)
{
    int ret = 0;
    struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure_target = NULL;
    struct mjs_icg_customizable_built_in_procedure *new_customizable_built_in_procedure = NULL;

    if (customizable_built_in_procedure_list == NULL) return -MULTIPLE_ERR_NULL_PTR;

    if (is_customizable_built_in_procedure(name, name_len) == 0) return 0;

    customizable_built_in_procedure_target = mjs_icg_customizable_built_in_procedure_list_lookup(customizable_built_in_procedure_list, name, name_len);
    if (customizable_built_in_procedure_target == NULL)
    {
        /* Not exists, create one */
        new_customizable_built_in_procedure = mjs_icg_customizable_built_in_procedure_new(name, name_len);
        if (new_customizable_built_in_procedure == NULL)
        {
            ret = -MULTIPLE_ERR_MALLOC;
            goto fail;
        }
        if ((ret = mjs_icg_customizable_built_in_procedure_list_append(customizable_built_in_procedure_list, \
                        new_customizable_built_in_procedure)) != 0)
        { goto fail; }
        customizable_built_in_procedure_target = new_customizable_built_in_procedure;
        new_customizable_built_in_procedure = NULL;
    }
    customizable_built_in_procedure_target->called = 1;
    goto done;
fail:
    if (new_customizable_built_in_procedure != NULL)
    {
        mjs_icg_customizable_built_in_procedure_destroy(new_customizable_built_in_procedure);
        new_customizable_built_in_procedure = NULL;
    }
done:
    return ret;
}

int mjs_icg_customizable_built_in_procedure_list_add_writeback( \
        struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list, \
        const char *name, const size_t name_len, uint32_t instrument_number)
{
    int ret = 0;
    struct mjs_icg_customizable_built_in_procedure *customizable_built_in_procedure_target = NULL;
    struct mjs_icg_customizable_built_in_procedure *new_customizable_built_in_procedure = NULL;

    if (customizable_built_in_procedure_list == NULL) return -MULTIPLE_ERR_NULL_PTR;

    if (is_customizable_built_in_procedure(name, name_len) == 0) return 0;

    customizable_built_in_procedure_target = mjs_icg_customizable_built_in_procedure_list_lookup(customizable_built_in_procedure_list, name, name_len);
    if (customizable_built_in_procedure_target == NULL)
    {
        /* Not exists, create one */
        new_customizable_built_in_procedure = mjs_icg_customizable_built_in_procedure_new(name, name_len);
        if (new_customizable_built_in_procedure == NULL)
        {
            ret = -MULTIPLE_ERR_MALLOC;
            goto fail;
        }
        if ((ret = mjs_icg_customizable_built_in_procedure_list_append(customizable_built_in_procedure_list, \
                        new_customizable_built_in_procedure)) != 0)
        { goto fail; }
        customizable_built_in_procedure_target = new_customizable_built_in_procedure;
        new_customizable_built_in_procedure = NULL;
    }
    if ((ret = mjs_icg_customizable_built_in_procedure_write_back_list_append_with_configure( \
                    customizable_built_in_procedure_target->write_backs, instrument_number)) != 0) { goto fail; }
    goto done;
fail:
    if (new_customizable_built_in_procedure != NULL)
    {
        mjs_icg_customizable_built_in_procedure_destroy(new_customizable_built_in_procedure);
        new_customizable_built_in_procedure = NULL;
    }
done:
    return ret;
}

