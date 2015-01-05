/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Built-in Tables
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "multiple_err.h"
#include "multiple_ir.h"

#include "multiply.h"
#include "multiply_assembler.h"

#include "vm_opcode.h"
#include "vm_types.h"
#include "vm_predef.h"

#include "mjs_lexer.h"
#include "mjs_ast.h"
#include "mjs_icg.h"
#include "mjs_icg_fcb.h"

#include "mjs_icg_stdlib_hdl.h"
#include "mjs_icg_stdlib.h"

#include "mjs_icg_built_in_table.h"

int mjs_icg_add_built_in_tables(struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id, \
        struct mjs_icg_fcb_block *icg_fcb_block_init, \
        struct mjs_icg_stdlib_table_list *table_list, \
        uint32_t insert_point, uint32_t *instrument_count)
{
    int ret = 0;
    struct mjs_icg_stdlib_table *table_cur;
    struct mjs_icg_stdlib_field *field_cur;
    struct mjs_icg_add_built_in_table_handler *table_handler;
    struct mjs_icg_add_built_in_field_handler *field_handler;

    uint32_t id, id_zero, id_two;
    uint32_t instrument_number;
    int hash_item_count_in_table;

    if ((ret = multiply_resource_get_int(err, icode, res_id, \
                    &id_zero,  \
                    0)) != 0)
    { goto fail; }
    if ((ret = multiply_resource_get_int(err, icode, res_id, \
                    &id_two,  \
                    2)) != 0)
    { goto fail; }

    for (table_cur = table_list->begin; table_cur != NULL; table_cur = table_cur->next)
    {
        if ((table_handler = mjs_icg_add_built_in_table_handler_lookup( \
                        mjs_icg_add_built_in_table_handlers, \
                        table_cur->name, \
                        table_cur->len)) == NULL)
        { continue; }

        hash_item_count_in_table = 0;

        for (field_cur = table_cur->fields->begin; field_cur != NULL; field_cur = field_cur->next)
        {
            if ((field_handler = mjs_icg_add_built_in_field_handler_lookup( \
                    table_handler->field_handler, \
                    field_cur->name, field_cur->len)) == NULL)
            { continue; }

            instrument_number = (uint32_t)(icode->text_section->size);

            if ((ret = field_handler->func(err, \
                            icode, \
                            res_id)) != 0)
            { goto fail; }

            /* Key */
            if ((ret = multiply_resource_get_str(err, icode, res_id, \
                            &id,  \
                            field_cur->name, field_cur->len)) != 0)
            { goto fail; }
            if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                            icg_fcb_block_init, insert_point++, \
                            OP_PUSH, id)) != 0) { goto fail; }
            /* Value: Function */
            if ((ret = mjs_icg_fcb_block_insert_with_configure_type( \
                            icg_fcb_block_init, \
                            insert_point++, \
                            OP_LAMBDAMK, instrument_number, \
                            MJS_ICG_FCB_LINE_TYPE_BLTIN_PROC_MK)) != 0) { goto fail; }
            (*instrument_count) += 2;

            if (field_handler->type == MJS_BUILT_IN_PROPERTY)
            {
                /* Call the function and get the property value */
                if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                                icg_fcb_block_init, insert_point++, \
                                OP_PUSH, id_zero)) != 0) { goto fail; }
                if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                                icg_fcb_block_init, insert_point++, \
                                OP_PUSH, id_two)) != 0) { goto fail; }
                if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                                icg_fcb_block_init, insert_point++, \
                                OP_PICK, 0)) != 0) { goto fail; }
                if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                                icg_fcb_block_init, insert_point++, \
                                OP_CALL, 0)) != 0) { goto fail; }
                (*instrument_count) += 4;
            }


            hash_item_count_in_table += 1;
        }

        if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                        icg_fcb_block_init, insert_point++, \
                        OP_HASHMK, (uint32_t)hash_item_count_in_table)) != 0) { goto fail; }

        if ((ret = multiply_resource_get_id(err, icode, res_id, \
                        &id,  \
                        table_cur->name, \
                        table_cur->len)) != 0)
        { goto fail; }
        if ((ret = mjs_icg_fcb_block_insert_with_configure( \
                        icg_fcb_block_init, insert_point++, \
                        OP_POPC, id)) != 0) { goto fail; }
    }

    goto done;
fail:
done:
    return ret;
}

