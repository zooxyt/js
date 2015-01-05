/* Multiple JavaScript Programming Language : Optimizer
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

#ifndef _MJS_OPTIMIZER_H_
#define _MJS_OPTIMIZER_H_

#include "multiple_err.h"

struct optimizer_options
{
    int constant_folding;
};

int mjs_optimize(struct multiple_error *err, \
        struct mjs_ast_program *program, \
        struct optimizer_options *options);

#endif

