/* Multiple JavaScript Programming Language : Intermediate Code Generator
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

#ifndef _MJS_ICG_H_
#define _MJS_ICG_H_

#include "multiple_ir.h"

#include "mjs_ast.h"
#include "mjs_icg_fcb.h"
#include "mjs_icg_context.h"

int mjs_irgen(struct multiple_error *err, \
        struct multiple_ir **icode_out, \
        struct mjs_ast_program *program, \
        int verbose);

#endif

