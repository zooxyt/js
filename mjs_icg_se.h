/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Source Elements
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

#ifndef _MJS_ICG_SE_H_
#define _MJS_ICG_SE_H_

#include "multiply_offset.h"

#include "mjs_ast.h"
#include "mjs_icg_context.h"
#include "mjs_icg_fcb.h"
#include "mjs_icg_aux.h"

int mjs_icodegen_source_element_list(struct multiple_error *err, \
        struct mjs_icg_context *context, \
        struct mjs_icg_fcb_block *icg_fcb_block, \
        struct mjs_map_offset_label_list *map_offset_label_list, \
        struct mjs_ast_source_element_list *list, \
        struct multiply_offset_item_pack *offset_pack_break);

#endif

