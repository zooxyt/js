/* Multiple JavaScript Programming Language : Intermediate Code Generator
 * Built-in Procedures
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

#ifndef _MJS_ICG_BUILT_IN_PROC_H_
#define _MJS_ICG_BUILT_IN_PROC_H_

#include <stdint.h>

#include "multiple_err.h"
#include "multiple_ir.h"

#include "multiply.h"

#include "mjs_icg_fcb.h"

int mjs_icg_add_built_in_procs(struct multiple_error *err, \
        struct multiple_ir *icode, \
        struct multiply_resource_id_pool *res_id, \
        struct mjs_icg_fcb_block *icg_fcb_block_init, \
        struct mjs_icg_customizable_built_in_procedure_list *customizable_built_in_procedure_list,
        uint32_t insert_point, uint32_t *instrument_count);

#endif

